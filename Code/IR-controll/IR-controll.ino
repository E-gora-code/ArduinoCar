#include <AccelStepper.h>
int cou = 0;
// Настройка пинов (тип FULL4WIRE = 4-проводной униполярный мотор)
int stp1_pins[] = {2,3,4,5};
AccelStepper stepper(AccelStepper::FULL4WIRE, stp1_pins[0], stp1_pins[2], stp1_pins[1], stp1_pins[3]);

void setup() {
  stepper.setMaxSpeed(400);    // Макс. скорость (шагов/сек)
  stepper.setAcceleration(50); // Ускорение (шагов/сек²)
}

void loop() {
  stepper.run();  // Запуск мотора (неблокирующий)
  if(cou<10){
    stepper.moveTo(2048*1);        // Целевая позиция (1 оборот ≈ 2048 шагов)
    wait_step();
    stepper.moveTo(0);
    wait_step();
    cou +=1;
  }
  else{
    delay(10000);
    disableMotor(stp1_pins);
    while(true);
  }
}
void wait_step(){
  while(stepper.distanceToGo()!=0){
    stepper.run(); 
  }
}
void disableMotor(int mot[]) {
  digitalWrite(mot[0], LOW);
  digitalWrite(mot[1], LOW);
  digitalWrite(mot[2], LOW);
  digitalWrite(mot[3], LOW);
}