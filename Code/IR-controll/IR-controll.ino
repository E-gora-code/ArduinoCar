#include <AccelStepper.h>
#include <IRremote.h>
int cou = 0;

int stp_R_pins[] = {2,3,4,5};
int stp_L_pins[] = {6,7,8,9};
AccelStepper stepper_Right(AccelStepper::FULL4WIRE, stp_R_pins[0], stp_R_pins[2], stp_R_pins[1], stp_R_pins[3]);
AccelStepper stepper_Left(AccelStepper::FULL4WIRE, stp_L_pins[0], stp_L_pins[2], stp_L_pins[1], stp_L_pins[3]);

int RECV_PIN = 10;

int speed = 400;
int acceleration = 50;


IRrecv irrecv(RECV_PIN);
decode_results  ir_results,_ir_results;

#define IR_blank 0xFFFFFFFF

#define IR_forward 0xFF18E7
#define IR_stop 0xFF38C7
#define IR_backwards 0xFF4AB5
#define IR_right 0xFF5AA5
#define IR_left 0xFF10EF
const uint32_t IR_all[]={IR_blank,IR_forward,IR_stop,IR_backwards,IR_right,IR_left};
int IR_all_size = 6;

bool is_moving = false;
unsigned long last_stopped = 0;
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("---");
  stepper_Right.setMaxSpeed(speed);
  stepper_Right.setAcceleration(acceleration);
  stepper_Left.setMaxSpeed(speed);
  stepper_Left.setAcceleration(acceleration);


}

void loop() {
  all_run();
  if(ir_results.value == IR_forward){
    stepper_Right.moveTo(stepper_Right.currentPosition()+10000);
    stepper_Left.moveTo(stepper_Right.currentPosition()-10000);
    is_moving = true;
  }
  else if(ir_results.value == IR_backwards){
    stepper_Right.moveTo(stepper_Right.currentPosition()-10000);
    stepper_Left.moveTo(stepper_Right.currentPosition()+10000);
    is_moving = true;
  }
  else if(ir_results.value == IR_right){
    stepper_Right.moveTo(stepper_Right.currentPosition()-10000);
    stepper_Left.moveTo(stepper_Right.currentPosition()-10000);
    is_moving = true;
  }
  else if(ir_results.value == IR_left){
    stepper_Right.moveTo(stepper_Right.currentPosition()+10000);
    stepper_Left.moveTo(stepper_Right.currentPosition()+10000);
    is_moving = true;
  }
  else{
    if(ir_results.value == IR_stop){
      stepper_Right.moveTo(stepper_Right.currentPosition());
      stepper_Left.moveTo(stepper_Left.currentPosition());
      if(is_moving == true){
        last_stopped = millis();
        is_moving = false;
      }
    }
  }
  if((!is_moving)&&(millis()-last_stopped >10000)){
    disableMotor(stp_R_pins);
    disableMotor(stp_L_pins);
  }
  // if(cou<10){
  //   stepper_Right.moveTo(2048*10);        // Целевая позиция (1 оборот ≈ 2048 шагов)
  //   stepper_Left.moveTo(-2048*10); 
  //   wait_all_step();
  //   stepper_Right.moveTo(0);
  //   stepper_Left.moveTo(0);
  //   wait_all_step();
  //   cou +=1;
  // }
  // else{
  //   delay(10000);
  //   disableMotor(stp_R_pins);
  //   disableMotor(stp_L_pins);
  //   while(true);
  // }
  
}
void wait_one_step(AccelStepper stepper){
  while(stepper.distanceToGo()!=0){
    all_run();
  }
}
void wait_all_step(){
  while((stepper_Right.distanceToGo()!=0)||(stepper_Left.distanceToGo()!=0)){
    all_run();
  }
}
void update_receve(){
  if (irrecv.decode(&_ir_results))  {   
    if((_ir_results.value==IR_blank)||(!isInCodes(_ir_results.value))){
      irrecv.resume();
      return;
    }
    ir_results = _ir_results;
    Serial.println(ir_results.value, HEX);
    irrecv.resume(); // Receive  the next value
  }
}
void all_run(){
  stepper_Right.run(); 
    stepper_Left.run(); 
    update_receve();
}
void disableMotor(int mot[]) {
  digitalWrite(mot[0], LOW);
  digitalWrite(mot[1], LOW);
  digitalWrite(mot[2], LOW);
  digitalWrite(mot[3], LOW);
}

bool isInCodes(uint32_t code) {
    for (int i = 0; i < IR_all_size; i++) {
        if (IR_all[i] == code) {
            return true;
        }
    }
    return false;
}