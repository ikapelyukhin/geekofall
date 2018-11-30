/*
Geekofall -- screensaver that has Geekos in it
Copyright (C) 2018  Ivan Kapelyukhin and contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <X11/Xlib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <random>

#ifndef DATA_PATH
#define DATA_PATH "."
#endif

#define JSON_FILE    DATA_PATH "/geeko.json"
#define TEXTURE_FILE DATA_PATH "/geeko.png"


using json = nlohmann::json;
using namespace std;

class Geekofall {

  private:

  json json_data;
  sf::RenderWindow* window;
  b2World* world;
  sf::Texture geekoTexture;

  int ticks = 0;
  int screenWidth, screenHeight;
  double b2screen = 0;

  std::mt19937 random;
  std::normal_distribution<double> dist;

  public:

  double scale = 0.25;
  double bounciness = 0.3;
  bool drawMesh = false;

  void createSFMLWindow() {
    char* wid_env;

    if ((wid_env = getenv("XSCREENSAVER_WINDOW")) != NULL) {
      Display *display;

      if ((display = XOpenDisplay(NULL)) == NULL) exit(1);

      Window x11win;
      x11win = (Window) strtol(wid_env, (char **) NULL, 0);

      XWindowAttributes wa;

      XGetWindowAttributes(display, x11win, &wa);
      screenWidth = wa.width;
      screenHeight = wa.height;

      Window x11view = XCreateSimpleWindow(
        display, x11win,
        0, 0, wa.width, wa.height, 0, 0, 0);

      XMapWindow(display, x11win);
      XFlush(display);

      window = new sf::RenderWindow(x11view);
    } else {
      sf::ContextSettings settings;
      settings.antialiasingLevel = 8;
      screenWidth = 1024;
      screenHeight = 768;
      window = new sf::RenderWindow(
        sf::VideoMode(screenWidth, screenHeight, 32),
        "Geekofall", sf::Style::Default, settings
      );
    }
    window->setFramerateLimit(60);
  }

  void createBox2DWorld() {
    b2Vec2 gravity(0.f, 9.8f);
    world = new b2World(gravity);
  }

  b2Body* createShelf() {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(screenWidth / b2screen / 2, screenHeight / b2screen - 0.025f);
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(screenWidth / b2screen / 2, 0.025f);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);

    return body;
  }

  b2Body* createGeeko(double x, double y) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x, y);
    bodyDef.type = b2_dynamicBody;
    b2Body* body = world->CreateBody(&bodyDef);
    body->ApplyLinearImpulse(b2Vec2(2.5*dist(random), 0), body->GetWorldCenter(), true);
    body->ApplyTorque(10*dist(random), true);

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
      delete[] b2vertices;

      /*****/

      b2FixtureDef fixtureDef;
      fixtureDef.shape = &polygonShape;
      fixtureDef.density = 1;
      fixtureDef.restitution = bounciness;

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

        convex.setPoint(i, sf::Vector2f(vert.x * b2screen, vert.y * b2screen));
      }

      convex.setOutlineThickness(1);
      convex.setOutlineColor(sf::Color(2, 211, 95));
      convex.setFillColor(sf::Color::Black);
      window->draw(convex);

      sf::CircleShape shape(1);
      shape.setPosition(pos.x * b2screen, pos.y * b2screen);
      shape.setFillColor(sf::Color(2, 211, 95));
      window->draw(shape);
    }
  }

  void drawBodySprite(b2Body* body) {
    b2Vec2 pos = body->GetPosition();

    sf::Sprite sprite;
    sprite.setTexture(geekoTexture);
    sprite.setOrigin(0, 150);
    sprite.setScale(scale, scale);
    sprite.setPosition(pos.x * b2screen, pos.y * b2screen);
    sprite.rotate(body->GetAngle()*180/b2_pi);
    window->draw(sprite);
  }

  void run() {
    random.seed(time(NULL));

    sf::Vector2u geekoSize = geekoTexture.getSize();
    b2screen = scale * geekoSize.x;

    auto shelf = createShelf();

    while (window->isOpen()) {
      sf::Event event;
      while (window->pollEvent(event)) {
          if (event.type == sf::Event::Closed)
              window->close();
      }

      if (ticks == 0) createGeeko(((double) screenWidth) / b2screen / 2 - 0.5, 0); // minus 0.5 geekos for centering

      window->clear(sf::Color::Black);

      if (drawMesh) drawBodyMesh(shelf);

      for (b2Body* body = world->GetBodyList(); body != 0;) {
        b2Body* nextBody = body->GetNext();

        if (body->GetType() == b2_dynamicBody) {
          drawMesh ? drawBodyMesh(body) : drawBodySprite(body);

          b2Vec2 pos = body->GetPosition();
          if (pos.y * b2screen > screenHeight + geekoSize.y) {
            world->DestroyBody(body);
          }
        }

        body = nextBody;
      }

      window->display();

      ticks = (ticks + 1) % 60;
      world->Step(1/60.f, 8, 3);
    }

    delete window;
  }

  Geekofall() {
    std::ifstream ifs(JSON_FILE);
    json_data = json::parse(ifs);

    createSFMLWindow();
    createBox2DWorld();

    geekoTexture.loadFromFile(TEXTURE_FILE);
    geekoTexture.setSmooth(true);
  }

  ~Geekofall() {
    delete(world);
  }
};

int main() {
  auto gf = Geekofall();
  gf.run();

  return 0;
}
