#ifndef SILNIK_FIZYCZNY_H
#define SILNIK_FIZYCZNY_H

class Obiekt;
class Silnik_fizyczny {
private:
    const float G = 9.81f;
    const float Poziom_morza = 200.00f;

public:
    Silnik_fizyczny();
    void aktualizuj(Obiekt &obj, float dt); //kontrola parametrow jednostki
    void Status_Pozycji(Obiekt &obj); //sprawdza czy obiekt nie lezy na ziemi
    void Status_Nakladania(Obiekt &objA, Obiekt &objB);//obiekty nie beda wchodzic w siebie nawzajem
    void Zasada_zachowania_pedu(Obiekt &objA, Obiekt &objB);//zasada zachowania pedu 
    
};

#endif