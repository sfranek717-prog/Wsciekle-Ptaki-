#include "silnik_fizyczny.h"
#include "obiekt.h"
#include "ksztalty.h"
#include <cmath>

Silnik_fizyczny::Silnik_fizyczny() {}

void Silnik_fizyczny::aktualizuj(Obiekt &obj, float dt) {

    obj.vy += G * dt;
    obj.x += obj.vx * dt;
    obj.y += obj.vy * dt;

    float v_calkowite = sqrt(obj.vx * obj.vx + obj.vy * obj.vy);
}
void Silnik_fizyczny::Status_Pozycji(Obiekt &obj) // 
{
    if(obj.y>=Poziom_morza)
    {
        obj.vy=0;
        obj.y=Poziom_morza;
    }

}
void Silnik_fizyczny::Zasada_zachowania_pedu(Obiekt &objA,Obiekt &objB)//zalozenie jest takie ze energia obiektu uderzajacego zostaje calkowicie przekazana na drugi blok[pierwszy blok przestaje sie ruszac]
{
    float vBx=objB.vx;
    objB.vx = (objA.vx*objA.masa +vBx*objB.masa)/objB.masa;   
    objA.vx=0;
    float vBy=objB.vy;
    objB.vy = (objA.vy*objA.masa +vBy*objB.masa)/objB.masa;   
    objA.vy=0;
}


void Silnik_fizyczny::Status_Nakladania(Obiekt &objA,Obiekt &objB)
{
    if(objA.typ==TypKsztaultu::KOLO)
    {
        if(objB.typ==TypKsztaultu::KOLO) //KOLO VS KOLO
        {
            Kolo &kA = static_cast<Kolo&>(objA);
            Kolo &kB = static_cast<Kolo&>(objB);
            // 1. Liczymy różnicę pozycji (wektor od A do B)
    float dx = kB.x - kA.x;
    float dy = kB.y - kA.y;
    
    // 2. Liczymy rzeczywisty dystans między środkami
    float dystans = sqrt(dx*dx + dy*dy);
    float suma_promieni = kA.r + kB.r;

    // 3. Sprawdzamy, czy doszło do kolizji
    if(dystans < suma_promieni) {
        
        // --- KOREKCJA POZYCJI (Wypychanie z siebie) ---
        // Zabezpieczenie przed dzieleniem przez zero (gdy są idealnie w tym samym punkcie)
        if (dystans == 0.0f) dystans = 0.01f; 

        // Ile pikseli weszły w siebie?
        float overlap = suma_promieni - dystans;

        // Normalizujemy wektor (żeby miał długość 1 - wyznacza tylko kierunek)
        float nx = dx / dystans;
        float ny = dy / dystans;

        // Rozsuwamy oba obiekty po połowie (żeby już na siebie nie nachodziły)
        kA.x -= nx * (overlap / 2.0f);
        kA.y -= ny * (overlap / 2.0f);
        
        kB.x += nx * (overlap / 2.0f);
        kB.y += ny * (overlap / 2.0f);

        // --- FIZYKA ZDERZENIA ---
        Zasada_zachowania_pedu(kA, kB);
    }

        }
        else if(objB.typ==TypKsztaultu::PROSTOKAT) //KOLO VS PROSTOKAT 
        {
            Kolo &kA = static_cast<Kolo&>(objA);
            Prostokat &kB = static_cast<Prostokat&>(objB);
            float najblizszy_X = std::max(kB.x, std::min(kA.x, kB.x + kB.s));
            float najblizszy_Y = std::max(kB.y, std::min(kA.y, kB.y + kB.w));

            float dx = kA.x - najblizszy_X;
            float dy = kA.y - najblizszy_Y;
            float dystans = sqrt(dx*dx + dy*dy);

if(dystans < kA.r) { {
      
        if (dystans == 0.0f) dystans = 0.01f; 

        float overlap = kA.r - dystans;

        float nx = dx / dystans;
        float ny = dy / dystans;

        kA.x -= nx * (overlap / 2.0f);
        kA.y -= ny * (overlap / 2.0f);
        
        kB.x += nx * (overlap / 2.0f);
        kB.y += ny * (overlap / 2.0f);

        Zasada_zachowania_pedu(kA, kB);
        }
    }
}
    }
    else if(objA.typ==TypKsztaultu::PROSTOKAT)
    {
        if(objB.typ==TypKsztaultu::KOLO) //PROSTOKAT VS KOLO
        {
            Prostokat &kA = static_cast<Prostokat&>(objA);
            Kolo &kB = static_cast<Kolo&>(objB);
    float dx = kB.x - kA.x;
    float dy = kB.y - kA.y;
    
    float dystans = sqrt(dx*dx + dy*dy);
    float suma_promieni = kB.r + sqrt(kA.w*kA.w+kA.s*kA.s);

    if(dystans < suma_promieni) {
      
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
        else if(objB.typ==TypKsztaultu::PROSTOKAT) //PROSTOKAT VS PROSTOKAT 
    
{
    Prostokat &pA = static_cast<Prostokat&>(objA);
    Prostokat &pB = static_cast<Prostokat&>(objB);

    // 1. Sprawdzamy klasyczny warunek AABB (czy się stykają)
    bool kolizjaX = pA.x < pB.x + pB.s && pA.x + pA.s > pB.x;
    bool kolizjaY = pA.y < pB.y + pB.w && pA.y + pA.w > pB.y;

    if (kolizjaX && kolizjaY) 
    {
        // 2. Obliczamy głębokość nałożenia na obu osiach
        float overlapX = std::min(pA.x + pA.s, pB.x + pB.s) - std::max(pA.x, pB.x);
        float overlapY = std::min(pA.y + pA.w, pB.y + pB.w) - std::max(pA.y, pB.y);

        // 3. Wypychanie: decydujemy, czy pchnąć w poziomie czy w pionie.
        // Zawsze wypychamy w stronę, gdzie nałożenie jest MNIEJSZE (najkrótsza droga wyjścia).
        if (overlapX < overlapY) 
        {
            if (pA.x < pB.x) { // pA jest po lewej
                pA.x -= overlapX / 2.0f;
                pB.x += overlapX / 2.0f;
            } else { // pA jest po prawej
                pA.x += overlapX / 2.0f;
                pB.x -= overlapX / 2.0f;
            }
            // Zerujemy prędkości X przy zderzeniu czołowym (opcjonalnie)
        } 
        else 
        {
            if (pA.y < pB.y) { // pA jest wyżej
                pA.y -= overlapY / 2.0f;
                pB.y += overlapY / 2.0f;
            } else { // pA jest niżej
                pA.y += overlapY / 2.0f;
                pB.y -= overlapY / 2.0f;
            }
        }

        // 4. Fizyka pędu
        Zasada_zachowania_pedu(pA, pB);
    }
}
}
        