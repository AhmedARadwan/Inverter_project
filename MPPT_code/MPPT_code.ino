#include <mppt_controller_.h>


// frequency should be 10-15 kHz
// or 10-20 kHz

static mppt_controller_ModelClass mppt_controller__Obj;



void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}






void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(mppt_controller__Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model for base rate
  mppt_controller__Obj.step();

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

float volt_sig = 0;
float cur_sig = 0;
float pow_sig = 0;
float comp_DC = 0;

#define volt_map 1000 //Should be changed
#define cur_map 1000  //Should be changed

#define PWM_pin 5
#define divisor 8 //best value ==> PWM frequency = 7812.50 Hz

void setup() {
  mppt_controller__Obj.initialize();
  setPwmFrequency(PWM_pin, divisor);
  Serial.begin(9600);

}

void loop() {
  volt_sig = ((analogRead(A0)/1023.0)*5.0)*volt_map;
  cur_sig = (((analogRead(A0)/1023.0)*5.0) - 2.5)*cur_map;
  pow_sig = volt_sig * cur_sig;

  
  
  mppt_controller__Obj.mppt_controller__U.VoltSig = volt_sig;
  mppt_controller__Obj.mppt_controller__U.PowSig = pow_sig;
  rt_OneStep();
  comp_DC = mppt_controller__Obj.mppt_controller__Y.DutyCycle_;
  analogWrite(PWM_pin, comp_DC);

  Serial.print("I = ");
  Serial.print(cur_sig);
  Serial.print("\tV = ");
  Serial.print(volt_sig);
  Serial.print("\tP = ");
  Serial.print(pow_sig);
  Serial.print("\tDC = ");
  Serial.println(comp_DC);
  

}



