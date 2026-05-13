#ifndef KSZTALTY_H
#define KSZTALTY_H

#include "obiekt.h"

class Kolo : public Obiekt {
public:
    float r;
    Kolo(float m, float h, float px, float py, float promien)
        : Obiekt(m, h, px, py, TypKsztaltu::KOLO), r(promien) {}
};

class Prostokat : public Obiekt {
public:
    float w, s;
    Prostokat(float m, float h, float px, float py, float wysokosc, float szerokosc)
        : Obiekt(m, h, px, py, TypKsztaltu::PROSTOKAT), w(wysokosc), s(szerokosc) {}
};

#endif