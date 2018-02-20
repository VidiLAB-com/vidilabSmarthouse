//--------------------------------------------------------------------------
//----------------------- VIDI 264 -----------------------------------------
//--- kod za upravljanje pametnom kućom uz pomoć bluetootha i smartphona ---
//--- napisan za potrebe članka u časopisu VIDI 264 - ožujak 2018. ---------
//--------------------------------------------------------------------------
//
// Ovaj kod radi s Arduino Uno i drugima
// Bluetooth modul HC-05 ili HC-06 spojiti s digitlnim pinovima 
//                (Serial port 1) 0 i 1 (Tx i Rx pinovi)
//-------------------------------------------------------------------
// napisao: Ivor Stojevski, Vidi - 20.2.2018.
//-------------------------------------------------------------------
// Kod je izdan pod Creative Commons licencom - CC BY 3.0 HR
// https://creativecommons.org/licenses/by/3.0/hr/
// Možete umnažati i redistribuirati kod  u bilo kojem mediju ili formatu.
// Možete remiksirati, mijenjati i prerađivati kod u bilo koju svrhu, pa i komercijalnu.
// Morate adekvatno navesti autora, uvrstiti link na licencu i naznačiti eventualne izmjene.
// Možete to učiniti na bilo koji razuman način, ali ne smijete sugerirati da davatelj 
// licence izravno podupire Vas ili Vaše korištenje djela.
// Ne smijete dodavati pravne uvjete ili tehnološke mjere zaštite koji će druge pravno 
// ograničiti da čine ono što im licenca dopušta.
//

#include "SFE_BMP180.h"
#include "Wire.h"

SFE_BMP180 tlak;

double tlakZraka, tempZraka; 
const int Relej1 = 2;
const int Relej2 = 3;
const int Relej3 = 4; 
const int Relej4 = 5;
const int IR=6;
const int FOTO=7;
byte serialA;

void setup(){
  Serial.begin(9600);
  pinMode(Relej1,OUTPUT);
  pinMode(Relej2,OUTPUT);
  pinMode(Relej3,OUTPUT); 
  pinMode(Relej4,OUTPUT);
  pinMode(IR,INPUT);
  pinMode(FOTO,INPUT);

    if(tlak.begin()) delay(1);
  else
  {
    delay(1);
    while(1);
  }

}

void loop() {
if (Serial.available()>0) {serialA =Serial.read(); Serial.println(serialA);}
  switch(serialA){
    case 49:
    digitalWrite(Relej1, !digitalRead(Relej1));
    delay(10);
    break;
    
    case 50:
    digitalWrite(Relej2, !digitalRead(Relej2));
    delay(10);
    break;
    
    case 51:
    digitalWrite(Relej3, !digitalRead(Relej3));
    delay(10);
    break;
    
    case 52:
    digitalWrite(Relej4,!digitalRead(Relej4));
    delay(10);
    break;
    
    case 53:
    digitalWrite(Relej1, HIGH);
    digitalWrite(Relej2, HIGH);
    digitalWrite(Relej3, HIGH);
    digitalWrite(Relej4, HIGH);
    break;
    
    case 54:
    digitalWrite(Relej1, LOW);
    digitalWrite(Relej2, LOW);
    digitalWrite(Relej3, LOW);
    digitalWrite(Relej4, LOW);
    break;
  }
    int StanjeIR = digitalRead(IR);
    Serial.println(",Vrata = " + String(StanjeIR) + ",");
    delay(10);

    int StanjeFOTO = digitalRead(FOTO);
    Serial.println("Svjetlo = " + String(StanjeFOTO) + ",");
    delay(10);

    tlakZraka = ocitajTlak();
    Serial.println("Tlak zraka = " + String(tlakZraka) + "hPa,");
  
    tempZraka = ocitajTemperaturu();
    Serial.println("Temp zraka = " + String(tempZraka) + "C,");
    delay(300);
    Serial.println();    
}

// FUNKCIJA ZA RAČUNANJE TLAKA I TEMPERATURE ZRAKA BMP SENZOR
double ocitajTlak()
{
  char status;

  double temp, Tlak, tlak0, nadVisina;
  
  status = tlak.startTemperature();
  if(status != 0)
  {
    delay(status);
    status = tlak.getTemperature(temp);
    if(status != 0)
    {
      status = tlak.startPressure(3);
      if(status != 0)
      {
        delay(status);
        status = tlak.getPressure(Tlak,temp);
        if(status != 0)
        {
          return(Tlak);
        }
      }
    }
  }
}

double ocitajTemperaturu()
{
  char status;
  double temp;
  
  status = tlak.startTemperature();
  if(status != 0)
  {
    delay(status);
    status = tlak.getTemperature(temp);
    if(status != 0)
    {
      return(temp);
    }
  }
}
