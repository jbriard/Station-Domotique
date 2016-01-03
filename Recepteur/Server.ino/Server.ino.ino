#include <Entropy.h>
#include <EEPROM.h>
/* Récepteur 433 MHz */
#include <VirtualWire.h>


String Uuid;
String eeprom0;
String eeprom1;
String eeprom2;
String eeprom3; 

void setup()
{
 
   Serial.begin(9600);
   Serial.println("Waiting for messages");
   vw_setup(2000); // initialisation de la communication à 2000 b/s
   vw_rx_start();  // Activation de la réception
 



 Uuid = GetID();
  Serial.print("Actual ID : "); 
  Serial.print(Uuid);

    
if (Uuid == "0-0-00") {
      Serial.println();
GeneratetID();
 Uuid = GetID();
  Serial.print("ID : "); 
  Serial.print(Uuid);
  }
}
 
void loop()
{
 Uuid = GetID();
  // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau
   if (vw_have_message) // Si un message est reçu
   {
      if (vw_get_message(buf, &buflen))
      {
         int i;
         for (i = 0; i < buflen; i++)

           Serial.print((char) buf[i]);
         Serial.println("");
  Serial.print("ID : "); 
  Serial.print(Uuid);
         Serial.println("");
      }
   }
}


String GetID() {
// code lancement pulverisation
  eeprom0 = EEPROM.read(0);
  eeprom1 = EEPROM.read(1);
  eeprom2 = EEPROM.read(2);
  eeprom3 = EEPROM.read(3);

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

Uuid = String(eeprom0 + "-" + eeprom1 + "-" + eeprom2 + eeprom3);
         Serial.println("");
  Serial.print("The generated UUID number sting is ");
  Serial.print(Uuid);
         Serial.println("");
return Uuid;
}
