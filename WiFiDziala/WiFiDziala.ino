#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire00(A0);
DallasTemperature sensors00(&oneWire00); //Przekazania informacji do biblioteki

OneWire oneWire01(A1);
DallasTemperature sensors01(&oneWire01);

OneWire oneWire02(A2);
DallasTemperature sensors02(&oneWire02);

OneWire oneWire03(A3);
DallasTemperature sensors03(&oneWire03);

char client[1];
char bufor[500];
String strona;
int room00 = 22;
int room01 = 30;
int room02 = 35;
int room03 = 40;
//int corridorLed = 45;

void setup()
{
  // put your setup code here, to run once:
  digitalWrite(room00, HIGH);
  digitalWrite(room01, HIGH);
  digitalWrite(room02, HIGH);
  digitalWrite(room03, HIGH);
//  digitalWrite(corridorLed, HIGH);
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(115200);
  Configuration();
  sensors00.begin();
  sensors01.begin();
  sensors02.begin();
  sensors03.begin();
}

void loop()
{
  sensors00.requestTemperatures();
  sensors01.requestTemperatures();
  sensors02.requestTemperatures();
  sensors03.requestTemperatures();
//     Serial.print("ZERO: ");
//      Serial.println(sensors03.getTempCByIndex(0));
//      Serial.print("            JEDEN: ");
//      Serial.println(sensors01.getTempCByIndex(0));
//      Serial.print("                       DWA: ");
//      Serial.println(sensors1.getTempCByIndex(0));
//      delay(1500);
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
        // room00
        strona += "[{";
        strona += JsonConstructor("Id", 1);
        strona += ",";
        strona += JsonConstructor("Name", "room00");
        strona += ",";
        strona += JsonConstructor("LedStatus", digitalRead(room00));
        strona += ",";
        strona += JsonConstructor("TempStatus", sensors00.getTempCByIndex(0));
        strona += "}";
        // END room00

        // room01
        strona += ",{";
        strona += JsonConstructor("Id", 2);
        strona += ",";
        strona += JsonConstructor("Name", "room01");
        strona += ",";
        strona += JsonConstructor("LedStatus", digitalRead(room01));
        strona += ",";
        strona += JsonConstructor("TempStatus", sensors01.getTempCByIndex(0));
        strona += "}";
        // END room01

        // room02
        strona += ",{";
        strona += JsonConstructor("Id", 3);
        strona += ",";
        strona += JsonConstructor("Name", "room02");
        strona += ",";
        strona += JsonConstructor("LedStatus", digitalRead(room02));
        strona += ",";
        strona += JsonConstructor("TempStatus", sensors02.getTempCByIndex(0));
        strona += "}";
        // END room02

        // BATHROOM
        strona += ",{";
        strona += JsonConstructor("Id", 4);
        strona += ",";
        strona += JsonConstructor("Name", "room03");
        strona += ",";
        strona += JsonConstructor("LedStatus", digitalRead(room03));
        strona += ",";
        strona += JsonConstructor("TempStatus", sensors03.getTempCByIndex(0));
        strona += "}";
        // END BATHROOM

//        // CORRIDOR
//        strona += ",{";
//        strona += JsonConstructor("Id", 5);
//        strona += ",";
//        strona += JsonConstructor("Name", "Korytarz");
//        strona += ",";
//        strona += JsonConstructor("LedStatus", digitalRead(corridorLed));
//        strona += ",";
//        strona += JsonConstructor("TempStatus", sensors.getTempCByIndex(4));
//        strona += "}";
//         END CORRIDOR

        strona += "]";
      }
      else if (serialString.indexOf("room00Led") > 0) {
        strona += "{";
        if (digitalRead(room00) == 0) {
          digitalWrite(room00, HIGH);
          strona += JsonConstructor("LedStatus", 1 );
        } else {
          digitalWrite(room00, LOW);
          strona += JsonConstructor("LedStatus", 0 );
        }

        strona += "}";
      }
      else if (serialString.indexOf("room01Led") > 0) {
        strona += "{";
        if (digitalRead(room01) == 0) {
          digitalWrite(room01, HIGH);
          strona += JsonConstructor("LedStatus", 1 );
        } else {
          digitalWrite(room01, LOW);
          strona += JsonConstructor("LedStatus", 0 );
        }

        strona += "}";
      }

      else if (serialString.indexOf("room02Led") > 0) {
        strona += "{";
        if (digitalRead(room02) == 0) {
          digitalWrite(room02, HIGH);
          strona += JsonConstructor("LedStatus", 1 );
        } else {
          digitalWrite(room02, LOW);
          strona += JsonConstructor("LedStatus", 0 );
        }

        strona += "}";
      }

      else if (serialString.indexOf("room03Led") > 0) {
        strona += "{";
        if (digitalRead(room03) == 0) {
          digitalWrite(room03, HIGH);
          strona += JsonConstructor("LedStatus", 1 );
        } else {
          digitalWrite(room03, LOW);
          strona += JsonConstructor("LedStatus", 0 );
        }

        strona += "}";
      }

//      else if (serialString.indexOf("KorytarzLed") > 0) {
//        strona += "{";
//        if (digitalRead(corridorLed) == 0) {
//          digitalWrite(corridorLed, HIGH);
//          strona += JsonConstructor("LedStatus", 1 );
//        } else {
//          digitalWrite(corridorLed, LOW);
//          strona += JsonConstructor("LedStatus", 0 );
//        }
//
//        strona += "}";
//      }
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
