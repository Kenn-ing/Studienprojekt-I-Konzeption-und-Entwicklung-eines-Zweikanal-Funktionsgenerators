/**
 * @file MCP42010.cpp
 * @brief Diese Datei enthält den Code zur Steuerung der MCP42010 digitalen Potentiometer über eine SPI-Schnittstelle.
 */

//---------------------------------------------------------------------------------

/**
 * Inkludierte Header-Dateien
 */

#include "MCP42010.h"

//-------------------------------------------------------------------------------------

const float P0_255[2]={9.93f, 11.22f};       // Widerstand bei Steps=255 von Pot 0
const float P0_0[2]={0.084f, 0.086f};         // Widerstand bei Steps=0 von Pot 0
const float P1_255[2]={9.91f, 11.21f};       // Widerstand bei Steps=255 von Pot 1
const float P1_0[2]={0.0645f, 0.0685f};         // Widerstand bei Steps=0 von Pot 1


uint8_t positionPot0[2]={0,0};  // Aktuelle Position von Potentiometer 0 (0–255)
uint8_t positionPot1[2]={0,0};  // Aktuelle Position von Potentiometer 1 (0–255)

int get_cs_pin(int chip){
  if (chip==0){
    return PIN_CS1;
  }
  else if (chip==1){
    return PIN_CS2;
  }
  return -1; // Ungültiger Chip
}

/**
 * Funktion zum senden eines vollständigen bytes über das SPI-Interface
 */
void spi_write_byte(uint8_t data){
  for (int bit = 7; bit>=0; --bit){
    digitalWrite(PIN_SI, (data >> bit) & 0x1 ? HIGH : LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_SCK, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_SCK, HIGH);
    delayMicroseconds(1);
  }
}

/**
 * Funktion zum senden eines vollständigen Kommandos an den MCP42010 (ausgewählter Chip, Command, Wert)
 */
bool mcp_send(int chip, uint8_t command, uint8_t value){
  // Abfragen, welcher CS-Pin verwendet werden muss
  int cs_pin = get_cs_pin(chip);

  digitalWrite(PIN_SCK, HIGH);
  delayMicroseconds(1);
  digitalWrite(cs_pin, LOW); // Übertragung starten
  delayMicroseconds(1);
  spi_write_byte(command);
  spi_write_byte(value);
  digitalWrite(cs_pin, HIGH); // Übertragung beenden
  delayMicroseconds(1);

  return true;
}

/**
 * Funktion zum Berechnen des eingestellen Widerstands des Potentiometers
 */
float getRes(int Pot, int chip){
  if (Pot ==0){
    float resistance = (positionPot0[chip]/255.0f * (P0_255[chip] - P0_0[chip])) + P0_0[chip];
    return resistance;
  } else if (Pot ==1){
    float resistance = (positionPot1[chip]/255.0f * (P1_255[chip] - P1_0[chip])) + P1_0[chip];
    return resistance;
  }
  return -1.0f; // Ungültiges Potentiometer
}

/**
 * Funktion zum Setzen des Potentiometerwerts für Potentiometer 0 eines bestimmten Chips.
 */
void setPot0(int value, int chip) {
  value = constrain(value, 0, 255);     // Sicherstellen, dass der Wert im gültigen Bereich liegt
  positionPot0[chip] = value;           // aktualisieren der position von Pot0

  bool ok = mcp_send(chip , CMD_POT0, (uint8_t)value);   // Neuen Wert an die Adresse des ICs für das Potentiometer 0 senden

  // Wenn die Übertragung erfolgreich war, den Widerstandswert berechnen und ausgeben, andernfalls eine Fehlermeldung ausgeben
  if (ok) { 
    float resistance = getRes(0, chip);
    Serial.printf("Pot0 von Chip %d gesetzt: %d/255  (~%.2f kΩ von %.2f kΩ)\n", chip, value, resistance, P0_255[chip]);
  } else {
    Serial.println("Fehler!");
  }
}

/**
 * Funktion zum Setzen des Potentiometerwerts für Potentiometer 1 eines bestimmten Chips.
 */
void setPot1(int value, int chip) {
  value = constrain(value, 0, 255); // Sicherstellen, dass der Wert im gültigen Bereich liegt
  positionPot1[chip] = value;       // aktualisieren der position von Pot1

  bool ok = mcp_send(chip, CMD_POT1, (uint8_t)value); // Neuen Wert an die Adresse des ICs für das Potentiometer 1 senden

  // Wenn die Übertragung erfolgreich war, den Widerstandswert berechnen und ausgeben, andernfalls eine Fehlermeldung ausgeben
  if (ok) {
    float resistance = getRes(1, chip);
    Serial.printf("Pot1 von Chip %d gesetzt: %d/255  (~%.2f kΩ von %.2f kΩ)\n", chip, value, resistance, P1_255[chip]);
  } else {
    Serial.println("Fehler!");
  }
}

/**
 * Funktion zum Setzen beider Potentiometer gleichzeitig auf denselben Wert für einen bestimmten Chip.
 */
void setBoth(int value, int chip) {
  value = constrain(value, 0, 255); // Sicherstellen, dass der Wert im gültigen Bereich liegt
  positionPot0[chip] = value;       // aktualisieren der position von Pot0
  positionPot1[chip] = value;       // aktualisieren der position von Pot1

  bool ok = mcp_send(chip, CMD_BOTH, (uint8_t)value); // Neuen Wert an die Adresse des ICs für beide Potentiometer gleichzeitig senden

  // Wenn die Übertragung erfolgreich war, den Widerstandswert berechnen und ausgeben, andernfalls eine Fehlermeldung ausgeben
  if (ok) {
    float resistanceP0 = getRes(0, chip);
    float resistanceP1 = getRes(1, chip);
    Serial.printf("Pot0 + Pot1 von Chip %d gesetzt: %d/255  (P0: ~%.2f kΩ von %.2f kΩ; P1: ~%.2f kΩ von %.2f kΩ)\n", chip, value, resistanceP0, P0_255[chip], resistanceP1, P1_255[chip]);
  } else {
    Serial.println("Fehler!");
  }
}

/**
 * Funktion zum Initialiseren der Potentiometer 
 */
void MCP42010_Init() {
  pinMode(PIN_SCK, OUTPUT); 
  digitalWrite(PIN_SCK, HIGH);
  pinMode(PIN_SI,  OUTPUT);
  pinMode(PIN_CS1, OUTPUT);
  digitalWrite(PIN_CS1, HIGH); // CS1 und CS2 auf HIGH setzen (Inaktiv)
  pinMode(PIN_CS2, OUTPUT);
  digitalWrite(PIN_CS2, HIGH);
  
  setPot0(255, 0);
  setPot1(255, 0);
  setPot0(255, 1);
  setPot1(255, 1);
  
}