
#include <Streaming.h>


//Outputs
const long RELAY1_DRIVER_PIN = 8; //Variable timing at night
long active = 0;

//Inputs
const long LIGHT_SENSOR_PIN = A0;
long LIGHT_SENSOR_VALUE = 0;

//Thresholds
#ifdef ONTEST 
  const long LIGHT_ON_SENSOR_THRESHOLD = 100;
  const long LIGHT_OFF_SENSOR_THRESHOLD = 75;
#else
  const long LIGHT_ON_SENSOR_THRESHOLD = 850;
  const long LIGHT_OFF_SENSOR_THRESHOLD = 300;
#endif  

//Timing
unsigned long now = 0;
unsigned long starting = 0;
unsigned long timer = 0;
unsigned long ending = 0;
long scount = 0;  //time till start
long ecount = 0;  //time till end

//Used to set schedules
boolean setSchedule = true;

void setup() 
{
  randomSeed(analogRead(0));
  pinMode(RELAY1_DRIVER_PIN, OUTPUT);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  delay(2000);
  digitalWrite(RELAY1_DRIVER_PIN, HIGH);
  delay(5000);
  digitalWrite(RELAY1_DRIVER_PIN, LOW);
  delay(2000);
}

void loop() 
{

  LIGHT_SENSOR_VALUE = smooth(LIGHT_SENSOR_PIN, 10);           //Sample the ambient light

  now = millis();                           //Set the timing baseline
     
  //Only operate at night
  if ( LIGHT_SENSOR_VALUE > LIGHT_ON_SENSOR_THRESHOLD )
  {

    flash(1);
    
    if (setSchedule == true)  //Check if a new schedule needs to be set
    {

      #ifdef ONTEST
        starting = now + randMinutes(1, 3);  //Minutes interval at which to turn on the lights
        timer = randMinutes(1, 3);
      #else
        starting = now + randMinutes(45, 90);  //Minutes interval at which to turn on the lights
        timer = randMinutes(2, 7);
      #endif
      
      ending = starting + timer;  //Number of minutes to keep the lights on
      setSchedule = false;   //Schedule has been set
    }

  }
  else if ( LIGHT_SENSOR_VALUE < LIGHT_OFF_SENSOR_THRESHOLD )
  { //Keep running till the end of the schedule. e.g. When dusk is transitioning to dawn.

    setSchedule = false;
    flash(2);
  }
  else
  {
    flash(0);    
  }


  if ( now <= starting ) 
  { //Wait for starting time
    scount = starting - now;
  }
  else if ((now > starting) && (now < ending))     
  { //Perform
    ecount = ending - now;
    digitalWrite(RELAY1_DRIVER_PIN, HIGH);  //Switch lights on
    active = 1;
  }
  else 
  { //Finish
    digitalWrite(RELAY1_DRIVER_PIN, LOW); //Switch lights off
    setSchedule = true;      //Allow creation of a new schedule
    active = 0;
  }  
}


//Generate a random interval of minutes based on a range
unsigned long randMinutes(int lower, int higher)
{
  unsigned long myMinutes = random(lower, higher);
  unsigned long mySeconds = myMinutes * 60;
  unsigned long myMillis = mySeconds * 1000;  
  return myMillis;
}


//Convert milliseconds to seconds
long SSS2ss(long value)
{
  return (value / 1000);
}


//Running average smoothing for an input
long smooth(int inputPin, int numSamples)
{
  long total = 0;                  // the running total
  long average = 0;                // the average

  for (int pos = 0; pos < numSamples; pos++)
  {
    total = total + analogRead(inputPin);
    delay(1);
  }

  average = total / numSamples;
  return average; 
}

//Define different flashing sequence depending on ambient light level 
void flash(int statusl)
{  

  if (statusl == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(200);                       
    digitalWrite(LED_BUILTIN, LOW);   
    delay(200);                       
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(200);                      
    digitalWrite(LED_BUILTIN, LOW);   
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(200);                       
    digitalWrite(LED_BUILTIN, LOW);    
    delay(500);                       
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(200);                       
    digitalWrite(LED_BUILTIN, LOW);    
    delay(2000);                      
  }
  else if (statusl == 2)
  {
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(1000);                       
    digitalWrite(LED_BUILTIN, LOW);    
    delay(1000);                       
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(100);                       
    digitalWrite(LED_BUILTIN, LOW);    
    delay(100);                       
    
  }

}
