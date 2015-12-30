#include <Regexp.h>


/* Récepteur 433 MHz */
#include <VirtualWire.h>


 
void setup()
{
   Serial.begin(9600);
   Serial.println("Attente reception");
   vw_setup(2000); // initialisation de la communication à 2000 b/s
   vw_rx_start();  // Activation de la réception
}
 
void loop()
{
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
      }
   }
}
