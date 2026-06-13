#ifndef MENUPAUZY_H
#define MENUPAUZY_H

#include <SFML/Graphics.hpp>
#include <iostream>

class MenuPauzy {
private:
    bool aktywne = false;
    sf::Font font;
    sf::RectangleShape ramka_exit, ramka_kontynuacja, ramka_reset;
    sf::Text wznow, reset, exit_txt;

public:
    MenuPauzy() : wznow(font), reset(font),exit_txt(font){
        if (!font.openFromFile("../segoeuib.ttf")) {
            std::cout << "Nie udalo sie wczytac czcionki w MenuPauzy!\n";
        }

        wznow.setFont(font); wznow.setString(U"Wznów"); wznow.setCharacterSize(75); wznow.setPosition({740.f, 300.f});
        wznow.setFillColor(sf::Color::White);
        sf::FloatRect zarys_1 = wznow.getGlobalBounds();
        ramka_kontynuacja.setSize({zarys_1.size.x + 40.f, zarys_1.size.y + 20.f});
        ramka_kontynuacja.setPosition({zarys_1.position.x - 20.f, zarys_1.position.y - 10.f});
        ramka_kontynuacja.setFillColor(sf::Color(0, 0, 0, 150));
        ramka_kontynuacja.setOutlineColor(sf::Color::White);
        ramka_kontynuacja.setOutlineThickness(3.f);

        reset.setFont(font); reset.setString(U"Restart levelu"); reset.setCharacterSize(75); reset.setPosition({740.f, 500.f});
        reset.setFillColor(sf::Color::White);
        sf::FloatRect zarys_3 = reset.getGlobalBounds();
        ramka_reset.setSize({zarys_3.size.x + 40.f, zarys_3.size.y + 20.f});
        ramka_reset.setPosition({zarys_3.position.x - 20.f, zarys_3.position.y - 10.f});
        ramka_reset.setFillColor(sf::Color(0, 0, 0, 150));
        ramka_reset.setOutlineColor(sf::Color::White);
        ramka_reset.setOutlineThickness(3.f);

        exit_txt.setFont(font); exit_txt.setString(U"Menu Główne"); exit_txt.setCharacterSize(75); exit_txt.setPosition({740.f, 700.f});
        exit_txt.setFillColor(sf::Color::White);
        sf::FloatRect zarys_2 = exit_txt.getGlobalBounds();
        ramka_exit.setSize({zarys_2.size.x + 40.f, zarys_2.size.y + 20.f});
        ramka_exit.setPosition({zarys_2.position.x - 20.f, zarys_2.position.y - 10.f});
        ramka_exit.setFillColor(sf::Color(0, 0, 0, 150));
        ramka_exit.setOutlineColor(sf::Color::White);
        ramka_exit.setOutlineThickness(3.f);
    }
//zarzadzanie stanem
    bool isAktywne() const { return aktywne; }
    void setAktywne(bool stan) { aktywne = stan; }
    void toggle() { aktywne = !aktywne; }
//sprawdzenie klikniec 
    bool isWznowClicked(sf::Vector2f mousePos) { return aktywne && ramka_kontynuacja.getGlobalBounds().contains(mousePos); }
    bool isResetClicked(sf::Vector2f mousePos) { return aktywne && ramka_reset.getGlobalBounds().contains(mousePos); }
    bool isExitClicked(sf::Vector2f mousePos) { return aktywne && ramka_exit.getGlobalBounds().contains(mousePos); }

    void update(sf::Vector2f mousePos) {
        if (!aktywne) return;

        auto hoverEffect = [&](sf::RectangleShape& ramka, sf::Text& tekst) {
            if (ramka.getGlobalBounds().contains(mousePos)) {
                ramka.setOutlineColor(sf::Color::Yellow);
                tekst.setFillColor(sf::Color::Yellow);
            } else {
                ramka.setOutlineColor(sf::Color::White);
                tekst.setFillColor(sf::Color::White);
            }
        };

        hoverEffect(ramka_kontynuacja, wznow);
        hoverEffect(ramka_reset, reset);
        hoverEffect(ramka_exit, exit_txt);
    }

    void draw(sf::RenderWindow& window) {
        if (!aktywne) return;
        window.draw(ramka_exit);
        window.draw(ramka_kontynuacja);
        window.draw(exit_txt);
        window.draw(wznow);
        window.draw(ramka_reset);
        window.draw(reset);
    }
};

#endif