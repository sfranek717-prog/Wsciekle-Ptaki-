#include "silnik_obrazen.h"
#include "obiekt.h"
#include <cmath>
#include <iostream>

Silnik_Obrazen::Silnik_Obrazen() {}

void Silnik_Obrazen::aplikuj_obrazenia(Obiekt &objA, Obiekt &objB, float sila_uderzenia)
{
    const float PROG_OBRAZEN = 0.0f; 
    
    
    if (sila_uderzenia > PROG_OBRAZEN) {
        float final_dmg = (sila_uderzenia * 10.f - PROG_OBRAZEN);
        if(objA.typulec ==Typ::PTAK &&objB.typulec != Typ::PTAK)
        {
            objB.hp -=final_dmg;
        }
        else if(objB.typulec== Typ::PTAK && objA.typulec != Typ::PTAK)
        {
            objA.hp -= final_dmg;
        }
        else if(objA.typulec == Typ::PTAK && objB.typulec == Typ::PTAK) 
        {

        }
        else {

        objA.hp -= final_dmg;
        objB.hp -= final_dmg;
        }

        std::cout << "TEST OBRAZEN Sila: " << sila_uderzenia << " HP_A: " << objA.hp << " HP_B: " << objB.hp << std::endl;
    }

    // Obiekty ulegają zniszczeniu
    if (objA.hp <= 0) {
        objA.swiezozniszczony = true; 
        std::cout << "OBIEKT ULEGL ZNISZCZENIU: " << objA.nazwa_obiektu << std::endl;
    }
    if (objB.hp <= 0) {
        objB.swiezozniszczony = true; 
        std::cout << "OBIEKT ULEGL ZNISZCZENIU: " << objB.nazwa_obiektu << std::endl;
        
    }
if (objA.hp <= 0 && objA.swiezozniszczony) {
        objA.swiezozniszczony = true; 
        std::cout << "SILNIK: Obiekt A zniszczony! Start animacji BOOM: " << objA.nazwa_obiektu << std::endl;
    }
    
    if (objB.hp <= 0 && objB.swiezozniszczony) {
        objB.swiezozniszczony = true; 
        std::cout << "SILNIK: Obiekt B zniszczony! Start animacji BOOM: " << objB.nazwa_obiektu << std::endl;
    }

if (objA.hp <= objA.mat.wytrzymalosc / 1.20f && objA.hp > 0) {
    objA.mat.zmieniamy = true;
}
if (objB.hp <= objB.mat.wytrzymalosc / 1.2f && objB.hp > 0) {
    objB.mat.zmieniamy = true;
}


}
