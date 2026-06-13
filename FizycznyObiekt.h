#ifndef FIZYCZNYOBIEKT_H
#define FIZYCZNYOBIEKT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <box2d/box2d.h>
#include "obiekt.h"
#include "silnik_obrazen.h"

class FizycznyObiekt
{
private:
    const float Scale = 30.f;
public:
    b2BodyId body;
    Obiekt wlasciwosci;
    bool usuwamy = false;
    bool zmiana = false;
    bool niewidzialny = false;

    float w, h, r;
    sf::RectangleShape pro; //prostokat
    sf::ConvexShape tro; //trojkat
    sf::CircleShape kol;    // kolo

    FizycznyObiekt(b2WorldId world, Material& m, float x, float y, float w1, float w2, TypKsztaltu t) 
    {
        wlasciwosci.mat = m;
        wlasciwosci.hp = m.wytrzymalosc;
        wlasciwosci.typ = t;
        
        b2BodyDef bDef = b2DefaultBodyDef();
        bDef.type = b2_dynamicBody;
        bDef.position = { x / Scale, y / Scale };
        bDef.userData = &wlasciwosci; 
        body = b2CreateBody(world, &bDef);

        if (t == TypKsztaltu::PROSTOKAT) {
            w = w1; h = w2;
            //tworzenie boxow 
            b2Polygon box = b2MakeBox((w / 2) / Scale, (h / 2) / Scale);
            b2ShapeDef sDef = b2DefaultShapeDef();
            sDef.density = m.gestosc;
            
            // odpalenie zderzenia
            sDef.enableContactEvents = true; 
            
            b2CreatePolygonShape(body, &sDef, &box);
            
            pro.setSize({w, h}); 
            pro.setTexture(&wlasciwosci.mat.tekstura);
            pro.setOrigin({w / 2.f, h / 2.f});
        }
        else if (t == TypKsztaltu::KOLO) {
            r = w1 / Scale; 
            
            b2Circle circle;
            circle.center = {0.0f, 0.0f};
            circle.radius = r;

            b2ShapeDef sDef = b2DefaultShapeDef();
            sDef.density = m.gestosc;
            sDef.material.friction = 0.9f; 
            
            
            sDef.enableContactEvents = true; 
            
            b2CreateCircleShape(body, &sDef, &circle);

            kol.setRadius(w1); 
            kol.setTexture(&wlasciwosci.mat.tekstura);
            kol.setOrigin({w1, w1}); 
        }
        else if (t == TypKsztaltu::TROJKAT) {
            
            float szerokosc = w1;
            float wysokosc = (w2 > 0.0f) ? w2 : w1; 

            
            b2Vec2 punktyTrojkata[3];
            punktyTrojkata[0] = { 0.0f, -(wysokosc * (2.0f / 3.0f)) / Scale };                 // Górny czubek
            punktyTrojkata[1] = { -(szerokosc / 2.0f) / Scale, (wysokosc * (1.0f / 3.0f)) / Scale }; // Lewy dół
            punktyTrojkata[2] = {  (szerokosc / 2.0f) / Scale, (wysokosc * (1.0f / 3.0f)) / Scale }; // Prawy dół
            
            b2Hull powloka = b2ComputeHull(punktyTrojkata, 3);
            b2Polygon ksztaltTrojkata = b2MakePolygon(&powloka, 0.0f);

            b2ShapeDef sDef = b2DefaultShapeDef();
            sDef.density = m.gestosc;
            sDef.material.friction = 0.9f; 
            sDef.enableContactEvents = true; 
            
            b2CreatePolygonShape(body, &sDef, &ksztaltTrojkata);

          
            tro.setPointCount(3);
            tro.setPoint(0, sf::Vector2f(szerokosc / 2.0f, 0.0f));       // Górny czubek
            tro.setPoint(1, sf::Vector2f(0.0f, wysokosc));               // Lewy dół
            tro.setPoint(2, sf::Vector2f(szerokosc, wysokosc));          // Prawy dół
            
            // Origin (punkt obrotu i pozycjonowania) musi być zsynchronizowany ze środkiem masy Box2D!
            // Czyli ustawiamy go na środek w poziomie i w 2/3 wysokości od góry.
            tro.setOrigin(sf::Vector2f(szerokosc / 2.0f, wysokosc * (2.0f / 3.0f))); 
tro.setTexture(&wlasciwosci.mat.tekstura); 
        }
    }
    void update() {
        b2Vec2 pos = b2Body_GetPosition(body);
        float angle = b2Rot_GetAngle(b2Body_GetRotation(body));
        
        if (wlasciwosci.typ == TypKsztaltu::KOLO)
        {
            kol.setPosition({pos.x * Scale, pos.y * Scale});
            kol.setRotation(sf::radians(angle));
        }
        else if (wlasciwosci.typ == TypKsztaltu::PROSTOKAT)
        {
            pro.setPosition({ pos.x * Scale, pos.y * Scale });
            pro.setRotation(sf::radians(angle));
        }
        else if (wlasciwosci.typ == TypKsztaltu::TROJKAT)
        {
            tro.setPosition({pos.x * Scale, pos.y * Scale});
            tro.setRotation(sf::radians(angle));
        }
        
        // Zmiana tekstury na uszkodzoną
        if (wlasciwosci.mat.zmieniamy) {
            if (wlasciwosci.typ == TypKsztaltu::PROSTOKAT) pro.setTexture(&wlasciwosci.mat.uszkodzonatekstura);
            if (wlasciwosci.typ == TypKsztaltu::KOLO) kol.setTexture(&wlasciwosci.mat.uszkodzonatekstura);
            if (wlasciwosci.typ == TypKsztaltu::TROJKAT) tro.setTexture(&wlasciwosci.mat.uszkodzonatekstura); 
        }
    }
};

#endif