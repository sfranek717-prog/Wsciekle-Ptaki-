#ifndef MENUPAUZY_H
#define MENUPAUZY_H

#include <SFML/Graphics.hpp>
#include <iostream>

class MenuPauzy {
private:
    bool aktywne = false; // czy w ogole wyswietlac menu
    sf::Font font;        // czcionka do napisow
    
    // ramki pod napisy zeby to jako tako wygladalo
    sf::RectangleShape ramka_exit, ramka_kontynuacja, ramka_reset;
    sf::Text wznow, reset, exit_txt;

public:
    MenuPauzy() : wznow(font), reset(font), exit_txt(font) {
        // wczytywanie czcionki z pliku
        if (!font.openFromFile("../segoeuib.ttf")) {
            std::cout << "Nie udalo sie wczytac czcionki w MenuPauzy!\n";
        }

        // ustawienia dla przycisku wznowienia gry
        wznow.setFont(font); wznow.setString(U"Wznów"); wznow.setCharacterSize(75); wznow.setPosition({740.f, 300.f});
        wznow.setFillColor(sf::Color::White);
        sf::FloatRect zarys_1 = wznow.getGlobalBounds();
        ramka_kontynuacja.setSize({zarys_1.size.x + 40.f, zarys_1.size.y + 20.f}); // marginesy zeby tekst nie wystawal
        ramka_kontynuacja.setPosition({zarys_1.position.x - 20.f, zarys_1.position.y - 10.f});
        ramka_kontynuacja.setFillColor(sf::Color(0, 0, 0, 150)); // czarne polprzezroczyste tlo
        ramka_kontynuacja.setOutlineColor(sf::Color::White);
        ramka_kontynuacja.setOutlineThickness(3.f);

        // ustawienia dla przycisku restartu levelu
        reset.setFont(font); reset.setString(U"Restart levelu"); reset.setCharacterSize(75); reset.setPosition({740.f, 500.f});
        reset.setFillColor(sf::Color::White);
        sf::FloatRect zarys_3 = reset.getGlobalBounds();
        ramka_reset.setSize({zarys_3.size.x + 40.f, zarys_3.size.y + 20.f});
        ramka_reset.setPosition({zarys_3.position.x - 20.f, zarys_3.position.y - 10.f});
        ramka_reset.setFillColor(sf::Color(0, 0, 0, 150));
        ramka_reset.setOutlineColor(sf::Color::White);
        ramka_reset.setOutlineThickness(3.f);

        // ustawienia dla przycisku powrotu do menu
        exit_txt.setFont(font); exit_txt.setString(U"Menu Główne"); exit_txt.setCharacterSize(75); exit_txt.setPosition({740.f, 700.f});
        exit_txt.setFillColor(sf::Color::White);
        sf::FloatRect zarys_2 = exit_txt.getGlobalBounds();
        ramka_exit.setSize({zarys_2.size.x + 40.f, zarys_2.size.y + 20.f});
        ramka_exit.setPosition({zarys_2.position.x - 20.f, zarys_2.position.y - 10.f});
        ramka_exit.setFillColor(sf::Color(0, 0, 0, 150));
        ramka_exit.setOutlineColor(sf::Color::White);
        ramka_exit.setOutlineThickness(3.f);
    }

    // gettery i settery do stanu pauzy
    bool isAktywne() const { return aktywne; }
    void setAktywne(bool stan) { aktywne = stan; }
    void toggle() { aktywne = !aktywne; } // do szybkiego przelaczania escem

    // sprawdzanie czy myszka najechala na ramki przyciskow
    bool isWznowClicked(sf::Vector2f mousePos) { return aktywne && ramka_kontynuacja.getGlobalBounds().contains(mousePos); }
    bool isResetClicked(sf::Vector2f mousePos) { return aktywne && ramka_reset.getGlobalBounds().contains(mousePos); }
    bool isExitClicked(sf::Vector2f mousePos) { return aktywne && ramka_exit.getGlobalBounds().contains(mousePos); }

    void update(sf::Vector2f mousePos) {
        if (!aktywne) return; // jak menu jest wylaczone to nie sprawdzaj podswietlenia

        // funkcja pomocnicza zeby nie pisac 3 razy tego samego if-else
        auto hoverEffect = [&](sf::RectangleShape& ramka, sf::Text& tekst) {
            if (ramka.getGlobalBounds().contains(mousePos)) {
                ramka.setOutlineColor(sf::Color::Yellow); // zmiana koloru na zolty po najechaniu myszka
                tekst.setFillColor(sf::Color::Yellow);
            } else {
                ramka.setOutlineColor(sf::Color::White);  // powrot do bialego jak myszka ucieka
                tekst.setFillColor(sf::Color::White);
            }
        };

        // odpalenie efektu dla kazdego przycisku
        hoverEffect(ramka_kontynuacja, wznow);
        hoverEffect(ramka_reset, reset);
        hoverEffect(ramka_exit, exit_txt);
    }

    void draw(sf::RenderWindow& window) {
        if (!aktywne) return; // ukryte to nic nie rysujemy
        
        // najpierw tlo potem tekst zeby napisy byly na wierzchu
        window.draw(ramka_exit);
        window.draw(ramka_kontynuacja);
        window.draw(exit_txt);
        window.draw(wznow);
        window.draw(ramka_reset);
        window.draw(reset);
    }
};

#endif