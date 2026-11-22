/**
 * @file Uart.h
 * @brief Implementierung der Kommunikation zwischen dem HMI und dem ESP32 Dev Kit C V4 (DAC Signalausgabe)
 * Aufrufen der Funktionen für den Start der UART Kommunikation, das Übertragen der geladenen Gruppen, 
 * das Signal welche Gruppe gestartet werden soll und wann die Ausgabe gestoppt werden soll
 */
//----------------------------------------------------------------------
#pragma once
#include <Arduino.h>
#include "signal_config.h"   

#define UART_PORT     Serial1     // Welcher UART-Port des ESP32-S3
#define UART_BAUD     115200      // Übertragungsgeschwindigkeit
#define UART_TX_PIN   43          // ESP32-S3 TX -> ESP32 Dev Module RX (GPIO16)
#define UART_RX_PIN   44          // ESP32-S3 RX <- ESP32 Dev Module TX (GPIO17)
#define UART_ACK_TIMEOUT_MS 500   // 500ms warten auf ACK bevor Timeout

bool uart_init(); // UART starten
bool uart_send_groups(const SignalGroup* groups, uint8_t amount); // Gruppen übertragen
bool uart_group_start(uint8_t group_id); // Gruppe X Start
bool uart_group_stop(); // Ausgabe stoppen