#ifndef SILNIK_OBRAZEN_H
#define SILNIK_OBRAZEN_H
class Obiekt;
class Silnik_fizyczny;
class Silnik_Obrazen
{
private:
public:
    Silnik_Obrazen();
    void aplikuj_obrazenia(Obiekt &objA,Obiekt &objB,float sila_uderzeniowa);

};

#endif 
