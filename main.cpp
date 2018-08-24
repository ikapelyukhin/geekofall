#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Box2D/Box2D.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;

int main() {
  std::ifstream ifs("geeko.json");
  json j = json::parse(ifs);
  
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Geekofall", sf::Style::Default, settings);
  window.setFramerateLimit(60);
  
  sf::Texture geekoTexture;
  geekoTexture.loadFromFile("geeko.png");
  geekoTexture.setSmooth(true);
  
  double angle = 0;
  
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    
    window.clear(sf::Color::White);
    
    /*sf::Sprite sprite;
    sprite.setTexture(geekoTexture);
    sprite.setOrigin(77, 41);
    sprite.setPosition(200, 200);
    sprite.setRotation(angle * 180/b2_pi);
    window.draw(sprite);*/
    
    auto polygons = j["rigidBodies"][0]["polygons"];
  for (auto iter = polygons.begin(); iter != polygons.end(); iter++) {
    // cout << iter.value() << "\n\n";
    auto vertices = iter.value();
    
    sf::ConvexShape convex;
    convex.setPointCount(vertices.size());
    for (auto vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++) {
      auto vertex = vertex_iter.value();
      convex.setPoint(
        std::distance(vertices.begin(), vertex_iter),
        sf::Vector2f((double)vertex["x"] * 100 + 100, (double)vertex["y"] * -100 + 100)
      );
    }
    convex.setOutlineThickness(1);
    convex.setOutlineColor(sf::Color(250, 150, 100));
    window.draw(convex);
  }
    
    window.display();
    
    angle += 0.1;
  }

  return 0;
}
