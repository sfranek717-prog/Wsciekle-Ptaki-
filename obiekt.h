#ifndef OBIEKT_H
#define OBIEKT_H

#include <string>

enum class TypKsztaltu { KOLO, PROSTOKAT };

class Obiekt {
public:
    float masa;
    float vx, vy;
    float x, y;
    float hp;
    TypKsztaltu typ;

    Obiekt(float m, float h, float px, float py, TypKsztaltu t);

    virtual ~Obiekt() {}
};

#endif