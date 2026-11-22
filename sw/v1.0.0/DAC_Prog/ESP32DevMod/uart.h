/**
 * @file uart_receiver.h
 * @brief Headerdatei für die UART Empfänger Kommunikation
 * UART Pins, Baudrate und Funktionsdeklarationen für den ESP32 Dev Modul 
 */
//----------------------------------------------------------------------
#pragma once
#include <Arduino.h>
#include "signal_config.h"

#define UART_PORT Serial2  // UART-Port des ESP32 Dev Moduls
#define UART_BAUD 115200   // Übertragungsgeschwindigkeit (muss mit Sender übereinstimmen)
#define UART_RX_PIN 16     // ESP32 Dev Modul RX <- ESP32-S3 TX (GPIO43)
#define UART_TX_PIN 17     // ESP32 Dev Modul TX -> ESP32-S3 RX (GPIO44)

bool uart_init();  // UART initialisieren
void uart_loop();  // Kontinuierlich in loop() aufrufen (verarbeitet eingehende Nachrichten)
