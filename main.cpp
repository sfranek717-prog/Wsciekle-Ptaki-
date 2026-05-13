#include <SFML/Graphics.hpp>
#include <iostream>
#include "guimain.h"
#include "panellvl.h"

enum class GameState { MENU, WYBOR_LVL, GRA };

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Wsciekle Ptaki");
    window.setFramerateLimit(60);

    GuiMain gui;
    Panellvl panelLvl;
    //Game state
    GameState currentState = GameState::MENU;

    while (window.isOpen()) {
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            // obsluga zdarzen
            if (currentState == GameState::MENU) {
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (gui.ExitPanel(worldPos)) window.close();
                    
                    if (gui.LvlPanel(worldPos)) {
                        currentState = GameState::WYBOR_LVL;
                        panelLvl.setActive(true);
                    }
                }
            } 
            else if (currentState == GameState::WYBOR_LVL) {
                panelLvl.exitujemy(*event); 
                if (!panelLvl.isActive()) {
                    currentState = GameState::MENU; // Powrót do menu
                }
            }
        }

        // update
        if (currentState == GameState::MENU) gui.update(worldPos);

        //rysowanie 
        window.clear(sf::Color::Cyan); 
        
        if (currentState == GameState::MENU) {
            gui.draw(window);
        } else if (currentState == GameState::WYBOR_LVL) {
            panelLvl.draw(window);
        }
        
        window.display();
    }
    return 0;
}