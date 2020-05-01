#ifndef _Line_follow_h
#define _Line_follow_h

//Number of readings to take for calibration
const int NUM_CALIBRATIONS = 50;

/* 
 *  Class to represent a single line sensor
 */
class LineSensor
{
  public:

    // Required function, class Constructor: 
    // Saves the pin passed in as argument and sets to input
    LineSensor(int line_pin) {
      pin = line_pin;
      pinMode(pin, INPUT);
    }

    // Suggested functions.
    void calibrate();       //Calibrate
    int readRaw();         //Return the uncalibrated value from the sensor
    int readCalibrated();  //Return the calibrated value from the sensor

    // You may wish to add other functions!
    // ...
    
  private:
  
    int pin;
    int cal;
    /*
     * Add any variables needed for calibration here
     */
    
};




// Returns unmodified reading.
int LineSensor::readRaw()
{
  return analogRead(pin);
}

// Write this function to measure any
// systematic error in your sensor and
// set some bias values.
void LineSensor::calibrate()
{

  int n = 0;
  for (int i=0; i < NUM_CALIBRATIONS; i++){
    n +=readRaw(); 
  }
  cal = n/NUM_CALIBRATIONS;
  
}


// Use the above bias values to return a
// compensated ("corrected") sensor reading.
int LineSensor::readCalibrated()
{
  return cal;
}


#endif
