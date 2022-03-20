/*
  Conexoes:
** CS/SS - pin 10 (for Nano)
** MOSI/DI - pin 11
** MISO/DO - pin 12
** CLK/SCK - pin 13
** Umidade 1 - A0
** Umidade 2 - A1
** Umidade 3 - A2
** Umidade 4 - A3
** SDA - A4
** SCL - A5
*/

#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>    // Inclui a Biblioteca
#include <RTClib.h>               // Inclui a Biblioteca

//RTC_DS1307 rtc;
RTC_DS3231 rtc;
//LiquidCrystal_I2C lcd(0x3F, 16, 2);   // Inicia o display 16x2 no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Inicia o display 16x2 no endereço 0x27

const int chipSelect = 10;

const int ledTest = 2;
const int ledPin =  9;      // the number of the LED pin
const int buttonPin = 6;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

int sensor1 = 0;         // the sensor value
int sensor2 = 0;         // the sensor value
int sensor3 = 0;         // the sensor value
int sensor4 = 0;         // the sensor value
int temperatura = 0;
int hora = 0;
int minuto = 0;
int segundo = 0;
int dia = 0;
int mes = 0;
int ano = 0;

void setup() {

  pinMode(ledTest, OUTPUT);
  pinMode(ledPin, OUTPUT); // initialize the LED pin as an output
  pinMode(buttonPin, INPUT_PULLUP); // initialize the pushbutton pin as an input

  rtc.begin();                                        // Inicia o módulo RTC

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));     // Ajuste Automático da hora e data
  //rtc.adjust(DateTime(2019, 11, 29, 10, 23, 00));   // Ajuste Manual (Ano, Mês, Dia, Hora, Min, Seg)

  lcd.init();           // Inicia o Display
  lcd.backlight();      // Inicia o Backlight

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  cabecalho ();
}

void loop() {

  buttonState = digitalRead(buttonPin);

  sensor1 = analogRead(A0);
  sensor2 = analogRead(A1);
  sensor3 = analogRead(A2);
  sensor4 = analogRead(A3);

  temperatura = rtc.getTemperature();

  dia = (rtc.now().day());       // Imprime o Dia
  mes = (rtc.now().month());     // Imprime o Mês
  ano = (rtc.now().year());      // Imprime o Ano

  hora = (rtc.now().hour());      // Imprime a Hora
  minuto = (rtc.now().minute());    // Imprime o Minuto
  segundo = (rtc.now().second());    // Imprime o Segundo

  if ((hora == 6) || (hora == 7) || (hora == 8) || (hora == 9) || (hora == 10) || (hora == 11) || (hora == 12) || (hora == 13) || (hora == 14) || (hora == 15) || (hora == 16) || (hora == 17)) {
    digitalWrite(ledTest, HIGH);
    Serial.println("Liga Luz");
  }
  else {
    digitalWrite(ledTest, LOW);
    Serial.println("Desliga Luz");
  }

  if (buttonState == LOW) {
    lcd.backlight();
    SDSave();
    LCDPrint();
    digitalWrite(ledPin, HIGH);    // turn LED on:
    //    Serial.println("Achou butao!");
  }
  if (buttonState == HIGH) {
    //    Serial.println("Cade butao?");
    digitalWrite(ledPin, LOW);     // turn LED off:
    lcd.noBacklight();
  }
}

void cabecalho () {
  // make a string for assembling the data to log:
  String cabecalhoString = "";

  cabecalhoString += "Dia";
  cabecalhoString += "/";
  cabecalhoString += "Mes";
  cabecalhoString += "/";
  cabecalhoString += "Ano";
  cabecalhoString += ";";
  cabecalhoString += "Hora";
  cabecalhoString += ":";
  cabecalhoString += "Minuto";
  cabecalhoString += ":";
  cabecalhoString += "Segundo";
  cabecalhoString += ";";
  cabecalhoString += "Sensor 1";
  cabecalhoString += ";";
  cabecalhoString += "Sensor 2";
  cabecalhoString += ";";
  cabecalhoString += "Sensor 3";
  cabecalhoString += ";";
  cabecalhoString += "Sensor 4";
  cabecalhoString += ";";
  cabecalhoString += "Temperatura";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File cabecalhoFile = SD.open("MOITA.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (cabecalhoFile) {
    cabecalhoFile.println(cabecalhoString);
    cabecalhoFile.close();
    // print to the serial port too:
    Serial.println(cabecalhoString);
  }
}

void SDSave () {
  // make a string for assembling the data to log:
  String dataString = "";

  dataString += dia;
  dataString += "/";
  dataString += mes;
  dataString += "/";
  dataString += ano;
  dataString += ";";
  dataString += hora;
  dataString += ":";
  dataString += minuto;
  dataString += ":";
  dataString += segundo;
  dataString += ";";
  dataString += sensor1;
  dataString += ";";
  dataString += sensor2;
  dataString += ";";
  dataString += sensor3;
  dataString += ";";
  dataString += sensor4;
  dataString += ";";
  dataString += temperatura;

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("MOITA.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void LCDPrint () {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);              // Posiciona o cursor na primeira linha
  lcd.print("Hora: ");              // Imprime o texto "Hora: "
  lcd.print(hora);      // Imprime a Hora
  lcd.print(":");                   // Imprime o texto entre aspas
  lcd.print(minuto);    // Imprime o Minuto
  lcd.print(":");                   // Imprime o texto entre aspas
  lcd.print(segundo);    // Imprime o Segundo

  lcd.setCursor(0, 1);              // Posiciona o cursor na segunda linha
  lcd.print("Data: ");              // Imprime o texto entre aspas
  lcd.print(dia);       // Imprime o Dia
  lcd.print("/");                   // Imprime o texto entre aspas
  lcd.print(mes);     // Imprime o Mês
  lcd.print("/");                   // Imprime o texto entre aspas
  lcd.print(ano);      // Imprime o Ano

  delay(10000);                      // Aguarda 10 segundo e reinicia
  lcd.clear();

  lcd.setCursor(0, 0);              // Posiciona o cursor na primeira linha
  lcd.print("Sensor 1: ");              // Imprime o texto "Hora: "
  lcd.print(sensor1);      // Imprime a Hora

  lcd.setCursor(0, 1);              // Posiciona o cursor na primeira linha
  lcd.print("Sensor 2: ");              // Imprime o texto "Hora: "
  lcd.print(sensor2);      // Imprime a Hora

  delay(10000);                      // Aguarda 10 segundo e reinicia
  lcd.clear();

  lcd.setCursor(0, 0);              // Posiciona o cursor na primeira linha
  lcd.print("Sensor 3: ");              // Imprime o texto "Hora: "
  lcd.print(sensor3);      // Imprime a Hora

  lcd.setCursor(0, 1);              // Posiciona o cursor na primeira linha
  lcd.print("Sensor 4: ");              // Imprime o texto "Hora: "
  lcd.print(sensor4);      // Imprime a Hora

  delay(10000);                      // Aguarda 10 segundo e reinicia
  lcd.clear();

  lcd.setCursor(0, 0);              // Posiciona o cursor na primeira linha
  lcd.print("Temp. Ambiente:");
  lcd.setCursor(5, 1);              // Posiciona o cursor na primeira linha
  lcd.print(temperatura);              // Imprime o texto "Hora: "
  lcd.print("C");      // Imprime a Hora

  delay(10000);                      // Aguarda 10 segundo e reinicia
  lcd.clear();
}
