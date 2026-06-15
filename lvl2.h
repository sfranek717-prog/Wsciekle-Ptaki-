#ifndef LVL2_H
#define LVL2_H

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

class Lvl2
{
private:
    const float Skala = 30.f;
    bool rysuj_kule = false;
    float promien_kuli = 60.f;
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
    sf::RectangleShape kaboom;
    sf::Texture tlo;
    sf::RectangleShape tlo_lvl;
    
    bool rysuj_wiatr = false;
    float czas_wiatru = 10.f;
    
    bool active = true;
    bool menu = false;
    bool koniec = false;
    float czas = 0.f;
    float czas_bezpieczenstwa = 0.2f;
    float czas_lotu_ptaka = 3.f; 
    sf::Texture podmuch;
    sf::RectangleShape wiatr;

public:
    bool wygrana = false;
    bool aktwyny = true;
    SpecjalnePtaki managerMocy;

    Lvl2(Material& mat_czerw, Material& mat_niebieski, Material& mat_zoltek, Material& mat_bomba, 
         Material& mat_swinia, Material& mat_swinia_zolnierz, Material& mat_swinia_dziad,
         Material& mat_drw_kwad, Material& mat_drw_belka, Material& mat_drw_troj,
         Material& mat_lod_kwad, Material& mat_lod_belka, Material& mat_lod_troj,
         Material& mat_kam_kwad, Material& mat_kam_belka, Material& mat_kam_troj,
         Silnik_Obrazen& silauderzenia) 
    {
        tlo.loadFromFile("../tekstury/lvl2.png"); 
        tlo.setSmooth(true);
        tlo_lvl.setTexture(&tlo);
        tlo_lvl.setPosition({0.f,0.f});
        tlo_lvl.setSize({1920.f,1080.f});

        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 15.1f };
        worldId = b2CreateWorld(&worldDef);
        
        if(!wybuch.loadFromFile("../tekstury/BOOM.png")) {
            std::cout << "Nie laduje wybuchu\n";
        }
        if(!chmura.loadFromFile("../tekstury/chmura_otoczka.png")) {
            std::cout << "Nie laduje chmury\n";
        }
        kulaognia.setTexture(&chmura);
        kaboom.setTexture(&wybuch);
        kaboom.setSize({120.f, 120.f});  
        kaboom.setOrigin({60.f, 60.f}); 
        
        proca = new ObslugaProcy({ 250.f, 600.f });
        
        // podloga
        podloga = new FizycznyObiekt(worldId, mat_drw_belka, 960.f, 935.f, 1920.f, 300.f, TypKsztaltu::PROSTOKAT);
        b2Body_SetType(podloga->body, b2_staticBody);
        obiekty.push_back(podloga);
        podloga->niewidzialny = true;
        podloga->wlasciwosci.hp = 10000000.f;

