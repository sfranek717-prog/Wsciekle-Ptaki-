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
        // 1. Pobierz zdarzenia kontaktów z tego kroku symulacji
        b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);

        // 2. Przeleć przez kontakty, które się rozpoczęły
        for (int i = 0; i < contactEvents.beginCount; ++i) {
            auto& event = contactEvents.beginEvents[i];
            b2BodyId idA = b2Shape_GetBody(event.shapeIdA);
            b2BodyId idB = b2Shape_GetBody(event.shapeIdB);

            // Pobierz wskaźniki na Obiekty (User Data z Box2D 3.0)
            Obiekt* objA = (Obiekt*)b2Body_GetUserData(idA);
            Obiekt* objB = (Obiekt*)b2Body_GetUserData(idB);

            if (objA && objB) {
                b2Vec2 vA = b2Body_GetLinearVelocity(idA);
                b2Vec2 vB = b2Body_GetLinearVelocity(idB);
                
                // Obliczanie siły uderzenia wektorów prędkości
                float diffX = vA.x - vB.x;
                float diffY = vA.y - vB.y;
                float sila = std::sqrt(diffX * diffX + diffY * diffY);

                // Reagujemy tylko na odpowiednio mocne jebnięcie
                if (sila > 2.0f) {
                    std::cout << "[KOLIZJA] " << objA->nazwa_obiektu 
                              << " <---> " << objB->nazwa_obiektu 
                              << " | Sila uderzenia: " << sila << "\n";
                    
                    // Aplikujemy obrażenia do obu obiektów na raz
                    obrazenia.aplikuj_obrazenia(*objA, *objB, sila);
                }
            }
        }
    }
};

#endif