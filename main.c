
 #include <MQ135.h>
#include<Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
int POLLUTION_LEVEL;
int IN1=3;
int St=0;
int l=0,k=0,range=0,p=0;


static const int RXPin = 5, TXPin = 6;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlusgps;
int temp=0;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
String stringVal = "";
void ON()
{
digitalWrite(IN1,HIGH);

}



void OFF()
{
digitalWrite(IN1,LOW);

}



void setup()
{
Serial.begin(9600);
lcd.begin(16,2);
pinMode(IN1,OUTPUT);

pinMode(LED_BUILTIN,OUTPUT);

Serial.begin(9600);
ss.begin(GPSBaud);
lcd.begin(16,2);
pinMode(8,OUTPUT);
digitalWrite(8,LOW);
DispTitle();

delay(2000);

gsm_init();
lcd.clear();  
Serial.println("AT+CNMI=2,2,0,0,0");
lcd.print("GPS Initializing");
lcd.setCursor(0,1);
lcd.print("  No GPS Range  "); 

           delay(2000);
lcd.clear();
lcd.print("GPS Range Found");
lcd.setCursor(0,1);
lcd.print("GPS is Ready");
delay(2000);
lcd.clear();
lcd.print("System Ready");
lcd.clear();
            temp=0;

}



         void loop()
{ 

  MQ135 gasSensor = MQ135(A0);
 float POLLUTION_LEVEL = gasSensor.getPPM();
//to get ppm values from mq135 library 

serialEvent();

 if(St==0)
        {
            ON();
            Serial.println("Vehicle on");

           if(POLLUTION_LEVEL<30)
          {
             Serial.println(POLLUTION_LEVEL);
             lcd.setCursor(0,0);
                        lcd.print("Normal");  
            lcd.clear();    
                    }

        if(POLLUTION_LEVEL>=30)
       {
           k=l++;
           p=10-k;
            Serial.println(k);
           if(p<=9)
             {
                 lcd.setCursor(0,1);
                 lcd.print("Due: ");
                 lcd.setCursor(5,1);
                 lcd.print(p);
                delay(8000);

             }

         }

lcd.setCursor(0,1);
lcd.print("RANGE:");
lcd.setCursor(6,1);
lcd.print(range);
lcd.clear();
         

if((POLLUTION_LEVEL>30) && (POLLUTION_LEVEL<40))
           {
              range=1;
             // digitalWrite(caution,HIGH);
              lcd.setCursor(0,0);
              lcd.print("Pollution level:");
              lcd.setCursor(0,1);    
              lcd.print(POLLUTION_LEVEL); 
              Serial.println(POLLUTION_LEVEL);
              lcd.clear();   
              lcd.print("Kindly Service Your Vehicle");

            }
  else if((POLLUTION_LEVEL>40) && (POLLUTION_LEVEL<50))
          {
              range=2;
              //digitalWrite(caution,HIGH);
             lcd.setCursor(0,0);
             lcd.print("Pollution level:");
             lcd.setCursor(0,1);    
             lcd.print(POLLUTION_LEVEL); 
            lcd.clear();
            Serial.println(POLLUTION_LEVEL);
            lcd.print("Remainder!! Service Your Vehicle");
           }
  else if(POLLUTION_LEVEL>50)
          {
             range=3;
             lcd.setCursor(0,0);
             lcd.print("Pollution level:");
            lcd.setCursor(0,1);    
            lcd.println(POLLUTION_LEVEL); 
            Serial.print(POLLUTION_LEVEL);

           }
         delay(1000);

         if(l>=10||range==3)
         {
            St=1;

         }
         while(St==1)
         {
             MQ135 gasSensor = MQ135(A0);
            POLLUTION_LEVEL = gasSensor.getPPM(); 
lcd.setCursor(0,0);
lcd.print("POLLUTION:");
lcd.setCursor(11,0);
lcd.print("High");
digitalWrite(LED_BUILTIN,HIGH);
               delay(1000);
lcd.setCursor(0,1);
lcd.print("LIMIT REACHED!!!");
               delay(1000);
lcd.setCursor(0,1);
lcd.print("Vehicle Blocked");
                delay(500);
               OFF();
Serial.println("Pollution detected Vehicle blocked");
Serial.println("Restart Your Vehicle");
              delay(500);
      while (ss.available() > 0)
        {
     gps.encode(ss.read());
                if (gps.location.isUpdated())
                {
                                       digitalWrite(8,HIGH);
                      tracking();
                } 
               digitalWrite(8,LOW);
               if(!temp)
               break;  
        }
   }
        }
}
          




 void serialEvent()
{
  while(Serial.available()>0)
  { 
    if(Serial.find("Track"))
    {
      temp=1;
      break;
    }

               else
    {
    temp=0;
    }
  }
}



void gsm_init()
{
lcd.clear();
lcd.print("Finding Module..");
booleanat_flag=1;
  while(at_flag)
  {
    Serial.println("AT");
    delay(1);
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
       at_flag=0;
    }

    delay(1000);
  }
lcd.clear();
lcd.print("Module Connected..");
  delay(1000);
lcd.clear();
lcd.print("Disabling ECHO");
booleanecho_flag=1;



  while(echo_flag)
  {
     Serial.println("ATE0");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }
lcd.clear();
lcd.print("Echo OFF");
  delay(1000);
lcd.clear();
lcd.print("Finding Network..");
booleannet_flag=1;
  while(net_flag)
  {
    Serial.println("AT+CPIN?");
    while(Serial.available()>0)
    {
      if(Serial.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
lcd.clear();
lcd.print("Network Found..");

  delay(1000);
lcd.clear();
}


void init_sms()
{
Serial.println("AT+CMGF=1");
  delay(400);
Serial.println("AT+CMGS=\"XXXXXXXXXX\"");   // use your 10 digit cell no. here
  delay(400);
}



void send_data(String message)
{
Serial.print(message);
 delay(200);
}

void send_sms()
{
Serial.write(26);
}

void lcd_status()
{
lcd.clear();
lcd.print("Message Sent");
  delay(2000);
lcd.clear();
lcd.print("System Ready");
  return;
}


void tracking()
{
init_sms();
send_data("Vehicle Tracking Alert:");
Serial.println(" "); 
send_data("Vehicle Current Location is:");
Serial.println(" "); 
Serial.print("Latitude: ");
Serial.print(gps.location.lat(), 6);
Serial.print("\n Longitude: ");
Serial.println(gps.location.lng(), 6);

   // https://www.google.com/maps/@8.2630696,77.3022699,14z
Serial.print("https://www.google.com/maps/@");
Serial.print(gps.location.lat(), 6);
Serial.print(',');
Serial.print(gps.location.lng(), 6);
Serial.print(",14z");
send_sms();
    delay(2000);
lcd_status();
}


void DispTitle(void){
lcd.clear();
lcd.setCursor(3,0);
lcd.print("WELCOME TO");
           delay(1000);

lcd.clear();
lcd.setCursor(3,0);
lcd.print("VEHICULAR");
lcd.setCursor(3,1);
lcd.print("POLLUTION");
           delay(1000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("CONTROL AND");
lcd.setCursor(0,1);
lcd.print("TRACKING SYSTEM");
           delay(1000);
lcd.clear();
}

