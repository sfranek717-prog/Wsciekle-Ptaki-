#ifndef KSZTALTY_H
#define KSZTALTY_H

#include <string>

enum class TypKsztaultu { KOLO, PROSTOKAT };

class Obiekt {
public:
    float masa, hp;
    float x, y;
    float vx, vy;
    TypKsztaultu typ;

    Obiekt(float m, float h, float px, float py, TypKsztaultu t)
        : masa(m), hp(h), x(px), y(py), vx(0), vy(0), typ(t) {}

    virtual ~Obiekt() {}
};

class Kolo : public Obiekt {
public:
    float r;
    Kolo(float m, float h, float px, float py, float promien)
        : Obiekt(m, h, px, py, TypKsztaultu::KOLO), r(promien) {}
};


class Prostokat : public Obiekt {
public:
    float w, s;

    Prostokat(float m, float h, float px, float py, float wysokosc, float szerokosc)
        : Obiekt(m, h, px, py, TypKsztaultu::PROSTOKAT), w(wysokosc), s(szerokosc) {}
};

#endif