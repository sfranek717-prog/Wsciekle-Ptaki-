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
    // przelicznik zeby fizyka box2d pasowala do pikseli na ekranie
    const float Skala = 30.f;
    bool rysuj_kule = false;
    float promien_kuli = 60.f;
    
    // zmienne od box2d
    b2WorldId worldId;
    b2BodyId groundId{};
    
    // bajery graficzne do wybuchow i tla
    sf::Texture chmura;
    sf::CircleShape kulaognia;
    sf::Texture wybuch;
    sf::RectangleShape kaboom;
    sf::Texture tlo;
    sf::RectangleShape tlo_lvl;
    sf::Texture podmuch;
    sf::RectangleShape wiatr;

    // kontenery na obiekty na mapie
    std::vector<FizycznyObiekt*> ptaki_w_kolejce;
    std::vector<FizycznyObiekt*> obiekty; 
    FizycznyObiekt* aktualny_ptak;
    ObslugaProcy* proca;
    FizycznyObiekt* podloga;

    // flagi do sterowania gra
    bool active = true;
    bool menu = false;
    bool koniec = false;
    float czas = 0.f;
    bool rysuj_wiatr = false;
    float czas_wiatru = 10.f;
    
    // zeby nie wylaczalo od razu jak ptak spadnie
    float czas_bezpieczenstwa = 0.2f;
    float czas_lotu_ptaka = 3.f; 

