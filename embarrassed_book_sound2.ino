// The Embarrassed Book
// By Andrew Cerrito

#include <Servo.h>
#include <SoftwareSerial.h>

//Servo setup
Servo servoMotor;
int servoPin = 4;

//Software serial setup for mp3 trigger
SoftwareSerial mySerial(2,3); // RX, TX

//Timer setup and cover-closing time (from 4s to 12s)
unsigned long timer;
long coverOpenTime;
long targetTime = random(4000,12000);

// Cover-related booleans
boolean coverOpened = false;
boolean resetOpenTime = true;
boolean midpointPlayed = false;

//Mp3 trigger variables
int ssinByte = 0;
int val = 0; 
boolean trialRun = true; 


void setup() {
  //Serial.begin(38400);
  servoMotor.attach(servoPin);
  servoMotor.write(60); // make sure arm begins retracted below slit

  mySerial.begin(38400);
  mySerial.println("READY");
  delay(1000);
}


void loop() {
  //update photocell reading on analog port 0
  int photocell = analogRead(A0);
  // Serial.println(photocell);

  // if cover is obscuring photocell:
  if (photocell < 600) {
    coverOpened = true; // it means the cover is open
    if (resetOpenTime == true) {
      coverOpenTime = millis(); // remember the time when the cover opened
      resetOpenTime = false; // don't overwrite the cover opening time in the next loop.
    }
  }

  if (coverOpened == true) {
    // adjust the timer to discard the time before the cover opened
    timer = millis() - coverOpenTime;

//    Serial.print("Target time: ");
//    Serial.println(targetTime);
//    Serial.print("Timer:");
//    Serial.println(timer);

    // if total time of interaction is going to be over 7s, play an additional
    // contextual sound at roughly the halway point of the target time
    if (targetTime > 7000) {
      if (timer > (targetTime/2 + random(-1000,1000)) && timer < targetTime)
      {
        if (midpointPlayed == false)
        {
          playRandom(5,7); // midpoint audio cue plays
          midpointPlayed = true; 
        }
      }
    }


    // if timer exceeds the targeted time:
    if (timer > targetTime) {
      playRandom(1,5); // audio cue plays
      delay(500);
      servoMotor.write(179); // arm fully extends to flip cover closed
      delay(500); 
      servoMotor.write(60); // arm retracts
      coverOpened = false; // cover is no longer open
      targetTime = random(4000,12000); // target time resets for next use
      midpointPlayed = false; // midpoint sound check resets
      resetOpenTime = true; // the time the cover is opened can be detected for next use
    }
  }
}

// MP3 Trigger functions below:

void runSSerial(){

  if(mySerial.available() > 0){
    ssinByte = mySerial.read();
    //Serial.println(ssinByte);

    if(ssinByte == 'x' || ssinByte == 'X'){ 
      trialRun = true;
      ssinByte = -1; 
    }   
    //mySerial.println(inByte);
  }
}

void playRandom(int rand1, int rand2){
  runSSerial();

  if(trialRun == true){ 
    //Serial.println("GO"); 

    val = random(rand1,rand2); 
    String stringOne = "T"; 
    String stringTwo = stringOne + val; 
    mySerial.println(stringTwo);
    //Serial.println(stringTwo); 
    trialRun = false;  
  }
}

void playTrack(int trackNum) {
  runSSerial();

  if(trialRun == true){ 
    Serial.println("GO"); 

    String stringOne = "T"; 
    String stringTwo = stringOne + trackNum; 
    mySerial.println(stringTwo);
    //Serial.println(stringTwo); 
    trialRun = false;  

  }
}

// Volume function for testing purposes - unused in final program
void volume(int volume) {
  runSSerial();

  if(trialRun == true){ 
    Serial.println("GO"); 

    String stringOne = "v"; 
    String stringTwo = stringOne + volume; 
    mySerial.println(stringTwo);
    //Serial.println(stringTwo); 
    trialRun = false;  

  }
}









