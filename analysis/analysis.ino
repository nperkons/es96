/* 

Analysis Script 
ES96 Design Project 
November 10, 2012
Nick Perkons, Justine Hasson, Alec Delaney

*/


// header files
#include <SD.h>
#include <Time.h>

// constants
const byte num_tsensors = 10; // in total
const byte num_psensors = 10; // in total
const float t_sr = .1; // in Hz
const byte t_sr = 50; // in Hz

const byte temp_threshold = 4;
const byte window_length = samp_rate * 60 * 5;  // currently collecting data for five minutes

// justines constants
const unsigned char numSensors = 1; // if we can hook both insoles up to the same arduino, this is the total number of sensors on both feet...if not this code must be duplicated for              
const unsigned int window = 1024; 
const unsigned int freqSampling = 50; // sampling rate
const unsigned int timeRun = 10; // not yet sure how long it will take for the loop to run..we will alter this
const unsigned char integratedThreshold = 100;
const unsigned int peakPressure = 100;
const unsigned int stepThreshold = 20;

// nicks array initializations
int lefttemps[window_length][(num_sensors/2)+1];
int righttemps[window_length][num_sensors/2+1];  
int leftavgtemp[num_sensors/2];  // note.. doesn't include timestamp  
int rightavgtemp[num_sensors/2];  
int tempdiffs[num_sensors/2];

// justines arrays
unsigned int pressures[window][numSensors + 1];
unsigned int integrated[window][numSensors + 1];

// justines index
int index = 0;


// initializes booleans  
boolean device_on = true;
boolean red_light = false;
boolean temp_flag = false;
boolean green_light = true;
  
// arduino pins
const unsigned int temppin = 9;
const unsigned int CS_pin = 10; //  CS = chip select for SPI??
const unsigned int pow_pin = 8;

// variable initialization
int windex = 0; 
  


void setup()
{
  
  // establishes connection with computer for debugging
  Serial.begin(9600);
  Serial.println("Initializing Card");
  pinMode(CS_pin, OUPUT);
  
  // card will draw power from Pin 8, so set it to high
  pinMode(pow_pin,OUTPUT);
  digitalWrite(pow_pin, HIGH);
  
  // checks if SD card is ready
  if(!SD.begin(CS_pin))
  {
    Serial.println("Card Failed");
    return;
  }
  Serial.println("Card Ready");

  // initializes file that we will be using
  File dataFile = SD.open("data.csv", FILE_WRITE);
  
  // prints header line to data file 
  if (datafile)
  {
    dataFile.println("Analysis file \n Time, Temp1, Temp2, Temp3, Temp4, Temp5, Temp6, Press1, Press2, Press3, Press4, Press5, Press6 \n"); 
  }
  // case where it is not able to write to file
  else
  {
    Serial.println("Couldn't access file");
    return;
  }  
  
  // initializes starting temperature matrix full of zeroes
  for (int i=0; i<window_length; i++)
  {
    for (int j=0; j<(num_sensors/2+1); j++)
    {
      lefttemps[i][j] = 0;
      righttemps[i][j] = 0;
    } 
  }
  
  // pressure initialized as 0
  for (int j = 0; j < window; j++)
  {
    for (int sensor = 0; sensor =< numSensors; sensor++)
    {
      pressures[index][sensor] = 0;
    }
  }
}



void loop()
{
  while(device_on == true)
  {
    
    // collects temp data from pin
    for (int i=0; i<num_sensors; i_++)
    {
      if (i == 0)
      {
        // insert timestamp function here, may need to buy a separate shield
        lefttemps[windex][i] = millis();
        righttemps[windex][i] = millis();          
      }
      else if (i <= (num_sensors/2) )
      {
        // also we need to add transfer function here to convert voltages to temperatures
        lefttemps[windex][i] = analogRead(temppin);  
      }
      else
      {
        righttemps[windex][i] = analogRead(temppin);  
      }
    }
    
    
    // calculates average temperatures on each foot
    for (int i=0; i<window_length; i++)
    {
      for (int j=1; j<(num_sensors/2+1); j++)
      {
        leftavgtemp[j-1] += lefttemps[i][j];
        rightavgtemp[j-1] += righttemps[i][j];  
      } 
    }
    for (k=1; k<(num_sensors/2); k++)
    {
      leftavgtemp[k] = leftavgtemp[k]/window_length;
      rightavgtemp[k] = leftavgtemp[k]/window_length;
    }
      
    // compares average temperatures on each foot
    for (m=1; m<(num_sensors/2); m++)
    {
      if ( (rightavgtemp[m] - leftavgtemp[m] > 4) || (rightavgtemp[m] - leftavgtemp[m] < -4) )
      {
        red_light = true;
        temp_flag = true;
      }
    }
    
    // if no red_light ever went off, set light to green
    if (temp_flag == false)
    {
      green_light = true;
      red_light = false;  
    }   
  
    // saves data every five minutes, when buffer fills up
    if (windex == window_length - 1)
    {
      for (int i=0; i<window_length; i++)
      {
        // saves timestamp
        dataFile.println(lefttemps[i][0]);
        dataFile.println(",");
        
        // saves left foot data
        for (int j=1; j<(num_sensors/2+1); j++);
        {   
          dataFile.println(lefttemps[i][j]);
          dataFile.println(",");
        }  
        
        // saves right foot data
        for (int k=1; k<(num_sensors/2+1); k++);
        {   
          dataFile.println(righttemps[i][k]);
          dataFile.println(",");
        }

        // appends new line
        dataFile.println("\n");

         
         // pressure code
         
         // read each sensor at the sampling rate and store in current indexed array
  if (analogRead(pin) > stepThreshold)
  {
  for (int sensor = 1; sensor =< numSensors; sensor++)
  {  
    // if a single really high pressure occurs, alert 
    if (analogRead(pin) > peakPressure)
    {  
      boolean red = true;
    } 
    pressures[index][sensor] = analogRead(pin);
    delay(1/freqSampling/6-timeRun);
  }
 
  
  // create integrated array
  for (int sensor; sensor < numSensor; sensor++)
  {
    integrated[index][sensor] = sum(pressures[sensor])>> window; 
    if (integrated[index][sensor] > integratedThreshold)
       boolean red = true; 
  }
  
  if (boolean red == true)
  {
    // unsure of mechanism to shine light
  }
  index = (index + 1) % window;
  }

        
      }
    }
    
    
    // reinitializes things for next iteration
    temp_flag = false;
    windex = (windex + 1) % window_length;

  
  // closes file at end
  dataFile.close();
}


 

