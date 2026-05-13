#ifndef GUIMAIN_H
#define GUIMAIN_H

#include <SFML/Graphics.hpp>
#include <iostream>

class GuiMain {
private:
    sf::Font font;
    sf::Text title;
    sf::Text btnExit;
    sf::Text btnLvl;
    sf::RectangleShape ramka_lvl;
    sf::RectangleShape ramka_exit;

public:
    GuiMain() : title(font), btnExit(font), btnLvl(font) {
        if (!font.openFromFile("../arial.ttf")) {
            std::cout << "Nie udalo sie wczytac czcionki!\n";
        }

        // Tytul
        title.setFont(font);
        title.setString(U"WŚCIEKŁE PTAKI");
        title.setCharacterSize(75);
        title.setPosition({700.f, 300.f});
        title.setFillColor(sf::Color::Red);


        //Przycisk levele 
        btnLvl.setFont(font);
        btnLvl.setString("2.WYBIERZ LEVELE");
        btnLvl.setCharacterSize(30);
        btnLvl.setPosition({860.f, 450.f});
        btnLvl.setFillColor(sf::Color::White);

        //ramka levele
         sf::FloatRect zarys_levele = btnLvl.getGlobalBounds();
        ramka_lvl.setSize({zarys_levele.size.x + 20.f, zarys_levele.size.y + 20.f});
        ramka_lvl.setPosition({zarys_levele.position.x - 10.f, zarys_levele.position.y - 10.f});
        ramka_lvl.setFillColor(sf::Color::Transparent);
        ramka_lvl.setOutlineColor(sf::Color::White);
        ramka_lvl.setOutlineThickness(3.f);

        // Wyjdz
        btnExit.setFont(font);
        btnExit.setString("3. WYJDZ");
        btnExit.setCharacterSize(30);
        btnExit.setPosition({860.f, 550.f});
        btnExit.setFillColor(sf::Color::White);

        sf::FloatRect zarys_exit = btnExit.getGlobalBounds();
        ramka_exit.setSize({zarys_exit.size.x + 20.f, zarys_exit.size.y + 20.f});
        ramka_exit.setPosition({zarys_exit.position.x - 10.f, zarys_exit.position.y - 10.f});
        ramka_exit.setFillColor(sf::Color::Transparent);
        ramka_exit.setOutlineColor(sf::Color::White);
        ramka_exit.setOutlineThickness(3.f);
    }

    //podswietlanie przycisków
    void update(sf::Vector2f mousePos) {
 
        if (ramka_exit.getGlobalBounds().contains(mousePos)) {
            ramka_exit.setOutlineColor(sf::Color::Yellow);
            btnExit.setFillColor(sf::Color::Yellow);
                    ramka_exit.setOutlineThickness(5.f);

        } else {
            ramka_exit.setOutlineColor(sf::Color::White);
            btnExit.setFillColor(sf::Color::White);
                    ramka_exit.setOutlineThickness(3.f);

        }
        if(ramka_lvl.getGlobalBounds().contains(mousePos))
        {
             ramka_lvl.setOutlineColor(sf::Color::Yellow);
            btnLvl.setFillColor(sf::Color::Yellow);
            ramka_lvl.setOutlineThickness(5.f);

        }
        else {
            ramka_lvl.setOutlineColor(sf::Color::White);
            btnLvl.setFillColor(sf::Color::White);
            ramka_lvl.setOutlineThickness(3.f);

        }
        
    }

    // Sprawdza kliknięcie

    bool isExitClicked(sf::Vector2f mousePos) {
        return ramka_exit.getGlobalBounds().contains(mousePos);
    }
     bool isLvlClicked(sf::Vector2f mousePos) {
        return ramka_lvl.getGlobalBounds().contains(mousePos);
    }
   


    bool LvlPanel(sf::Vector2f mousePos){
    if(isLvlClicked(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        return true;
    }
    else 
    {
        return false;
    }
    }
    bool ExitPanel(sf::Vector2f mousePos){
        if(isExitClicked(mousePos)&& sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
    

    void draw(sf::RenderWindow& window) {
        window.draw(title);
        window.draw(btnExit);
        window.draw(btnLvl);
        window.draw(ramka_lvl);
        window.draw(ramka_exit);
       
    }
};

#endif