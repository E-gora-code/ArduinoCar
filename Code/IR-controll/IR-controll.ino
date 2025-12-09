#include <AccelStepper.h>
#include <IRremote.h>
int cou = 0;

int stp_R_pins[] = {2,3,4,5};
int stp_L_pins[] = {6,7,8,9};
AccelStepper stepper_Right(AccelStepper::FULL4WIRE, stp_R_pins[0], stp_R_pins[2], stp_R_pins[1], stp_R_pins[3]);
AccelStepper stepper_Left(AccelStepper::FULL4WIRE, stp_L_pins[0], stp_L_pins[2], stp_L_pins[1], stp_L_pins[3]);

int RECV_PIN = 10;

int speed = 200;
int acceleration = 50;


IRrecv irrecv(RECV_PIN);
decode_results  ir_results,_ir_results;
int move_dist_change = 465;
int move_dist = move_dist_change*4;
int move_count_id_now = 0;
int move_count_id_last = 0;
#define IR_blank 0xFFFFFFFF

#define IR_forward 0xFF18E7
#define IR_stop 0xFF38C7
#define IR_backwards 0xFF4AB5
#define IR_right 0xFF5AA5
#define IR_left 0xFF10EF
#define IR_more_dist 0xFFA857
#define IR_less_dist 0xFFE01F
const uint32_t IR_all[]={IR_blank,IR_forward,IR_stop,IR_backwards,IR_right,IR_left,IR_more_dist,IR_less_dist};
int IR_all_size = 8;

bool is_moving = false;
unsigned long last_moved = 0;
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
    if(move_dist <= 0){
      stepper_Right.moveTo(stepper_Right.currentPosition()+10000);
      stepper_Left.moveTo(stepper_Left.currentPosition()-10000);
    }else{
      if(check_command_update()){
        stepper_Right.moveTo(stepper_Right.currentPosition()+move_dist);
        stepper_Left.moveTo(stepper_Left.currentPosition()-move_dist);
      }
    }
  }
  else if(ir_results.value == IR_backwards){
    if(move_dist <= 0){
      stepper_Right.moveTo(stepper_Right.currentPosition()-10000);
      stepper_Left.moveTo(stepper_Left.currentPosition()+10000);
    }else{
      if(check_command_update()){
        stepper_Right.moveTo(stepper_Right.currentPosition()-move_dist);
        stepper_Left.moveTo(stepper_Left.currentPosition()+move_dist);
      }
    }
  }
  else if(ir_results.value == IR_right){
    if(move_dist <= 0){
      stepper_Right.moveTo(stepper_Right.currentPosition()-10000);
      stepper_Left.moveTo(stepper_Left.currentPosition()-10000);
    }else{
      if(check_command_update()){
        stepper_Right.moveTo(stepper_Right.currentPosition()-move_dist/4);
        stepper_Left.moveTo(stepper_Left.currentPosition()-move_dist/4);
      }
    }
  }
  else if(ir_results.value == IR_left){
    if(move_dist <= 0){
      stepper_Right.moveTo(stepper_Right.currentPosition()+10000);
      stepper_Left.moveTo(stepper_Left.currentPosition()+10000);
    }else{
      if(check_command_update()){
        stepper_Right.moveTo(stepper_Right.currentPosition()+move_dist/4);
        stepper_Left.moveTo(stepper_Left.currentPosition()+move_dist/4);
      }
    }
  }
  else if(ir_results.value == IR_more_dist){
    if(check_command_update()){
      move_dist += move_dist_change;
      Serial.println(move_dist);
    }
  }
  else if(ir_results.value == IR_less_dist){
    if(check_command_update()){
      move_dist -= move_dist_change;
      if(move_dist<0){
        move_dist = 0;
      }
      Serial.println(move_dist);
    }
  }
  else{
    if(ir_results.value == IR_stop){
      stepper_Right.moveTo(stepper_Right.currentPosition());
      stepper_Left.moveTo(stepper_Left.currentPosition());
    }
  }
  if((stepper_Right.distanceToGo()!=0)||(stepper_Left.distanceToGo()!=0)){
    is_moving = true;
    last_moved = millis();
  }else{
    is_moving = false;
  }
  if((!is_moving)&&(millis()-last_moved >30*1000)){
    disableMotor(stp_R_pins);
    disableMotor(stp_L_pins);
  }
  
}
bool check_command_update(){
  if(move_count_id_now!=move_count_id_last){
    move_count_id_last = move_count_id_now;
    return true;
  }else{
    return false;
  }
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
    Serial.println(_ir_results.value, HEX);
    if((_ir_results.value==IR_blank)||(!isInCodes(_ir_results.value))){
      irrecv.resume();
      return;
    }
    ir_results = _ir_results;
    move_count_id_now += 1;
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