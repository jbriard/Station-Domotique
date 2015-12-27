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

// --- Fonctionnalités utilisées ---

// -------- Circuit à réaliser ---------

// --- Déclaration des constantes ---

// --- Inclusion des librairies ---

#include "DHT.h"
#include <VirtualWire.h>

// --- Déclaration des constantes utiles ---


#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)


// --- Déclaration des variables globales ---


// --- Déclaration des objets utiles pour les fonctionnalités utilisées ---
DHT dht(DHTPIN, DHTTYPE);


// ////////////////////////// 2. FONCTION SETUP = Code d'initialisation //////////////////////////
// La fonction setup() est exécutée en premier et 1 seule fois, au démarrage du programme

void setup()   { // debut de la fonction setup()
Serial.begin(9600); 

dht.begin();

// Initialise the IO and ISR
vw_set_ptt_inverted(true); // Required for DR3100
vw_setup(2000);  // Bits per sec
vw_set_tx_pin(12); 

// --- ici instructions à exécuter 1 seule fois au démarrage du programme ---


// ------- Initialisation des variables utilisées -------  

// ------- Codes d'initialisation utile -------  

//Initialisation
Serial.println("Ready");




} // fin de la fonction setup()
// ********************************************************************************

float BoucleLecture = 10;

////////////////////////////////// 3. FONCTION LOOP = Boucle sans fin = coeur du programme //////////////////
// la fonction loop() s'exécute sans fin en boucle aussi longtemps que l'Arduino est sous tension

void loop(){ // debut de la fonction loop()
float Temperature = 0;
float Humidite = 0;
float TemperatureT = 0;
float HumiditeT = 0;

// boucle lecture temperature humidite
for (int i=1; i <= BoucleLecture; i++){ 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
   if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
 Temperature = t + Temperature;
 Humidite = h + Humidite;
  }
delay(500); 
  } // fin de la boucle for
  
  // moyenne des valeurs
TemperatureT = Temperature / BoucleLecture + 1;
HumiditeT = Humidite / BoucleLecture;
  // Affichage Valeurs
    Serial.println("##############################"); 
    Serial.print("Humidity: "); 
    Serial.print(HumiditeT);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(TemperatureT);
    Serial.println(" *C");
    Serial.println("##############################"); 
    // Alerte temperature humidite led 


  
if (sendData(TemperatureT, HumiditeT)) { /* Réussite */
      /* Message de debug */
    Serial.println(F("Envoi ok"));
  /* Tentative d'envoi des rapports en attente */
   } else { /* Echec */
  /* Message de debug */
    Serial.println(F("Echec envoi"));
  }

//while(1); // stop loop

} // fin de la fonction loop() - le programme recommence au début de la fonction loop sans fin
// ********************************************************************************


// ////////////////////////// FONCTIONS DE GESTION DES INTERRUPTIONS ////////////////////





// ////////////////////////// AUTRES FONCTIONS DU PROGRAMME ////////////////////






/** Fonction d'envoi au serveur Arduino 433MHz */
boolean sendData(float TemperatureT, float HumiditeT) {
 
char TemperatureTChar[10]; 
dtostrf(TemperatureT, 4, 2, TemperatureTChar);

char HumiditeTChar[10]; 
dtostrf(HumiditeT, 4, 2, HumiditeTChar);

//char result[100];   // array to hold the result.
//strcpy(result,TemperatureTChar); // copy string one into the result.
//strcat(result,HumiditeTChar); // append string two to the result.


const char* t = "t=";
const char* h = "&h=";


char result[40];
sprintf(result, "%s%s%s%s", t, TemperatureTChar, h, HumiditeTChar);



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

