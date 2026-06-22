#ifndef SYSTEMKOLIZJI_H
#define SYSTEMKOLIZJI_H

#include <box2d/box2d.h>
#include <iostream>
#include <cmath>
#include "obiekt.h"
#include "silnik_obrazen.h"

class SystemKolizji {
public:
    static void przetworzZderzenia(b2WorldId worldId, Silnik_Obrazen& obrazenia) {
        // pobieranie listy wszystkich aktualnych zderzen z fizyki Box2D
        b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);

        // lecimy po kazdym zarejestrowanym poczatku kolizji
        for (int i = 0; i < contactEvents.beginCount; ++i) {
            auto& event = contactEvents.beginEvents[i];
            
            // wyciaganie id ciał, ktore w siebie wjechały
            b2BodyId idA = b2Shape_GetBody(event.shapeIdA);
            b2BodyId idB = b2Shape_GetBody(event.shapeIdB);

            // rzutowanie wskaznika UserData na nasze obiekty gry
            Obiekt* objA = (Obiekt*)b2Body_GetUserData(idA);
            Obiekt* objB = (Obiekt*)b2Body_GetUserData(idB);

            // sprawdzamy czy oba obiekty w ogole istnieja w grze
            if (objA && objB) {
                // pobieranie aktualnych predkosci liniowych obu ciał
                b2Vec2 vA = b2Body_GetLinearVelocity(idA);
                b2Vec2 vB = b2Body_GetLinearVelocity(idB);
                
                // liczenie wzglednej predkosci zderzenia (roznica wektorow)
                float diffX = vA.x - vB.x;
                float diffY = vA.y - vB.y;
                float sila = std::sqrt(diffX * diffX + diffY * diffY); // pitagoras pod sile uderzenia

                // prog minimalnej sily, zeby nie naliczac dmg przy lekkim tarciu
                if (sila > 2.0f) {
                    std::cout << "[KOLIZJA] " << objA->nazwa_obiektu 
                              << " <---> " << objB->nazwa_obiektu 
                              << " | Sila uderzenia: " << sila << "\n";
                    
                    // wysylamy dane do silnika obrazen, zeby odjac hp klockom/swinkom
                    obrazenia.aplikuj_obrazenia(*objA, *objB, sila);
                }
            }
        }
    }
};

#endif