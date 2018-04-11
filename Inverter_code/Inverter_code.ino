#include <InverterController.h>

static InverterControllerModelClass InverterControllerModelObj_;

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
    rtmSetErrorStatus(InverterControllerModelObj_.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model for base rate
  InverterControllerModelObj_.step();

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}



void setup() {
  InverterControllerModelObj_.initialize();
  // put your setup code here, to run once:
  
}

void loop() {

//  InverterControllerModelObj_.InverterController_U.Vac1 =
//  InverterControllerModelObj_.InverterController_U.Vac2 =
//  InverterControllerModelObj_.InverterController_U.Vac3 =
//  InverterControllerModelObj_.InverterController_U.Iac1 =
//  InverterControllerModelObj_.InverterController_U.Iac2 =
//  InverterControllerModelObj_.InverterController_U.Vdc_m =
  rt_OneStep();
//   = InverterControllerModelObj_.InverterController_Y.
  


  
  // put your main code here, to run repeatedly:

}
