#ifndef OBSLUGAPROCY_H
#define OBSLUGAPROCY_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h> 
#include <cmath>
#include <vector>
#include <iostream>
#include "FizycznyObiekt.h"

class ObslugaProcy
{
private:
    sf::Vector2f srodek_procy;          // punkt bazowy, od ktorego liczymy naciag
    sf::CircleShape chmurka;            // pojedyncza kropka trajektorii
    float sila_naciagu_max = 120.f;     // maksymalny promien naciagniecia gumy (w pikselach)
    float Skala = 30.f;                 // przelicznik pikseli na metry dla box2d (1 metr = 30px)
    std::vector<sf::Vector2f> trajektoria; // punkty prognozowanego toru lotu

    const float PI = 3.14159265f;
    sf::Vector2f lewe_ramie;            // punkt zaczepienia lewej gumy
    sf::Vector2f prawe_ramie;           // punkt zaczepienia prawej gumy
    sf::Texture tex_noga;
    sf::Texture tex_guma_lewa;
    sf::Texture tex_guma_prawa;
    sf::RectangleShape noga;            // graficzny model drewnianych widelek
    sf::RectangleShape guma_lewa;       // prostokat imitujacy naciagana gume
    sf::RectangleShape guma_prawa;
    const float bazowa_grubosc = 90.f;  // poczatkowa szerokosc tekstury gumy

public:
    ObslugaProcy(sf::Vector2f pozycja_procy) : srodek_procy(pozycja_procy) 
    {
        // inicjalizacja kropki do rysowania linii pomocniczej
        chmurka.setRadius(3.f);
        chmurka.setFillColor(sf::Color::White);

        // ładowanie tekstur procy i gumy
        if (!tex_noga.loadFromFile("../tekstury/noga-procy.png") ||
            !tex_guma_lewa.loadFromFile("../tekstury/guma-lewa.png") ||
            !tex_guma_prawa.loadFromFile("../tekstury/guma-prawa.png")) {
            std::cout << "[BLAD] Nie udalo sie zaladowac tekstur procy!\n";
        }
        // zapętlenie tekstury gum, żeby rozciągały się bez brzydkiego rozmycia
        tex_guma_lewa.setRepeated(true);
        tex_guma_prawa.setRepeated(true);

        // ustawianie pozycji drewnianego stojaka
        noga.setTexture(&tex_noga);
        noga.setSize({ 220.f, 300.f });
        noga.setOrigin({0.f, 0.f }); 
        noga.setPosition(srodek_procy);

        // hardkodowane punkty na widelkach, z ktorych wychodza gumy
        lewe_ramie  = srodek_procy + sf::Vector2f(140.f,70.f);
        prawe_ramie = srodek_procy + sf::Vector2f(140+110.f, 70.f);

        // przygotowanie lewego paska gumy
        guma_lewa.setTexture(&tex_guma_lewa);
        guma_lewa.setOrigin({ 0.f, bazowa_grubosc / 2.f }); // punkt obrotu na srodku lewej krawedzi
        guma_lewa.setPosition(lewe_ramie);

        // przygotowanie prawego paska gumy
        guma_prawa.setTexture(&tex_guma_prawa);
        guma_prawa.setOrigin({ 0.f, bazowa_grubosc / 2.f });
        guma_prawa.setPosition(prawe_ramie);
    }
        
