#ifndef LVL11_H
#define LVL11_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <box2d/box2d.h> 
#include <iostream> 
#include "obiekt.h"
#include "FizycznyObiekt.h"
#include "ObslugaProcy.h"
#include "SpecjalnePtaki.h"
#include "FizycznyDebbugger.h" 
#include "SystemKolizji.h" 

class Lvl11
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

    bool active = true;
    bool menu = false;
    bool koniec = false;
    float czas = 0.f;
    bool rysuj_wiatr = false;
    float czas_wiatru = 10.f;
    
    float czas_bezpieczenstwa = 0.2f;
    float czas_lotu_ptaka = 3.f; 
      sf::Texture podmuch;
    sf::RectangleShape wiatr;

public:
    bool wygrana = false;

    bool aktwyny = true;
    FizycznyDebugger debugger; 
    SpecjalnePtaki managerMocy;

Lvl11(Material& mat_czerw,Material& mat_bomba, Material& mat_zoltek,  
          Material& mat_swinia, Material& mat_swinia_zolnierz, Material& mat_swinia_dziad,
          Material& mat_drw_kwad, Material& mat_drw_belka, Material& mat_drw_troj,
          Material& mat_lod_kwad, Material& mat_lod_belka, Material& mat_lod_troj,
          Material& mat_kam_kwad, Material& mat_kam_belka, Material& mat_kam_troj,
          Silnik_Obrazen& silauderzenia) 
    {
        tlo.loadFromFile("../tekstury/lvl1.jpg");
        tlo.setSmooth(true);
        tlo_lvl.setTexture(&tlo);
        tlo_lvl.setPosition({0.f,0.f});
        tlo_lvl.setSize({1920.f,1080.f});

        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 15.1f };
        worldId = b2CreateWorld(&worldDef);
        
        if(!wybuch.loadFromFile("../tekstury/BOOM.png"))
        {
            std::cout << "Nie laduje wybuchu\n";
        }
        
        if(!chmura.loadFromFile("../tekstury/chmura_otoczka.png"))
        {
            std::cout << "Nie laduje chmury\n";
        }
        
        kulaognia.setTexture(&chmura);
        kaboom.setTexture(&wybuch);
        kaboom.setSize({120.f, 120.f});  
        kaboom.setOrigin({60.f, 60.f}); 
        
        proca = new ObslugaProcy({ 250.f, 600.f });

        // --- PODŁOGA ---
        podloga = new FizycznyObiekt(worldId, mat_drw_belka, 960.f, 1100.f, 1920.f, 300.f, TypKsztaltu::PROSTOKAT);
        b2Body_SetType(podloga->body, b2_staticBody);
        obiekty.push_back(podloga);
        podloga->niewidzialny = true;
        podloga->wlasciwosci.hp = 10000000.f;

        // ==========================================
        //    POTĘŻNA KONSTRUKCJA (POPRAWIONE WSPÓŁRZĘDNE)
        // ==========================================

        // --- PARTER: DUŻE, CIĘŻKIE FILARY (Drewno) ---
        FizycznyObiekt* filarL1 = new FizycznyObiekt(worldId, mat_drw_belka, 1200.f, 870.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS1 = new FizycznyObiekt(worldId, mat_drw_belka, 1450.f, 870.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP1 = new FizycznyObiekt(worldId, mat_drw_belka, 1700.f, 870.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL1); obiekty.push_back(filarS1); obiekty.push_back(filarP1);

        // --- STROP 1: DŁUGIE DESKI ---
        FizycznyObiekt* stropL1 = new FizycznyObiekt(worldId, mat_drw_belka, 1325.f, 770.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* stropP1 = new FizycznyObiekt(worldId, mat_drw_belka, 1575.f, 770.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropL1); obiekty.push_back(stropP1);

        // --- 1. PIĘTRO: SMUKLEJSZE KLOCKI ---
        FizycznyObiekt* filarL2 = new FizycznyObiekt(worldId, mat_drw_belka, 1250.f, 690.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS2 = new FizycznyObiekt(worldId, mat_drw_belka, 1450.f, 690.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP2 = new FizycznyObiekt(worldId, mat_drw_belka, 1650.f, 690.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL2); obiekty.push_back(filarS2); obiekty.push_back(filarP2);

        // --- STROP 2: ŚRODKOWA PLATFORMA ---
        FizycznyObiekt* strop2 = new FizycznyObiekt(worldId, mat_drw_belka, 1450.f, 615.f, 440.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(strop2);

        // --- 2. PIĘTRO: MAŁE WSPORNIKI ---
        FizycznyObiekt* malyL = new FizycznyObiekt(worldId, mat_drw_belka, 1350.f, 560.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* malyP = new FizycznyObiekt(worldId, mat_drw_belka, 1550.f, 560.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(malyL); obiekty.push_back(malyP);

        // --- DACH: BELKA + OZDOBNE TRÓJKĄTY ---
        FizycznyObiekt* dachBelka = new FizycznyObiekt(worldId, mat_drw_belka, 1450.f, 505.f, 260.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(dachBelka);

        FizycznyObiekt* trojkatL = new FizycznyObiekt(worldId, mat_drw_troj, 1370.f, 460.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        FizycznyObiekt* trojkatP = new FizycznyObiekt(worldId, mat_drw_troj, 1530.f, 460.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        obiekty.push_back(trojkatL); obiekty.push_back(trojkatP);

        // ==========================================
        //             ARMIA 6 ŚWIŃ
        // ==========================================

        FizycznyObiekt* swinia1 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1325.f, 905.f, 45.f, 0.f, TypKsztaltu::KOLO);    
        FizycznyObiekt* swinia2 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1575.f, 905.f, 45.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* swinia3 = new FizycznyObiekt(worldId, mat_swinia, 1350.f, 715.f, 35.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* swinia4 = new FizycznyObiekt(worldId, mat_swinia, 1550.f, 715.f, 35.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* swinia5 = new FizycznyObiekt(worldId, mat_swinia_zolnierz, 1450.f, 560.f, 40.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* swinia6 = new FizycznyObiekt(worldId, mat_swinia_dziad, 1450.f, 450.f, 40.f, 0.f, TypKsztaltu::KOLO);

        obiekty.push_back(swinia1);
        obiekty.push_back(swinia2);
        obiekty.push_back(swinia3);
        obiekty.push_back(swinia4);
        obiekty.push_back(swinia5);
        obiekty.push_back(swinia6);

        // Ustawianie typu dla każdej świni
        swinia1->wlasciwosci.typulec = Typ::SWINIA;
        swinia2->wlasciwosci.typulec = Typ::SWINIA;
        swinia3->wlasciwosci.typulec = Typ::SWINIA;
        swinia4->wlasciwosci.typulec = Typ::SWINIA;
        swinia5->wlasciwosci.typulec = Typ::SWINIA;
        swinia6->wlasciwosci.typulec = Typ::SWINIA;
    

        FizycznyObiekt* ptak1 = new FizycznyObiekt(worldId, mat_czerw, 150.f, 900.f, 40.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* ptak2 = new FizycznyObiekt(worldId, mat_bomba, 100.f, 900.f, 40.f, 0.f, TypKsztaltu::KOLO);
        FizycznyObiekt* ptak3 = new FizycznyObiekt(worldId, mat_zoltek, 50.f, 900.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        
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

    ~Lvl11()
    {
        for (auto obj : obiekty) delete obj;
        delete proca;
        b2DestroyWorld(worldId);
    }

    b2WorldId getWorldId() const { return worldId; }

void obslugujZdarzenia(const sf::Event& event, sf::RenderWindow& window)
    {
        if (!active || menu) return;

        if (debugger.czyAktywny()) {
            std::vector<FizycznyObiekt*> aktywne_obiekty;
            for (auto obj : obiekty) {
                if (obj != nullptr && obj->wlasciwosci.aktywny && !obj->wlasciwosci.swiezozniszczony && b2Body_IsValid(obj->body)) {
                    aktywne_obiekty.push_back(obj);
                }
            }
            debugger.obslugujZdarzenia(event, window, aktywne_obiekty);
        } else {
            if (aktualny_ptak != nullptr) {
                proca->ObslugaWystrzalu(event, window, *aktualny_ptak, czas);
                
                // Wywołujemy mechanizm speców
                managerMocy.specjalnefunkcjeptakow(aktualny_ptak, event, obiekty, kulaognia, rysuj_kule, promien_kuli, wiatr, rysuj_wiatr, czas_wiatru);

                // NOWOŚĆ: Jeśli po kliknięciu odpaliła się kula ognia, a ptak jeszcze nie został oznaczony jako zniszczony:
                if (rysuj_kule && !aktualny_ptak->wlasciwosci.swiezozniszczony && aktualny_ptak->wlasciwosci.typek == RodzajPtaka::BOMBA) {
                    // 1. Pobieramy aktualną pozycję bomby (zależnie od kształtu)
                    sf::Vector2f pozBomby = (aktualny_ptak->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? 
                                             aktualny_ptak->pro.getPosition() : aktualny_ptak->kol.getPosition();
                    
                    // 2. Blokujemy chmurę w tym miejscu
                    kulaognia.setPosition(pozBomby);

                    // 3. Zabijamy ptaka na miejscu, żeby zniknął i wywołał efekt kaboom
                    aktualny_ptak->wlasciwosci.hp = 0.f;
                    aktualny_ptak->wlasciwosci.swiezozniszczony = true; 
                    aktualny_ptak->wlasciwosci.czas_wybuchu = 1.2f; 
                    std::cout << "[SYSTEM] Bomba zdetonowana przez gracza! Znika.\n";
                }
            }
        }
    }

 void update(float deltaTime, sf::RenderWindow& window, Silnik_Obrazen &obrazenia)
    {
        if (!active || menu) return;
        if (czas_bezpieczenstwa > 0.0f) {
            czas_bezpieczenstwa -= deltaTime;
        }

        bool stanDebuggera = debugger.czyAktywny();
        debugger.update(window);

        if (stanDebuggera) {
            for (auto obj : obiekty) {
                if (obj != nullptr && obj->wlasciwosci.aktywny && !obj->wlasciwosci.swiezozniszczony && b2Body_IsValid(obj->body)) {
                    obj->update(); 
                }
            }
        } else {
            b2World_Step(worldId, deltaTime, 4);
            SystemKolizji::przetworzZderzenia(worldId, obrazenia);

            // Logika wielkiej chmury wybuchu
            if (rysuj_kule) {
                promien_kuli += 1000.f * deltaTime; 
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
    }
    int sprawdzStanGry() const {
    if (czas_bezpieczenstwa > 0.0f) return 0; // Gra trwa

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

    if (znalezionoSwinie && zyweSwinie == 0) {
        return 1; // WYGRANA!
    }
    else if (znalezionoptaki && zyweptaki == 0 && zyweSwinie > 0) {
        return 2; // PRZEGRANA!
    }

    return 0; // Gra trwa nadal
}
void draw(sf::RenderWindow& window, float deltaTime)
    {
        window.draw(tlo_lvl);
        proca->drawWielowarstwowy(window, aktualny_ptak);
        
        // Rysujemy chmurę ognia 
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

        // debugger
        if (debugger.czyAktywny()) {
            debugger.draw(window, obiekty); 
        }        
    }
};

#endif