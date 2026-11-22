# Studienprojekt-I-Konzeption-und-Entwicklung-eines-Zweikanal-Funktionsgenerators
 
Dieses Repository ist Teil des ersten Studienprojekts der Studenten **Dejan Dimitrijević** und **Alexander Klose** an der [**Hochschule für Wirtschaft und Recht (HWR) Berlin**](https://www.hwr-berlin.de/) in Zusammenarbeit mit den [**Berliner Wasserbetrieben AöR**](https://www.bwb.de/de/index.php).
 
|   |   |
|---|---|
| **Hochschulbetreuer** | Prof. Dr. Luís Fernando Ferreira Furtado |
| **Unternehmensbetreuer** | Dipl.-Ing. (FH) Thomas Freitag |
 
## Grundlegende Informationen
 
Dieses GitHub-Repository befasst sich mit der Konzeption und Entwicklung zweier Zweikanal-Funktionsgeneratoren auf Basis eines [Waveshare ESP32-S3 2,8-Zoll-Touch-LCD-Displays](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-2.8?srsltid=AfmBOorKiXWUO0Dnj3kRFyE_hHw5MEsdxoDEOFpi6EGPevpcTJwSkPUV). Die Funktionsgeneratoren geben auf zwei Kanälen (A und B) sinus- und rechteckförmige Spannungssignale mit einstellbarer Frequenz, Amplitude und Phasenverschiebung aus. Über das Display können verschiedene Gruppen mit jeweils individuellen Parametern ausgewählt werden. Zusätzlich zur Gruppenauswahl sind Hilfestellungen zur Verwendung der Laborgeräte in die Benutzeroberfläche integriert.
 
Die Gruppenparameter für Amplitude, Frequenz, Phasenverschiebung und Signalform werden über eine CSV-Datei, die auf einer microSD-Karte gespeichert wird, eingelesen. Im Rahmen des Projekts wurden zwei unterschiedliche Implementierungsmethoden entwickelt und gegenübergestellt.
 
## Implementierungsmethode A: Signalgenerierung mittels interner Digital-Analog-Wandler eines ESP32
 
- Ausgabe der analogen Spannungswerte über die DAC-Ausgänge eines [ESP32 DevKit C V4](https://www.az-delivery.de/products/esp32-dev-kit-c-v4-nodemcu-wlan-wifi-development-board-mit-usb-c-anschluss-kompatibel-mit-arduino?_pos=1&_sid=e904cefd8&_ss=r), welcher per UART vom ESP32-S3 angesteuert wird.
- Die benötigten Parameter werden per UART übermittelt. Aus diesen wird ein Spannungssignal berechnet und über die internen DAC-Pins ausgegeben.

## Implementierungsmethode B: Signalgenerierung mittels Direct Digital Synthesis
 
- Ausgabe der unterschiedlichen Spannungssignale mittels zwei [AD9833-DDS-ICs](https://asset.conrad.com/media10/add/160267/c1/-/en/002993328DS00/datenblatt-2993328-analog-devices-ad9833brmz-reel7-schnittstellen-ic-dds-direct-digital-synthesizer-tape-on-full-reel.pdf).
- Zur Anpassung der Ausgangsamplitude wird die Eingangsspannung eines [OPA1612](https://www.ti.com/lit/ds/symlink/opa1612.pdf?ts=1782131200931&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252Fde-de%252FOPA1612)-Operationsverstärkers über zwei digitale Potentiometer des Typs [MCP42010](https://cdn-reichelt.de/documents/datenblatt/A200/MCP42010_MIC.pdf) gesteuert.

  
## Repository-Struktur
 
```
├── sw/
│   └── libraries/
|       └── lvgl/                                # lvgl Treiber
|       └── Adafruit_NeoPixel/                   # NeoPixel/ws2812b Treiber
|   └── 2 Hauptgruppen - 3 Aufgaben - 60 Gruppen # Sourcecode für einen Datensatz mit 2 Hauptgruppen, 3 Aufgaben und 60 Arbeitsgruppen
│       └── DAC_Prog/                            # Implementierungsmethode A
|           └── ESP32DevMod/                     # Code für die Steuerung der Komponenten und Ausgabe der Spannung
|           └── ESP32S3/                         # Code für die Benutzeroberfläche und das Einlesen des Parameter
│       └── DDS_Prog/                            # Implementierungsmethode B
|           └── ESP32DevMod/                     # Code für die Steuerung der Komponenten und Ausgabe der Spannung
|           └── ESP32S3/                         # Code für die Benutzeroberfläche und das Einlesen des Parameter
|   └── 30 Gruppen                               # Sourcecode für einen Datensatz mit 30 Arbeitsgruppen
│       └── DAC_Prog/                            # Implementierungsmethode A
|           └── ESP32DevMod/                     # Code für die Steuerung der Komponenten und Ausgabe der Spannung
|           └── ESP32S3/                         # Code für die Benutzeroberfläche und das Einlesen des Parameter
│       └── DDS_Prog/                            # Implementierungsmethode B
|           └── ESP32DevMod/                     # Code für die Steuerung der Komponenten und Ausgabe der Spannung
|           └── ESP32S3/                         # Code für die Benutzeroberfläche und das Einlesen des Parameter
├── proj/
|   └── 2 Hauptgruppen - 3 Aufgaben - 60 Gruppen # UI für einen Datensatz mit 2 Hauptgruppen, 3 Aufgaben und 60 Arbeitsgruppen
│       └── EEZ Studio Projekt/                  # Allgemeines EEZ Studio Projekt der Benutzeroberfläche
|   └── 30 Gruppen                               # UI für einen Datensatz mit 30 Arbeitsgruppen
│       └── EEZ Studio Projekt/                  # Allgemeines EEZ Studio Projekt der Benutzeroberfläche
├── docs/
│   └── SP-I_Klose_Dimitrijevic.pdf              # Schriftliche Ausarbeitung inklusive aller verwendeten Quellen
│   └── Schaltpläne/                             # Schaltpläne der beiden Methoden
│   └── 3D-Design/                               # 3D Dateien und technische Zeichnungen der Gehäuse
│   └── Datenblätter/                            # Datenblätter der Komponenten
│   └── Pinouts/                                 # Pinouts der Komponenten
└── README.md
```
 
## Installation
 
1. **Arduino IDE** mit [Unterstützung für ESP32 installieren](https://docs.waveshare.com/ESP32-Arduino-Tutorials/Arduino-IDE-Setup) (Version 3.3.7 oder höher).
2. LVGL Bibliothek aus diesem Projekt herunterladen und in den `Arduino\libraries` Pfad kopieren.
3. Adafruit NeoPixel Bibliotheken aus diesem Projekt herunterladen und in den `Arduino\libraries` Pfad kopieren.
4. Je nach gewünschter Implementierungsmethode den entsprechenden Ordner ([`DAC_Prog`](https://github.com/Kenn-ing/Studienprojekt-I-Konzeption-und-Entwicklung-eines-Zweikanal-Funktionsgenerators/tree/main/sw/DAC_Prog) oder [`DDS_Prog`](https://github.com/Kenn-ing/Studienprojekt-I-Konzeption-und-Entwicklung-eines-Zweikanal-Funktionsgenerators/tree/main/sw/DDS_Prog)) in der Arduino IDE öffnen, das passende Board auswählen und flashen.
   - Für das ESP32-S3-Display müssen folgende Einstellungen in der Arduino IDE vorgenommen werden:
     - **Board:** ESP32S3 Dev Module
     - **USB CDC On Boot:** Enabled 
     - **Flash Size:** 16MB
     - **Partition Scheme:** 16M Flash (3MB APP/9.9MB FATFS)
     - **PSRAM:** OPI PSRAM
   - Für das ESP32 DevKit C V4 müssen folgende Einstellungen Arduino IDE vorgenommen werden:
     - **Board:** ESP32 Dev Module

## Nutzung

Für die Nutzung stehen zwei unterschiedliche Versionen zur Verfügung: Version [`v1.0.0`](https://github.com/Kenn-ing/Studienprojekt-I-Konzeption-und-Entwicklung-eines-Zweikanal-Funktionsgenerators/releases/tag/v1.0.0) und Version [`v2.0.0`](https://github.com/Kenn-ing/Studienprojekt-I-Konzeption-und-Entwicklung-eines-Zweikanal-Funktionsgenerators/releases/tag/v2.0.0).

### v1.0.0
 
1. CSV-Datei mit den gewünschten Parametern für alle 30 Gruppen auf eine microSD-Karte kopieren und in das Display-Board einlegen.
3. System einschalten und nach dem die microSD-Karte erkannt wurde über den Button *Daten Laden* die Gruppenparameter einlesen.
4. Gewünschte Gruppe über die  Scroll-Liste auswählen, um das entsprechende Signal auf den Kanälen A und B auszugeben.
5. Über den Button *Start* die Signalausgabe starten.
6. Über den Button *Stop* die Signalausgabe stoppen.
7. Über den Button *Hilfe* kann die Navigationsansicht aufgerufen werden, um zwischen den Hilfestellungen zu navigieren.
8. Uber den Zurück-Button in der oberen rechten Ecke gelangt man von jeder Unterseite zurück in die Navigationsansicht und von der Navigationsansicht zurück auf den Hauptbildschirm.

### CSV-Dateiformat v1.0.0

| Spalte | Beschreibung | Einheit / Wertebereich | Beispiel |
|---|---|---|---|
| `Gruppe` | Fortlaufende ID der Parametergruppe | Ganzzahl | `1` |
| `Amplitude CH1` | Ausgangsamplitude auf Kanal 1 | mV | `410` |
| `Frequenz CH1` | Ausgangsfrequenz auf Kanal 1 | Hz | `1000` |
| `Phase CH1` | Phasenverschiebung auf Kanal 1 | ° (Grad) | `0` |
| `Signalform CH1` | Signalform für Kanal 1 | `0` = Sinus, `1` = Rechteck | `0` |
| `Amplitude CH2` | Ausgangsamplitude auf Kanal 2 | mV | `830` |
| `Frequenz CH2` | Ausgangsfrequenz auf Kanal 2 | Hz | `1000` |
| `Phase CH2` | Phasenverschiebung auf Kanal 2 | ° (Grad) | `11` |
| `Signalform CH2` | Signalform für Kanal 2 | `0` = Sinus, `1` = Rechteck | `0` |

**Beispielzeilen:**

```csv
Gruppe,Amplitude CH1,Frequenz CH1,Phase CH1,Signalform CH1,Amplitude CH2,Frequenz CH2,Phase CH2,Signalform CH2
1,410,1000,0,0,830,1000,11,0
2,830,1500,0,0,1650,45,27,0
3,240,2000,0,0,620,56,29,0
```

### v2.0.0

Diese Version unterstützt das Hinzufügen einzelner Hauptgruppen, Untergruppen und Aufgaben. Die Anzahl der maximal möglichen Gruppen ist lediglich durch die Speicherkapazität der Mikrokontroller begrenzt.

1. CSV-Datei mit den gewünschten Parametern für alle Gruppen auf eine microSD-Karte kopieren und in das Display-Board einlegen.
3. System einschalten und nach dem die microSD-Karte erkannt wurde über den Button *Daten Laden* die Gruppenparameter einlesen.
4. Gewünschte Hauptgruppe über das Dropdown *Hauptgruppe* auswählen.
5. Gewünschte Untergruppe über das Dropdown *Untergruppe* auswählen.
6. Gewünschte Aufgabe über das Dropdown *Aufgabe* auswählen.
7. Über den Button *Start* die Signalausgabe starten.
8. Über den Button *Stop* die Signalausgabe stoppen.
9. Über den Button *Hilfe* kann die Navigationsansicht aufgerufen werden, um zwischen den Hilfestellungen zu navigieren.
10. Uber den Zurück-Button in der oberen rechten Ecke gelangt man von jeder Unterseite zurück in die Navigationsansicht und von der Navigationsansicht zurück auf den Hauptbildschirm.

### CSV-Dateiformat v2.0.0

Die CSV-Datei teilt sich in einen Konfigurations- und einen Parameterteil. Der Konfigurationsteil legt die Gesamtzahl der Hauptgruppen, Untergruppen und Aufgaben fest und hat die folgende Struktur:

| Spalte | Beschreibung | Einheit / Wertebereich | Beispiel |
|---|---|---|---|
| `Gesamtzahl Hauptgruppen` | Legt die Gesamtzahl der Hauptgruppen fest | Ganzzahl | `2` |
| `Gesamtzahl Aufgaben` | Legt die Gesamtzahl der Aufgaben fest | Ganzzahl | `3` |
| `Gesamtzahl Untergruppen` | Legt die Gesamtzahl der Untergruppen fest | Ganzzahl | `4` |

über den Parameterteil, welcher sich direkt unter dem Konfigurationsteil befindet, werden erneut die Parameter der einzelnen Gruppen festgelegt.

| Spalte | Beschreibung | Einheit / Wertebereich | Beispiel |
|---|---|---|---|
| `Hauptgruppe` | Legt fest zu welcher Hauptgruppe der Eintrag gehört | Ganzzahl | `1` |
| `Aufgabe` | Legt fest zu welcher Aufgabe der Eintrag gehört | Ganzzahl | `2` |
| `Untergruppe` | Legt fest zu welcher Untergruppe der Eintrag gehört | Ganzzahl | `3` |
| `Amplitude CH1` | Ausgangsamplitude auf Kanal 1 | mV | `410` |
| `Frequenz CH1` | Ausgangsfrequenz auf Kanal 1 | Hz | `1000` |
| `Phase CH1` | Phasenverschiebung auf Kanal 1 | ° (Grad) | `0` |
| `Signalform CH1` | Signalform für Kanal 1 | `0` = Sinus, `1` = Rechteck | `0` |
| `Amplitude CH2` | Ausgangsamplitude auf Kanal 2 | mV | `830` |
| `Frequenz CH2` | Ausgangsfrequenz auf Kanal 2 | Hz | `1000` |
| `Phase CH2` | Phasenverschiebung auf Kanal 2 | ° (Grad) | `11` |
| `Signalform CH2` | Signalform für Kanal 2 | `0` = Sinus, `1` = Rechteck | `0` |

Da die Speicherung der Parameter über eine Bitmap erfolgt müssen die Gruppen nicht direkt in einer festen Reihenfolge in die CSV-Datei eingetragen werden. Es ist lediglich notwendig, für jede Gruppe einen Eintrag in der CSV-Datei zu hinterlegen.

**Beispielzeilen:**

```csv
Gesamtzahl Hauptgruppen, Gesamtzahl Aufgaben, Gesamtzahl Untergruppen
2, 2, 2
Hauptgruppe, Aufgabe, Untergruppe, Amplitude CH1, Frequenz CH1, Phase CH1, Signal CH1, Amplitude CH2, Frequenz CH2, Phase CH2, Signal CH2
1, 1, 1,  410,  10,  81, 1,  830,  10,  11, 1
1, 1, 2,  520,  20,  45, 0,  900,  20,  90, 0
1, 2, 1,  830,  45,  14, 0, 1650,  45,  27, 0
1, 2, 2,  600,  60,  30, 1, 1200,  60,  60, 1
2, 1, 1,  240,  56,   3, 1,  620,  56,  29, 0
2, 1, 2,  300,  70,  10, 0,  700,  70,  20, 1
2, 2, 1, 1500,  83,  35, 0, 1030,  83,  64, 1
2, 2, 2, 1100, 100,  90, 1,  950, 100, 180, 0
```

## Grenzwerte

| Variante | Frequenzbereich (Sinus)| Amplitudenbereich (Sinus)| Frequenzbereich (Rechteck)| Amplitudenbereich (Rechteck)|
|---|---|---|---|---|
| Implementierungsmethode A | `100 Hz - 500 Hz` | `500 mV - 1 V` | `10 Hz - 100 Hz` | `300 mV - 1,1 V` |
| Implementierungsmethode B | `1 kHz - 80 kHz` | `0,5 V - 10 V` | `10 kHz - 80 kHz` | `2 V - 10 V` |
