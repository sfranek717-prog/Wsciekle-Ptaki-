#ifndef PANELLVL_H
#define PANELLVL_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Panellvl {
private:
    bool active = false; // czy panel jest teraz wyswietlany
    sf::Font font;       // czcionka do wszystkich napisow
    sf::Text title;      // napis glowny menu
    sf::Text lvl1;
    sf::Text lvl2;
     sf::Text lvl3;
    sf::Text Exit;       // napis wyjscia/powrotu
    sf::Texture tlo;     // obrazek tła wczytany z pliku

    // ramki pod napisy zeby bylo w co klikac
    sf::RectangleShape ramka_exit;
    sf::RectangleShape ramka_lvl1;
    sf::RectangleShape ramka_lvl2;
    sf::RectangleShape ramka_lvl3;
    sf::RectangleShape obraz; // prostokat na ktory nakladamy teksture tła

public: 
    Panellvl() : title(font), lvl1(font), lvl2(font),lvl3(font), Exit(font) {
        // wczytywanie czcionki z folderu projektu
        if (!font.openFromFile("../segoeuib.ttf")) {
            std::cout << "Nie udalo sie wczytac czcionki!\n";
        }
        // ladowanie i ustawianie tła na caly ekran
        tlo.loadFromFile("../tekstury/tlo.jpg");
        tlo.setSmooth(true);
        obraz.setPosition({0.f,0.f});
        obraz.setSize({1920.f,1080.f});
        obraz.setTexture(&tlo);
        
        // konfiguracja napisu tytulowego
        title.setFont(font);
        title.setString(U"WYBIERZ LEVEL");
        title.setCharacterSize(75);
        title.setPosition({680.f, 100.f});
        title.setFillColor(sf::Color::Cyan);

        // konfiguracja tekstu dla levelu 1
        lvl1.setFont(font);
        lvl1.setString("LEVEL 1");
        lvl1.setCharacterSize(50);
        lvl1.setPosition({800.f, 250.f});
        lvl1.setFillColor(sf::Color::White);

        // robienie ramki wokol tekstu levelu 1
        sf::FloatRect zarys_1 = lvl1.getGlobalBounds();
        ramka_lvl1.setSize({zarys_1.size.x + 40.f, zarys_1.size.y + 20.f});
        ramka_lvl1.setPosition({zarys_1.position.x - 20.f, zarys_1.position.y - 10.f});
        ramka_lvl1.setFillColor(sf::Color::Transparent);
        ramka_lvl1.setOutlineColor(sf::Color::White);
        ramka_lvl1.setOutlineThickness(3.f);

        // konfiguracja tekstu dla levelu 2
        lvl2.setFont(font);
        lvl2.setString("LEVEL 2");
        lvl2.setCharacterSize(50);
        lvl2.setPosition({800.f, 450.f});
        lvl2.setFillColor(sf::Color::White);

        // ramka dla levelu 2
        sf::FloatRect zarys_2 = lvl2.getGlobalBounds();
        ramka_lvl2.setSize({zarys_2.size.x + 40.f, zarys_2.size.y + 20.f});
        ramka_lvl2.setPosition({zarys_2.position.x - 20.f, zarys_2.position.y - 10.f});
        ramka_lvl2.setFillColor(sf::Color::Transparent);
        ramka_lvl2.setOutlineColor(sf::Color::Cyan);
        ramka_lvl2.setOutlineThickness(3.f);

        // konfiguracja tekstu dla levelu 3
        lvl3.setFont(font);
        lvl3.setString("LEVEL 3");
        lvl3.setCharacterSize(50);
        lvl3.setPosition({800.f, 650.f});
        lvl3.setFillColor(sf::Color::White);

        // ramka dla levelu 3
        sf::FloatRect zarys_3 = lvl3.getGlobalBounds();
        ramka_lvl3.setSize({zarys_3.size.x + 40.f, zarys_3.size.y + 20.f});
        ramka_lvl3.setPosition({zarys_3.position.x - 20.f, zarys_3.position.y - 10.f});
        ramka_lvl3.setFillColor(sf::Color::Transparent);
        ramka_lvl3.setOutlineColor(sf::Color::Cyan);
        ramka_lvl3.setOutlineThickness(3.f);

        // konfiguracja przycisku powrotu
        Exit.setFont(font);
        Exit.setString("POWROT");
        Exit.setCharacterSize(50);
        Exit.setPosition({800.f, 850.f});
        Exit.setFillColor(sf::Color::Cyan);

        // ramka dla przycisku powrotu
        sf::FloatRect zarys_e = Exit.getGlobalBounds();
        ramka_exit.setSize({zarys_e.size.x + 40.f, zarys_e.size.y + 20.f});
        ramka_exit.setPosition({zarys_e.position.x - 20.f, zarys_e.position.y - 10.f});
        ramka_exit.setFillColor(sf::Color::Transparent);
        ramka_exit.setOutlineColor(sf::Color::Cyan);
        ramka_exit.setOutlineThickness(3.f);
    }

    void update(sf::Vector2f mousePos) {
        if (!active) return; // jak panel jest wylaczony to nic nie sprawdzaj

        // podswietlanie na fioletowo po najechaniu myszka na lvl 1
        if (ramka_lvl1.getGlobalBounds().contains(mousePos)) {
            ramka_lvl1.setOutlineColor(sf::Color::Magenta);
            lvl1.setFillColor(sf::Color::Magenta);
            ramka_lvl1.setOutlineThickness(5.f);
        } else {
            ramka_lvl1.setOutlineColor(sf::Color::Cyan);
            lvl1.setFillColor(sf::Color::Cyan);
            ramka_lvl1.setOutlineThickness(3.f);
        }

        // podswietlanie po najechaniu na lvl 2
        if (ramka_lvl2.getGlobalBounds().contains(mousePos)) {
            ramka_lvl2.setOutlineColor(sf::Color::Magenta);
            lvl2.setFillColor(sf::Color::Magenta);
            ramka_lvl2.setOutlineThickness(5.f);
        } else {
            ramka_lvl2.setOutlineColor(sf::Color::Cyan);
            lvl2.setFillColor(sf::Color::Cyan);
            ramka_lvl2.setOutlineThickness(3.f);
        }
        
        // podswietlanie po najechaniu na lvl 3
        if (ramka_lvl3.getGlobalBounds().contains(mousePos)) {
            ramka_lvl3.setOutlineColor(sf::Color::Magenta);
            lvl3.setFillColor(sf::Color::Magenta);
            ramka_lvl3.setOutlineThickness(5.f);
        } else {
            ramka_lvl3.setOutlineColor(sf::Color::Cyan);
            lvl3.setFillColor(sf::Color::Cyan);
            ramka_lvl3.setOutlineThickness(3.f);
        }

        // podswietlanie po najechaniu na powrot
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

    // wylaczanie panelu po nacisnieciu escape
    void exitujemy(const sf::Event& event) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                active = false; 
            }
        }
    }

    // metody do sprawdzania klikniec myszki w petli glownej gry
    bool isLvl1Clicked(sf::Vector2f mousePos) {
        return active && ramka_lvl1.getGlobalBounds().contains(mousePos);
    }

    bool isLvl2Clicked(sf::Vector2f mousePos) {
        return active && ramka_lvl2.getGlobalBounds().contains(mousePos);
    }
     bool isLvl3Clicked(sf::Vector2f mousePos) {
        return active && ramka_lvl3.getGlobalBounds().contains(mousePos);
    }

    bool isExitClicked(sf::Vector2f mousePos) {
        return active && ramka_exit.getGlobalBounds().contains(mousePos);
    }

    // gettery i settery do widocznosci panelu
    void setActive(bool status) { active = status; }
    bool isActive() const { return active; }

    void draw(sf::RenderWindow& window) {
        if (!active) return; // jak ukryte to nie rysujemy niczego
        
        // najpierw rysujemy tlo, potem teksty i ramki zeby nic nie zniknelo pod spodem
        window.draw(obraz);
        window.draw(title);
        window.draw(lvl1);
        window.draw(lvl2);
        window.draw(lvl3);
        window.draw(Exit);
        window.draw(ramka_exit);
        window.draw(ramka_lvl1);
        window.draw(ramka_lvl2);
        window.draw(ramka_lvl3);
    }
};

#endif