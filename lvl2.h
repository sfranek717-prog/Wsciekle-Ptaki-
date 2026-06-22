#ifndef LVL2_H
#define LVL2_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <box2d/box2d.h> 
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
    std::vector<FizycznyObiekt*> obiekty; // Kontener na wszystkie obiekty (wykorzystanie polimorfizmu)
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
    SpecjalnePtaki managerMocy;

    Lvl2(Material& mat_czerw, Material& mat_bomba, Material& mat_zoltek,  
         Material& mat_swinia, Material& mat_swinia_zolnierz, Material& mat_swinia_dziad,
         Material& mat_drw_kwad, Material& mat_drw_belka, Material& mat_drw_troj,
         Material& mat_lod_kwad, Material& mat_lod_belka, Material& mat_lod_troj,
         Material& mat_kam_kwad, Material& mat_kam_belka, Material& mat_kam_troj,
         Silnik_Obrazen& silauderzenia) 
    {
        // Ustawienia tła poziomu
        tlo.loadFromFile("../tekstury/lvl2.png");
        tlo.setSmooth(true);
        tlo_lvl.setTexture(&tlo);
        tlo_lvl.setPosition({0.f,0.f});
        tlo_lvl.setSize({1920.f,1080.f});

        // Inicjalizacja świata Box2D z grawitacją pionową
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 15.1f };
        worldId = b2CreateWorld(&worldDef);
        
        // Ładowanie zasobów graficznych efektów
        wybuch.loadFromFile("../tekstury/BOOM.png");
        chmura.loadFromFile("../tekstury/chmura_otoczka.png");
        
        kulaognia.setTexture(&chmura);
        kaboom.setTexture(&wybuch);
        kaboom.setSize({120.f, 120.f});  
        kaboom.setOrigin({60.f, 60.f}); 
        
        proca = new ObslugaProcy({ 250.f, 600.f });

        // Tworzenie statycznego podłoża poziomu
        podloga = new BlokGry(worldId, mat_drw_belka, 960.f, 1100.f, 1920.f, 300.f, TypKsztaltu::PROSTOKAT);
        b2Body_SetType(podloga->body, b2_staticBody);
        obiekty.push_back(podloga);
        podloga->niewidzialny = true;
        podloga->wlasciwosci.hp = 10000000.f;

        // Konstrukcja drewniana - filary i stropy
        FizycznyObiekt* filarL1 = new BlokGry(worldId, mat_drw_belka, 1200.f, 870.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS1 = new BlokGry(worldId, mat_drw_belka, 1450.f, 870.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP1 = new BlokGry(worldId, mat_drw_belka, 1700.f, 870.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL1); obiekty.push_back(filarS1); obiekty.push_back(filarP1);

        FizycznyObiekt* stropL1 = new BlokGry(worldId, mat_drw_belka, 1325.f, 770.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* stropP1 = new BlokGry(worldId, mat_drw_belka, 1575.f, 770.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropL1); obiekty.push_back(stropP1);

        FizycznyObiekt* filarL2 = new BlokGry(worldId, mat_drw_belka, 1250.f, 690.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS2 = new BlokGry(worldId, mat_drw_belka, 1450.f, 690.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP2 = new BlokGry(worldId, mat_drw_belka, 1650.f, 690.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL2); obiekty.push_back(filarS2); obiekty.push_back(filarP2);

        FizycznyObiekt* strop2 = new BlokGry(worldId, mat_drw_belka, 1450.f, 615.f, 440.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(strop2);

        FizycznyObiekt* malyL = new BlokGry(worldId, mat_drw_belka, 1350.f, 560.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* malyP = new BlokGry(worldId, mat_drw_belka, 1550.f, 560.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(malyL); obiekty.push_back(malyP);

        FizycznyObiekt* dachBelka = new BlokGry(worldId, mat_drw_belka, 1450.f, 505.f, 260.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(dachBelka);

        FizycznyObiekt* trojkatL = new BlokGry(worldId, mat_drw_troj, 1370.f, 460.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        FizycznyObiekt* trojkatP = new BlokGry(worldId, mat_drw_troj, 1530.f, 460.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        obiekty.push_back(trojkatL); obiekty.push_back(trojkatP);

        // Rozmieszczenie przeciwników na konstrukcji
        FizycznyObiekt* swinia1 = new SwiniaGry(worldId, mat_swinia_zolnierz, 1325.f, 905.f, 45.f);    
        FizycznyObiekt* swinia2 = new SwiniaGry(worldId, mat_swinia_zolnierz, 1575.f, 905.f, 45.f);
        FizycznyObiekt* swinia3 = new SwiniaGry(worldId, mat_swinia, 1350.f, 715.f, 35.f);
        FizycznyObiekt* swinia4 = new SwiniaGry(worldId, mat_swinia, 1550.f, 715.f, 35.f);
        FizycznyObiekt* swinia5 = new SwiniaGry(worldId, mat_swinia_zolnierz, 1450.f, 560.f, 40.f);
        FizycznyObiekt* swinia6 = new SwiniaGry(worldId, mat_swinia_dziad, 1450.f, 450.f, 40.f);

        obiekty.push_back(swinia1);
        obiekty.push_back(swinia2);
        obiekty.push_back(swinia3);
        obiekty.push_back(swinia4);
        obiekty.push_back(swinia5);
        obiekty.push_back(swinia6);
    
        // Inicjalizacja ptaków gracza
        FizycznyObiekt* ptak1 = new PtakGry(worldId, mat_czerw, 150.f, 900.f, 40.f);
        FizycznyObiekt* ptak2 = new PtakGry(worldId, mat_bomba, 100.f, 900.f, 40.f);
        FizycznyObiekt* ptak3 = new BlokGry(worldId, mat_zoltek, 50.f, 900.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        
        // Blokowanie fizyki przed wystrzałem (ciała statyczne)
        b2Body_SetType(ptak1->body, b2_staticBody);
        b2Body_SetType(ptak2->body, b2_staticBody);
        b2Body_SetType(ptak3->body, b2_staticBody);
        
        obiekty.push_back(ptak1);
        ptak1->wlasciwosci.typek = RodzajPtaka::CZERWONIAK;

        obiekty.push_back(ptak2);
        ptak2->wlasciwosci.typek = RodzajPtaka::BOMBA;

        obiekty.push_back(ptak3);
        ptak3->wlasciwosci.typulec = Typ::PTAK; 
        ptak3->wlasciwosci.typek = RodzajPtaka::ZOLTEK;

        // Konfiguracja kolejki amunicji
        ptaki_w_kolejce.push_back(ptak1);
        ptaki_w_kolejce.push_back(ptak2);
        ptaki_w_kolejce.push_back(ptak3);

        // Załadowanie pierwszego ptaka na procę
        aktualny_ptak = ptaki_w_kolejce.front(); 
        ptaki_w_kolejce.erase(ptaki_w_kolejce.begin()); 
        aktualny_ptak->wlasciwosci.stan = StanPtaka::CZEKA_NA_PROCY;
        
        b2Body_SetTransform(aktualny_ptak->body, { 250.f / Skala, 600.f / Skala }, b2Body_GetRotation(aktualny_ptak->body));
        managerMocy.odnow_umiejetnosci();
    }

    ~Lvl2()
    {
        // Zwolnienie pamięci dynamicznej i usunięcie świata Box2D
        for (auto obj : obiekty) delete obj;
        delete proca;
        b2DestroyWorld(worldId);
    }

    b2WorldId getWorldId() const { return worldId; }

    void obslugujZdarzenia(const sf::Event& event, sf::RenderWindow& window)
    {
        if (!active || menu) return;

        if (aktualny_ptak != nullptr) {
            // Obsługa sterowania procą oraz aktywacji umiejętności
            proca->ObslugaWystrzalu(event, window, *aktualny_ptak, czas);
            managerMocy.specjalnefunkcjeptakow(aktualny_ptak, event, obiekty, kulaognia, rysuj_kule, promien_kuli, wiatr, rysuj_wiatr, czas_wiatru);

            // Detonacja ptaka-bomby i zainicjowanie fali uderzeniowej
            if (rysuj_kule && !aktualny_ptak->wlasciwosci.swiezozniszczony && aktualny_ptak->wlasciwosci.typek == RodzajPtaka::BOMBA) {
                sf::Vector2f pozBomby = (aktualny_ptak->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? 
                                         aktualny_ptak->pro.getPosition() : aktualny_ptak->kol.getPosition();
                
                kulaognia.setPosition(pozBomby);

                aktualny_ptak->wlasciwosci.hp = 0.f;
                aktualny_ptak->wlasciwosci.swiezozniszczony = true; 
                aktualny_ptak->wlasciwosci.czas_wybuchu = 1.2f; 
            }
        }
    }

    void update(float deltaTime, sf::RenderWindow& window, Silnik_Obrazen &obrazenia)
    {
        if (!active || menu) return;
        if (czas_bezpieczenstwa > 0.0f) {
            czas_bezpieczenstwa -= deltaTime;
        }

        // Aktualizacja symulacji fizycznej i detekcja kolizji
        b2World_Step(worldId, deltaTime, 4);
        SystemKolizji::przetworzZderzenia(worldId, obrazenia);

        // Skalowanie efektu wizualnego eksplozji bomby
        if (rysuj_kule) {
            promien_kuli += 1000.f * deltaTime; 
            kulaognia.setRadius(promien_kuli);
            kulaognia.setOrigin({promien_kuli, promien_kuli}); 
            
            if (promien_kuli >= 400.f) {
                rysuj_kule = false;
            }
        }

        // Zarządzanie stanem obiektów i usuwanie zniszczonych ciał z Box2D
        for (auto obj : obiekty) {
            if (obj->wlasciwosci.swiezozniszczony && b2Body_IsValid(obj->body)) {
                obj->update(); 
                b2DestroyBody(obj->body); 
            }

            // Odliczanie czasu wyświetlania grafiki wybuchu
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

        // Monitorowanie stanu i czasu lotu wystrzelonego ptaka
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

        // Automatyczne załadowanie następnego ptaka z kolejki
        if (aktualny_ptak == nullptr && !ptaki_w_kolejce.empty()) {
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

        // Zliczanie aktywnych elementów na planszy
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

        // Weryfikacja warunków końca rozgrywki
        if (znalezionoSwinie && zyweSwinie == 0) {
            return 1; // Wygrana (brak świń)
        }
        else if (znalezionoptaki && zyweptaki == 0 && zyweSwinie > 0) {
            return 2; // Przegrana (brak ptaków)
        }

        return 0; 
    }

    void draw(sf::RenderWindow& window, float deltaTime)
    {
        // Renderowanie tła i elementów stałych
        window.draw(tlo_lvl);
        proca->drawWielowarstwowy(window, aktualny_ptak);
        
        if (rysuj_kule) {
            window.draw(kulaognia);
        }
        
        // Renderowanie obiektów planszy i efektów zanikania wybuchów (alfa-blending)
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
            
            // Polimorficzne wywołanie rysowania konkretnego obiektu
            obj->rysuj(window);
        }
    }
};

#endif