// Serial test script
#include <Servo.h>

//String readString;
char readString[4*2];

// defines pins numbers
//const int trigPin = 10;
//const int echoPin = 12;

Servo accServo;


// defines variables
long duration;
int distance;
//const char term = '\n';
//int scale = 3;


void setup()
{
  //pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  //pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  accServo.attach(9);
  Serial.begin(115200);  // initialize serial communications at 9600 bps

}

/*float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}*/

void loop()
{
  while(!Serial.available()) {}
  // serial read section

  // Clears the trigPin
  int rawX = analogRead(A0);
  int rawY = analogRead(A1);
  int rawZ = analogRead(A2);

  while (Serial.available())
  {
    if (Serial.available() > 0)
    { 
      byte size = Serial.readBytes(readString, 7);
      readString[size] = 0;
      //readString = Serial.readStringUntil('\n');
    }

    char* pos_c = strtok(readString, ":");
    int count = 0;
    while (pos_c != NULL)
    {

        int position = atoi(pos_c);
        if (count == 0) {
          accServo.write(position);
        }
        //Do something with servoId and position
        pos_c = strtok(NULL, ":");
        count++;
    }
    
  
    //pos = atoi(readString);
    //accServo.write(pos);
    /*      
    float scaledX, scaledY, scaledZ; // Scaled values for each axis
    scaledX = mapf(rawX, 0, 675, -scale, scale); // 3.3/5 * 1023 =~ 675
    scaledY = mapf(rawY, 0, 675, -scale, scale); // 3.3/5 * 1023 =~ 675
    scaledZ = mapf(rawZ, 0, 675, -scale, scale); // 3.3/5 * 1023 =~ 675
      */

    // Ultrasound probe
    /*
    digitalWrite(trigPin, LOW);
    delayMicroseconds(10);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH); // HIGH
    // Calculating the distance
    distance= duration*0.34/2;
    Serial.print(distance);
    Serial.print(" ");
    */
    
    Serial.print(rawX); //(scaledX); //, rawY, rawZ);
    Serial.print(" ");
    Serial.print(rawY); //(scaledY); //, rawY, rawZ);
    Serial.print(" ");
    Serial.println(rawZ); //(scaledZ); //, rawY, rawZ);
  
    delay(2);
    Serial.flush();
    
    
  }
  
  
}

