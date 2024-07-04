void BlaulichtEF(int Effekt){       //Hier werden die Blaulichteffekte gesteuert. Der Aktuelle BlaulichtMode wird in die Variable Effekt gespeichert
static bool Timer;    //schalter um Lichter blinken zu lassen
  
  //Wird abgespielt wenn BlaulichtMode 1 ist
  if(Effekt==1){
    
    //stellt Timer um sobald die eingestellte Zeit abgelaufen ist
    if (millis() > myTimeout + myTimer ) {
    myTimer = millis();
    Timer = !Timer;
    }

    //Effekt 1 Wird abgespielt wenn Timer true ist
    if(Timer==true){
      for(int i=0; i<NUMPIXELS/2; i++) { 
        pixels.setPixelColor(i, pixels.Color(50, 0, 0));
      }
             for(int i=NUMPIXELS/2; i<NUMPIXELS; i++) { 
        pixels.setPixelColor(i, pixels.Color(0, 0, 50));
      }
      pixels.show();
    }
    //Effekt 2 Wird abgespielt wenn Timer false ist
        if(Timer==false){
      for(int i=0; i<NUMPIXELS/2; i++) { 
        pixels.setPixelColor(i, pixels.Color(0, 0, 50));
      }
             for(int i=NUMPIXELS/2; i<NUMPIXELS; i++) { 
        pixels.setPixelColor(i, pixels.Color(50, 0, 0));
      }
      pixels.show();
    }
  }


    //Wird abgespielt wenn BlaulichtMode 2 ist
  if(Effekt==2){
  
    //stellt Timer um sobald die eingestellte Zeit abgelaufen ist
    if (millis() > myTimeout + myTimer ) {
    myTimer = millis();
    Timer = !Timer;
    }

    //Effekt 1 Wird abgespielt wenn Timer true ist
    if(Timer==true){
      for(int i=0; i<NUMPIXELS/2-2; i++) { 
        pixels.setPixelColor(i, pixels.Color(50, 0, 0));
      }
            for(int i=NUMPIXELS/2-2; i<NUMPIXELS/2+2; i++) { 
        pixels.setPixelColor(i, pixels.Color(50, 50, 50));
      }
             for(int i=NUMPIXELS/2+2; i<NUMPIXELS; i++) { 
        pixels.setPixelColor(i, pixels.Color(0, 0, 50));
      }
      pixels.show();
    }
    //Effekt 2 Wird abgespielt wenn Timer false ist
        if(Timer==false){
      for(int i=0; i<NUMPIXELS/2-2; i++) { 
        pixels.setPixelColor(i, pixels.Color(0, 0, 50));
      }
            for(int i=NUMPIXELS/2-2; i<NUMPIXELS/2+2; i++) { 
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
             for(int i=NUMPIXELS/2+2; i<NUMPIXELS; i++) { 
        pixels.setPixelColor(i, pixels.Color(50, 0, 0));
      }
      pixels.show();
    }
  }


  //Wird abgespielt wenn BlaulichtMode 3 ist
  if(Effekt==3){
    
    //stellt Timer um sobald die eingestellte Zeit abgelaufen ist
    if (millis() > myTimeout + myTimer ) {
    myTimer = millis();
    Timer = !Timer;
    }

    //Effekt 1 Wird abgespielt wenn Timer true ist
    if(Timer==true){
      for(int i=0; i<NUMPIXELS; i++) { 
        pixels.setPixelColor(i, pixels.Color(50, 0, 0));
      }
      pixels.show();
    }
    //Effekt 2 Wird abgespielt wenn Timer false ist
        if(Timer==false){
      for(int i=0; i<NUMPIXELS; i++) { 
        pixels.setPixelColor(i, pixels.Color(0, 0, 50));
      }
      pixels.show();
    }
  }



}