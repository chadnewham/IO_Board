#include <ShiftIn.h>
#include <ShiftRegister74HC595.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Dont forget to install these libraries. They all should be available in the library manager

//DFPlayer software serial 
SoftwareSerial mySoftwareSerial(7, 6); // RX, TX
//Input
ShiftIn<3> shift;
//Output
ShiftRegister74HC595<3> sr(4, 2, 3); //serialDataPin, clockPin, latchPin

DFRobotDFPlayerMini myDFPlayer;
bool startUp = true;
int audioSelection = 1;
int rndAudioFile = 0;
int fileCount = 0;

void setup() {
  //Pin 6 on the arduino is the Tx line from the DFPlayer. 
  //It's dropped LOW after a command is sent to avoid speaker noise
  pinMode(6, OUTPUT);
  Serial.begin(9600);
   // declare pins: pLoadPin, clockEnablePin, dataPin, clockPin (Input)
  shift.begin(10, 11, 13, 9);
  shift.setPulseWidth(7);

  //Begin DFPlayer serial
  mySoftwareSerial.begin(9600);
  delay(1000);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  
  myDFPlayer.volume(25);  //Set volume value (0~30)
  
  Serial.println(F("DFPlayer Mini online."));
//  Set default aduio selection
  sr.set(9, HIGH); sr.set(10, LOW);sr.set(11, LOW); sr.set(13, LOW);
  
}

void loop() {
  //startUp prevents a random sound being played on start up
  if(startUp == false){
    //shift.update() returns true if an input has changed states
    if(shift.update()){
      loopValues();
    }
  
    if (myDFPlayer.available()) {
      printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
    }
  }else{
    shift.update();
    startUp = false;
  }

}


void loopValues() {
  //Make sure data is accurate
  for(int i = 0; i < shift.getDataWidth(); i++){
    if(shift.state(i) == 1 || shift.state(i) == 0){
       actionController(i);
       audioActions(i);
    }
  }
}

//actionController(btn) handles actions for each button.

void actionController(int btn){
  //shift.state(x) returns the opposite of the input state (switch on = LOW, shitch off = HIGH)
  //sr.set(pin, state) sets the corrisponding led HIGH or LOW
  switch (btn){
    
//  5 toggle switches right side bottom
    case 0:
      sr.set(17, !shift.state(btn));
      //TODO This statement can probably be moved to the loopValues() to dry things up
      if(shift.hasChanged(btn)){
           if(!shift.state(btn)){
           myDFPlayer.playFolder(6, 12);
        }else{
          myDFPlayer.playFolder(6, 11);
        }
      }

      
      break;
    case 1:
      sr.set(16, !shift.state(btn));
      if(shift.hasChanged(btn)){
           if(!shift.state(btn)){
           myDFPlayer.playFolder(6, 4);
        }else{
          myDFPlayer.playFolder(6, 3);
        }
      }
      break;
    case 2:
      sr.set(15, !shift.state(btn));
      if(shift.hasChanged(btn)){
           if(!shift.state(btn)){
           myDFPlayer.playFolder(6, 6);
        }else{
          myDFPlayer.playFolder(6, 5);
        }
      }
      break;
    case 14:
      sr.set(14, !shift.state(btn));
      if(shift.hasChanged(btn)){
           if(!shift.state(btn)){
           myDFPlayer.playFolder(6, 8);
        }else{
          myDFPlayer.playFolder(6, 7);
        }
      }
      break;
    case 15:
      sr.set(12, !shift.state(btn));
      if(shift.hasChanged(btn)){
         if(!shift.state(btn)){
           myDFPlayer.playFolder(6, 10);
        }else{
          myDFPlayer.playFolder(6, 9);
        }
      }
      break;
      
//  6 buttons Yellow, green, and blue left bottom
    case 16:
      if(shift.released(btn)){
        sr.set(0, !sr.get(0));
      }
      break;
    case 17:
      if(shift.released(btn)){
        sr.set(1, !sr.get(1));
      }
      break;
    case 18:
      if(shift.released(btn)){
        sr.set(2, !sr.get(2));
      }
      break;
    case 19:
      if(shift.released(btn)){
        sr.set(3, !sr.get(3));
      }
      break;
    case 20:
      if(shift.released(btn)){
        sr.set(4, !sr.get(4));
      }
      break;
    case 21:
      if(shift.released(btn)){
        sr.set(5, !sr.get(5));
      }      
      break;
      
//  3 toggle switches center left
    case 22:
      sr.set(6, !shift.state(btn));
      if(shift.hasChanged(btn)){
        if(!shift.state(btn)){
           myDFPlayer.playFolder(7, 1);
        }else{
          myDFPlayer.playFolder(6, 11);
        }
      }
      break;
    case 23:
      sr.set(7, !shift.state(btn));
      if(shift.hasChanged(btn)){
        if(!shift.state(btn)){
           myDFPlayer.playFolder(7, 2);
        }else{
          myDFPlayer.playFolder(6, 11);
        }
      }
      break;
    case 8:
      sr.set(8, !shift.state(btn));
      if(shift.hasChanged(btn)){
        if(!shift.state(btn)){
           myDFPlayer.playFolder(7, 3);
        }else{
          myDFPlayer.playFolder(6, 11);
        }
      }
      break;
  }
}

//audioActions(btn) handles the 4 audio selector buttons and 1 toggle switch top left, and red button
void audioActions(int btn){
  switch(btn){
    //Big red button
    case 3:
      if(shift.released(btn)){
        myDFPlayer.playFolder(5, 1);
      }
      
      break;
//  Audio settings
    case 9:
    if(shift.released(btn)){
      sr.set(9, HIGH); sr.set(10, LOW);sr.set(11, LOW); sr.set(13, LOW);
      audioSelection = 1;
    }
      break;
    case 10:
    if(shift.released(btn)){
      sr.set(9, LOW); sr.set(10, HIGH);sr.set(11, LOW); sr.set(13, LOW);
      audioSelection = 2;
    }
      break;
    case 11:
    if(shift.released(btn)){
      sr.set(9, LOW); sr.set(10, LOW);sr.set(11, HIGH); sr.set(13, LOW);
      audioSelection = 3;
    }
      break;
    case 12:
    if(shift.released(btn)){
      sr.set(9, LOW); sr.set(10, LOW);sr.set(11, LOW); sr.set(13, HIGH);
      audioSelection = 4;
    }
      break;
    case 13:
      if(shift.hasChanged(btn)){
        fileCount = myDFPlayer.readFileCountsInFolder(audioSelection);
        Serial.print("number of files in folder "); Serial.print(audioSelection);Serial.print(" "); Serial.println(fileCount);
        rndAudioFile = random(1, fileCount + 1);
        myDFPlayer.playFolder(audioSelection, rndAudioFile);
      }
      break;
  }
}

//Debug for the DFPlayer
//**********************************************************************
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
