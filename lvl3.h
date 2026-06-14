#ifndef LVL3_H
#define LVL3_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <box2d/box2d.h> 
#include <iostream> 
#include "obiekt.h"
#include "FizycznyObiekt.h"
#include "ObslugaProcy.h"
#include "SpecjalnePtaki.h"
#include "SystemKolizji.h" 

class Lvl3
{
private:
    const float Skala = 30.f;
    bool rysuj_kule = false;
    float promien_kuli = 60.f;
    
    bool rysuj_wiatr = false;
    float czas_wiatru = 10.f;
    bool wiatr_pozycja_ustawiona = false; 
    
    b2WorldId worldId;
    b2BodyId groundId{};
    sf::Texture chmura;
    sf::CircleShape kulaognia;
    std::vector<FizycznyObiekt*> ptaki_w_kolejce;
    std::vector<FizycznyObiekt*> obiekty;
    FizycznyObiekt* aktualny_ptak;
    ObslugaProcy* proca;
    FizycznyObiekt* podloga;
    sf::Texture wybuch;
    sf::Texture podmuch;
    sf::RectangleShape kaboom;
    sf::RectangleShape wiatr;
    sf::Texture tlo;
    sf::RectangleShape tlo_lvl;

    bool active = true;
    bool menu = false;
    bool koniec = false;
    float czas = 0.f;
    float czas_bezpieczenstwa = 0.2f;
    float czas_lotu_ptaka = 3.f; 

public:
    bool wygrana = false;
    bool aktwyny = true;
    SpecjalnePtaki managerMocy;

    // POPRAWIONA KOLEJNOŚĆ MATERIAŁÓW (Czerwony, Niebieski, Żółty, Bomba)
    Lvl3(Material& mat_czerw, Material& mat_niebieski, Material& mat_zoltek, Material& mat_bomba,
         Material& mat_swinia, Material& mat_swinia_zolnierz, Material& mat_swinia_dziad,
         Material& mat_drw_kwad, Material& mat_drw_belka, Material& mat_drw_troj,
         Material& mat_kam_kwad, Material& mat_kam_belka, Material& mat_kam_troj,
         Silnik_Obrazen& silauderzenia) 
    {
        tlo.loadFromFile("../tekstury/lvl3.png"); 
        tlo.setSmooth(true);
        tlo_lvl.setTexture(&tlo);
        tlo_lvl.setPosition({0.f,0.f});
        tlo_lvl.setSize({1920.f,1080.f});

        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 15.1f };
        worldId = b2CreateWorld(&worldDef);
        
        if(!wybuch.loadFromFile("../tekstury/BOOM.png")) std::cout << "Nie laduje wybuchu\n";
        if(!chmura.loadFromFile("../tekstury/chmura_otoczka.png")) std::cout << "Nie laduje chmury\n";
        
        if(!podmuch.loadFromFile("../tekstury/wiatr.png")) std::cout << "Nie laduje wiatru\n";
        wiatr.setTexture(&podmuch);
        wiatr.setSize({240.f, 360.f}); 
        wiatr.setOrigin({120.f, 0.f}); 
        
        kulaognia.setTexture(&chmura);
        kaboom.setTexture(&wybuch);
        kaboom.setSize({120.f, 120.f});  
        kaboom.setOrigin({60.f, 60.f}); 
        
        proca = new ObslugaProcy({ 250.f, 600.f });

        // --- PODŁOGA --- 
        podloga = new FizycznyObiekt(worldId, mat_drw_belka, 1920/2.f, 1100.f, 1920.f, 300.f, TypKsztaltu::PROSTOKAT);
        b2Body_SetType(podloga->body, b2_staticBody);
        obiekty.push_back(podloga);
        podloga->niewidzialny = true;
        podloga->wlasciwosci.hp = 10000000.f;

        // =========================================================================
        //    DWIE BLIŹNIACZE WIEŻE 
        // =========================================================================

