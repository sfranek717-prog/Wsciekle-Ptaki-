#ifndef PANELLVL_H
#define PANELLVL_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Panellvl {
private:
    bool active = false;
    sf::Font font;
    sf::Text title;
    sf::Text lvl1;
    sf::Text lvl2;
    sf::Text Exit;
    sf::RectangleShape ramka_exit;
    sf::RectangleShape ramka_lvl1;
    sf::RectangleShape ramka_lvl2;
    public : 
     Panellvl() : title(font), lvl1(font), lvl2(font), Exit(font){
        if (!font.openFromFile("../arial.ttf")) {
            std::cout << "Nie udalo sie wczytac czcionki!\n";
        }
        // Tytul
        title.setFont(font);
        title.setString(U"Wybierz Level");
        title.setCharacterSize(50);
        title.setPosition({700.f, 300.f});
        title.setFillColor(sf::Color::White);

        // Lvl 1
        lvl1.setFont(font);
        lvl1.setString("Level 1");
        lvl1.setCharacterSize(30);
        lvl1.setPosition({860.f, 450.f});
        lvl1.setFillColor(sf::Color::White);

        // Ramka lvl 1
        sf::FloatRect zarys_1 = lvl1.getGlobalBounds();
        ramka_lvl1.setSize({zarys_1.size.x + 20.f, zarys_1.size.y + 20.f});
        ramka_lvl1.setPosition({zarys_1.position.x - 10.f, zarys_1.position.y - 10.f});
        ramka_lvl1.setFillColor(sf::Color::Transparent);
        ramka_lvl1.setOutlineColor(sf::Color::White);
        ramka_lvl1.setOutlineThickness(3.f);

        //lvl2
        lvl2.setFont(font);
        lvl2.setString("Level 2");
        lvl2.setCharacterSize(30);
        lvl2.setPosition({860.f, 550.f});
        lvl2.setFillColor(sf::Color::White);

        //ramka lvl2
         sf::FloatRect zarys_2 = lvl2.getGlobalBounds();
        ramka_lvl2.setSize({zarys_2.size.x + 20.f, zarys_2.size.y + 20.f});
        ramka_lvl2.setPosition({zarys_2.position.x - 10.f, zarys_2.position.y - 10.f});
        ramka_lvl2.setFillColor(sf::Color::Transparent);
        ramka_lvl2.setOutlineColor(sf::Color::White);
        ramka_lvl2.setOutlineThickness(3.f);
        // exit
        Exit.setFont(font);
        Exit.setString("Exit");
        Exit.setCharacterSize(30);
        Exit.setPosition({860.f, 650.f});
        Exit.setFillColor(sf::Color::White);

        // Ramka exit
        sf::FloatRect zarys_e = Exit.getGlobalBounds();
        ramka_exit.setSize({zarys_e.size.x + 20.f, zarys_e.size.y + 20.f});
        ramka_exit.setPosition({zarys_e.position.x - 10.f, zarys_e.position.y - 10.f});
        ramka_exit.setFillColor(sf::Color::Transparent);
        ramka_exit.setOutlineColor(sf::Color::White);
        ramka_exit.setOutlineThickness(3.f);


}
// obsluga exitu 
void exitujemy(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            active = false; 
        }
    }
}
void setActive(bool status) { active = status; }
    bool isActive() const { return active; }

    void draw(sf::RenderWindow& window) {
        if (!active) return; 
        window.draw(title);
        window.draw(lvl1);
        window.draw(lvl2);
        window.draw(Exit);
        window.draw(ramka_exit);
        window.draw(ramka_lvl1);
        window.draw(ramka_lvl2);
        
    };
};


#endif 
