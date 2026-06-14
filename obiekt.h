#ifndef OBIEKT_H
#define OBIEKT_H

#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <string>

enum class TypKsztaltu { KOLO, PROSTOKAT, TROJKAT };
enum StanPtaka {
    CZEKA_NA_PROCY, 
    NACIAGANY,      
    LECI,
    PO_LOCIE           
};
enum class RodzajPtaka {
    CZERWONIAK,
    BOMBA,
    ZOLTEK,
    NIEBIESKI
};

struct Material {
    std::string nazwa;
    float gestosc;     
    float wytrzymalosc; 
    bool zmieniamy = false;
    sf::Color kolor;
    sf::Texture tekstura; 
    sf::Texture uszkodzonatekstura;

};
enum class Typ
{
    BLOK,
    SWINIA,
    PTAK
};

class Obiekt {
public:
    float masa;
        bool swiezozniszczony = false;
        float czas_wybuchu = 1.2f;
    std::string nazwa_obiektu;
    float hp;
    bool aktywny = true;
    float vx = 0.0f; 
    float vy = 0.0f;
    Material mat; 
    TypKsztaltu typ;
    sf::Vector2f pos;
    RodzajPtaka typek;
    Typ typulec = Typ::BLOK;
    StanPtaka stan = StanPtaka::CZEKA_NA_PROCY;
    
    Obiekt() 
        : masa(1.0) , hp(100.0f), pos({0.0f, 0.0f}), typ(TypKsztaltu::PROSTOKAT), typek(RodzajPtaka::CZERWONIAK), typulec(Typ::BLOK){}

    Obiekt(float h) 
        : masa(1.0f),nazwa_obiektu("Nieznany"), hp(h), pos({0.0f, 0.0f}), typ(TypKsztaltu::PROSTOKAT) {}
    
    Obiekt(float m, float h, float px, float py, Material ma, TypKsztaltu t) 
        : masa(m), nazwa_obiektu(ma.nazwa), hp(h), mat(ma), typ(t), pos({px, py}) {}
        
    Obiekt(float h, std::string n) 
        : masa(1.0f), nazwa_obiektu(n), hp(h), pos({0.0f, 0.0f}), typ(TypKsztaltu::PROSTOKAT), aktywny(true) {}

    virtual ~Obiekt() {}

    static void zainicjalizujMaterialy(
        Material& ptak_czerw, Material& ptak_nieb, Material& ptak_zolt, Material& ptak_czar,
        Material& swinia, Material&swinia_zolnierz,Material& swinia_dziad,
        Material& drw_kulka, Material& drw_kwad, Material& drw_belka, Material& drw_troj,
        Material& kam_kulka, Material& kam_kwad, Material& kam_belka, Material& kam_troj,
        Material& lod_kulka, Material& lod_kwad, Material& lod_belka, Material& lod_troj
    ) {auto zrob = [](Material& m, std::string n, float g, float w, std::string plik, Typ ,TypKsztaltu) {
    m.nazwa = n;
    m.gestosc = g;
    m.wytrzymalosc = w;
    m.zmieniamy = false;

    m.tekstura.loadFromFile("../tekstury/" + plik);
    m.tekstura.setSmooth(true);

    if (!m.uszkodzonatekstura.loadFromFile("../tekstury/uszkodzona" + plik)) {
        m.uszkodzonatekstura = m.tekstura; 
    }
    m.uszkodzonatekstura.setSmooth(true);
};
   
        // ptaki
        zrob(ptak_czerw, "Czerwony", 10.0f, 200.f, "Czerwony_ptak.png", Typ::PTAK,TypKsztaltu::KOLO);
        zrob(ptak_nieb,  "Niebieski", 10.0f, 100.f, "Ptak_niebieski.png", Typ::PTAK,TypKsztaltu::KOLO);
        zrob(ptak_zolt,  "Zolty",    10.0f, 100.f, "Zolty_ptak.png",Typ::PTAK,TypKsztaltu::TROJKAT);
        zrob(ptak_czar,  "Czarny",   10.0f, 500.f, "Czarny_ptak.png",Typ::PTAK,TypKsztaltu::KOLO);

        // swinie
        zrob(swinia, "Swinia", 10.0f, 10.f, "Swinia.png",Typ::SWINIA,TypKsztaltu::KOLO);
        zrob(swinia_zolnierz, "Swinia zolnierz", 15.0f, 50.f, "swinia_zolnierz.png",Typ::SWINIA,TypKsztaltu::KOLO);
        zrob(swinia_dziad, "Swinia dziad", 10.0f, 25.f, "swinia_dziad.png",Typ::SWINIA ,TypKsztaltu::KOLO);

        // drewno
        // zrob(drw_kulka, "Drewno Kulka",   0.5f, 50.f, "kulka-drewno.png");
        zrob(drw_kwad,  "Drewno Kwadrat", 5.f, 100.f, "Kwadrat-drewno.png",Typ::BLOK,TypKsztaltu::PROSTOKAT);
        zrob(drw_belka, "Drewno Belka",   5.f, 100.f, "belka-drewno.png",Typ::BLOK,TypKsztaltu::PROSTOKAT);
        zrob(drw_troj,  "Drewno Trojkat", 0.5f, 100.f, "trojkat-drewno.png",Typ::BLOK,TypKsztaltu::TROJKAT);

        // kamien
        // zrob(kam_kulka, "Kamien Kulka",   2.5f, 200.f, "kulka-kamien.png");
        zrob(kam_kwad,  "Kamien Kwadrat", 25.f, 200.f, "Kwadrat-kamien.png",Typ::BLOK,TypKsztaltu::PROSTOKAT);
        zrob(kam_belka, "Kamien Belka",   25.f, 200.f, "belka-kamien.png",Typ::BLOK, TypKsztaltu::PROSTOKAT);
        zrob(kam_troj,  "Kamien Trojkat", 25.f, 200.f, "trojkat-kamien.png",Typ::BLOK,TypKsztaltu::TROJKAT);

        // lod
        // zrob(lod_kulka, "Lod Kulka",   0.1f, 10.f, "kulka-lod.png");
        zrob(lod_kwad,  "Lod Kwadrat", 3.f, 20.f, "Kwadrat-lod.png",Typ::BLOK,TypKsztaltu::PROSTOKAT);
        zrob(lod_belka, "Lod Belka",   3.1f, 20.f, "belka-lod.png",Typ::BLOK,TypKsztaltu::PROSTOKAT);
        zrob(lod_troj,  "Lod Trojkat", 3.1f, 20.f, "trojkat-lod.png",Typ::BLOK,TypKsztaltu::TROJKAT);
    }
};

#endif