        // -------------------------------------------------------------------------
        // WIEŻA LEWA: KAMIENNA PODSTAWA + DREWNIANE PIĘTRO 
        // -------------------------------------------------------------------------
        FizycznyObiekt* kamienL1 = new FizycznyObiekt(worldId, mat_kam_belka, 1050.f, 875.f, 40.f, 150.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* kamienL2 = new FizycznyObiekt(worldId, mat_kam_belka, 1150.f, 875.f, 40.f, 150.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* kamienL3 = new FizycznyObiekt(worldId, mat_kam_belka, 1250.f, 875.f, 40.f, 150.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(kamienL1); obiekty.push_back(kamienL2); obiekty.push_back(kamienL3);

        FizycznyObiekt* drewnoWypL1 = new FizycznyObiekt(worldId, mat_drw_kwad, 1100.f, 920.f, 60.f, 60.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* drewnoWypL2 = new FizycznyObiekt(worldId, mat_drw_kwad, 1200.f, 920.f, 60.f, 60.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(drewnoWypL1); obiekty.push_back(drewnoWypL2);

        FizycznyObiekt* stropKamienL1 = new FizycznyObiekt(worldId, mat_kam_belka, 1090.f, 780.f, 120.f, 40.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* stropKamienL2 = new FizycznyObiekt(worldId, mat_kam_belka, 1210.f, 780.f, 120.f, 40.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropKamienL1); obiekty.push_back(stropKamienL2);

        FizycznyObiekt* drewnoL1 = new FizycznyObiekt(worldId, mat_drw_belka, 1050.f, 700.f, 30.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* drewnoL2 = new FizycznyObiekt(worldId, mat_drw_belka, 1150.f, 700.f, 30.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* drewnoL3 = new FizycznyObiekt(worldId, mat_drw_belka, 1250.f, 700.f, 30.f, 120.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(drewnoL1); obiekty.push_back(drewnoL2); obiekty.push_back(drewnoL3);

        FizycznyObiekt* kamienWypL1 = new FizycznyObiekt(worldId, mat_kam_kwad, 1100.f, 735.f, 50.f, 50.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* kamienWypL2 = new FizycznyObiekt(worldId, mat_kam_kwad, 1200.f, 735.f, 50.f, 50.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(kamienWypL1); obiekty.push_back(kamienWypL2);

        FizycznyObiekt* wierzchKamienL = new FizycznyObiekt(worldId, mat_kam_troj, 1150.f, 590.f, 260.f, 100.f, TypKsztaltu::TROJKAT);
        obiekty.push_back(wierzchKamienL);

        // -------------------------------------------------------------------------
        // WIEŻA PRAWA: DREWNIANA PODSTAWA + KAMIENNE PIĘTRO (Centrum X = 1650)
        // -------------------------------------------------------------------------
        FizycznyObiekt* drewnoP1 = new FizycznyObiekt(worldId, mat_drw_belka, 1550.f, 875.f, 40.f, 150.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* drewnoP2 = new FizycznyObiekt(worldId, mat_drw_belka, 1650.f, 875.f, 40.f, 150.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* drewnoP3 = new FizycznyObiekt(worldId, mat_drw_belka, 1750.f, 875.f, 40.f, 150.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(drewnoP1); obiekty.push_back(drewnoP2); obiekty.push_back(drewnoP3);

        FizycznyObiekt* kamienWypP1 = new FizycznyObiekt(worldId, mat_kam_kwad, 1600.f, 920.f, 60.f, 60.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* kamienWypP2 = new FizycznyObiekt(worldId, mat_kam_kwad, 1700.f, 920.f, 60.f, 60.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(kamienWypP1); obiekty.push_back(kamienWypP2);

        sf::Vector2f stropPosP1{1590.f, 780.f};
        sf::Vector2f stropSizeP1{120.f, 40.f};
        FizycznyObiekt* stropDrewnoP1 = new FizycznyObiekt(worldId, mat_drw_belka, stropPosP1.x, stropPosP1.y, stropSizeP1.x, stropSizeP1.y, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* stropDrewnoP2 = new FizycznyObiekt(worldId, mat_drw_belka, 1710.f, 780.f, 120.f, 40.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropDrewnoP1); obiekty.push_back(stropDrewnoP2);

        FizycznyObiekt* kamienP1 = new FizycznyObiekt(worldId, mat_kam_belka, 1550.f, 700.f, 30.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* kamienP2 = new FizycznyObiekt(worldId, mat_kam_belka, 1650.f, 700.f, 30.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* kamienP3 = new FizycznyObiekt(worldId, mat_kam_belka, 1750.f, 700.f, 30.f, 120.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(kamienP1); obiekty.push_back(kamienP2); obiekty.push_back(kamienP3);

        FizycznyObiekt* drewnoWypP3 = new FizycznyObiekt(worldId, mat_drw_kwad, 1600.f, 735.f, 50.f, 50.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* drewnoWypP4 = new FizycznyObiekt(worldId, mat_drw_kwad, 1700.f, 735.f, 50.f, 50.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(drewnoWypP3); obiekty.push_back(drewnoWypP4);

        FizycznyObiekt* wierzchDrewnoP = new FizycznyObiekt(worldId, mat_drw_troj, 1650.f, 590.f, 260.f, 100.f, TypKsztaltu::TROJKAT);
        obiekty.push_back(wierzchDrewnoP);

        // =========================================================================
        //    ARMIA 12 ŚWIŃ (ODSUNIĘTE O +150 W PRAWO)
        // =========================================================================

        // --- WIEŻA LEWA ---
        FizycznyObiekt* s1 = new FizycznyObiekt(worldId, mat_swinia, 1100.f, 865.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s2 = new FizycznyObiekt(worldId, mat_swinia, 1200.f, 865.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s4 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1100.f, 685.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s5 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1200.f, 685.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s6 = new FizycznyObiekt(worldId, mat_swinia, 1150.f, 515.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s10 = new FizycznyObiekt(worldId, mat_swinia_dziad, 950.f, 915.f, 35.f, 0.f, TypKsztaltu::KOLO); 

        // --- WIEŻA PRAWA ---
        FizycznyObiekt* s3 = new FizycznyObiekt(worldId, mat_swinia, 1600.f, 865.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s12 = new FizycznyObiekt(worldId, mat_swinia, 1700.f, 865.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s8 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1600.f, 685.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s9 = new FizycznyObiekt(worldId, mat_swinia, 1700.f, 685.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s7 = new FizycznyObiekt(worldId, mat_swinia, 1650.f, 515.f, 25.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* s11 = new FizycznyObiekt(worldId, mat_swinia_dziad, 1850.f, 915.f, 35.f, 0.f, TypKsztaltu::KOLO); 

        s1->wlasciwosci.typulec = Typ::SWINIA;  s2->wlasciwosci.typulec = Typ::SWINIA;  s3->wlasciwosci.typulec = Typ::SWINIA;
        s4->wlasciwosci.typulec = Typ::SWINIA;  s5->wlasciwosci.typulec = Typ::SWINIA;  s6->wlasciwosci.typulec = Typ::SWINIA;
        s7->wlasciwosci.typulec = Typ::SWINIA;  s8->wlasciwosci.typulec = Typ::SWINIA;  s9->wlasciwosci.typulec = Typ::SWINIA;
        s10->wlasciwosci.typulec = Typ::SWINIA; s11->wlasciwosci.typulec = Typ::SWINIA; s12->wlasciwosci.typulec = Typ::SWINIA;

        obiekty.push_back(s1);  obiekty.push_back(s2);  obiekty.push_back(s3);
        obiekty.push_back(s4);  obiekty.push_back(s5);  obiekty.push_back(s6);
        obiekty.push_back(s7);  obiekty.push_back(s8);  obiekty.push_back(s9);
        obiekty.push_back(s10); obiekty.push_back(s11); obiekty.push_back(s12);

        // =========================================================================
        //    PTAKI (POWIĘKSZONE ROZMIARY)
        // =========================================================================
        FizycznyObiekt* p1 = new FizycznyObiekt(worldId, mat_czerw, 190.f, 915.f, 35.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* p2 = new FizycznyObiekt(worldId, mat_niebieski, 130.f, 920.f, 30.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* p3 = new FizycznyObiekt(worldId, mat_zoltek, 70.f, 920.f, 60.f, 60.f, TypKsztaltu::TROJKAT); 
        FizycznyObiekt* p4 = new FizycznyObiekt(worldId, mat_bomba, -10.f, 905.f, 45.f, 0.f, TypKsztaltu::KOLO);
        
        b2Body_SetType(p1->body, b2_staticBody);
        b2Body_SetType(p2->body, b2_staticBody);
        b2Body_SetType(p3->body, b2_staticBody);
        b2Body_SetType(p4->body, b2_staticBody);
        
        p1->wlasciwosci.typek = RodzajPtaka::CZERWONIAK; p1->wlasciwosci.typulec = Typ::PTAK;
        p2->wlasciwosci.typek = RodzajPtaka::NIEBIESKI;  p2->wlasciwosci.typulec = Typ::PTAK;
        p3->wlasciwosci.typek = RodzajPtaka::ZOLTEK;     p3->wlasciwosci.typulec = Typ::PTAK;
        p4->wlasciwosci.typek = RodzajPtaka::BOMBA;      p4->wlasciwosci.typulec = Typ::PTAK;

        obiekty.push_back(p1); obiekty.push_back(p2); obiekty.push_back(p3); obiekty.push_back(p4); 

        ptaki_w_kolejce.push_back(p1);
        ptaki_w_kolejce.push_back(p2);
        ptaki_w_kolejce.push_back(p3);
        ptaki_w_kolejce.push_back(p4);

        aktualny_ptak = ptaki_w_kolejce.front(); 
        ptaki_w_kolejce.erase(ptaki_w_kolejce.begin()); 
        aktualny_ptak->wlasciwosci.stan = StanPtaka::CZEKA_NA_PROCY;
        
        b2Body_SetTransform(aktualny_ptak->body, { 250.f / Skala, 600.f / Skala }, b2Body_GetRotation(aktualny_ptak->body));
        managerMocy.odnow_umiejetnosci();
    }
    
    ~Lvl3()
    {
        for (auto obj : obiekty) delete obj;
        delete proca;
        b2DestroyWorld(worldId);
    }

    b2WorldId getWorldId() const { return worldId; }

    void obslugujZdarzenia(const sf::Event& event, sf::RenderWindow& window)
    {
        if (!active || menu) return;

        if (aktualny_ptak != nullptr) {
            proca->ObslugaWystrzalu(event, window, *aktualny_ptak, czas);
            
            managerMocy.specjalnefunkcjeptakow(aktualny_ptak, event, obiekty, kulaognia, rysuj_kule, promien_kuli, wiatr, rysuj_wiatr, czas_wiatru);

            if (rysuj_kule && !aktualny_ptak->wlasciwosci.swiezozniszczony && aktualny_ptak->wlasciwosci.typek == RodzajPtaka::BOMBA) {
                sf::Vector2f pozBomby = (aktualny_ptak->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? 
                                         aktualny_ptak->pro.getPosition() : aktualny_ptak->kol.getPosition();
                
                kulaognia.setPosition(pozBomby);

                aktualny_ptak->wlasciwosci.hp = 0.f;
                aktualny_ptak->wlasciwosci.swiezozniszczony = true; 
                aktualny_ptak->wlasciwosci.czas_wybuchu = 1.2f; 
                std::cout << "[SYSTEM] Bomba zdetonowana przez gracza! Znika.\n";
            }
        }
    }

    void update(float deltaTime, sf::RenderWindow& window, Silnik_Obrazen &obrazenia)
    {
        if (!active || menu) return;
        if (czas_bezpieczenstwa > 0.0f) {
            czas_bezpieczenstwa -= deltaTime;
        }

        b2World_Step(worldId, deltaTime, 4);
        SystemKolizji::przetworzZderzenia(worldId, obrazenia);

        if (rysuj_kule) {
            promien_kuli += 1600.f * deltaTime; 
            kulaognia.setRadius(promien_kuli);
            kulaognia.setOrigin({promien_kuli, promien_kuli}); 
            
            if (promien_kuli >= 400.f) {
                rysuj_kule = false;
            }
        }
        
        if (rysuj_wiatr) {
            if (!wiatr_pozycja_ustawiona && aktualny_ptak != nullptr) {
                sf::Vector2f pozPtaka = (aktualny_ptak->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? aktualny_ptak->pro.getPosition() :
                                        (aktualny_ptak->wlasciwosci.typ == TypKsztaltu::TROJKAT ? aktualny_ptak->tro.getPosition() : aktualny_ptak->kol.getPosition());
                wiatr.setPosition(pozPtaka);
                wiatr_pozycja_ustawiona = true;
            }

            czas_wiatru -= deltaTime;
            wiatr.move({30.f*deltaTime, 100.f * deltaTime}); 
            
            float alfa = (czas_wiatru / 0.5f) * 255.0f;
            if (alfa < 0.f) alfa = 0.f;
            if (alfa > 255.f) alfa = 255.f;
            
            wiatr.setFillColor(sf::Color(255, 255, 255, alfa));

            if (czas_wiatru <= 0.0f) {
                rysuj_wiatr = false;
            }
        } else {
            wiatr_pozycja_ustawiona = false; 
        }

        for (auto obj : obiekty) {
            if (obj->wlasciwosci.swiezozniszczony && b2Body_IsValid(obj->body)) {
                obj->update(); 
                b2DestroyBody(obj->body); 
            }

            if (obj->wlasciwosci.swiezozniszczony) {
                obj->wlasciwosci.czas_wybuchu -= deltaTime; 

                if (obj->wlasciwosci.czas_wybuchu <= 0.0f) {
                    obj->wlasciwosci.czas_wybuchu = 0.0f;
                    obj->wlasciwosci.swiezozniszczony = false; 
                    obj->wlasciwosci.aktywny = false; 
                }
            }

            if (obj->wlasciwosci.aktywny && !obj->wlasciwosci.swiezozniszczony) {
                obj->update();
            }
        }

        if (aktualny_ptak != nullptr) {
            if (aktualny_ptak->wlasciwosci.swiezozniszczony || aktualny_ptak->wlasciwosci.hp <= 0.f) {
                aktualny_ptak = nullptr;
                czas_lotu_ptaka = 3.0f; 
            } 
            else if (aktualny_ptak->wlasciwosci.stan == StanPtaka::LECI) {
                czas_lotu_ptaka -= deltaTime;
                
                if (czas_lotu_ptaka <= 0.0f) {
                    aktualny_ptak->wlasciwosci.hp = 0.f;
                    aktualny_ptak->wlasciwosci.swiezozniszczony = true; 
                    aktualny_ptak->wlasciwosci.czas_wybuchu = 1.2f; 
                    
                    aktualny_ptak = nullptr; 
                    czas_lotu_ptaka = 3.0f;  
                }
            }
        }

        if (aktualny_ptak == nullptr && !ptaki_w_kolejce.empty() && !rysuj_kule) {
            aktualny_ptak = ptaki_w_kolejce.front();
            ptaki_w_kolejce.erase(ptaki_w_kolejce.begin());
            aktualny_ptak->wlasciwosci.stan = StanPtaka::CZEKA_NA_PROCY;
            
            b2Body_SetTransform(aktualny_ptak->body, { 250.f / Skala, 600.f / Skala }, b2Body_GetRotation(aktualny_ptak->body));
            managerMocy.odnow_umiejetnosci();
            
            czas_lotu_ptaka = 3.0f; 
        }
    }

    int sprawdzStanGry() const {
        if (czas_bezpieczenstwa > 0.0f) return 0;

        bool znalezionoSwinie = false;
        bool znalezionoptaki = false;
        int zyweSwinie = 0;
        int zyweptaki = 0;

        for (auto obj : obiekty) {
            if (obj != nullptr) {
                if (obj->wlasciwosci.typulec == Typ::SWINIA) {
                    znalezionoSwinie = true;
                    if (obj->wlasciwosci.aktywny && !obj->wlasciwosci.swiezozniszczony) {
                        zyweSwinie++;
                    }
                }
                if (obj->wlasciwosci.typulec == Typ::PTAK) {
                    znalezionoptaki = true;
                    if (obj->wlasciwosci.aktywny && !obj->wlasciwosci.swiezozniszczony) {
                        zyweptaki++;
                    }
                }
            }
        }

        if (znalezionoSwinie && zyweSwinie == 0)         return 1; 
        else if (znalezionoptaki && zyweptaki == 0 && zyweSwinie > 0) return 2; 

        return 0; 
    }

    void draw(sf::RenderWindow& window, float deltaTime)
    {
        window.draw(tlo_lvl);
        proca->drawWielowarstwowy(window, aktualny_ptak);
        
        if (rysuj_kule) {
            window.draw(kulaognia);
        }

        if (rysuj_wiatr) {
            window.draw(wiatr);
        }
        
        for (auto obj : obiekty) {
            if (obj->wlasciwosci.swiezozniszczony) {
                sf::Vector2f pozycjaBumu = (obj->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? obj->pro.getPosition() : obj->kol.getPosition();
                
                kaboom.setPosition(pozycjaBumu);
                kaboom.setRotation(sf::degrees(0.f)); 
                
                float alfa = (obj->wlasciwosci.czas_wybuchu / 1.2f) * 255.0f;   
                if (alfa < 0.f) alfa = 0.f;
                if (alfa > 255.f) alfa = 255.f;
                
                kaboom.setFillColor(sf::Color(255, 255, 255, alfa));    
                window.draw(kaboom);
                continue; 
            }

            if (!obj->wlasciwosci.aktywny) continue;
            if (obj->niewidzialny) continue;
            
            if (obj->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) window.draw(obj->pro);
            else if (obj->wlasciwosci.typ == TypKsztaltu::KOLO) window.draw(obj->kol);
            else if (obj->wlasciwosci.typ == TypKsztaltu::TROJKAT) window.draw(obj->tro);
        }
    }
};

#endif