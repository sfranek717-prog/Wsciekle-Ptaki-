#include "silnik_obrazen.h"
#include "obiekt.h"
#include "silnik_fizyczny.cpp"
#include <cmath>

Silnik_Obrazen::Silnik_Obrazen() {}

void Silnik_Obrazen::aktualizuj_stan(Obiekt &objA, Obiekt &objB)
{
    objA.hp -= sqrt(objB.vx*objB.vx+objB.vy*objB.vy) * E;

    }
