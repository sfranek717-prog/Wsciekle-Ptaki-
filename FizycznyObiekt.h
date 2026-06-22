#ifndef FIZYCZNYOBIEKT_H
#define FIZYCZNYOBIEKT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <box2d/box2d.h>
#include "obiekt.h"
#include "silnik_obrazen.h"

// abstrakcyjna klasa bazowa - wspolny fundament dla ptakow, swin i klockow
class FizycznyObiekt
{
protected:
    const float Scale = 30.f;

public:
    // wspolne dane dla wszystkich obiektow w grze
    b2BodyId body;
    Obiekt wlasciwosci;
    bool usuwamy = false;
    bool zmiana = false;
    bool niewidzialny = false;

    float w = 0.f, h = 0.f, r = 0.f;

    // ksztalty sfml  
    sf::RectangleShape pro; 
    sf::ConvexShape tro; 
    sf::CircleShape kol;    

    // konstruktor bazy - odpala box2d i przypisuje wlasciwosci
    FizycznyObiekt(b2WorldId world, Material& m, float x, float y, TypKsztaltu t) 
    {
        wlasciwosci.mat = m;
        wlasciwosci.hp = m.wytrzymalosc;
        wlasciwosci.typ = t;
        
        b2BodyDef bDef = b2DefaultBodyDef();
        bDef.type = b2_dynamicBody;
        bDef.position = { x / Scale, y / Scale };
        bDef.userData = &wlasciwosci; 
        body = b2CreateBody(world, &bDef);
    }

    virtual ~FizycznyObiekt() {}

    // zmusza to kazdego ptaka, swinie do wlasnej obslugi fizyki i rysowania
    virtual void update() = 0;
    virtual void rysuj(sf::RenderWindow& window) = 0;
};

//c wlasna logika dla ptaka
class PtakGry : public FizycznyObiekt {
public:
    PtakGry(b2WorldId world, Material& m, float x, float y, float promien)
        : FizycznyObiekt(world, m, x, y, TypKsztaltu::KOLO) 
    {
        wlasciwosci.typulec = Typ::PTAK;
        r = promien / Scale; 

        b2Circle circle;
        circle.center = {0.0f, 0.0f};
        circle.radius = r;

        b2ShapeDef sDef = b2DefaultShapeDef();
        sDef.density = m.gestosc;
        sDef.material.friction = 0.9f; 
        sDef.enableContactEvents = true; 
        b2CreateCircleShape(body, &sDef, &circle);

        // ustawienie wygladu 
        kol.setRadius(promien); 
        kol.setTexture(&wlasciwosci.mat.tekstura);
        kol.setOrigin({promien, promien}); 
    }

    // nadpisujemy update specjalnie pod kolo (ptaka)
    void update() override {
        b2Vec2 pos = b2Body_GetPosition(body);
        float angle = b2Rot_GetAngle(b2Body_GetRotation(body));
        
        kol.setPosition({pos.x * Scale, pos.y * Scale});
        kol.setRotation(sf::radians(angle));

        if (wlasciwosci.mat.zmieniamy) {
            kol.setTexture(&wlasciwosci.mat.uszkodzonatekstura);
        }
    }

    // hermetyzacja - obiekt sam mowi jak ma sie narysowac
    void rysuj(sf::RenderWindow& window) override {
        if (!niewidzialny) window.draw(kol);
    }
};

// swinia - wlasciwie to samo co ptak, ale mozecie pozniej dodac jej wlasne funkcje
class SwiniaGry : public FizycznyObiekt {
public:
    SwiniaGry(b2WorldId world, Material& m, float x, float y, float promien)
        : FizycznyObiekt(world, m, x, y, TypKsztaltu::KOLO) 
    {
        wlasciwosci.typulec = Typ::SWINIA;
        r = promien / Scale; 

        b2Circle circle;
        circle.center = {0.0f, 0.0f};
        circle.radius = r;

        b2ShapeDef sDef = b2DefaultShapeDef();
        sDef.density = m.gestosc;
        sDef.material.friction = 0.9f; 
        sDef.enableContactEvents = true; 
        b2CreateCircleShape(body, &sDef, &circle);

        kol.setRadius(promien); 
        kol.setTexture(&wlasciwosci.mat.tekstura);
        kol.setOrigin({promien, promien}); 
    }