        FizycznyObiekt* kamL = new FizycznyObiekt(worldId, mat_kam_belka, 1350.f, 705.f, 80.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* kamP = new FizycznyObiekt(worldId, mat_kam_belka, 1550.f, 705.f, 80.f, 160.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(kamL); obiekty.push_back(kamP);

        FizycznyObiekt* stropLod = new FizycznyObiekt(worldId, mat_lod_belka, 1450.f, 605.f, 300.f, 40.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropLod);

        FizycznyObiekt* lodKwadL = new FizycznyObiekt(worldId, mat_lod_kwad, 1380.f, 545.f, 80.f, 80.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* lodKwadP = new FizycznyObiekt(worldId, mat_lod_kwad, 1520.f, 545.f, 80.f, 80.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(lodKwadL); obiekty.push_back(lodKwadP);

        FizycznyObiekt* stropDrw = new FizycznyObiekt(worldId, mat_drw_belka, 1450.f, 490.f, 240.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropDrw);

        FizycznyObiekt* kamTroj = new FizycznyObiekt(worldId, mat_kam_troj, 1450.f, 435.f, 80.f, 80.f, TypKsztaltu::TROJKAT);
        obiekty.push_back(kamTroj);

        FizycznyObiekt* swinia1 = new FizycznyObiekt(worldId, mat_swinia, 1450.f, 750.f, 35.f, 0.f, TypKsztaltu::KOLO);    
        FizycznyObiekt* swinia2 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1450.f, 545.f, 40.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* swinia3 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1200.f, 745.f, 40.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* swinia4 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1700.f, 745.f, 40.f, 0.f, TypKsztaltu::KOLO);

        obiekty.push_back(swinia1);
        obiekty.push_back(swinia2);
        obiekty.push_back(swinia3);
        obiekty.push_back(swinia4);

        swinia1->wlasciwosci.typulec = Typ::SWINIA;
        swinia2->wlasciwosci.typulec = Typ::SWINIA;
        swinia3->wlasciwosci.typulec = Typ::SWINIA;
        swinia4->wlasciwosci.typulec = Typ::SWINIA;

        FizycznyObiekt* ptak1 = new FizycznyObiekt(worldId, mat_czerw, 150.f, 900.f, 40.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* ptak2 = new FizycznyObiekt(worldId, mat_bomba, 100.f, 900.f, 40.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* ptak3 = new FizycznyObiekt(worldId, mat_zoltek, 50.f, 900.f, 40.f, 40.f, TypKsztaltu::TROJKAT); 
        
        b2Body_SetType(ptak1->body, b2_staticBody);
        b2Body_SetType(ptak2->body, b2_staticBody);
        b2Body_SetType(ptak3->body, b2_staticBody);
        
        obiekty.push_back(ptak1);
        ptak1->wlasciwosci.typek = RodzajPtaka::CZERWONIAK;
        ptak1->wlasciwosci.typulec = Typ::PTAK;

        obiekty.push_back(ptak2);
        ptak2->wlasciwosci.typek = RodzajPtaka::BOMBA;
        ptak2->wlasciwosci.typulec = Typ::PTAK;

        obiekty.push_back(ptak3);
        ptak3->wlasciwosci.typek = RodzajPtaka::ZOLTEK;
        ptak3->wlasciwosci.typulec = Typ::PTAK;

        ptaki_w_kolejce.push_back(ptak1);
        ptaki_w_kolejce.push_back(ptak2);
        ptaki_w_kolejce.push_back(ptak3);

        aktualny_ptak = ptaki_w_kolejce.front(); 
        ptaki_w_kolejce.erase(ptaki_w_kolejce.begin()); 
        aktualny_ptak->wlasciwosci.stan = StanPtaka::CZEKA_NA_PROCY;
        
        b2Body_SetTransform(aktualny_ptak->body, { 250.f / Skala, 600.f / Skala }, b2Body_GetRotation(aktualny_ptak->body));
        managerMocy.odnow_umiejetnosci();
    }

    ~Lvl2()
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

        for (auto obj : obiekty) {
            if (obj->wlasciwosci.swiezozniszczony && b2Body_IsValid(obj->body)) {
                obj->update(); 
                b2DestroyBody(obj->body); 
                std::cout << "[SYSTEM] Zniszczono fizyczny collider!\n";
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
                std::cout << "[SYSTEM] Ptak zniszczony, odpinam.\n";
            } 
            else if (aktualny_ptak->wlasciwosci.stan == StanPtaka::LECI) {
                czas_lotu_ptaka -= deltaTime;
                
                if (czas_lotu_ptaka <= 0.0f) {
                    aktualny_ptak->wlasciwosci.hp = 0.f;
                    aktualny_ptak->wlasciwosci.swiezozniszczony = true; 
                    aktualny_ptak->wlasciwosci.czas_wybuchu = 1.2f; 
                    
                    aktualny_ptak = nullptr; 
                    czas_lotu_ptaka = 3.0f;  
                    std::cout << "[SYSTEM] Czas lotu minął! Ptak wyparował.\n";
                }
            }
        }

        if (aktualny_ptak == nullptr && !ptaki_w_kolejce.empty()) {
            aktualny_ptak = ptaki_w_kolejce.front(); 
            ptaki_w_kolejce.erase(ptaki_w_kolejce.begin()); 
            aktualny_ptak->wlasciwosci.stan = StanPtaka::CZEKA_NA_PROCY; 
            
            b2Body_SetTransform(aktualny_ptak->body, { 250.f / Skala, 600.f / Skala }, b2Body_GetRotation(aktualny_ptak->body));
            managerMocy.odnow_umiejetnosci();
            
            czas_lotu_ptaka = 3.0f; 
            std::cout << "[SYSTEM] Załadowano kolejnego ptaka na procę.\n";
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

        if (znalezionoSwinie && zyweSwinie == 0) return 1; 
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