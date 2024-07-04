// Verwendete Libs
// JC_Button
// Adafruit NeoPixel
// DFPlayer Mini MP3 by Makuna


//Chip Arduino Nano
//Atmega328P (Old Bootloder)

// Programm definiert 4 Taster mit denen eine Sirene an und ausgeschalten werden kann und ein Blaulicht
// Die Effekte des Blaulichts und die Sirenen Sounds können gewechselt werden

//Taster anlegen
#include <JC_Button.h>  
const byte
    BUTTON_PIN(10),             // von Pin10 zu Ground
    BUTTON_PIN2(9),             // von Pin9 zu Ground
    BUTTON_PIN3(8),             // von Pin8 zu Ground
    BUTTON_PIN4(7),             // von Pin7 zu Ground
    LED_PIN(13);                // Interne LED Arduino

Button SirOn(BUTTON_PIN);       // Definiert Schalter Sirene an
Button SirC(BUTTON_PIN2);       // Definiert Schalter Sirene wechseln
Button BlOn(BUTTON_PIN3);       // Definiert Schalter Blaulicht an
Button BlC(BUTTON_PIN4);        // Definiert Schalter Blaulicht wechseln

//LED Steuerung implementieren mit Neopixel
#include <Adafruit_NeoPixel.h>
#define PIN        12 // LED DATA PIN
#define NUMPIXELS 20  // Anzahl zu steuernede LEDs
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


// Variablen zum Steuern der Funktionen
int BlaulichtMode = 1;
static bool update;
static bool Blaulicht;
long myTimer = 0;
long myTimeout = 500;

//DFMiniMp3
#include "Arduino.h"
#include "SoftwareSerial.h"
#include <DFMiniMp3.h>

int SirenenSound = 1;

class Mp3Notify; 
SoftwareSerial secondarySerial(4, 3); // RX, TX
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
DfMp3 dfmp3(secondarySerial);

class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);  
/*
      start next track
      track += 1;
      // this example will just start back over with 1 after track 3
      if (track > 3) 
      {
      track = 1;
       }
*/
    dfmp3.playMp3FolderTrack(track);  // sd:/mp3/0001.mp3, sd:/mp3/0002.mp3, sd:/mp3/0003.mp3
  }
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

void setup() {

  Serial.begin(115200);
  Serial.println("initializing...");

    SirOn.begin();              // initialize the button object
    SirC.begin();  
    BlOn.begin();  
    BlC.begin();  
    pinMode(LED_PIN, OUTPUT);   // set the LED pin as an output
    pixels.begin(); 

  dfmp3.begin();
  dfmp3.reset();

    uint16_t version = dfmp3.getSoftwareVersion();
  Serial.print("version ");
  Serial.println(version);

  uint16_t volume = dfmp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  dfmp3.setVolume(15);        //Lautstärke regelung 0-30
  
  uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("files ");
  Serial.println(count);
  
  Serial.println("starting...");
  //dfmp3.playMp3FolderTrack(1); 
}

//Void für loop der Sounds
void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  while ((millis() - start) < msWait)
  {
    dfmp3.loop(); 
    delay(1);
  }
}


void loop() {
  pixels.clear();
    static bool ledState;       
    static bool Sirene;
    SirOn.read();         //Tasterabfrage
    SirC.read();          //Tasterabfrage
    BlOn.read();          //Tasterabfrage
    BlC.read();           //Tasterabfrage      


    //Schaltet onboard LED um sobal irgend ein Taster betätigt wurde
    if (SirOn.wasReleased()||SirC.wasReleased()||BlOn.wasReleased()||BlC.wasReleased())    
    {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }

    //Schaltet Blaulicht an wenn Blaulichttaste gedrückt
    if (BlOn.wasReleased())    
    { 
        Blaulicht = !Blaulicht;  
        if(Blaulicht==true) Serial.println("BL ON");
        if(Blaulicht==false) Serial.println("BL OFF");
    }

    //Schaltet Blaulichtmodus um
        if (BlC.wasReleased())    
    { 
        BlaulichtMode += 1;
        if (BlaulichtMode > 3) 
        {
        BlaulichtMode = 1;
        } 
        Serial.print("Blaulichtmodus");
        Serial.println(BlaulichtMode);
    }

    //Schaltet Sirene an oder aus
    if (SirOn.wasReleased())    
    { 
        Sirene = !Sirene;  
            if (Sirene==true)
          {
            Serial.print("play Track");
            dfmp3.playMp3FolderTrack(1);
          }
          if (Sirene==false)
          {
            dfmp3.pause();
          }
    }

    //Schaltet Sirenensound um aber nur wenn die Sirene auch an ist
    if (SirC.wasReleased() && Sirene==true)
    {
      SirenenSound += 1;
        if (SirenenSound > 3) 
        {
        SirenenSound = 1;
        }
    dfmp3.playMp3FolderTrack(SirenenSound);
    }


    if (Blaulicht==true)    //wird ausgeführt wenn das Blaulicht an ist
    {
      BlaulichtEF(BlaulichtMode);     //Springt in die Blaulicht Steuerung "BlaulichtEF" und gibt den aktuellen Modus weiter
    }

    if (Blaulicht==false)   //wird ausgeführt wenn das Blaulicht aus ist
    {
      pixels.clear();       //Stellt alle LEDs auf 0
      pixels.show();        //Schaltet LEDs um
    }
waitMilliseconds(50);       //abfrage für loop des Sounds

}