    void ObslugaWystrzalu(const sf::Event& event,  sf::RenderWindow& window, FizycznyObiekt& ptak, float deltaTime)
    {
        // pobieranie i mapowanie pozycji myszki na wspolrzedne swiata gry
        sf::Vector2i pozycja_myszy_okno = sf::Mouse::getPosition(window);
        sf::Vector2f pozycja_myszy = window.mapPixelToCoords(pozycja_myszy_okno);

        // resetowanie ptaka do wnetrza procy, jesli spokojnie w niej czeka
        if (ptak.wlasciwosci.stan == StanPtaka::CZEKA_NA_PROCY) {
            b2Body_SetType(ptak.body, b2_staticBody); // blokada fizyki box2d na czas celowania
            b2Body_SetTransform(ptak.body, { srodek_procy.x / Skala, srodek_procy.y / Skala }, b2Rot_identity);
            ptak.update(); 
        }

        // 1. chwycenie procy (kliknięcie na ptaka)
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left && 
                ptak.wlasciwosci.stan == StanPtaka::CZEKA_NA_PROCY) {
                
                // pobranie obwodu odpowiedniego kształtu, zeby wykryc klikniecie
                sf::FloatRect bounds;
                if (ptak.wlasciwosci.typ == TypKsztaltu::PROSTOKAT) bounds = ptak.pro.getGlobalBounds();
                else if (ptak.wlasciwosci.typ == TypKsztaltu::KOLO) bounds = ptak.kol.getGlobalBounds();
                else if (ptak.wlasciwosci.typ == TypKsztaltu::TROJKAT) bounds = ptak.tro.getGlobalBounds();

                // jesli myszka trafia w ptaka, zaczynamy naciaganie
                if (bounds.contains(pozycja_myszy)) {
                    ptak.wlasciwosci.stan = StanPtaka::NACIAGANY;
                }
            }
        }

        // 2. naciąganie procy (ruch myszką z wciśniętym klawiszem)
        if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
            if (ptak.wlasciwosci.stan == StanPtaka::NACIAGANY) {
                // wektor od srodka procy do pozycji kursora
                float dx = pozycja_myszy.x - srodek_procy.x;
                float dy = pozycja_myszy.y - srodek_procy.y;
                float dystans = std::sqrt(dx * dx + dy * dy);

                b2Vec2 nowa_pozycja_b2;
                // pilnowanie zeby nie naciagnac gumy mocniej niz pozwala maks promien
                if (dystans > sila_naciagu_max) {
                    float px = srodek_procy.x + (dx / dystans) * sila_naciagu_max;
                    float py = srodek_procy.y + (dy / dystans) * sila_naciagu_max;
                    nowa_pozycja_b2 = { px / Skala, py / Skala };
                } else {
                    nowa_pozycja_b2 = { pozycja_myszy.x / Skala, pozycja_myszy.y / Skala };
                }

                // wymuszenie pozycji ciala w box2d i zaktualizowanie sprite'a sfml
                b2Body_SetTransform(ptak.body, nowa_pozycja_b2, b2Rot_identity);
                ptak.update(); 
                
                // --- symulacja trajektorii lotu ---
                sf::Vector2f aktualna_pozycja_ptaka = {nowa_pozycja_b2.x * Skala, nowa_pozycja_b2.y * Skala};
                // wektor sily odwrotny do naciagu procy pomnozony przez mnozniki predkosci
                float sim_vy = (srodek_procy.y - aktualna_pozycja_ptaka.y) * 5.f;
                float sim_vx = (srodek_procy.x - aktualna_pozycja_ptaka.x) * 7.f;
                float grawitacja_y = 2000.f; // przyblizone przyspieszenie grawitacyjne dla wizualizacji
                float sim_dt = 0.08f;        // odstep czasu miedzy kropkami
                
                trajektoria.clear(); 
                for (int i = 1; i <= 8; ++i) { 
                    float t = i * sim_dt;
                    // rownania ruchu jednostajnego (x) i jednostajnie przyspieszonego (y)
                    float x = aktualna_pozycja_ptaka.x + sim_vx * t; 
                    float y = aktualna_pozycja_ptaka.y + sim_vy * t + 0.1f * grawitacja_y * t * t;

                    if (y < 900.f) { // nie rysujemy kropek ponizej poziomu ziemi
                        trajektoria.push_back({x, y}); 
                    }
                }

                // --- obliczanie wyglądu gum ---
                sf::Vector2f srodek_ptaka = aktualna_pozycja_ptaka;
                sf::FloatRect b;
                if (ptak.wlasciwosci.typ == TypKsztaltu::PROSTOKAT) b = ptak.pro.getGlobalBounds();
                else if (ptak.wlasciwosci.typ == TypKsztaltu::KOLO) b = ptak.kol.getGlobalBounds();
                else if (ptak.wlasciwosci.typ == TypKsztaltu::TROJKAT) b = ptak.tro.getGlobalBounds();
                
                srodek_ptaka = { b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f };

                // przeliczenia dla lewej gumy (odleglosc, kat obrotu i zwezanie przy rozciaganiu)
                float dlx = srodek_ptaka.x - lewe_ramie.x;
                float dly = srodek_ptaka.y - lewe_ramie.y;
                float dlugosc_lewa = std::sqrt(dlx * dlx + dly * dly);
                float kat_lewy = std::atan2(dly, dlx) * (180.f / PI);
                float grubosc_lewa = bazowa_grubosc * (sila_naciagu_max / (sila_naciagu_max + dlugosc_lewa * 0.5f));

                guma_lewa.setSize({ dlugosc_lewa+10, grubosc_lewa });
                guma_lewa.setOrigin({ 0.f, grubosc_lewa / 2.f });
                guma_lewa.setRotation(sf::degrees(kat_lewy)); 

                // przeliczenia dla prawej gumy
                float dpx = srodek_ptaka.x - prawe_ramie.x;
                float dpy = srodek_ptaka.y - prawe_ramie.y;
                float dlugosc_prawa = std::sqrt(dpx * dpx + dpy * dpy);
                float kat_prawy = std::atan2(dpy, dpx) * (180.f / PI);
                float grubosc_prawa = bazowa_grubosc * (sila_naciagu_max / (sila_naciagu_max + dlugosc_prawa * 0.5f));

                guma_prawa.setSize({ dlugosc_prawa+60, grubosc_prawa });
                guma_prawa.setOrigin({ 0.f, grubosc_prawa / 2.f });
                guma_prawa.setRotation(sf::degrees(kat_prawy)); 
            }
        }

        // 3. puszczenie myszki (wystrzał i nadanie pędu)
        if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseReleased->button == sf::Mouse::Button::Left && ptak.wlasciwosci.stan == StanPtaka::NACIAGANY) {
                
                ptak.wlasciwosci.stan = StanPtaka::LECI;
                trajektoria.clear(); // ukrywamy linie pomocnicza po strzale
                
                // aktywacja pelnej fizyki box2d (ptak staje sie obiektem dynamicznym poddanym grawitacji)
                b2Body_SetType(ptak.body, b2_dynamicBody);
                b2Vec2 pos_b2 = b2Body_GetPosition(ptak.body);
                sf::Vector2f pos_px = { pos_b2.x * Skala, pos_b2.y * Skala };

                // wyliczanie wektora predkosci poczatkowej na podstawie wektora naciagu procy
                float b2_vx = (srodek_procy.x - pos_px.x) * 0.4f;
                float b2_vy = (srodek_procy.y - pos_px.y) * 0.2f; 
                float rzeczywiste_srodek_x = srodek_procy.x;
                float rzeczywiste_srodek_y = srodek_procy.y;

                // nadanie predkosci poczatkowej do ciala fizycznego box2d
                b2Body_SetLinearVelocity(ptak.body, { (rzeczywiste_srodek_x - pos_px.x) * 0.4f, (rzeczywiste_srodek_y - pos_px.y) * 0.2f });
            }
        }
    }

    void drawWielowarstwowy(sf::RenderWindow& window, FizycznyObiekt* ptak)
    {
        // rysowanie warstwowe (z-buffer), zeby ptak znajdowal sie optycznie "pomiędzy" dwiema gumami procy
        if (ptak != nullptr && ptak->wlasciwosci.stan == StanPtaka::NACIAGANY) {
            window.draw(guma_lewa); // tylna guma
        }

        if (ptak != nullptr && (ptak->wlasciwosci.stan == StanPtaka::CZEKA_NA_PROCY || ptak->wlasciwosci.stan == StanPtaka::NACIAGANY)) {
            if (ptak->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) window.draw(ptak->pro);
            else if (ptak->wlasciwosci.typ == TypKsztaltu::KOLO) window.draw(ptak->kol);
            else if(ptak->wlasciwosci.typ == TypKsztaltu::TROJKAT) window.draw(ptak->tro);
        }

        window.draw(noga); // stojak procy zaslania lewa gume i ptaka od przodu

        if (ptak != nullptr && ptak->wlasciwosci.stan == StanPtaka::NACIAGANY) {
            window.draw(guma_prawa); // przednia guma dociskajaca ptaka
        }

        // rysowanie punktow celowniczych na samym wierzchu
        for (auto& pos : trajektoria) {
            chmurka.setPosition(pos);
            window.draw(chmurka);
        }
    }
};

#endif