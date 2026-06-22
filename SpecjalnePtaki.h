#ifndef SPECJALNEPTAKI_H
#define SPECJALNEPTAKI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath> 
#include <box2d/box2d.h> 
#include <iostream> 
#include "FizycznyObiekt.h"
#include "SystemKolizji.h" 

class SpecjalnePtaki 
{
private:
    bool umijetnosc = true; // czy mozna uzyc skilla (tylko raz na lot)
    bool aktywacja = false;
  
public: 
    void odnow_umiejetnosci()
    {
        umijetnosc = true; // reset flagi przy nowym strzale
    }

    void specjalnefunkcjeptakow(FizycznyObiekt* obj, const sf::Event& event, std::vector<FizycznyObiekt*>& obiekty, 
                                sf::CircleShape& kulaognia, bool& rysuj_kule, float& promien_kuli,
                                sf::RectangleShape& wiatr, bool& rysuj_wiatr, float& czas_wiatru)    {
       
        // --- SKILL: CZERWONIAK (Gwałtowne przyspieszenie w pionie pod klawiszem S) ---
        if(obj->wlasciwosci.typek == RodzajPtaka::CZERWONIAK && obj->wlasciwosci.stan == StanPtaka::LECI)
        {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::S && umijetnosc)
                 {
                    b2Vec2 v = b2Body_GetLinearVelocity(obj->body);
                    if(v.y > 0){
                        b2Body_SetLinearVelocity(obj->body, { v.x, v.y * 5.f }); // leci w dol? to jebnij nim 5 razy szybciej
                    }
                    else if(v.y < 0)
                    {
                        b2Body_SetLinearVelocity(obj->body, { v.x, v.y * -5.f }); // leci w gore? odbijamy go mocno w dol
                    }
                    umijetnosc = false; // wykorzystano umiejke
                }
            }
        } 
        
        // --- SKILL: BOMBA (Wybuch obszarowy) ---
        else if(obj->wlasciwosci.typek == RodzajPtaka::BOMBA && obj->wlasciwosci.stan == StanPtaka::LECI)
        {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::S && umijetnosc)
                {
                    b2Vec2 srodek_wybuchu = b2Body_GetPosition(obj->body);
                    
                    // statystyki eksplozji
                    float promien_wybuchu = 12.0f;  
                    float max_obrazenia = 1500.f;   
                    float moc_odrzutu = 45.f;      

                    // pobieranie pozycji ze struktury graficznej SFML pod efekt wizualny
                    sf::Vector2f pozPtakaSfml = (obj->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) ? obj->pro.getPosition() : obj->kol.getPosition();
                    rysuj_kule = true; // flaga do rysowania efektu wybuchu
                    promien_kuli = 10.f; 
                    kulaognia.setPosition(pozPtakaSfml);

                    // petla lecaca po wszystkich obiektach na mapie, zeby zadac dmg i odepchnac
                    for (auto target : obiekty) {
                        if (target == nullptr || !target->wlasciwosci.aktywny || target == obj) continue;
                        if (target->niewidzialny || target->wlasciwosci.swiezozniszczony) continue;

                        b2Vec2 pozycja_celu = b2Body_GetPosition(target->body);
                        
                        // liczenie dystansu od bomby do celu (pitagoras)
                        float dx = pozycja_celu.x - srodek_wybuchu.x;
                        float dy = pozycja_celu.y - srodek_wybuchu.y;
                        float dystans = std::sqrt(dx * dx + dy * dy);

                        if (dystans < promien_wybuchu) {
                            // im dalej od srodka, tym mniejszy dmg
                            float intensywnosc = 1.0f - (dystans / promien_wybuchu);
                            if (intensywnosc < 0.f) intensywnosc = 0.f;

                            target->wlasciwosci.hp -= max_obrazenia * intensywnosc;

                            // fizyczne odpychanie klockow (tylko obiekty dynamiczne)
                            if (b2Body_GetType(target->body) == b2_dynamicBody) {
                                if (dystans > 0.01f) {
                                    b2Vec2 wektor_kierunku = { dx / dystans, dy / dystans }; // normalizacja wektora
                                    
                                    float masa = b2Body_GetMass(target->body);
                                    if (masa < 0.1f) masa = 1.0f;

                                    float moc_impulsu = moc_odrzutu * intensywnosc * masa; 
                                    b2Vec2 wektor_odrzutu = { wektor_kierunku.x * moc_impulsu, wektor_kierunku.y * moc_impulsu };
                                    
                                    b2Body_SetAwake(target->body, true); // budzimy obiekt w Box2D
                                    b2Vec2 srodek_masy = b2Body_GetWorldCenterOfMass(target->body);
                                    b2Body_ApplyLinearImpulse(target->body, wektor_odrzutu, srodek_masy, true); // jeb impuls
                                }
                            }
                            
                            // sprawdzanie czy klocek padl od wybuchu
                            if (target->wlasciwosci.hp <= 0.f) {
                                target->wlasciwosci.swiezozniszczony = true;
                                target->wlasciwosci.czas_wybuchu = 1.2f; // timer na animacje usuwania
                            }
                        }
                    }
                    // po wybuchu zabijamy samego ptaka-bombe
                    obj->wlasciwosci.hp = 0; 
                    umijetnosc = false;
                    obj->wlasciwosci.swiezozniszczony = true; 
                    obj->wlasciwosci.czas_wybuchu = 1.2f;
                }
            }
        }
        
        // --- SKILL: ŻÓŁTEK (Klasyczny dash / przyspieszenie) ---
        else if(obj->wlasciwosci.typek == RodzajPtaka::ZOLTEK && obj->wlasciwosci.stan == StanPtaka::LECI)
        {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::S && umijetnosc)
                 {
                    b2Vec2 v = b2Body_GetLinearVelocity(obj->body);
                    b2Body_SetLinearVelocity(obj->body, { v.x * 2.f, v.y * 2.f }); // podwojenie predkosci lotu
                    umijetnosc = false;
                }
            }
        }
        
        // --- SKILL: NIEBIESKI (Podmuch wiatru pionowo w dol) ---
        else if(obj->wlasciwosci.typek == RodzajPtaka::NIEBIESKI && obj->wlasciwosci.stan == StanPtaka::LECI)
        {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::S && umijetnosc)
                 {
                    b2Vec2 poz_ptaka = b2Body_GetPosition(obj->body);
                    
                    // wymiary strefy wiatru i siła
                    float zasieg_x = 10.0f;  
                    float zasieg_y = 30.0f; 
                    float moc_podmuchu = 20.f; 

                    // wyhamowanie x i lekkie podbicie ptaka w gore przy uzyciu wiatru
                    b2Vec2 v = b2Body_GetLinearVelocity(obj->body);
                    b2Body_SetLinearVelocity(obj->body, { v.x * 0.9f, -5.f });

                    // szukanie celow pod ptakiem do zdmuchniecia
                    for (auto target : obiekty) {
                        if (target == nullptr || !target->wlasciwosci.aktywny || target == obj) continue;
                        if (target->niewidzialny || target->wlasciwosci.swiezozniszczony) continue;

                        b2Vec2 poz_celu = b2Body_GetPosition(target->body);
                        
                        float dx = poz_celu.x - poz_ptaka.x;
                        float dy = poz_celu.y - poz_ptaka.y;

                        // czy cel lezy w prostokacie podmuchu pod niebieskim
                        if (dy > 0.0f && dy < zasieg_y && std::abs(dx) < zasieg_x) {
                            if (b2Body_GetType(target->body) == b2_dynamicBody) {
                                b2Body_SetAwake(target->body, true);
                                b2Vec2 srodek_masy = b2Body_GetWorldCenterOfMass(target->body);
                                float masa = b2Body_GetMass(target->body);
                                b2Vec2 wektor_odrzutu = { 10.f, moc_podmuchu * masa }; // spychanie klockow w dol z uwzglednieniem masy

                                b2Body_ApplyLinearImpulse(target->body, wektor_odrzutu, srodek_masy, true);
                                target->wlasciwosci.hp -= 100.f; // wiatr tez zadaje troche dmg
                            }
                        }
                    }
                    
                    // wlaczenie rysowania prostokata jako wizualnego efektu wiatru
                    rysuj_wiatr = true;
                    czas_wiatru = 0.5f; 
                    
                    // pobranie pozycji niezaleznie od tego jaki kształt ma obiekt
                    sf::Vector2f piksele;
                    if(obj->wlasciwosci.typ == TypKsztaltu::PROSTOKAT) piksele = obj->pro.getPosition();
                    else if(obj->wlasciwosci.typ == TypKsztaltu::KOLO) piksele = obj->kol.getPosition();
                    else if(obj->wlasciwosci.typ == TypKsztaltu::TROJKAT) piksele = obj->tro.getPosition();
                    
                    wiatr.setPosition(piksele);
                    
                    std::cout << "[SYSTEM] Niebieski robi PIONOWY PODMUCH!\n";
                    umijetnosc = false;
                }
            }
        }
    }
};

#endif