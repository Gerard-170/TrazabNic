//Librerias a usar
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"


//Funciones de recoleccion de datos
String SEXO (char n);
String EDAD (void);
String Animal_tag (void); 
String RTC_Fun (void); 

//Declaracion de pines para LCD RTC
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_PCF8523 rtc;

//Creacion de matriz char para el teclado
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned int Animal_Cont = 0; 

File myFile;  //Objeto de la memoria SD

String strEDAD = ""; 
String strSEXO = "";
String CUIA = "";
String data_string = ""; 
String data_RTC = "";
char Tecla = '0'; 

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();   
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Bienvenido "); 
  lcd.setCursor(0,1); 
  lcd.print("Trazab-NIC-2017"); 
  delay(1000);  

  //Inicia configuracion de la memoria SD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initializing SD");
  delay(100); 
  if (!SD.begin(10)) {
  lcd.setCursor(0,1);
  lcd.print("failed!");
  delay(200);   
    return 1;
  }
  lcd.setCursor(0,1);
  lcd.print("done.");
  delay(1000); 

  if (! rtc.initialized()) {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop() 
{
  // put your main code here, to run repeatedly
  lcd.clear(); 
  while(Tecla == '0')
  {
    lcd.setCursor(0,0); 
    lcd.print("* Formatos IPSA"); 
    Tecla = keypad.getKey();
    if(Tecla == NO_KEY){
      Tecla = '0'; 
    }
  }
  lcd.clear(); 
if (Tecla == '*') {
    while (Tecla == '*'){
        lcd.setCursor(0,0); 
        lcd.print("1) ID"); 
        lcd.setCursor(7,0); 
        lcd.print("2) REPOS"); 
        lcd.setCursor(0,1); 
        lcd.print("0) Regresar");
        Tecla = keypad.getKey();
        if(Tecla == NO_KEY){
          Tecla = '*'; 
          }
    }
}
            if (Tecla == '1'){
             Animal_Cont = 0;
             while (Tecla == '1')
             {
              lcd.clear(); 
              
              Animal_Cont = Animal_Cont + 1; 
              String Anc = String (Animal_Cont); 
              CUIA = Animal_tag();
              lcd.clear(); 
              strEDAD = EDAD();
              lcd.clear(); 
              strSEXO = SEXO(Tecla);
              data_string = String(Anc + "," + CUIA + "," + strEDAD + "," + strSEXO);
              myFile = SD.open("TRAZAB-2.csv", FILE_WRITE); 
              if (myFile){
              myFile.print(data_string);
              myFile.println();
              }
              myFile.close();
                if (SD.exists("TRAZAB-2.csv")) {
                    lcd.clear(); 
                    lcd.setCursor(0,0); 
                    lcd.print("existe.");
                    delay(500);
                    } 
                  else {
                    lcd.clear(); 
                    lcd.setCursor(0,0); 
                    lcd.print("no existe."); 
                    delay(500);
                       } 
              lcd.clear(); 
              lcd.setCursor(0,0); 
              lcd.print("Identificado..."); 
              delay(500);
              boolean Regre = 1;
              while (Regre == 1){
                lcd.setCursor(0,1); 
                lcd.print("* Continuar");
                Tecla = keypad.getKey();
                 if(Tecla == '#'){
                      Regre = 0;
                      Tecla = '*';  
                   }
                   else if(Tecla == '*'){
                    Regre = 0;
                    Tecla = '1'; 
                   }
                }
             }
            }
             else if (Tecla == '2'){
               while (Tecla == '2'){
                lcd.clear(); 
                lcd.setCursor(0,0); 
                lcd.print("CUIA anterior");
                delay(1000); 
                CUIA =  Animal_tag();
                lcd.clear(); 
                lcd.setCursor(0,0); 
                lcd.print("CUIA nueva");
                delay(1000); 
                String CUIA2 =  Animal_tag();
                data_RTC = RTC_Fun(); 
                delay(500);
              data_string = String(CUIA + "," + CUIA2 + "," + data_RTC);
              myFile = SD.open("TRAZAB-3.csv", FILE_WRITE); 
              if (myFile){
                myFile.print(data_string);
                myFile.println();
              }
              myFile.close();
              lcd.clear(); 
              boolean Regre = 1;
              while (Regre == 1){
                lcd.setCursor(0,1); 
                lcd.print("* Continuar");
                Tecla = keypad.getKey();
                 if(Tecla == '#'){
                      Regre = 0;
                      Tecla = '*';  
                   }
                   else if(Tecla == '*'){
                    Regre = 0;
                    Tecla = '2'; 
                   }
                }
               }
             }
  
}

String SEXO (char n) { 
  while (n == '1'){
     lcd.setCursor(0,0); 
     lcd.print("SEXO: ");
     lcd.setCursor(8,0); 
     lcd.print("*) H");
     lcd.setCursor(8,1); 
     lcd.print("#) M");
     n = keypad.getKey();
     if(n == NO_KEY){
      n = '1'; 
     }
  }
    if (n == '*')
    {
        return "H";
     }
    else if (n == '#')
     {
      return "M";  
     }
 return "0"; 
}
String EDAD (void){
  char value1, value2;
  char n = 'A';
  while (n == 'A'){
  lcd.setCursor(0,0); 
  lcd.print("EDAD: ");
        value1 = keypad.getKey();
        if(value1 == NO_KEY){
        n = 'A';
        } 
        else{
        lcd.setCursor(6,0); 
        lcd.print(value1);
        n = value1;
        }
  }
  while (n == value1){
        value2 = keypad.getKey();
        if(value2 == NO_KEY){
        n = value1;
        }  
        else{
        lcd.setCursor(7,0); 
        lcd.print(value2);
        n = 'B';
        }
  }
  while (n == 'B'){
        n = keypad.getKey();
        if(n == NO_KEY){
        n = 'B';
        }  
  }
  return String(value1 + value2);  
}
String Animal_tag (void){
    char inc[17];
    String c;
    lcd.clear(); 
    while(!Serial.available())
    {
      lcd.setCursor(0,0); 
      lcd.print("Esperando tag");
          
    }
   if(Serial.available() > 0){
      for(int i = 0; i <= 16; i++){
      inc[i] = Serial.read();
      delay(5); 
      }
   }
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print(inc);
  delay(1000); 
  return String(inc); 
}

String RTC_Fun (void){
String Timenow = "";
   DateTime now = rtc.now();
    Timenow = String((now.year(), DEC));
    Timenow.concat(String("/"));
    Timenow.concat(String(now.month(), DEC));
    Timenow.concat(String("/"));
    Timenow.concat(String((now.day(), DEC)));
    Timenow.concat(String("-"));
    Timenow.concat(String(now.hour(), DEC));
    Timenow.concat(String(':'));
    Timenow.concat(String(now.minute(), DEC));
    Timenow.concat(String(':'));
    Timenow.concat(String(now.second(), DEC));
return Timenow;   
}
