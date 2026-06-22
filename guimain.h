#ifndef GUIMAIN_H
#define GUIMAIN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "obiekt.h"

class GuiMain {
private:
    sf::Texture p;
    sf::Image img;
    sf::Texture g;        // tekstura tla menu glownego
    sf::Font font;        // glowna czcionka menu
    sf::Text title;       // wielki napis tytulowy gry
    sf::Text btnExit;     // napis wyjscia
    sf::Text btnLvl;      // napis wyboru poziomow
    sf::RectangleShape obraz; // prostokat pod caloekranowe tlo
    sf::RectangleShape ramka_lvl;
    sf::RectangleShape ramka_exit;

    // dekoracyjne ptaki wyswietlane w menu glownym
    sf::CircleShape Ptaszor1;
    sf::CircleShape Ptaszor2{40.f, 3}; // trojkatny zoltek
    sf::CircleShape Ptaszor3;

public:

    GuiMain(
        const Material& ptak_czerw, const Material& ptak_nieb, const Material& ptak_zolt, const Material& ptak_czar,
        const Material& swinia,
        const Material& drw_kulka, const Material& drw_kwad, const Material& drw_belka, const Material& drw_troj,
        const Material& kam_kulka, const Material& kam_kwad, const Material& kam_belka, const Material& kam_troj,
        const Material& lod_kulka, const Material& lod_kwad, const Material& lod_belka, const Material& lod_troj
    ) : title(font), btnExit(font), btnLvl(font) {
        // ladowanie czcionki z pliku projektowego
        if (!font.openFromFile("../segoeuib.ttf")) {
            std::cout << "Nie udalo sie wczytac czcionki!\n";
        }

        // ustawianie grafiki tła na pelen ekran
        obraz.setPosition({0.f, 0.f});
        obraz.setSize({1920.f, 1080.f});
        g.loadFromFile("../tekstury/mainmenu.png");
        obraz.setTexture(&g);

        // konfiguracja i stylizowanie glównego tytulu
        title.setFont(font);
        title.setString(U"WŚCIEKŁE PTAKI");
        title.setCharacterSize(90);
        title.setPosition({620.f, 120.f});
        title.setFillColor(sf::Color::Red);
        title.setOutlineThickness(5.f);
        title.setOutlineColor(sf::Color::Black);
        title.setStyle(sf::Text::Bold);

        // przycisk wejscia do menu poziomow
        btnLvl.setFont(font);
        btnLvl.setString(U"WYBIERZ LEVELE");
        btnLvl.setCharacterSize(42);
        btnLvl.setStyle(sf::Text::Bold);
        btnLvl.setPosition({760.f, 360.f});
        btnLvl.setFillColor(sf::Color::White);

        // robienie polprzezroczystej ramki z marginesem wokól tekstu poziomow
        sf::FloatRect zarys_levele = btnLvl.getGlobalBounds();
        ramka_lvl.setSize({zarys_levele.size.x + 40.f, zarys_levele.size.y + 40.f});
        ramka_lvl.setPosition({zarys_levele.position.x - 20.f, zarys_levele.position.y - 20.f});
        ramka_lvl.setFillColor(sf::Color(0, 0, 0, 80));
        ramka_lvl.setOutlineColor(sf::Color::White);
        ramka_lvl.setOutlineThickness(5.f);

        // przycisk do zamykania gry
        btnExit.setFont(font);
        btnExit.setString(U"WYJDŹ");
        btnExit.setCharacterSize(42);
        btnExit.setStyle(sf::Text::Bold);
        btnExit.setPosition({850.f, 500.f});
        btnExit.setFillColor(sf::Color::White);

        // ramka pod przycisk wyjscia
        sf::FloatRect zarys_exit = btnExit.getGlobalBounds();
        ramka_exit.setSize({zarys_exit.size.x + 40.f, zarys_exit.size.y + 40.f});
        ramka_exit.setPosition({zarys_exit.position.x - 20.f, zarys_exit.position.y - 20.f});
        ramka_exit.setFillColor(sf::Color(0, 0, 0, 80));
        ramka_exit.setOutlineColor(sf::Color::White);
        ramka_exit.setOutlineThickness(5.f);

        // nakladanie tekstur z materialow na ptaki dekoracyjne w rogu
        Ptaszor1.setRadius(45.f);
        Ptaszor1.setPosition({40.f, 780.f});
        Ptaszor1.setTexture(&ptak_czar.tekstura);

        Ptaszor2.setPosition({280.f, 800.f});
        Ptaszor2.setTexture(&ptak_zolt.tekstura);

        Ptaszor3.setRadius(40.f);
        Ptaszor3.setPosition({140.f, 790.f});
        Ptaszor3.setTexture(&ptak_czerw.tekstura);
    }

    void update(sf::Vector2f mousePos, sf::RenderWindow& window) {

        // efekt najechania na przycisk wyjdź (zmiana koloru i lekkie powiekszenie)
        if (ramka_exit.getGlobalBounds().contains(mousePos)) {
            ramka_exit.setOutlineColor(sf::Color::Yellow);
            btnExit.setFillColor(sf::Color::Yellow);
            ramka_exit.setOutlineThickness(6.f);
            ramka_exit.setScale({1.03f, 1.03f});
        } else {
            ramka_exit.setOutlineColor(sf::Color::White);
            btnExit.setFillColor(sf::Color::White);
            ramka_exit.setOutlineThickness(5.f);
            ramka_exit.setScale({1.f, 1.f});
        }

        // efekt najechania na przycisk wyboru levelu
        if (ramka_lvl.getGlobalBounds().contains(mousePos)) {
            ramka_lvl.setOutlineColor(sf::Color::Yellow);
            btnLvl.setFillColor(sf::Color::Yellow);
            ramka_lvl.setOutlineThickness(6.f);
            ramka_lvl.setScale({1.03f, 1.03f});
        } else {
            ramka_lvl.setOutlineColor(sf::Color::White);
            btnLvl.setFillColor(sf::Color::White);
            ramka_lvl.setOutlineThickness(5.f);
            ramka_lvl.setScale({1.f, 1.f});
        }
    }

    void obslugujZdarzenia(const sf::Event& event, sf::RenderWindow& window) {
        
    }

    // podstawowe gettery do sprawdzania pozycji myszki
    bool isExitClicked(sf::Vector2f mousePos) { return ramka_exit.getGlobalBounds().contains(mousePos); }
    bool isLvlClicked(sf::Vector2f mousePos) { return ramka_lvl.getGlobalBounds().contains(mousePos); }
    
    // sprawdzanie jednoczesnego najechania i klikniecia lewym przyciskiem myszy
    bool LvlPanel(sf::Vector2f mousePos) { return isLvlClicked(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); }
    bool ExitPanel(sf::Vector2f mousePos) { return isExitClicked(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); }

    void draw(sf::RenderWindow& window) {
        // najpierw caly podklad, potem UI i na koncu dekoracje
        window.draw(obraz);
        window.draw(title);

        window.draw(ramka_lvl);
        window.draw(ramka_exit);
        window.draw(btnExit);
        window.draw(btnLvl);

        window.draw(Ptaszor1);
        window.draw(Ptaszor2);
        window.draw(Ptaszor3);
    }
};

#endif