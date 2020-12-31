#include <Stepper.h> 
#define STEPS 150

Stepper stepper(STEPS, 2, 3); // Pin 2 connected to DIRECTION & Pin 3 connected to STEP Pin of Driver
#define motorInterfaceType 1
int Pval = 0;
int potVal = 0;

void setup() {
  stepper.setSpeed(1000);
}
void loop() {
  potVal = map(analogRead(A0),0,1024,0,500);
  if (potVal>Pval)
      stepper.step(20);
  if (potVal<Pval)
      stepper.step(-20);
  Pval = potVal;
   
}
