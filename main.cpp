#include <SFML/Graphics.hpp>
#include "map.h"
#include "snake.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "snake", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::Image badge;
    if (!badge.loadFromFile("image/badge.png")) { return 1; }
    badge.createMaskFromColor(sf::Color(255, 255, 255));
    window.setIcon(225, 225, badge.getPixelsPtr());
    maps map;
    snake_t snake(&map);
    sf::Texture textureMap;
    textureMap.loadFromFile("image/map.jpg");
    sf::Sprite spriteMap;
    spriteMap.setTexture(textureMap);
    char keyboard = 'w';
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        keyboard = snake.AImove();
        if (!snake.move(keyboard)) { window.close(); }
        sf::sleep(sf::microseconds(10000));
        window.clear(sf::Color::White);
        for (int i = 0; i < map.HEIGTH; i++)
        {
            for (int j = 0; j < map.LENGTH; j++)
            {
                if (map.field[i][j] == '#') { spriteMap.setTextureRect(sf::IntRect(0, 0, 20, 20)); }    //стена
                if (map.field[i][j] == ' ') { spriteMap.setTextureRect(sf::IntRect(20, 0, 20, 20)); }   //пуста€ клетка
                if (map.field[i][j] == 'h') { spriteMap.setTextureRect(sf::IntRect(40, 0, 20, 20)); }   //голова
                if (map.field[i][j] == 'b') { spriteMap.setTextureRect(sf::IntRect(60, 0, 20, 20)); }   //хвост
                if (map.field[i][j] == '<') { spriteMap.setTextureRect(sf::IntRect(80, 0, 20, 20)); }   //еда 1
                if (map.field[i][j] == '>') { spriteMap.setTextureRect(sf::IntRect(100, 0, 20, 20)); }  //еда 2
                if (map.field[i][j] == 'p') { spriteMap.setTextureRect(sf::IntRect(120, 0, 20, 20)); }  //маршрут
                spriteMap.setPosition(i * 20, j * 20);
                window.draw(spriteMap);
            }
        }
        window.display();
    }
	return 0;
}