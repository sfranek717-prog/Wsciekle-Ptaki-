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
    std::vector<FizycznyObiekt*> obiekty; // kontener na wszystkie obiekty (wykorzystanie polimorfizmu)
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

    // ZMIANA: mat_silnik zmienione na mat_nieb dla czytelności!
    Lvl2(Material& mat_czerw, Material& mat_bomba, Material& mat_zoltek,  
         Material& mat_swinia, Material& mat_swinia_zolnierz, Material& mat_swinia_dziad,
         Material& mat_drw_kwad, Material& mat_drw_belka, Material& mat_drw_troj,
         Material& mat_lod_kwad, Material& mat_lod_belka, Material& mat_lod_troj,
         Material& mat_kam_kwad, Material& mat_kam_belka, Material& mat_kam_troj,
         Material& mat_nieb, Silnik_Obrazen& silauderzenia) 
    {
        // ustawienia tła poziomu
        tlo.loadFromFile("../tekstury/lvl2.png");
        tlo.setSmooth(true);
        tlo_lvl.setTexture(&tlo);
        tlo_lvl.setPosition({0.f,0.f});
        tlo_lvl.setSize({1920.f,1080.f});

        // inicjalizacja świata box2d z grawitacją pionową
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 15.1f };
        worldId = b2CreateWorld(&worldDef);
        
        // ładowanie zasobów graficznych efektów
        wybuch.loadFromFile("../tekstury/BOOM.png");
        chmura.loadFromFile("../tekstury/chmura_otoczka.png");
        
        kulaognia.setTexture(&chmura);
        kaboom.setTexture(&wybuch);
        kaboom.setSize({120.f, 120.f});  
        kaboom.setOrigin({60.f, 60.f}); 
        
        proca = new ObslugaProcy({ 250.f, 600.f });

        // tworzenie statycznego podłoża poziomu (góra podłogi wychodzi na y = 810.f)
        podloga = new BlokGry(worldId, mat_kam_belka, 960.f, 960.f, 1920.f, 300.f, TypKsztaltu::PROSTOKAT);
        b2Body_SetType(podloga->body, b2_staticBody);
        obiekty.push_back(podloga);
        podloga->niewidzialny = true;
        podloga->wlasciwosci.hp = 10000000.f;

        // konstrukcja drewniana - filary i stropy
        // warstwa 1 (stoją na podłodze y = 810): środki y = 810 - 80 = 730
        FizycznyObiekt* filarL1 = new BlokGry(worldId, mat_kam_belka, 1200.f, 730.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS1 = new BlokGry(worldId, mat_lod_belka, 1450.f, 730.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP1 = new BlokGry(worldId, mat_kam_belka, 1700.f, 730.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL1); obiekty.push_back(filarS1); obiekty.push_back(filarP1);

        // warstwa 2: stropy na wysokości 40 leżą na filarach (góra y = 650), środki y = 650 - 20 = 630
        FizycznyObiekt* stropL1 = new BlokGry(worldId, mat_drw_belka, 1325.f, 630.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* stropP1 = new BlokGry(worldId, mat_drw_belka, 1575.f, 630.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropL1); obiekty.push_back(stropP1);

        // warstwa 3: filary o wysokości 120 leżą na stropach (góra y = 610), środki y = 610 - 60 = 550
        FizycznyObiekt* filarL2 = new BlokGry(worldId, mat_lod_belka, 1250.f, 550.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS2 = new BlokGry(worldId, mat_kam_belka, 1450.f, 550.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP2 = new BlokGry(worldId, mat_lod_belka, 1650.f, 550.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL2); obiekty.push_back(filarS2); obiekty.push_back(filarP2);

        // warstwa 4: strop dachu o wysokości 30 leży na filarach 2 (góra y = 490), środek y = 490 - 15 = 475
        FizycznyObiekt* strop2 = new BlokGry(worldId, mat_kam_belka, 1450.f, 475.f, 440.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(strop2);

        // warstwa 5: małe belki o wysokości 80 stoją na stropie 2 (góra y = 460), środki y = 460 - 40 = 420
        FizycznyObiekt* malyL = new BlokGry(worldId, mat_drw_belka, 1350.f, 420.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* malyP = new BlokGry(worldId, mat_drw_belka, 1550.f, 420.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(malyL); obiekty.push_back(malyP);

        // warstwa 6: górna belka dachowa o wysokości 30 leży na małych belkach (góra y = 380), środek y = 380 - 15 = 365
        FizycznyObiekt* dachBelka = new BlokGry(worldId, mat_lod_belka, 1450.f, 365.f, 260.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(dachBelka);

        // warstwa 7: trójkąty o wysokości 60 na samej górze belki (góra y = 350), środki y = 350 - 30 = 320
        FizycznyObiekt* trojkatL = new BlokGry(worldId, mat_lod_troj, 1370.f, 320.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        FizycznyObiekt* trojkatP = new BlokGry(worldId, mat_lod_troj, 1530.f, 320.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        obiekty.push_back(trojkatL); obiekty.push_back(trojkatP);

        // rozmieszczenie przeciwników na konstrukcji
        FizycznyObiekt* swinia1 = new SwiniaGry(worldId, mat_swinia_zolnierz, 1325.f, 765.f, 45.f);    
        FizycznyObiekt* swinia2 = new SwiniaGry(worldId, mat_swinia_zolnierz, 1575.f, 765.f, 45.f);
        FizycznyObiekt* swinia3 = new SwiniaGry(worldId, mat_swinia, 1350.f, 575.f, 35.f);
        FizycznyObiekt* swinia4 = new SwiniaGry(worldId, mat_swinia, 1550.f, 575.f, 35.f);
        FizycznyObiekt* swinia5 = new SwiniaGry(worldId, mat_swinia_zolnierz, 1450.f, 420.f, 40.f);
        FizycznyObiekt* swinia6 = new SwiniaGry(worldId, mat_swinia_dziad, 1450.f, 310.f, 40.f);

        obiekty.push_back(swinia1);
        obiekty.push_back(swinia2);
        obiekty.push_back(swinia3);
        obiekty.push_back(swinia4);
        obiekty.push_back(swinia5);
        obiekty.push_back(swinia6);
    
        // ZMIANA: inicjalizacja ptaków gracza, przesunięte X i dodany niebieski
        FizycznyObiekt* ptak1 = new PtakGry(worldId, mat_czerw, 200.f, 900.f, 40.f);
        FizycznyObiekt* ptak2 = new PtakGry(worldId, mat_bomba, 150.f, 900.f, 40.f);
        FizycznyObiekt* ptak3 = new BlokGry(worldId, mat_zoltek, 100.f, 900.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        FizycznyObiekt* ptak4 = new PtakGry(worldId, mat_nieb, 50.f, 900.f, 25.f);       
        // blokowanie fizyki przed wystrzałem (ciała statyczne)
        b2Body_SetType(ptak1->body, b2_staticBody);
        b2Body_SetType(ptak2->body, b2_staticBody);
        b2Body_SetType(ptak3->body, b2_staticBody);
        b2Body_SetType(ptak4->body, b2_staticBody);
        
        obiekty.push_back(ptak1);
        ptak1->wlasciwosci.typulec = Typ::PTAK;
        ptak1->wlasciwosci.typek = RodzajPtaka::CZERWONIAK;

        obiekty.push_back(ptak2);
        ptak2->wlasciwosci.typulec = Typ::PTAK;
        ptak2->wlasciwosci.typek = RodzajPtaka::BOMBA;

        obiekty.push_back(ptak3);
        ptak3->wlasciwosci.typulec = Typ::PTAK; 
        ptak3->wlasciwosci.typek = RodzajPtaka::ZOLTEK;

        obiekty.push_back(ptak4);
        ptak4->wlasciwosci.typulec = Typ::PTAK; 
        ptak4->wlasciwosci.typek = RodzajPtaka::NIEBIESKI;

        // konfiguracja kolejki amunicji
        ptaki_w_kolejce.push_back(ptak1);
        ptaki_w_kolejce.push_back(ptak2);
        ptaki_w_kolejce.push_back(ptak3);
        ptaki_w_kolejce.push_back(ptak4);

        // załadowanie pierwszego ptaka na procę
        aktualny_ptak = ptaki_w_kolejce.front(); 
        ptaki_w_kolejce.erase(ptaki_w_kolejce.begin()); 
        aktualny_ptak->wlasciwosci.stan = StanPtaka::CZEKA_NA_PROCY;
        
        b2Body_SetTransform(aktualny_ptak->body, { 250.f / Skala, 600.f / Skala }, b2Body_GetRotation(aktualny_ptak->body));
        managerMocy.odnow_umiejetnosci();
    }

    ~Lvl2()
    {
        // zwolnienie pamięci dynamicznej i usunięcie świata box2d
        for (auto obj : obiekty) delete obj;
        delete proca;
        b2DestroyWorld(worldId);
    }

    b2WorldId getWorldId() const { return worldId; }

    void obslugujZdarzenia(const sf::Event& event, sf::RenderWindow& window)
    {
        if (!active || menu) return;

        if (aktualny_ptak != nullptr) {
            // obsługa sterowania procą oraz aktywacji umiejętności
            proca->ObslugaWystrzalu(event, window, *aktualny_ptak, czas);
            managerMocy.specjalnefunkcjeptakow(aktualny_ptak, event, obiekty, kulaognia, rysuj_kule, promien_kuli, wiatr, rysuj_wiatr, czas_wiatru);

            // detonacja ptaka-bomby i zainicjowanie fali uderzeniowej
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

        // aktualizacja symulacji fizycznej i detekcja kolizji
        b2World_Step(worldId, deltaTime, 4);
        SystemKolizji::przetworzZderzenia(worldId, obrazenia);

        // skalowanie efektu wizualnego eksplozji bomby
        if (rysuj_kule) {
            promien_kuli += 1000.f * deltaTime; 
            kulaognia.setRadius(promien_kuli);
            kulaognia.setOrigin({promien_kuli, promien_kuli}); 
            
            if (promien_kuli >= 400.f) {
                rysuj_kule = false;
            }
        }

        // zarządzanie stanem obiektów i usuwanie zniszczonych ciał z box2d
        for (auto obj : obiekty) {
            if (obj->wlasciwosci.swiezozniszczony && b2Body_IsValid(obj->body)) {
                obj->update(); 
                b2DestroyBody(obj->body); 
            }

            // odliczanie czasu wyświetlania grafiki wybuchu
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

        // monitorowanie stanu i czasu lotu wystrzelonego ptaka
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

        // automatyczne załadowanie następnego ptaka z kolejki
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

        // zliczanie aktywnych elementów na planszy
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

        // weryfikacja warunków końca rozgrywki
        if (znalezionoSwinie && zyweSwinie == 0) {
            return 1; // wygrana (brak świń)
        }
        else if (znalezionoptaki && zyweptaki == 0 && zyweSwinie > 0) {
            return 2; // przegrana (brak ptaków)
        }

        return 0; 
    }

    void draw(sf::RenderWindow& window, float deltaTime)
    {
        // renderowanie tła i elementów stałych
        window.draw(tlo_lvl);
        proca->drawWielowarstwowy(window, aktualny_ptak);
        
        if (rysuj_kule) {
            window.draw(kulaognia);
        }
        
        // renderowanie obiektów planszy i efektów zanikania wybuchów (alfa-blending)
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
            
            // polimorficzne wywołanie rysowania konkretnego obiektu
            obj->rysuj(window);
        }
    }
};

#endif