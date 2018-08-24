#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Box2D/Box2D.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;

class Geekofall {

  private:

  json json_data;
  sf::RenderWindow* window;
  b2World* world;
  sf::Texture geekoTexture;
  
  int ticks = 0;
  
  public:
  
  void createSFMLWindow() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Geekofall", sf::Style::Default, settings);
    window->setFramerateLimit(60);
  }
  
  void createBox2DWorld() {
    b2Vec2 gravity(0.f, 9.8f);
    world = new b2World(gravity);
  }
  
  b2Body* createShelf() {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(4, 6);
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);
    
    b2PolygonShape shape;
    shape.SetAsBox(5, 0.025f);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
    
    return body;
  }
  
  b2Body* createGeeko(int x, int y) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(3, 0);
    bodyDef.type = b2_dynamicBody;
    b2Body* body = world->CreateBody(&bodyDef);
    
    auto polygons = json_data["rigidBodies"][0]["polygons"];
    for (auto iter = polygons.begin(); iter != polygons.end(); iter++) {
      auto vertices = iter.value();
      
      b2PolygonShape polygonShape;
      b2Vec2 *b2vertices = new b2Vec2[vertices.size()];
    
      for (auto vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++) {
        auto vertex = vertex_iter.value();
        int idx = std::distance(vertices.begin(), vertex_iter);
        
        b2vertices[idx].Set((double)vertex["x"], -(double)vertex["y"]);
      }
      
      polygonShape.Set(b2vertices, vertices.size());
      
      /*****/

      b2FixtureDef fixtureDef;
      fixtureDef.shape = &polygonShape;
      fixtureDef.density = 1;
      fixtureDef.restitution = 0.3f;

      body->CreateFixture(&fixtureDef);
    }
    
    return body;
  }
  
  void drawBodyMesh(b2Body* body) {
    b2Vec2 pos = body->GetPosition();
    
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
      b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
      
      int count = polygonShape->GetVertexCount();
      
      sf::ConvexShape convex;
      convex.setPointCount(count);

      for(int i = 0; i < count; i++) {
        b2Vec2 vert = body->GetWorldPoint(polygonShape->GetVertex(i));
        
        convex.setPoint(i, sf::Vector2f(vert.x * 100, vert.y * 100));
      }
      
      convex.setOutlineThickness(1);
      convex.setOutlineColor(sf::Color(250, 150, 100));
      convex.setFillColor(sf::Color::Black);
      window->draw(convex);
      
      sf::CircleShape shape(3);
      shape.setPosition(pos.x*100, pos.y*100);
      shape.setFillColor(sf::Color(100, 250, 50));
      window->draw(shape);
    }
  }
  
  void drawBodySprite(b2Body* body) {
    b2Vec2 pos = body->GetPosition();
    
    sf::Sprite sprite;
    sprite.setTexture(geekoTexture);
    sprite.setOrigin(0, 82);
    sprite.setScale(0.65, 0.65);
    sprite.setPosition(pos.x * 100, pos.y * 100);
    sprite.rotate(body->GetAngle()*180/b2_pi);
    window->draw(sprite);
  }
  
  Geekofall() {
    std::ifstream ifs("geeko.json");
    json_data = json::parse(ifs);
    
    createSFMLWindow();
    createBox2DWorld();
    
    auto shelf = createShelf();
    
    geekoTexture.loadFromFile("geeko.png");
    geekoTexture.setSmooth(true);
    
    while (window->isOpen()) {
      sf::Event event;
      while (window->pollEvent(event)) {
          if (event.type == sf::Event::Closed)
              window->close();
      }

      /*
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
          int MouseX = sf::Mouse::getPosition(Window).x;
          int MouseY = sf::Mouse::getPosition(Window).y;
      }
      */
      
      if (ticks == 0) createGeeko(3,2);
      
      window->clear(sf::Color::Black);
      
      //drawBodyMesh(shelf);
      
      for (b2Body* body = world->GetBodyList(); body != 0; body = body->GetNext()) {
        if (body->GetType() == b2_dynamicBody) {
          //drawBodyMesh(body);
          drawBodySprite(body);
        }
      }
      
      window->display();
      
      ticks = (ticks + 1) % 60;
      world->Step(1/60.f, 8, 3);
    }
  }
};

int main() {
  new Geekofall();

  return 0;
}
