// --- Programme Arduino ---

// Auteur du Programme : Justin Briard -
// Programme écrit le : 24/12/2015.

// ------- Licence du code de ce programme -----
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License,
//  or any later version.
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

// ////////////////////  PRESENTATION DU PROGRAMME ////////////////////

// -------- Que fait ce programme ? ---------
 /* Ce programme


*/


// --- Inclusion des librairies ---
#include <Entropy.h>
#include <EEPROM.h>
#include "DHT.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <VirtualWire.h>

// --- Déclaration des constantes utiles ---


#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define DHTPIN_POWER 5 //alimentation du capteur DHT22


String Uuid;
String eeprom0;
String eeprom1;
String eeprom2;
String eeprom3; 


// --- Déclaration des objets utiles pour les fonctionnalités utilisées ---
DHT dht(DHTPIN, DHTTYPE);


//ISR (WDT_vect)
//{
//  wdt_disable(); //désactive le watchdog
//}


void mywatchdogenable()
{
  MCUSR = 0;
  WDTCSR = _BV (WDCE) | _BV (WDE);
  WDTCSR = _BV (WDIE) | _BV (WDP3) | _BV (WDP0); //délai de 8 secondes
  wdt_reset();
  ADCSRA = 0; //désactive ADC
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  MCUCR = _BV (BODS) | _BV (BODSE);
  MCUCR = _BV (BODS);
  sleep_cpu();
  sleep_disable();
}


void setup()   { // debut de la fonction setup()
Serial.begin(9600); 
pinMode(DHTPIN_POWER, OUTPUT);

 // Start ID section
 Uuid = GetID();
  Serial.print("Actual ID : "); 
  Serial.print(Uuid);
  Serial.println();
    
if (Uuid == "0-0-00") {
     
GeneratetID();
 Uuid = GetID();
  Serial.println();
  Serial.print("ID : "); 
  Serial.print(Uuid);
  }

if (Uuid == "255-255-255255") {
     
GeneratetID();
 Uuid = GetID();
  Serial.println();
  Serial.print("ID : "); 
  Serial.print(Uuid);
  }
    Serial.println();
// Stop ID section

dht.begin();

// Initialise the IO and ISR
vw_set_ptt_inverted(true); // Required for DR3100
vw_setup(2000);  // Bits per sec
vw_set_tx_pin(12); 


//Initialisation
Serial.println("Ready");

} 

void loop(){ // debut de la fonction loop()


 digitalWrite(DHTPIN_POWER, HIGH); //alimente le capteur DHT22
  delay(5000);
  
  float Humidite = dht.readHumidity();
  float Temperature = dht.readTemperature();
   if (isnan(Temperature) || isnan(Humidite)) {
    Serial.println("Failed to read from DHT");
  }


  // Affichage Valeurs
  //   Serial.println("##############################"); 
  //   Serial.print("Humidity: "); 
   //  Serial.print(Humidite);
   //  Serial.print(" %\t");
   //  Serial.print("Temperature: "); 
   // Serial.print(Temperature);
   //  Serial.println(" *C");
    // Serial.println("##############################"); 
 



if (sendData(Temperature, Humidite)) { /* Réussite */
      /* Message de debug */
    Serial.println("Sending message ok");
  /* Tentative d'envoi des rapports en attente */
   } else { /* Echec */
  /* Message de debug */
    Serial.println("Sending message failed");
  }

 digitalWrite(DHTPIN_POWER, LOW); //arrêt de l’alimentation du DTH22
  for (int i=0; i < 8; i++) //mise en veille pendant 64 secondes passer à 8
    mywatchdogenable();

} 



/** Fonction d'envoi au serveur Arduino 433MHz */
boolean sendData(float Temperature, float Humidite) {
 
char TemperatureTChar[10]; 
dtostrf(Temperature, 4, 2, TemperatureTChar);

char HumiditeTChar[10]; 
dtostrf(Humidite, 4, 2, HumiditeTChar);

Uuid = GetID();
char UuidChar[50];
Uuid.toCharArray(UuidChar, 50) ;

const char* t = "&t=";
const char* h = "&h=";
const char* id = "id=";

//const char* UuidChar = "21-222261-2";

char result[40];
sprintf(result, "%s%s%s%s%s%s", id, UuidChar, t, TemperatureTChar, h, HumiditeTChar);

const char *msg = result;


    digitalWrite(13, true); // Flash a light to show transmitting
    Serial.println("Sending message");
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    Serial.println("Sending message done");
    digitalWrite(13, false);
    delay(2000);
  
  /* Pas d'erreur */
  return true;
}
// ////////////////////////// Fin du programme ////////////////////


String GetID() {
//Read the EEPROM to get the ID
  eeprom0 = EEPROM.read(0);
  eeprom1 = EEPROM.read(1);
  eeprom2 = EEPROM.read(2);
  eeprom3 = EEPROM.read(3);

//Making the ID
String Uuid = String(eeprom0 + "-" + eeprom1 + "-" + eeprom2 + eeprom3);
return Uuid;
}


String GeneratetID() {
Serial.print("ID GENERATION");    
 
  Entropy.Initialize();  

  uint32_t random_id;
  random_id = Entropy.random();

//Setting ID into the EEPROM
EEPROM.put(0, random_id);

 //Read the EEPROM to get the ID
  eeprom0 = EEPROM.read(0);
  eeprom1 = EEPROM.read(1);
  eeprom2 = EEPROM.read(2);
  eeprom3 = EEPROM.read(3);

//Making the ID
Uuid = String(eeprom0 + "-" + eeprom1 + "-" + eeprom2 + eeprom3);
         Serial.println("");
  Serial.print("The generated UUID number sting is ");
  Serial.print(Uuid);
         Serial.println("");
return Uuid;
}
