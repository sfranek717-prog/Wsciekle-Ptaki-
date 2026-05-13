#include "silnik_fizyczny.h"
#include "obiekt.h"
#include "ksztalty.h"
#include <cmath>
#include <algorithm>

Silnik_fizyczny::Silnik_fizyczny() {}

void Silnik_fizyczny::aktualizuj(Obiekt &obj, float dt) {

    obj.vy += G * dt;
    obj.x += obj.vx * dt;
    obj.y += obj.vy * dt;

    float v_calkowite = sqrt(obj.vx * obj.vx + obj.vy * obj.vy);
}
float Silnik_fizyczny::Zasada_zachowania_pedu(Obiekt &objA, Obiekt &objB) 
{
    
    float vBx_stare = objB.vx;
    objB.vx = (objA.vx * objA.masa + vBx_stare * objB.masa) / objB.masa;   
    objA.vx = 0;

    float vBy_stare = objB.vy;
    objB.vy = (objA.vy * objA.masa + vBy_stare * objB.masa) / objB.masa;   
    objA.vy = 0;
}
void Silnik_fizyczny::Status_Pozycji(Obiekt &obj) // 
{
    if(obj.y>=Poziom_morza)
    {
        obj.vy=0;
        obj.y=Poziom_morza;
    }

}
void Silnik_fizyczny::Status_Nakladania(Obiekt &objA, Obiekt &objB) 
{
    //  objA to KOŁO ---
    if (objA.typ == TypKsztaltu::KOLO) 
    {
        if (objB.typ == TypKsztaltu::KOLO) // KOLO VS KOLO
        {
            Kolo &kA = static_cast<Kolo&>(objA);
            Kolo &kB = static_cast<Kolo&>(objB);

            float dx = kB.x - kA.x;
            float dy = kB.y - kA.y;
            float dystans = sqrt(dx * dx + dy * dy);
            float suma_promieni = kA.r + kB.r;

            if (dystans < suma_promieni) {
                if (dystans == 0.0f) dystans = 0.01f;
                float overlap = suma_promieni - dystans;
                float nx = dx / dystans;
                float ny = dy / dystans;

                kA.x -= nx * (overlap / 2.0f);
                kA.y -= ny * (overlap / 2.0f);
                kB.x += nx * (overlap / 2.0f);
                kB.y += ny * (overlap / 2.0f);

                Zasada_zachowania_pedu(kA, kB);
            }
        } 
        else if (objB.typ == TypKsztaltu::PROSTOKAT) // KOLO VS PROSTOKAT
        {
            Kolo &kA = static_cast<Kolo&>(objA);
            Prostokat &kB = static_cast<Prostokat&>(objB);

            float najblizszy_X = std::max(kB.x, std::min(kA.x, kB.x + kB.s));
            float najblizszy_Y = std::max(kB.y, std::min(kA.y, kB.y + kB.w));

            float dx = kA.x - najblizszy_X;
            float dy = kA.y - najblizszy_Y;
            float dystans = sqrt(dx * dx + dy * dy);

            if (dystans < kA.r) {
                if (dystans == 0.0f) dystans = 0.01f;
                float overlap = kA.r - dystans;
                float nx = dx / dystans;
                float ny = dy / dystans;

                kA.x += nx * overlap; // Wypychamy koło
                kA.y += ny * overlap;

                Zasada_zachowania_pedu(kA, kB);
            }
        }
    } 
    //  objA to PROSTOKĄT ---
    else if (objA.typ == TypKsztaltu::PROSTOKAT) 
    {
        if (objB.typ == TypKsztaltu::KOLO) // PROSTOKAT VS KOLO
        {
            Status_Nakladania(objB, objA); 
        } 
        else if (objB.typ == TypKsztaltu::PROSTOKAT) // PROSTOKAT VS PROSTOKAT
        {
            Prostokat &pA = static_cast<Prostokat&>(objA);
            Prostokat &pB = static_cast<Prostokat&>(objB);

            bool kolizjaX = pA.x < pB.x + pB.s && pA.x + pA.s > pB.x;
            bool kolizjaY = pA.y < pB.y + pB.w && pA.y + pA.w > pB.y;

            if (kolizjaX && kolizjaY) {
                float overlapX = std::min(pA.x + pA.s, pB.x + pB.s) - std::max(pA.x, pB.x);
                float overlapY = std::min(pA.y + pA.w, pB.y + pB.w) - std::max(pA.y, pB.y);

                if (overlapX < overlapY) {
                    float sign = (pA.x < pB.x) ? -1.0f : 1.0f;
                    pA.x += sign * (overlapX / 2.0f);
                    pB.x -= sign * (overlapX / 2.0f);
                } else {
                    float sign = (pA.y < pB.y) ? -1.0f : 1.0f;
                    pA.y += sign * (overlapY / 2.0f);
                    pB.y -= sign * (overlapY / 2.0f);
                }
                Zasada_zachowania_pedu(pA, pB);
            }
        }
    }
}