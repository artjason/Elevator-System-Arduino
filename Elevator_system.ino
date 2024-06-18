#include <Stepper.h>

#define STEPS 2100

Stepper stepper (STEPS, 9,10,11,12);
int IRSensor = A4; 
  const int trigPin = A0, echoPin = A1;
  const int firstFloorButton = 13, secondFloorButton = A2, thirdFloorButton = A3;
  int  firstFloorButtonState = HIGH, secondFloorButtonState = HIGH, thirdFloorButtonState=HIGH;
  int firstFloorLastButtonState = HIGH, secondFloorLastButtonState = HIGH, thirdFloorLastButtonState = HIGH;
  const int LEDFirstFloor = A4, LEDSecondFloor = A5, LEDThirdFloor = 1;

  
  long duration;
  int distance;
  int floorNum = 0;
 
void setup() {
    DDRC = 0x01; 
    DDRD = 0xFC;
    DDRB = 0x1E;
    pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
    for(int i=2; i<=8;i++){
      pinMode(i,OUTPUT);
    }
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(firstFloorButton, INPUT_PULLUP);
    pinMode(secondFloorButton, INPUT_PULLUP);
    pinMode(thirdFloorButton, INPUT_PULLUP);
    Serial.begin(9600);
    stepper.setSpeed(60);
}

void loop() {
 
  measureDistance();
  //read button
  checkButtons();

}

//Elevator Going Up
void upElevator(){
   digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11,LOW);
    digitalWrite(12, LOW);
    delay(5);
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11,LOW);
    digitalWrite(12, LOW);
    delay(5);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11,HIGH);
    digitalWrite(12, LOW);
    delay(5);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11,LOW);
    digitalWrite(12, HIGH);
    delay(5);
}

//Elevator Going Down
void downElevator(){
  digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11,LOW);
    digitalWrite(12, HIGH);
    delay(5);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11,HIGH);
    digitalWrite(12, LOW);
    delay(5);
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11,LOW);
    digitalWrite(12, LOW);
    delay(5);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11,LOW);
    digitalWrite(12, LOW);
}

//Stop Elevator
void stopElevator() {
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
}


void displayFloorNumber(int floorNum){
   PORTD= 0x00;
    PORTB= 0x00;
    if(floorNum == 1){
    PORTD = 0x18;
    PORTB = 0x00;
    }
    else if(floorNum == 2){
      PORTD = 0x6C;
      PORTB = 0x01;
    }
    else if(floorNum == 3){
    PORTD = 0x3C;
    PORTB = 0x01;
    }
}

void checkButtons(){

int reading1 = digitalRead(firstFloorButton);  // Read the current state of the button
int reading2 = digitalRead(secondFloorButton);
int reading3 = digitalRead(thirdFloorButton);

// Store the current button state into last button state variable

firstFloorLastButtonState = firstFloorButtonState;
secondFloorLastButtonState = secondFloorButtonState;
thirdFloorLastButtonState = thirdFloorButtonState;
firstFloorButtonState = reading1;
secondFloorButtonState = reading2;
thirdFloorButtonState = reading3;

// Check if the button state has changed from LOW to HIGH (indicating a button press)

//button1
if (firstFloorButtonState == HIGH && firstFloorLastButtonState == LOW){
Serial.println("MOVE TO FIRST FLOOR");  // Print "FIRST FLOOR" to serial monitor
    moveToFloor(24, 1);
}

//button2
if (secondFloorButtonState == HIGH && secondFloorLastButtonState == LOW) {
Serial.println("MOVE TO SECOND FLOOR");  
    moveToFloor(15, 2);
    // Example of toggling an LED (uncomment if needed, make sure ledPin1 is defined)
    // digitalWrite(ledPin1, !digitalRead(ledPin1));
}


//button3
if (thirdFloorButtonState == HIGH && thirdFloorLastButtonState == LOW) {
  Serial.println(" MOVE TO THIRD FLOOR");  
    moveToFloor(2, 3);
    // Example of toggling an LED (uncomment if needed, make sure ledPin1 is defined)
    //digitalWrite(ledPin1, !digitalRead(ledPin1));
}

}

void measureDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;
Serial.print("Distance:");
Serial.println(distance);

  if(distance >= 20){
   displayFloorNumber(1);
  }
  else if(distance >=14 && distance <=15){
     displayFloorNumber(2);
  }

   else if(distance >=1 && distance <=3){
     displayFloorNumber(3);
  }
}

void moveToFloor (int targetDistance, int targetFloor){
   while(distance != targetDistance){
     if(distance < targetDistance){
//      Serial.println("Elevator Going Down");
         downElevator();
     }
     else if(distance > targetDistance){
//     Serial.println("Elevator Going Up");
         upElevator();
     }

    measureDistance();
    
  //IR SENSOR
  int sensorStatus = digitalRead(IRSensor); // Set the GPIO as Input
  if (sensorStatus == 0) // Check if the pin high or not
  {
    break;
  }
   }
   floorNum = targetFloor;
   Serial.println("Stop Elevator");
   stopElevator();
   
}