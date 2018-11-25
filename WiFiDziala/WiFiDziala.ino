#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(A4);
DallasTemperature sensors(&oneWire); //Przekazania informacji do biblioteki
//OneWire oneWireLiving(A4);
//DallasTemperature livingTemp(&oneWireLiving); //Przekazania informacji do biblioteki

char client[1];
char bufor[500];
String strona;
int kitchenLed = 22;
int livingLed = 30;

void setup()
{
  // put your setup code here, to run once:
  digitalWrite(kitchenLed, HIGH);
  digitalWrite(livingLed, HIGH);
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(115200);
  Configuration();
  sensors.begin();
//  livingTemp.begin();
}

void loop()
{
  sensors.requestTemperatures();
//   Serial.println("ZERO");
//    Serial.println(sensors.getTempCByIndex(0));
//    Serial.println("JEDEN");
//    Serial.println(sensors.getTempCByIndex(1));
//    delay(1000);
  while (Serial1.available() > 0)
  {
   
    if (Serial1.find("+IPD,"))
    {
      Serial1.readBytesUntil(',', client, 1);
      Serial.println("Zapytanie ID: ");
      strona = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: *\r\nAccess-Control-Allow-Headers: *\r\nContent-Type: application/json; charset=UTF-8\r\n\r\n";
      String serialString = Serial1.readString();
      // KORYTARZ LED
      if (serialString.indexOf("GetAllRoomStatus") > 0)
      {
        // KITCHEN
        strona += "[{";        
        strona += JsonConstructor("Id", 1);
        strona += ",";
        strona += JsonConstructor("Name", "kuchnia");
        strona += ",";
        strona += JsonConstructor("LedStatus", digitalRead(kitchenLed));
        strona += ",";
        strona += JsonConstructor("TempStatus", sensors.getTempCByIndex(0));
        strona += "}";
        // END KITCHEN

        // LIVING ROOM
        strona += ",{";        
        strona += JsonConstructor("Id", 2);
        strona += ",";
        strona += JsonConstructor("Name", "salon");
        strona += ",";
        strona += JsonConstructor("LedStatus", digitalRead(livingLed));
        strona += ",";
        strona += JsonConstructor("TempStatus", sensors.getTempCByIndex(1));
        strona += "}";
        // END LIVING ROOM

        strona += "]";
      }
      else if (serialString.indexOf("kuchniaLed") > 0) {
        strona += "{";
        if (digitalRead(kitchenLed) == 0) {
          digitalWrite(kitchenLed, HIGH);
          strona += JsonConstructor("LedStatus",1 );  
        } else {
          digitalWrite(kitchenLed, LOW);
          strona += JsonConstructor("LedStatus",0 );  
        }
            
          strona += "}";
      }
      else if (serialString.indexOf("salonLed") > 0) {
        strona += "{";
        if (digitalRead(livingLed) == 0) {
          digitalWrite(livingLed, HIGH);
          strona += JsonConstructor("LedStatus",1 );  
        } else {
          digitalWrite(livingLed, LOW);
          strona += JsonConstructor("LedStatus",0 );  
        }
            
          strona += "}";
      }



      // KORYTARZ LED END
      sprintf(bufor, "AT+CIPSEND=%c,%d", client[0], strona.length());


      if (Send(bufor, ">", 50))
      {
        Serial.println("Żądanie wysłania");
      }

      if (Send(strona, "OK", 50))
      {
        Serial.println("Wysłano dane");
      }

      sprintf(bufor, "AT+CIPCLOSE=%c", client[0]);

      if (Send(bufor, "OK", 50))
      {
        Serial.println("Zamknieto połączenie");
      }
    }
  }
}
boolean Send(String Komenda_AT, char *Odpowiedz_AT, int czas_czekania)
{
  Serial1.println(Komenda_AT);
  delay(czas_czekania);
  while (Serial1.available() > 0)
  {
    if (Serial1.find(Odpowiedz_AT))
    {
      return 1;
    }
    return 0;
  }
}

String JsonConstructor(String key, float value) {
  String toReturn = "";
  toReturn += "\"" + key + "\":";
  toReturn += value;
  return toReturn;
}
String JsonConstructor(String key, String value) {
  String toReturn = "";
  toReturn += "\"" + key + "\":";
  toReturn += "\"";
  toReturn += value;
  toReturn += "\"";
  return toReturn;
}
void Configuration() {
  while (!Send("AT", "OK", 200))
  {
    Serial.println("Blad komunikacji z modułem");
  }

  Serial.println("Rozpoczęto komunikacje!");

  if (Send("AT+CWMODE=1", "OK", 200))
  {
    Serial1.println("CW MODE -- OK");
  }

  if (Send("AT+CIPMODE=0", "OK", 500))
  {
    Serial1.println("CIPMODE -- OK");
  }

  if (Send("AT+CIPMUX=1", "OK", 200))
  {
    Serial1.println("CIPMUX -- OK");
  }

  if (Send("AT+CWJAP=\"Siec\",\"12341234\"", "OK", 5000))

  {
    Serial.println("Połączono z siecią!");
  }

  if (Send("AT+CIPSERVER=1,80", "OK", 1000))

  {
    Serial.println("Uruchomiono serwer!");
  }

  Serial1.println("AT+CIFSR");
  delay(300);
  Serial.println(Serial1.readString());
}
