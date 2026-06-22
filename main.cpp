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

// Stany gry, żeby łatwo zarządzać ekranami
enum class GameState { MENU, WYBOR_LVL, LEVEL_1, LEVEL_2 , LEVEL_3, GAME_OVER };

int main() {
    // Inicjalizacja generatora liczb losowych
    srand(time(NULL));
    
    // Tworzenie głównego okna aplikacji (Full HD, 60 FPS)
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Wsciekle Ptaki");
    window.setFramerateLimit(60);

    // Deklaracja materiałów dla ptaków, świń oraz bloków konstrukcyjnych
    Material p_czerw, p_nieb, p_zolt, p_czar, swinia, swinia_zolnierz, swinia_dziad;
    Material drw_kulka, drw_kwad, drw_belka, drw_troj;
    Material kam_kulka, kam_kwad, kam_belka, kam_troj;
    Material lod_kulka, lod_kwad, lod_belka, lod_troj;

    // Ładowanie tekstur i parametrów fizycznych do materiałów
    Obiekt::zainicjalizujMaterialy(p_czerw, p_nieb, p_zolt, p_czar, swinia, swinia_zolnierz, swinia_dziad, 
                                   drw_kulka, drw_kwad, drw_belka, drw_troj, 
                                   kam_kulka, kam_kwad, kam_belka, kam_troj, 
                                   lod_kulka, lod_kwad, lod_belka, lod_troj);

    // Inicjalizacja obiektów odpowiedzialnych za UI i ekrany menu
    GuiMain gui(p_czerw, p_nieb, p_zolt, p_czar, swinia, drw_kulka, drw_kwad, drw_belka, drw_troj, kam_kulka, kam_kwad, kam_belka, kam_troj, lod_kulka, lod_kwad, lod_belka, lod_troj);
    Panellvl panelLvl;
    MenuPauzy menuPauzy;
    GameOver gameover;
    
    // Wskaźniki na poziomy gry (inicjalizowane dynamicznie przez 'new')
    Lvl11* lvl11 = nullptr;
    Lvl2* lvl2 = nullptr;
    Lvl3* lvl3 = nullptr;

    // Początkowy stan gry to menu główne
    GameState currentState = GameState::MENU;
    Silnik_Obrazen obrazenia; 
    sf::Clock zegar; 

    // Główna pętla gry
    while (window.isOpen()) {
        // Obliczanie czasu delta (dt) dla płynnej fizyki i animacji
        float dt = zegar.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f; // Zabezpieczenie przed nagłym lagiem komputera

        // Pobranie aktualnej pozycji myszki przeliczonej na współrzędne świata gry
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Obsługa zdarzeń systemu i okna (klawiatura, myszka)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            // MENU GŁÓWNE
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
            // MENU WYBORU POZIOMU
            else if (currentState == GameState::WYBOR_LVL) {
                panelLvl.exitujemy(*event); 
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    // Ładowanie Poziomu 1
                    if (panelLvl.isLvl1Clicked(worldPos)) {
                        currentState = GameState::LEVEL_1;
                        panelLvl.setActive(false);
                        if (lvl11 != nullptr) delete lvl11; 
                        lvl11 = new Lvl11(p_czerw, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                          drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                          kam_kwad, kam_belka, kam_troj, obrazenia);
                    }
                    // Ładowanie Poziomu 2
                    else if (panelLvl.isLvl2Clicked(worldPos)) {
                        currentState = GameState::LEVEL_2;
                        panelLvl.setActive(false);
                        if (lvl2 != nullptr) delete lvl2; 
                        // POPRAWIONE: Dopasowano argumenty do nowego konstruktora Lvl2 (dokładnie 17 argumentów)
                        lvl2 = new Lvl2(p_czerw, p_nieb, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                        drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                        kam_kwad, kam_belka, kam_troj,  obrazenia);
                    }
                    // Ładowanie Poziomu 3
                    else if (panelLvl.isLvl3Clicked(worldPos)) {
                        currentState = GameState::LEVEL_3;
                        panelLvl.setActive(false);
                        if (lvl3 != nullptr) delete lvl3; 
                        // POPRAWIONE: Dopasowano argumenty do nowego konstruktora Lvl3 tak jak w Lvl2
                        lvl3 = new Lvl3(p_czerw, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                        drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                        kam_kwad, kam_belka, kam_troj,  obrazenia);
                    }
                    else if (panelLvl.isExitClicked(worldPos)) {
                        panelLvl.setActive(false);
                    }
                }
                if (!panelLvl.isActive() && currentState == GameState::WYBOR_LVL) {
                    currentState = GameState::MENU;
                }
            }
            // LOGIKA I STRZAŁY W POZIOMIE 1
            else if (currentState == GameState::LEVEL_1 && lvl11 != nullptr) {
                if (!menuPauzy.isAktywne()) {
                    lvl11->obslugujZdarzenia(*event, window);
                }

                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        menuPauzy.toggle();
                    }
                }

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
            // LOGIKA I STRZAŁY W POZIOMIE 2
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
                        lvl2 = new Lvl2(p_czerw, p_czar, p_zolt, swinia, swinia_zolnierz, swinia_dziad, 
                                        drw_kwad, drw_belka, drw_troj, lod_kwad, lod_belka, lod_troj, 
                                        kam_kwad, kam_belka, kam_troj, p_nieb, obrazenia);
                        menuPauzy.setAktywne(false);
                    }
                    else if (menuPauzy.isExitClicked(worldPos)) {
                        delete lvl2; lvl2 = nullptr;
                        menuPauzy.setAktywne(false);
                        currentState = GameState::MENU;
                    }
                }
            }
            // LOGIKA I STRZAŁY W POZIOMIE 3
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
            // EKRAN KOŃCA GRY (GAME OVER / WYGRANA)
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

        // SEKCJA UPDATE - AKTUALIZACJA LOGIKI ŚWIATA I POZYCJI MYSZY
        if (currentState == GameState::MENU) {
            gui.update(worldPos, window);
        }
        else if (currentState == GameState::WYBOR_LVL) {
            panelLvl.update(worldPos);
        }
        else if (currentState == GameState::GAME_OVER) {
            gameover.update(worldPos);
        }
        // Aktualizacja i sprawdzanie wygranej/przegranej dla Lvl 3
        else if (currentState == GameState::LEVEL_3 && lvl3 != nullptr) {
            if (menuPauzy.isAktywne()) {
                menuPauzy.update(worldPos); 
            } else {
                lvl3->update(dt, window, obrazenia);  
                int stan = lvl3->sprawdzStanGry();

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
        // Aktualizacja i sprawdzanie wygranej/przegranej dla Lvl 2
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
        // Aktualizacja i sprawdzanie wygranej/przegranej dla Lvl 1
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
    
        // SEKCJA RYSOWANIA (RENDERING)
        window.clear(sf::Color::Cyan); // Czyszczenie bufora kolorem nieba
        
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
    
    // SPRZĄTANIE PAMIĘCI - usuwanie obiektów utworzonych dynamicznie
    if (lvl11 != nullptr) delete lvl11;
    if (lvl2 != nullptr) delete lvl2;
    if (lvl3 != nullptr) delete lvl3;
    
    return 0;
}