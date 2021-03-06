#include <EEPROM.h>   
#include <SoftwareSerial.h>


SoftwareSerial BTserial(2, 4); // SRX | STX

#define ledPin 13
#define analogPin A0

int val = 0;
char c=' '; //initializes to a space
int numOfReadings=10;

// RGB Variables
int redPin= 7;
int greenPin = 6;
int bluePin = 5;


void Myoread();
void CalibratingSensor();
int GettingAverage();
void setColor();
void Colorloop();

 
void setup() 
{
    //setting up the pins for RGB LED 
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    Serial.begin(9600);
    //BTserial.println("Arduino is ready"); 
    // HC-05 default serial speed for communication mode is 9600
      BTserial.begin(9600);  
   // BTserial.println("BTserial started at 9600");

      //LED Device is  on
      setColor(0, 150, 90); // Green Color
      delay(2000);

   
}
 
void loop()
{
   // Keep reading from HC-05 and send to Arduino Serial Monitor
    
    
    if (BTserial.available())
    {  
        c = BTserial.read();
        //d = BTserial.write();
    }
     if (c == '1') {
        digitalWrite(ledPin, LOW); // Turn LED OFF
        BTserial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
        c = ' ';
    }
    else if (c == '2') {
      
      Serial.println("Myoware 1: sensor readings: ");
       CalibratingSensor(A0);      
       int ave1 =GettingAverage();
       Serial.println("the average is: ");
       Serial.println(ave1);

       BTserial.println("Calibration of sensor 1 is complete");
       BTserial.println();
       BTserial.print("Threshold is: ");
       BTserial.println(ave1);
       
      //White LED shows that process is complete
      setColor(255, 255, 255);
      delay(1000);
      
      exit(0);

      
    } 
    else if (c == '3')
    {
    
      val = analogRead(analogPin);     // read the input pin
     //float voltage = val * 500.0;
    // voltage /= 1024.0; 
     val =  constrain(val, 100, 1000);
     BTserial.println(val); 
      delay(500);
      
      }
}


void CalibratingSensor(int pinNumber){
    int address=0;
   //I need to pass the sensor value
    for(int i=0; i<numOfReadings;i++){
    Serial.print(i);
    Serial.println();
    int sensorValue1 = analogRead(pinNumber)/4; //lower forearm
    Serial.println("sensor 1 value: ");
    Serial.println(sensorValue1);
    EEPROM.put(address, sensorValue1);    

    // maybe put a delay here to get different readdings
    
    address= address+1;
    if(address==EEPROM.length()){
      address=0;
      }
    //LED to show sensor readings
    setColor(0, 0, 255);
    delay(200); 
    }

  }


void Myoread(){

    int address=0;
    int value;
    int ave=0;
    
    for(int i=0; i<numOfReadings;i++){
    value=EEPROM.read(address);
    Serial.print(address);
    Serial.print("\t");
    Serial.print(value);
    Serial.println();
    ave=ave+value;
    Serial.println();
    
    address= address+1;
    if(address==EEPROM.length()){
      address=0;
      }  
      delay(100);}

      Serial.println("The average from the sensor reading is: ");
      Serial.print(ave/10);

  }  

  int GettingAverage(){

    int address=0;
    int value;
    int ave=0;

    //purple LED to show that average is being calculated 
    setColor(255, 0, 255);
    
    for(int i=0; i<numOfReadings;i++){
    value=EEPROM.read(address);
    ave=ave+value;    
    address= address+1;
    if(address==EEPROM.length()){
      address=0;
      }  delay(100);}

      ave=ave/10;
      delay(300);

      return ave;
} 

 
 //function the setup RBG variables into pins 
 void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
} 
