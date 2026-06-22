#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <memory>
#include <random>
#include "guimain.h"
#include "panellvl.h"
#include <cstdlib>
#include "obiekt.h"
#include "silnik_obrazen.h" 
#include "FizycznyObiekt.h"
#include "lvl11.h"          
#include "lvl2.h"
#include "lvl3.h"
#include "GameOver.h"
#include "MenuPauzy.h"

// stany gry zeby bylo latwiej przelaczac ekrany
enum class GameState { MENU, WYBOR_LVL, LEVEL_1, LEVEL_2 , LEVEL_3, GAME_OVER };

int main() {
    // ziarno do losowania
    srand(time(NULL));
    
    // okno w full hd i limit klatek zeby procesor nie odlecial
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Wsciekle Ptaki");
    window.setFramerateLimit(60);

    // deklaracje zmiennych na materialy
    Material p_czerw, p_nieb, p_zolt, p_czar, swinia, swinia_zolnierz, swinia_dziad;
    Material drw_kulka, drw_kwad, drw_belka, drw_troj;
    Material kam_kulka, kam_kwad, kam_belka, kam_troj;
    Material lod_kulka, lod_kwad, lod_belka, lod_troj;

    // ladujemy wszystko raz, zeby potem nie mulilo przy wlaczaniu leveli
    Obiekt::zainicjalizujMaterialy(p_czerw, p_nieb, p_zolt, p_czar, swinia, swinia_zolnierz, swinia_dziad, 
                                   drw_kulka, drw_kwad, drw_belka, drw_troj, 
                                   kam_kulka, kam_kwad, kam_belka, kam_troj, 
                                   lod_kulka, lod_kwad, lod_belka, lod_troj);

    // instancje menu i innych ekranow
    GuiMain gui(p_czerw, p_nieb, p_zolt, p_czar, swinia, drw_kulka, drw_kwad, drw_belka, drw_troj, kam_kulka, kam_kwad, kam_belka, kam_troj, lod_kulka, lod_kwad, lod_belka, lod_troj);
    Panellvl panelLvl;
    MenuPauzy menuPauzy;
    GameOver gameover;
    
    // levele na razie puste, zrobimy new jak gracz kliknie
    Lvl11* lvl11 = nullptr;
    Lvl2* lvl2 = nullptr;
    Lvl3* lvl3 = nullptr;

    // startujemy od glownego menu
    GameState currentState = GameState::MENU;
    Silnik_Obrazen obrazenia; 
    sf::Clock zegar; 

    // glowna petla gry
    while (window.isOpen()) {
        // liczenie czasu do fizyki (zabezpieczenie na 0.1s max)
        float dt = zegar.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        // pozycja myszki zeby wiedziec gdzie gracz klika
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // sprawdzanie co tam gracz wciska
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == GameState::MENU) {
                gui.obslugujZdarzenia(*event, window); 
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
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (panelLvl.isLvl1Clicked(worldPos)) {
                        currentState = GameState::LEVEL_1;
                        panelLvl.setActive(false);
                        if (lvl11 != nullptr) delete lvl11; // sprzatanie pamieci jak gracz gra kolejny raz
                        lvl11 = new Lvl11(p_czerw, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                          drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                          kam_kwad, kam_belka, kam_troj, obrazenia);
                    }
                    else if (panelLvl.isLvl2Clicked(worldPos)) {
                        currentState = GameState::LEVEL_2;
                        panelLvl.setActive(false);
                        if (lvl2 != nullptr) delete lvl2; 
                        
                        // uwaga tu byla sroga sraka z kolejnoscia wiec teraz jest tak jak w lvl2.h:
                        // czerwony, niebieski, zoltek, bomba, 3x swinie, 3x drewno, 3x kamien, 3x lod, obrazenia
                        lvl2 = new Lvl2(p_czerw, p_nieb, p_zolt, p_czar, swinia, swinia_zolnierz, swinia_dziad, 
                                        drw_kwad, drw_belka, drw_troj, kam_kwad, kam_belka, kam_troj, 
                                        lod_kwad, lod_belka, lod_troj, obrazenia);
                    }
                    else if (panelLvl.isLvl3Clicked(worldPos)) {
                        currentState = GameState::LEVEL_3;
                        panelLvl.setActive(false);
                        if (lvl3 != nullptr) delete lvl3; 
                        lvl3 = new Lvl3(p_czerw, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                        drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                        kam_kwad, kam_belka, kam_troj,  obrazenia);
                    }
                    else if (panelLvl.isExitClicked(worldPos)) {
                        panelLvl.setActive(false);
                    }
                }
                // powrot do menu jak wyjdzie z ekranu wyboru
                if (!panelLvl.isActive() && currentState == GameState::WYBOR_LVL) {
                    currentState = GameState::MENU;
                }
            }
            // --- OBSLUGA LEVELI ---
            else if (currentState == GameState::LEVEL_1 && lvl11 != nullptr) {
                if (!menuPauzy.isAktywne()) {
                    lvl11->obslugujZdarzenia(*event, window);
                }

                // escape wlacza pauze
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        menuPauzy.toggle();
                    }
                }

                // co robi gracz w pauzie
                if (event->is<sf::Event::MouseButtonPressed>() && menuPauzy.isAktywne()) {
                    if (menuPauzy.isWznowClicked(worldPos)) menuPauzy.setAktywne(false);
                    else if (menuPauzy.isResetClicked(worldPos)) {
                        delete lvl11;
                        lvl11 = new Lvl11(p_czerw, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                          drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                          kam_kwad, kam_belka, kam_troj, obrazenia);
                        menuPauzy.setAktywne(false);
                    }
                    else if (menuPauzy.isExitClicked(worldPos)) {
                        delete lvl11; lvl11 = nullptr;
                        menuPauzy.setAktywne(false);
                        currentState = GameState::MENU;
                    }
                }
            }
            else if (currentState == GameState::LEVEL_2 && lvl2 != nullptr) {
                if (!menuPauzy.isAktywne()) {
                    lvl2->obslugujZdarzenia(*event, window);
                }

                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        menuPauzy.toggle();
                    }
                }

                if (event->is<sf::Event::MouseButtonPressed>() && menuPauzy.isAktywne()) {
                    if (menuPauzy.isWznowClicked(worldPos)) menuPauzy.setAktywne(false);
                    else if (menuPauzy.isResetClicked(worldPos)) {
                        delete lvl2;
                        lvl2 = new Lvl2(p_czerw, p_nieb, p_zolt, p_czar, swinia, swinia_zolnierz, swinia_dziad, 
                                        drw_kwad, drw_belka, drw_troj, kam_kwad, kam_belka, kam_troj, 
                                        lod_kwad, lod_belka, lod_troj, obrazenia);
                        menuPauzy.setAktywne(false);
                    }
                    else if (menuPauzy.isExitClicked(worldPos)) {
                        delete lvl2; lvl2 = nullptr;
                        menuPauzy.setAktywne(false);
                        currentState = GameState::MENU;
                    }
                }
            }
            else if (currentState == GameState::LEVEL_3 && lvl3 != nullptr) {
                if (!menuPauzy.isAktywne()) {
                    lvl3->obslugujZdarzenia(*event, window);
                }

                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        menuPauzy.toggle();
                    }
                }

                if (event->is<sf::Event::MouseButtonPressed>() && menuPauzy.isAktywne()) {
                    if (menuPauzy.isWznowClicked(worldPos)) menuPauzy.setAktywne(false);
                    else if (menuPauzy.isResetClicked(worldPos)) {
                        delete lvl3;
                        lvl3 = new Lvl3(p_czerw, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                        drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                        kam_kwad, kam_belka, kam_troj, obrazenia);
                        menuPauzy.setAktywne(false);
                    }
                    else if (menuPauzy.isExitClicked(worldPos)) {
                        delete lvl3; lvl3 = nullptr;
                        menuPauzy.setAktywne(false);
                        currentState = GameState::MENU;
                    }
                }
            }
            // obsluga game overa
            else if (currentState == GameState::GAME_OVER) {
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (gameover.isLvl1Clicked(worldPos)) { 
                        gameover.setActive(false);
                        currentState = GameState::WYBOR_LVL;
                        panelLvl.setActive(true);
                    }
                    else if (gameover.isLvl2Clicked(worldPos)) {
                        gameover.setActive(false);
                        currentState = GameState::MENU;
                    }
                    else if (gameover.isExitClicked(worldPos)) {
                        window.close();
                    }
                }
            }
        }

        // --- SEKCJA UPDATE (LOGIKA) ---
        if (currentState == GameState::MENU) {
            gui.update(worldPos, window);
        }
        else if (currentState == GameState::WYBOR_LVL) {
            panelLvl.update(worldPos);
        }
        else if (currentState == GameState::GAME_OVER) {
            gameover.update(worldPos);
        }
        else if (currentState == GameState::LEVEL_3 && lvl3 != nullptr) {
            if (menuPauzy.isAktywne()) {
                menuPauzy.update(worldPos); 
            } else {
                lvl3->update(dt, window, obrazenia);  
                int stan = lvl3->sprawdzStanGry();

                // 1 = wygrana (brak swin), 2 = przegrana (brak ptakow)
                if (stan == 1) { 
                    gameover.ustawWynik(true);          
                    currentState = GameState::GAME_OVER; 
                    gameover.setActive(true); 
                } 
                else if (stan == 2) { 
                    gameover.setActive(true); 
                    gameover.ustawWynik(false);          
                    currentState = GameState::GAME_OVER; 
                }
            }
        }
        else if (currentState == GameState::LEVEL_2 && lvl2 != nullptr) {
            if (menuPauzy.isAktywne()) {
                menuPauzy.update(worldPos); 
            } else {
                lvl2->update(dt, window, obrazenia);  
                int stan = lvl2->sprawdzStanGry();

                if (stan == 1) { 
                    gameover.ustawWynik(true);          
                    currentState = GameState::GAME_OVER; 
                    gameover.setActive(true); 
                } 
                else if (stan == 2) { 
                    gameover.setActive(true); 
                    gameover.ustawWynik(false);          
                    currentState = GameState::GAME_OVER; 
                }
            }
        }
        else if (currentState == GameState::LEVEL_1 && lvl11 != nullptr) {
            if (menuPauzy.isAktywne()) {
                menuPauzy.update(worldPos); 
            } else {
                lvl11->update(dt, window, obrazenia);  
                int stan = lvl11->sprawdzStanGry();

                if (stan == 1) { 
                    gameover.ustawWynik(true);          
                    currentState = GameState::GAME_OVER; 
                    gameover.setActive(true); 
                } 
                else if (stan == 2) { 
                    gameover.setActive(true); 
                    gameover.ustawWynik(false);          
                    currentState = GameState::GAME_OVER; 
                }
            }
        }
    
        // --- SEKCJA RYSOWANIA ---
        window.clear(sf::Color::Cyan); // kolor tla jak nieba nie wczyta
        
        if (currentState == GameState::MENU) {
            gui.draw(window);
        }
        else if (currentState == GameState::WYBOR_LVL) {
            panelLvl.draw(window);
        }
        else if (currentState == GameState::LEVEL_1 && lvl11 != nullptr) {
            lvl11->draw(window, dt); 
            menuPauzy.draw(window); 
        }
        else if (currentState == GameState::LEVEL_2 && lvl2 != nullptr) {
            lvl2->draw(window, dt); 
            menuPauzy.draw(window); 
        }
        else if (currentState == GameState::LEVEL_3 && lvl3 != nullptr) {
            lvl3->draw(window, dt); 
            menuPauzy.draw(window); 
        }
        else if (currentState == GameState::GAME_OVER) {
            gameover.draw(window);
        }
        window.display();
    }
    
    // sprzatanie pamieci przy zamykaniu
    if (lvl11 != nullptr) delete lvl11;
    if (lvl2 != nullptr) delete lvl2;
    if (lvl3 != nullptr) delete lvl3;
    
    return 0;
}