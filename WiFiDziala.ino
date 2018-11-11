void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(115200);
  while (!Send("AT", "OK", 100))
  {
    Serial.println("Blad komunikacji z modułem");
  }

  Serial.println("Rozpoczęto komunikacje!");

  if (Send("AT+CWMODE=1", "OK", 300))
  {
    Serial1.println("CW MODE -- OK");
  }

  if (Send("AT+CIPMODE=0", "OK", 300))
  {
    Serial1.println("CIPMODE -- OK");
  }

  if (Send("AT+CIPMUX=1", "OK", 300))
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
  delay(100);
  // Serial.println(Serial1.readString());
}

char client[1];
char bufor[500];
String strona;

void loop()
{
  // put your main code here, to run repeatedly:

  while (Serial1.available() > 0)
  {
    if (Serial1.find("+IPD,"))
    {
      Serial1.readBytesUntil(',', client, 1);

      Serial.println("Zapytanie ID: ");

      strona = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"value\":4}";

      sprintf(bufor, "AT+CIPSEND=%c,%d", client[0], strona.length());

      if (Send(bufor, ">", 200))
      {
        Serial.println("Żądanie wysłania");
      }

      if (Send(strona, "OK", 200))
      {
        Serial.println("Wysłano dane");
      }

      sprintf(bufor, "AT+CIPCLOSE=%c", client[0]);

      if (Send(bufor, "OK", 1000))
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
