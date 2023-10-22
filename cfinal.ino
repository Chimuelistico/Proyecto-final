// INCLUIR LIBRERIAS
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
// DEFINICION DE VARIABLES
RTC_DS3231 rtc;
#define EV 2
unsigned int HUMEDAD=0;
unsigned int inicia=0;
int tiempo = 12;
int e=1;
unsigned int LECTURAP, LECTURAQ;
LiquidCrystal_I2C lcd(0x27,16,2);

// INICIALIZACION DE DISPOSITIVOS
void setup(){
 Serial.begin(9600); 
                                     // Inicializacion de RTC. Revisa si está conectado o no.
    if (! rtc.begin()) {            
    Serial.println("Modulo RTC3231 no conectado");
    while (1);}
  rtc.begin(); 


  lcd.init();                         // Inicializacion LCD
  lcd.backlight();
  lcd.clear();


pinMode(EV, OUTPUT);	                // Pin 2 como salida  

// Calibrado de RTC. Extrae la hora del dispositivo al que esté conectado el arduino.
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
}
                                // CODIGO PRINCIPAL/BUCLE
void loop() {
DateTime now = rtc.now();             // Ponemos una variable que significa el tiempo
HUMEDAD=analogRead(A0);               // Leemos la humedad de 0 a 1023 en el sensor de humedad

                                // Contador.
  if(now.hour()-inicia>=tiempo ){     // Cada 12 horas,
  // SI LA PLANTA ESTÁ SECA:
          if(HUMEDAD>670 && e==1){    // Revisa si la planta está seca.
            digitalWrite (EV, HIGH);  // Enciende el la regadora,
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("REGANDO");
            e=2;                      // Y nos lleva al estado 2
          }
             inicia=now.hour();
          }

                                //CUANDO LA PLANTA YA ESTÁ REGADA:
  if(HUMEDAD<669 && e==2){            // Comprobamos si se rego la planta fijandonos en la humedad, y si estamos en el estado 2 (estado de apagado)
           digitalWrite(EV, LOW);     // Apagamos la electrovalvula
           lcd.setCursor(0,0);
        lcd.print("Regado-");         // Guardamos la hora a la que se termino de regar el LCD.
        	lcd.print(now.hour());			// Le pide la hora al rtc
          lcd.print(":");				      // : Para separar
          lcd.print(now.minute());		// Ahora pide los minutos	
          lcd.print(":");			        // : Para separar
          lcd.print(now.second());		// Y por ultimo los segundos
                e=1;
          }

                                  // IMPRESION DEL PORCENTAJE DE HUMEDAD
LECTURAP=map (HUMEDAD, 1023, 205, 0, 100); // pasamos los valores de 0 a 1023 a porcentaje (0 a 100)
lcd.setCursor(0,1);                   // ponemos el cursor en el prinicipio del LCD
  lcd.print("Humedad= ");             // en las siguientes 3 lineas imprimimos el porcentaje de humedad
  lcd.print(LECTURAP);

                                             // EXTRAS
                                                // Arreglo de falla que hacia que el porcentaje quede impreso 2 veces en el LCD.
if(LECTURAP!=LECTURAQ+1 && LECTURAP!=LECTURAQ-1){ 
  if(LECTURAP<10){
  lcd.setCursor(10,1);
  lcd.print("% ");
  }

  else{
lcd.setCursor(11,1);
lcd.print("%");
  }

}
                                               // parte de codigo adicional para hacer revisiones.
  LECTURAQ=LECTURAP;      
  Serial.println(LECTURAP);
  Serial.println(HUMEDAD);
  delay(100);
}
