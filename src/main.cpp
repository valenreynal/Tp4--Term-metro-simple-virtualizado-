#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>
#define CLK 11
#define DIO 12
#define SENSOR 10

float temperatura;
TM1637Display display(CLK, DIO);

DHT dht(SENSOR, DHT22);

// Función para agregar punto decimal
uint8_t punto(uint8_t numero) {
    return numero | 0b10000000;
}

void setup() {
    display.setBrightness(5);  // brillo 
    dht.begin();
}

void loop() {
 temperatura = dht.readTemperature(); //leer temperatura
 // separo las partes enteras de decimal
 int entero = (int)temperatura;
 float decimal = temperatura - entero;
 int digitoDecimal = (int)(decimal * 10);
// Si decimal es negativo, convertirlo a positivo
    if (digitoDecimal < 0) digitoDecimal = -digitoDecimal;

    uint8_t digito[4]; //array de 4 digitos
// SI TENGO NUMEROS POSITIVOS
    if (temperatura >= 0) { 
      digito[0] = 0;  // dejo espacio vacio
        if (entero < 10) { //si es de un solo digito
           digito[2] = display.encodeDigit(entero); //muestra el numero en la posicion 2 del display
        } else {
            digito[1] = display.encodeDigit(entero / 10); //si tiene 2 digitos al dividirlo por 10 muestro la decena en la posicion 1
            digito[2] = display.encodeDigit(entero % 10); // y aca la unidad en la posicion 2
        }
        digito[2] = punto(display.encodeDigit(entero % 10));  //muestra el punto
        digito[3] = display.encodeDigit(digitoDecimal ); //muestra el numero decimal en la posicion 3

    } else {
        
      // SI TENGO NUMEROS NEGATIVOS
        int e = -entero; //si es negativo el numero lo paso a entero ( el display no admite numeros negativos)
        digito[0] = 0b01000000; // le agrego el signo "-" (segmento g)
        digito[1] = display.encodeDigit(e / 10); //muestro la decena
       digito[2] = punto(display.encodeDigit(0)); 
        digito[2] = punto(display.encodeDigit(e % 10));
        digito[3] = display.encodeDigit(digitoDecimal );//agrego el numero decimaL
        
    }

    display.setSegments( digito ); //muestra los segmentos
    delay(500);
}


