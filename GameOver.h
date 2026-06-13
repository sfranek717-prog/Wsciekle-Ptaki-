#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "lvl11.h"

class GameOver {
private:
    bool active = false;
    sf::Font font;
    sf::Text title;
    sf::Text lvl1;
    sf::Text lvl2;
    sf::Text Exit;
    sf::Texture tlo;

    sf::RectangleShape ramka_exit;
    sf::RectangleShape ramka_lvl1;
    sf::RectangleShape ramka_lvl2;
    sf::RectangleShape obraz;

public: 
    Lvl11 lvl111(Material& mat_czerw, Material& mat_bomba, Material& mat_zoltek, Material& mat_drewno, Material& mat_swinia, Silnik_Obrazen& silauderzenia);

    GameOver() : title(font), lvl1(font), lvl2(font), Exit(font) {
        
        if (!font.openFromFile("../segoeuib.ttf")) {
            std::cout << "Nie udalo sie wczytac czcionki!\n";
        }
        obraz.setPosition({0.f,0.f});
        obraz.setSize({1920.f,1080.f});
        obraz.setFillColor(sf::Color::Black);
        // Tytul
        title.setFont(font);
        title.setFillColor(sf::Color::Cyan);

        // Lvl 1
        lvl1.setFont(font);
        lvl1.setString("Wybór levelu");
        lvl1.setCharacterSize(50);
        lvl1.setPosition({800.f, 450.f});
        lvl1.setFillColor(sf::Color::Cyan);

        // Ramka lvl 1
        sf::FloatRect zarys_1 = lvl1.getGlobalBounds();
        ramka_lvl1.setSize({zarys_1.size.x + 40.f, zarys_1.size.y + 20.f});
        ramka_lvl1.setPosition({zarys_1.position.x - 20.f, zarys_1.position.y - 10.f});
        ramka_lvl1.setFillColor(sf::Color::Transparent);
        ramka_lvl1.setOutlineColor(sf::Color::Cyan);
        ramka_lvl1.setOutlineThickness(3.f);

        //lvl2
        lvl2.setFont(font);
        lvl2.setString("Main menu");
        lvl2.setCharacterSize(50);
        lvl2.setPosition({800.f, 550.f});
        lvl2.setFillColor(sf::Color::White);

        //ramka lvl2
        sf::FloatRect zarys_2 = lvl2.getGlobalBounds();
        ramka_lvl2.setSize({zarys_2.size.x + 40.f, zarys_2.size.y + 20.f});
        ramka_lvl2.setPosition({zarys_2.position.x - 20.f, zarys_2.position.y - 10.f});
        ramka_lvl2.setFillColor(sf::Color::Transparent);
        ramka_lvl2.setOutlineColor(sf::Color::White);
        ramka_lvl2.setOutlineThickness(3.f);
        
        // exit
        Exit.setFont(font);
        Exit.setString("Exit z gry");
        Exit.setCharacterSize(50);
        Exit.setPosition({800.f, 650.f});
        Exit.setFillColor(sf::Color::Cyan);

        // Ramka exit
        sf::FloatRect zarys_e = Exit.getGlobalBounds();
        ramka_exit.setSize({zarys_e.size.x + 40.f, zarys_e.size.y + 20.f});
        ramka_exit.setPosition({zarys_e.position.x - 20.f, zarys_e.position.y - 10.f});
        ramka_exit.setFillColor(sf::Color::Transparent);
        ramka_exit.setOutlineColor(sf::Color::Cyan);
        ramka_exit.setOutlineThickness(3.f);
    }
void ustawWynik(bool czyWygrana) {
        if (czyWygrana) {
            title.setString(U"BRAWO! Pokonałeś te brudne świnie!");
                    title.setCharacterSize(75);
                    title.setPosition({200.f, 250.f});


        } else {
            title.setString(U"PRZEGRANA! Świnie bezczelnie się z Ciebie śmieją...");
                    title.setCharacterSize(60);
                            title.setPosition({200.f, 250.f});


        }
    }
    void update(sf::Vector2f mousePos) {
        if (!active) return;

        if (ramka_lvl1.getGlobalBounds().contains(mousePos)) {
            ramka_lvl1.setOutlineColor(sf::Color::Magenta);
            lvl1.setFillColor(sf::Color::Magenta);
            ramka_lvl1.setOutlineThickness(5.f);
        } else {
            ramka_lvl1.setOutlineColor(sf::Color::Cyan);
            lvl1.setFillColor(sf::Color::Cyan);
            ramka_lvl1.setOutlineThickness(3.f);
        }

        if (ramka_lvl2.getGlobalBounds().contains(mousePos)) {
            ramka_lvl2.setOutlineColor(sf::Color::Magenta);
            lvl2.setFillColor(sf::Color::Magenta);
            ramka_lvl2.setOutlineThickness(5.f);
        } else {
            ramka_lvl2.setOutlineColor(sf::Color::Cyan);
            lvl2.setFillColor(sf::Color::Cyan);
            ramka_lvl2.setOutlineThickness(3.f);
        }

        if (ramka_exit.getGlobalBounds().contains(mousePos)) {
            ramka_exit.setOutlineColor(sf::Color::Magenta);
            Exit.setFillColor(sf::Color::Magenta);
            ramka_exit.setOutlineThickness(5.f);
        } else {
            ramka_exit.setOutlineColor(sf::Color::Cyan);
            Exit.setFillColor(sf::Color::Cyan);
            ramka_exit.setOutlineThickness(3.f);
        }
    }

    void exitujemy(const sf::Event& event) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                active = false; 
            }
        }
    }

    bool isLvl1Clicked(sf::Vector2f mousePos) {
        return active && ramka_lvl1.getGlobalBounds().contains(mousePos);
    }

    bool isLvl2Clicked(sf::Vector2f mousePos) {
        return active && ramka_lvl2.getGlobalBounds().contains(mousePos);
    }

    bool isExitClicked(sf::Vector2f mousePos) {
        return active && ramka_exit.getGlobalBounds().contains(mousePos);
    }

    void setActive(bool status) { active = status; }
    bool isActive() const { return active; }

    void draw(sf::RenderWindow& window) {
        if (!active) return; 
        window.draw(obraz);
        window.draw(title);
        window.draw(lvl1);
        window.draw(lvl2);
        window.draw(Exit);
        window.draw(ramka_exit);
        window.draw(ramka_lvl1);
        window.draw(ramka_lvl2);
    }
};

#endif