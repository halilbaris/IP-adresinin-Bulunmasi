#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>
#include <LiquidCrystal.h>
 
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
int delayvalue = 7200000; // 2 SAAT
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00 };
char serverName[] = "checkip.dyndns.org";
char externalIP[17];
char lastExternalIP[17];
EthernetClient client;
TextFinder finder( client);
char c = ' ';
char matchfail = 0;
long starttime = 0;
 
void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("BASLATILIYOR");
   
  lcd.begin(16,2);           
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BASLATILIYOR...");
  while (Ethernet.begin(mac) == 0)
  { Serial.println("DHCP den IP ADRES ALAMADI ");
    lcd.setCursor(0,1);
    lcd.print("IP ADRES ALAMADI");
    delay(10000);
  }
  
   
    Serial.print("DHCP IP: ");
    for (byte thisByte = 0; thisByte < 4; thisByte++)
    {  
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print(".");
    }
    Serial.println();
    show_dhcp_lcd();
  
  delay(1000);
}
void loop()
{
 Serial.print("BAGLANIYOR... ");
 Serial.println(serverName);
 
 if (client.connect(serverName, 80))
 { Serial.println("BAGLANDI");
 
   client.println("GET /");
   client.println();
   delay(1000);
 }
 else
 {
 
   Serial.println("BAGLANAMADI");
 }
 
 if (client.available())
 { if(finder.find("IP Address: "))
   { for (char k = 0; k < 17; k++)
     { c = client.read();
       if(c != '<')
       { Serial.print(c);
         externalIP[k] = c;
       }
       else
        break;
      }
   }
 }
 client.flush();
 client.stop();
 Serial.println();
 for (char k = 0; k < 17; k++)
 { if (lastExternalIP[k] != externalIP[k])
   { matchfail = 1;
     break;
   }
   else
    matchfail = 0;
    
 }
  
 if (matchfail == 1)
 { Serial.println("");
 
 }
 else
 { Serial.println("IP ");
 }
 Serial.println();
 show_extip_lcd();
 
 for(char j = 0; j < 17; j++)
 { //Serial.print(externalIP[j]);   
   lastExternalIP[j] = externalIP[j];
 }
  
 Serial.println();
 Serial.println("BAGLANTI KAPATILIYOR.");
 client.stop();
    
 
 delay(delayvalue);
}
 
void waitforresponse()
{
  starttime = millis();
  while (!client.available())
    { if ((millis() - starttime) > 100000)
       {
         Serial.println("Timeout");
         break;
       }
    }   
  
    while (client.available())
    {
      char c = client.read();
      Serial.print(c);
    }
     Serial.println();
     
}
 
void show_dhcp_lcd()
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("DHCP IP: ");
   lcd.setCursor(0,1);
   for (byte thisByte = 0; thisByte < 4; thisByte++)
   {
    lcd.print(Ethernet.localIP()[thisByte], DEC);
    if(thisByte < 3)
      lcd.print(".");
   }
}
 
void show_extip_lcd()
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("  EXTERNAL IP");
   lcd.setCursor(0,1);
   for(char j = 0; j < 17; j++)
   {
     if(externalIP[j] < 39)
       lcd.print(" ");
     else
       lcd.print(externalIP[j]);
   }
}