    void update() override {
        b2Vec2 pos = b2Body_GetPosition(body);
        float angle = b2Rot_GetAngle(b2Body_GetRotation(body));
        
        kol.setPosition({pos.x * Scale, pos.y * Scale});
        kol.setRotation(sf::radians(angle));

        if (wlasciwosci.mat.zmieniamy) {
            kol.setTexture(&wlasciwosci.mat.uszkodzonatekstura);
        }
    }

    void rysuj(sf::RenderWindow& window) override {
        if (!niewidzialny) window.draw(kol);
    }
};

// klocki - tu obslugujemy deski i trojkaty
class BlokGry : public FizycznyObiekt {
public:
    BlokGry(b2WorldId world, Material& m, float x, float y, float w1, float w2, TypKsztaltu t)
        : FizycznyObiekt(world, m, x, y, t) 
    {
        wlasciwosci.typulec = Typ::BLOK;

        if (t == TypKsztaltu::PROSTOKAT) {
            w = w1; h = w2;
            b2Polygon box = b2MakeBox((w / 2) / Scale, (h / 2) / Scale);
            b2ShapeDef sDef = b2DefaultShapeDef();
            sDef.density = m.gestosc;
            sDef.enableContactEvents = true; 
            b2CreatePolygonShape(body, &sDef, &box);
            
            pro.setSize({w, h}); 
            pro.setTexture(&wlasciwosci.mat.tekstura);
            pro.setOrigin({w / 2.f, h / 2.f});
        }
        else if (t == TypKsztaltu::TROJKAT) {
            float szerokosc = w1;
            float wysokosc = (w2 > 0.0f) ? w2 : w1; 

            b2Vec2 punktyTrojkata[3];
            punktyTrojkata[0] = { 0.0f, -(wysokosc * (2.0f / 3.0f)) / Scale };
            punktyTrojkata[1] = { -(szerokosc / 2.0f) / Scale, (wysokosc * (1.0f / 3.0f)) / Scale };
            punktyTrojkata[2] = {  (szerokosc / 2.0f) / Scale, (wysokosc * (1.0f / 3.0f)) / Scale };
            
            b2Hull powloka = b2ComputeHull(punktyTrojkata, 3);
            b2Polygon ksztaltTrojkata = b2MakePolygon(&powloka, 0.0f);

            b2ShapeDef sDef = b2DefaultShapeDef();
            sDef.density = m.gestosc;
            sDef.material.friction = 0.9f; 
            sDef.enableContactEvents = true; 
            b2CreatePolygonShape(body, &sDef, &ksztaltTrojkata);

            tro.setPointCount(3);
            tro.setPoint(0, sf::Vector2f(szerokosc / 2.0f, 0.0f));
            tro.setPoint(1, sf::Vector2f(0.0f, wysokosc));
            tro.setPoint(2, sf::Vector2f(szerokosc, wysokosc));
            tro.setOrigin(sf::Vector2f(szerokosc / 2.0f, wysokosc * (2.0f / 3.0f))); 
            tro.setTexture(&wlasciwosci.mat.tekstura); 
        }
    }

    void update() override {
        b2Vec2 pos = b2Body_GetPosition(body);
        float angle = b2Rot_GetAngle(b2Body_GetRotation(body));
        
        if (wlasciwosci.typ == TypKsztaltu::PROSTOKAT) {
            pro.setPosition({ pos.x * Scale, pos.y * Scale });
            pro.setRotation(sf::radians(angle));
            if (wlasciwosci.mat.zmieniamy) pro.setTexture(&wlasciwosci.mat.uszkodzonatekstura);
        }
        else if (wlasciwosci.typ == TypKsztaltu::TROJKAT) {
            tro.setPosition({pos.x * Scale, pos.y * Scale});
            tro.setRotation(sf::radians(angle));
            if (wlasciwosci.mat.zmieniamy) tro.setTexture(&wlasciwosci.mat.uszkodzonatekstura);
        }
    }

    void rysuj(sf::RenderWindow& window) override {
        if (niewidzialny) return;
        if (wlasciwosci.typ == TypKsztaltu::PROSTOKAT) window.draw(pro);
        else if (wlasciwosci.typ == TypKsztaltu::TROJKAT) window.draw(tro);
    }
};

#endif