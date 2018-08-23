#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Box2D/Box2D.h>

int main() {
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
    
    sf::Sprite sprite;
    sprite.setTexture(geekoTexture);
    sprite.setOrigin(77, 41);
    sprite.setPosition(200, 200);
    sprite.setRotation(angle * 180/b2_pi);
    window.draw(sprite);
    
    window.display();
    
    angle += 0.1;
  }

  return 0;
}
