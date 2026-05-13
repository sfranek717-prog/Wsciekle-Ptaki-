#ifndef SILNIK_OBRAZEN_H
#define SILNIK_OBRAZEN_H
class Obiekt;
class Silnik_fizyczny;
class Silnik_Obrazen
{
private:
const float E = 0.1f;
public:
    Silnik_Obrazen();
    void aktualizuj_stan(Obiekt &objA,Obiekt &objB);

};

#endif // SILNIK_OBRAZEN_H
