#include <AccelStepper.h>
int cou = 0;
// Настройка пинов (тип FULL4WIRE = 4-проводной униполярный мотор)
int stp_1 = 2;
int stp_2 = 3;
int stp_3 = 4;
int stp_4 = 5;
AccelStepper stepper(AccelStepper::FULL4WIRE, stp_1, stp_3, stp_2, stp_4);

void setup() {
  stepper.setMaxSpeed(500);    // Макс. скорость (шагов/сек)
  stepper.setAcceleration(200); // Ускорение (шагов/сек²)
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
    disableMotor();
    while(true);
  }
}
void wait_step(){
  while(stepper.distanceToGo()!=0){
    stepper.run(); 
  }
}
void disableMotor() {
  digitalWrite(stp_1, LOW);
  digitalWrite(stp_2, LOW);
  digitalWrite(stp_3, LOW);
  digitalWrite(stp_4, LOW);
}