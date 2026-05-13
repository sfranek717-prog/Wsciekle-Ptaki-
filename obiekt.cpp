#include "obiekt.h"

Obiekt::Obiekt(float m, float h, float px, float py, TypKsztaltu t)
    : masa(m), hp(h), x(px), y(py), vx(0.0f), vy(0.0f), typ(t)
{
}