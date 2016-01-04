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

   vw_setup(2000); // initialisation de la communication à 2000 b/s
   vw_rx_start();  // Activation de la réception

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

// Stop ID section
Serial.println("Waiting for messages");
}
 
void loop()
{
 //Uuid = GetID();
  // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau

   if (vw_have_message) // Si un message est reçu
   {
    
      if (vw_get_message(buf, &buflen))
      {
         char link;
         int i;
         for (i = 0; i < buflen; i++){

         Serial.print((char) buf[i]);
         Serial.println("");
char value = ((char) buf[i]);
//link = link + value;

char *strcpy (char *link, const char *value);

Serial.print("buf[i]");
Serial.print((char) buf[i]);
Serial.println("");

//link = (char) buf[2] + (char) buf[3];

Serial.println("link");
  Serial.print(link);
  Serial.println("");
  if (i = buflen ){
TestMessage(link);
  }
         }
      }

   }
}


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

void TestMessage(char dalink) {
 Serial.println("Le lien");
 Serial.println(dalink);

}

