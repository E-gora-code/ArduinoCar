#include <AccelStepper.h>
int cou = 0;

int stp_R_pins[] = {2,3,4,5};
int stp_L_pins[] = {6,7,8,9};
AccelStepper stepper_Right(AccelStepper::FULL4WIRE, stp_R_pins[0], stp_R_pins[2], stp_R_pins[1], stp_R_pins[3]);
AccelStepper stepper_Left(AccelStepper::FULL4WIRE, stp_L_pins[0], stp_L_pins[2], stp_L_pins[1], stp_L_pins[3]);

int speed = 600+200;
int acceleration = 50;

void setup() {
  stepper_Right.setMaxSpeed(speed);
  stepper_Right.setAcceleration(acceleration);
  stepper_Left.setMaxSpeed(speed);
  stepper_Left.setAcceleration(acceleration);
}

void loop() {
  stepper_Right.run();
  stepper_Left.run();
  if(cou<10){
    stepper_Right.moveTo(2048*10);        // Целевая позиция (1 оборот ≈ 2048 шагов)
    stepper_Left.moveTo(-2048*10); 
    wait_all_step();
    stepper_Right.moveTo(0);
    stepper_Left.moveTo(0);
    wait_all_step();
    cou +=1;
  }
  else{
    delay(10000);
    disableMotor(stp_R_pins);
    disableMotor(stp_L_pins);
    while(true);
  }
}
void wait_one_step(AccelStepper stepper){
  while(stepper.distanceToGo()!=0){
    stepper.run(); 
  }
}
void wait_all_step(){
  while((stepper_Right.distanceToGo()!=0)||(stepper_Left.distanceToGo()!=0)){
    stepper_Right.run(); 
    stepper_Left.run(); 
  }
}
void disableMotor(int mot[]) {
  digitalWrite(mot[0], LOW);
  digitalWrite(mot[1], LOW);
  digitalWrite(mot[2], LOW);
  digitalWrite(mot[3], LOW);
}