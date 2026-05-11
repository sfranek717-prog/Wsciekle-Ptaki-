#ifndef OBIEKT_H
#define OBIEKT_H
#include <string>

enum class TypKsztaultu { KOLO, PROSTOKAT };

class Obiekt {
public:
    float masa;
    float vx, vy;
    float x, y;
    float hp;

    TypKsztaultu typ; //do okreslenia pozycjonowania

    Obiekt(float m, float h, float px, float py, TypKsztaultu t)
    : masa(m), hp(h), x(px), y(py), vx(0), vy(0), typ(t){}

    virtual ~Obiekt() {}
};
#endif