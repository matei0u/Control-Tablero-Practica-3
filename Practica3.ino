#include <Controllino.h>
#include "Stone_HMI_Define.h"    // Libreria HMI
#include "Procesar_HMI.h"       // Libreria para datos HMI

// --- 1. DEFINICIÓN DE PINES ---
// LED 1
const int led1_pin = CONTROLLINO_D1;        // Pin LED 1
const int btn1_pin = CONTROLLINO_I17;       // Pin Boton 1

// LED 1
const int led2_pin = CONTROLLINO_D2;        // Pin LED 2
const int btn2_pin = CONTROLLINO_I18;       // Pin Boton 2

// --- 2. VARIABLES DE BRILLO (HMI) ---
// LED 1
int   pwmValue1         = 0;                // Brillo LED 1 (0-255)
float dutyCyclePercent1 = 0;                // Valor HMI LED 1 (0-100)

// LED 2
int   pwmValue2         = 0;                // Brillo LED 2 (0-255)
float dutyCyclePercent2 = 0;                // Valor HMI LED 2 (0-100)

// --- 3. VARIABLES DE ESTADO (BOTONES) ---
// Multiplicador On/Off
int multiplicadorLed1 = 0; // 0=Off, 1=On
int multiplicadorLed2 = 0; // 0=Off, 1=On

// Variables para botones
int  lastBtn1State  = LOW;                  
int  lastBtn2State  = LOW;                  


void setup() {
  Serial.begin(115200);   // Serial PC
  Serial2.begin(115200);  // Serial HMI
  
  // LEDs como salida
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);

  // Botones como entrada
  pinMode(btn1_pin, INPUT);
  pinMode(btn2_pin, INPUT);

  // Iniciar HMI
  HMI_init();
  
  // Poner HMI en 0
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 0); 
  Stone_HMI_Set_Value("spin_box", "spin_box2", NULL, 0); 
}

void loop() {
  
  // --- BOTONES ---
  
  // Logica Boton 1
  int currentBtn1State = digitalRead(btn1_pin);
  if (currentBtn1State != lastBtn1State) { // Si cambio el estado
    if (currentBtn1State == HIGH) { // Si se presiono
      // Cambia 0 a 1, o 1 a 0
      multiplicadorLed1 = 1 - multiplicadorLed1; 
      
      Serial.print("LED 1 Multiplicador: ");
      Serial.println(multiplicadorLed1);
    }
    delay(50); // Anti-rebote
  }
  lastBtn1State = currentBtn1State; // Guarda el estado actual

  // Logica Boton 2
  int currentBtn2State = digitalRead(btn2_pin);
  if (currentBtn2State != lastBtn2State) { // Si cambio el estado
    if (currentBtn2State == HIGH) { // Si se presiono
      // Cambia 0 a 1, o 1 a 0
      multiplicadorLed2 = 1 - multiplicadorLed2;

      Serial.print("LED 2 Multiplicador: ");
      Serial.println(multiplicadorLed2);
    }
    delay(50); // Anti-rebote
  }
  lastBtn2State = currentBtn2State; // Guarda el estado actual

  
  // --- HMI (LEER SPIN BOX) ---

  // Leer spin_box1
  dutyCyclePercent1 = HMI_get_value("spin_box", "spin_box1"); 
  
  // Leer spin_box2
  dutyCyclePercent2 = HMI_get_value("spin_box", "spin_box2"); 

  // Convertir valor LED 1 (0-100 a 0-255)
  if (dutyCyclePercent1 >= 0 && dutyCyclePercent1 <= 100) {
    pwmValue2 = map(dutyCyclePercent1, 0, 100, 0, 255);
  }
  
  // Convertir valor LED 2 (0-100 a 0-255)
  if (dutyCyclePercent2 >= 0 && dutyCyclePercent2 <= 100) {
    pwmValue1 = map(dutyCyclePercent2, 0, 100, 0, 255);
  }

  
  // --- SALIDA FINAL A LEDS ---
  
  // Calcular PWM final
  int finalPwm1 = multiplicadorLed1 * pwmValue1;
  int finalPwm2 = multiplicadorLed2 * pwmValue2;

  // Escribir en el LED
  // Si multiplicador=0, LED se apaga
  // Si multiplicador=1, LED usa brillo
  analogWrite(led1_pin, finalPwm1);
  analogWrite(led2_pin, finalPwm2);
}