public:
    bool wygrana = false;
    bool aktwyny = true;
    SpecjalnePtaki managerMocy;

    // mega dlugi konstruktor bo ladujemy tu caly level
    Lvl2(Material& mat_czerw, Material& mat_nieb, Material& mat_zoltek, Material& mat_bomba,  
         Material& mat_swinia, Material& mat_swinia_zolnierz, Material& mat_swinia_dziad,
         Material& mat_drw_kwad, Material& mat_drw_belka, Material& mat_drw_troj,
         Material& mat_kam_kwad, Material& mat_kam_belka, Material& mat_kam_troj,
         Material& mat_lod_kwad, Material& mat_lod_belka, Material& mat_lod_troj,
         Silnik_Obrazen& silauderzenia) 
    {
        // ustawiamy tlo na pelny ekran
        tlo.loadFromFile("../tekstury/lvl2.png");
        tlo.setSmooth(true);
        tlo_lvl.setTexture(&tlo);
        tlo_lvl.setPosition({0.f,0.f});
        tlo_lvl.setSize({1920.f,1080.f});

        // inicjalizacja fizyki i grawitacji w dol
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, 15.1f };
        worldId = b2CreateWorld(&worldDef);
        
        // ladowanie tekstur do efektow wybuchu
        wybuch.loadFromFile("../tekstury/BOOM.png");
        chmura.loadFromFile("../tekstury/chmura_otoczka.png");
        
        kulaognia.setTexture(&chmura);
        kaboom.setTexture(&wybuch);
        kaboom.setSize({120.f, 120.f});  
        kaboom.setOrigin({60.f, 60.f}); 
        
        // tworzymy proce i podloge zeby bylo na czym stawiac klocki
        proca = new ObslugaProcy({ 250.f, 600.f });

        podloga = new BlokGry(worldId, mat_kam_belka, 960.f, 960.f, 1920.f, 300.f, TypKsztaltu::PROSTOKAT);
        b2Body_SetType(podloga->body, b2_staticBody); // podloga sie nie rusza
        obiekty.push_back(podloga);
        podloga->niewidzialny = true;
        podloga->wlasciwosci.hp = 10000000.f; // duzo hp zeby nie pekla

        // --- BUDOWANIE PLANSZY ---
        // warstwa 1 z klockow
        FizycznyObiekt* filarL1 = new BlokGry(worldId, mat_kam_belka, 1200.f, 730.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS1 = new BlokGry(worldId, mat_lod_belka, 1450.f, 730.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP1 = new BlokGry(worldId, mat_kam_belka, 1700.f, 730.f, 60.f, 160.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL1); obiekty.push_back(filarS1); obiekty.push_back(filarP1);

        FizycznyObiekt* stropL1 = new BlokGry(worldId, mat_drw_belka, 1325.f, 630.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* stropP1 = new BlokGry(worldId, mat_drw_belka, 1575.f, 630.f, 280.f, 40.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(stropL1); obiekty.push_back(stropP1);

        // warstwa 2
        FizycznyObiekt* filarL2 = new BlokGry(worldId, mat_lod_belka, 1250.f, 550.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarS2 = new BlokGry(worldId, mat_kam_belka, 1450.f, 550.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* filarP2 = new BlokGry(worldId, mat_lod_belka, 1650.f, 550.f, 40.f, 120.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(filarL2); obiekty.push_back(filarS2); obiekty.push_back(filarP2);

        FizycznyObiekt* strop2 = new BlokGry(worldId, mat_kam_belka, 1450.f, 475.f, 440.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(strop2);

        // warstwa 3 i dach
        FizycznyObiekt* malyL = new BlokGry(worldId, mat_drw_belka, 1350.f, 420.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        FizycznyObiekt* malyP = new BlokGry(worldId, mat_drw_belka, 1550.f, 420.f, 30.f, 80.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(malyL); obiekty.push_back(malyP);

        FizycznyObiekt* dachBelka = new BlokGry(worldId, mat_lod_belka, 1450.f, 365.f, 260.f, 30.f, TypKsztaltu::PROSTOKAT);
        obiekty.push_back(dachBelka);

        FizycznyObiekt* trojkatL = new BlokGry(worldId, mat_lod_troj, 1370.f, 320.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        FizycznyObiekt* trojkatP = new BlokGry(worldId, mat_lod_troj, 1530.f, 320.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        obiekty.push_back(trojkatL); obiekty.push_back(trojkatP);

        // dorzucamy swinie zeby bylo do czego strzelac
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
    
        // generujemy ptaki za proca
        FizycznyObiekt* ptak1 = new PtakGry(worldId, mat_czerw, 200.f, 900.f, 40.f);
        FizycznyObiekt* ptak2 = new PtakGry(worldId, mat_bomba, 150.f, 900.f, 40.f);
        FizycznyObiekt* ptak3 = new BlokGry(worldId, mat_zoltek, 100.f, 900.f, 60.f, 60.f, TypKsztaltu::TROJKAT);
        FizycznyObiekt* ptak4 = new PtakGry(worldId, mat_nieb, 50.f, 900.f, 25.f);       
        
        // na starcie ptaki maja stac w miejscu i nie reagowac na grawitacje
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

        // wrzucamy do wektora zeby brac po kolei
        ptaki_w_kolejce.push_back(ptak1);
        ptaki_w_kolejce.push_back(ptak2);
        ptaki_w_kolejce.push_back(ptak3);
        ptaki_w_kolejce.push_back(ptak4);

        // bierzemy pierwszego ptaka z kolejki na proce
        aktualny_ptak = ptaki_w_kolejce.front(); 
        ptaki_w_kolejce.erase(ptaki_w_kolejce.begin()); 
        aktualny_ptak->wlasciwosci.stan = StanPtaka::CZEKA_NA_PROCY;
        
        b2Body_SetTransform(aktualny_ptak->body, { 250.f / Skala, 600.f / Skala }, b2Body_GetRotation(aktualny_ptak->body));
        managerMocy.odnow_umiejetnosci(); // reset skilli
    }

    ~Lvl2()
    {
        // sprzatanie pamieci zeby ram nie plakal
        for (auto obj : obiekty) delete obj;
        delete proca;
        b2DestroyWorld(worldId);
    }

    b2WorldId getWorldId() const { return worldId; }

    void obslugujZdarzenia(const sf::Event& event, sf::RenderWindow& window)
    {
        if (!active || menu) return;

        // klikanie i naciaganie
        if (aktualny_ptak != nullptr) {
            proca->ObslugaWystrzalu(event, window, *aktualny_ptak, czas);
            managerMocy.specjalnefunkcjeptakow(aktualny_ptak, event, obiekty, kulaognia, rysuj_kule, promien_kuli, wiatr, rysuj_wiatr, czas_wiatru);

            // obsluga zdolnosci bomby
            if (rysuj_kule && !aktualny_ptak->wlasciwosci.swiezozniszczony && aktualny_ptak->wlasciwosci.typek == RodzajPtaka::BOMBA) {
                sf::Vector2f pozBomby = (aktualny_ptak->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? 
                                         aktualny_ptak->pro.getPosition() : aktualny_ptak->kol.getPosition();
                
                kulaognia.setPosition(pozBomby);

                // zabijamy ptaka przy wybuchu
                aktualny_ptak->wlasciwosci.hp = 0.f;
                aktualny_ptak->wlasciwosci.swiezozniszczony = true; 
                aktualny_ptak->wlasciwosci.czas_wybuchu = 1.2f; 
            }
        }
    }

    void update(float deltaTime, sf::RenderWindow& window, Silnik_Obrazen &obrazenia)
    {
        if (!active || menu) return;
        
        // odliczanie czasu zeby kolizje nie zepsuly gry od razu po starcie
        if (czas_bezpieczenstwa > 0.0f) {
            czas_bezpieczenstwa -= deltaTime;
        }

        // posuwamy fizyke do przodu
        b2World_Step(worldId, deltaTime, 4);
        SystemKolizji::przetworzZderzenia(worldId, obrazenia);

        // jak zrobimy bum to powiekaszamy kule ognia
        if (rysuj_kule) {
            promien_kuli += 1000.f * deltaTime; 
            kulaognia.setRadius(promien_kuli);
            kulaognia.setOrigin({promien_kuli, promien_kuli}); 
            
            if (promien_kuli >= 400.f) {
                rysuj_kule = false; // chowamy po max zasiegu
            }
        }

        for (auto obj : obiekty) {
            // jak cos spadlo na 0 hp to usuwamy z box2d zeby juz nie kolidowalo
            if (obj->wlasciwosci.swiezozniszczony && b2Body_IsValid(obj->body)) {
                obj->update(); 
                b2DestroyBody(obj->body); 
            }

            // zanikanie animacji wybuchu
            if (obj->wlasciwosci.swiezozniszczony) {
                obj->wlasciwosci.czas_wybuchu -= deltaTime; 

                if (obj->wlasciwosci.czas_wybuchu <= 0.0f) {
                    obj->wlasciwosci.czas_wybuchu = 0.0f;
                    obj->wlasciwosci.swiezozniszczony = false; 
                    obj->wlasciwosci.aktywny = false;  // obiekt zupelnie martwy
                }
            }

            // synchronizacja pozycji fizyki z grafika
            if (obj->wlasciwosci.aktywny && !obj->wlasciwosci.swiezozniszczony) {
                obj->update();
            }
        }

        // sprawdzanie co u naszego ptaka
        if (aktualny_ptak != nullptr) {
            // jak ptak zginal albo wybuchl
            if (aktualny_ptak->wlasciwosci.swiezozniszczony || aktualny_ptak->wlasciwosci.hp <= 0.f) {
                aktualny_ptak = nullptr;
                czas_lotu_ptaka = 3.0f; 
            } 
            else if (aktualny_ptak->wlasciwosci.stan == StanPtaka::LECI) {
                // ptak moze zyc max 3 sekundy w locie a potem znika
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

        // jak obecny zniknal, a mamy jeszcze ptaki w rezerwie to bierzemy nastepnego
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
        if (czas_bezpieczenstwa > 0.0f) return 0; // zeby nie wygrac/przegrac od razu w 1 klatce

        bool znalezionoSwinie = false;
        bool znalezionoptaki = false;
        int zyweSwinie = 0;
        int zyweptaki = 0;

        // liczenie kto przezyl
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

        // brak swin = wygrana (1)
        if (znalezionoSwinie && zyweSwinie == 0) {
            return 1; 
        }
        // brak ptakow a swinie zyja = przegrana (2)
        else if (znalezionoptaki && zyweptaki == 0 && zyweSwinie > 0) {
            return 2; 
        }

        // gramy dalej
        return 0; 
    }

    void draw(sf::RenderWindow& window, float deltaTime)
    {
        // tlo i podstawa
        window.draw(tlo_lvl);
        proca->drawWielowarstwowy(window, aktualny_ptak);
        
        // efekty
        if (rysuj_kule) {
            window.draw(kulaognia);
        }
        
        // lecimy po wszystkich obiektach
        for (auto obj : obiekty) {
            // najpierw animacje wybuchu rozwalonych rzeczy
            if (obj->wlasciwosci.swiezozniszczony) {
                sf::Vector2f pozycjaBumu = (obj->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? obj->pro.getPosition() : obj->kol.getPosition();
                
                kaboom.setPosition(pozycjaBumu);
                kaboom.setRotation(sf::degrees(0.f)); 
                
                // zanikanie tekstury z czasem
                float alfa = (obj->wlasciwosci.czas_wybuchu / 1.2f) * 255.0f;   
                if (alfa < 0.f) alfa = 0.f;
                if (alfa > 255.f) alfa = 255.f;
                
                kaboom.setFillColor(sf::Color(255, 255, 255, alfa));    
                window.draw(kaboom);
                continue; 
            }

            // jak juz nieaktywny calkiem to go nie rysujemy
            if (!obj->wlasciwosci.aktywny) continue;
            
            // rysowanie fizycznego obiektu
            obj->rysuj(window);
        }
    }
};

#endif