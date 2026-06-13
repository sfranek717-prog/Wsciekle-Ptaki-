#ifndef FIZYCZNYDEBUGGER_H
#define FIZYCZNYDEBUGGER_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <string>
#include <iostream> 
#include "FizycznyObiekt.h"

class FizycznyDebugger {
private:
    bool aktywny = false;
    FizycznyObiekt* chwyconyObiekt = nullptr;
    const float Skala = 30.f;

public:
    void przelacz() {
        aktywny = !aktywny;
        std::cout << "[DEBUGGER] Przełączono stan! Aktualny status: " << (aktywny ? "WLACZONY" : "WYLACZONY") << "\n";
        if (!aktywny) chwyconyObiekt = nullptr;
    }

    bool czyAktywny() const { return aktywny; }

    void obslugujZdarzenia(const sf::Event& event, sf::RenderWindow& window, const std::vector<FizycznyObiekt*>& obiekty) {
        if (!aktywny) return;

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                
                std::cout << "[DEBUGGER] Kliknięto LPM na pozycji ekranu: X:" << mPos.x << " Y:" << mPos.y << "\n";
                std::cout << "[DEBUGGER] Przeszukuję " << obiekty.size() << " obiektów...\n";

                for (auto obj : obiekty) {
                    bool klikniety = false;
                    
                    if (obj->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) {
                        auto bounds = obj->pro.getGlobalBounds();
                        if (bounds.contains(mPos)) klikniety = true;
                    }
                    if (obj->wlasciwosci.typ == TypKsztaltu::KOLO) {
                        auto bounds = obj->kol.getGlobalBounds();
                        if (bounds.contains(mPos)) klikniety = true;
                    }
                    if (obj->wlasciwosci.typ == TypKsztaltu::TROJKAT) {
                        auto bounds = obj->tro.getGlobalBounds();
                        if (bounds.contains(mPos)) klikniety = true;
                    }
                    
                    if (klikniety) {
                        std::cout << "[DEBUGGER] SUKCES! Trafiono obiekt. Typ: " 
                                  << (int)obj->wlasciwosci.typ << " | Adres: " << obj << "\n";
                        chwyconyObiekt = obj;
                        break;
                    }
                }
                
                if (!chwyconyObiekt) {
                    std::cout << "[DEBUGGER] Pudło! Myszka nie trafiła w żaden globalBounds klocka.\n";
                }
            }
        }
        else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseReleased->button == sf::Mouse::Button::Left) {
                if (chwyconyObiekt) std::cout << "[DEBUGGER] Upuszczono obiekt.\n";
                chwyconyObiekt = nullptr;
            }
        }
    }

    void update(sf::RenderWindow& window) {
        if (!aktywny) {
            window.setTitle("Wsciekle Ptaki");
            return;
        }

        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        window.setTitle("Wsciekle Ptaki DEBUGGER | X: " + std::to_string((int)mPos.x) + " Y: " + std::to_string((int)mPos.y));

        if (chwyconyObiekt) {
            b2Rot rot = b2Body_GetRotation(chwyconyObiekt->body);
            b2Body_SetTransform(chwyconyObiekt->body, {mPos.x / Skala, mPos.y / Skala}, rot);
            b2Body_SetLinearVelocity(chwyconyObiekt->body, {0.f, 0.f});
            b2Body_SetAngularVelocity(chwyconyObiekt->body, 0.f);
        }
    }

    // Zmieniona nazwa i usunięte static!
    void draw(sf::RenderWindow& window, const std::vector<FizycznyObiekt*>& obiekty) {
        if (!aktywny) return; // Rysujemy tylko, jeśli jest włączony
        
        for (auto obj : obiekty) {
            if (!obj->wlasciwosci.aktywny) continue; 

            if (obj->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) {
                sf::RectangleShape rentgen = obj->pro;
                rentgen.setFillColor(sf::Color::Transparent); 
                rentgen.setOutlineColor(sf::Color::Green);    
                rentgen.setOutlineThickness(2.f);
                window.draw(rentgen);
            } 
            else if (obj->wlasciwosci.typ == TypKsztaltu::KOLO) {
                sf::CircleShape rentgen = obj->kol;
                rentgen.setFillColor(sf::Color::Transparent);
                rentgen.setOutlineColor(sf::Color::Green);
                rentgen.setOutlineThickness(2.f);
                window.draw(rentgen);
            }
            else if (obj->wlasciwosci.typ == TypKsztaltu::TROJKAT) {
                sf::ConvexShape rentgen = obj->tro;
                rentgen.setFillColor(sf::Color::Transparent);
                rentgen.setOutlineColor(sf::Color::Green);
                rentgen.setOutlineThickness(2.f);
                window.draw(rentgen);
            }
        }
    }
};

#endif