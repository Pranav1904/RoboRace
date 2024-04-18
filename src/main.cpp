#include <Arduino.h>
#include <CytronMD.h>
#include <PS4Controller.h>

/*Four Wheel Drive Using Cytron MDD10A motor driver
  and ESP32 */


//MOTOR PIN CONNECTIONS
#define pwmA 13
#define pwmB 27

#define dirA 32
#define dirB 33

int const_limit = 5;

/* Function to stop the Bot*/
void bot_stop() {
  digitalWrite(dirA, 0);
  analogWrite(pwmB, 0);

  digitalWrite(dirB, 0);
  analogWrite(pwmA, 0);
}

/* Function for movement of left track*/
void left_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirA, 1);
   }
   else {
    digitalWrite(dirA, 0);
   }

   if (abs_power*2>240){
      abs_power = 127;
   }
   analogWrite(pwmA, abs_power*2);
   Serial.printf("L_PWM: %d\t", abs_power*2);
}

/* Function for movement of right track*/
void right_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirB, 1);
   }
   else { 
    digitalWrite(dirB, 0);
   }
   if (abs_power*2>240){
      abs_power = 127;
   }

   analogWrite(pwmB, abs_power*2);
   Serial.printf("R_PWM: %d\n", abs_power*2);
}


/* Debugging PS4 Data*/
void print_ps4_data(){
      Serial.printf("Left Stick x at %d: ", PS4.LStickX());
      Serial.printf("Left Stick y at %d: ", PS4.LStickY());
      Serial.printf("Right Stick x at %d: ", PS4.RStickX());
      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
    
}

int dead_zone(int val, int limit = const_limit) {
  if (val >= -limit && val <= limit) {
    return 0;
  }
  else {
    if (val>0){
      return val-limit;
    }

    else {
      return val+limit;
    }
  }
}

CytronMD right_motor,left_motor;


void setup() {
  // put your setup code here, to run once:
  PS4.begin("9C:B6:D0:90:37:C2");
  Serial.begin(115200);

  left_motor.initialize(dirA,pwmA);
  right_motor.initialize(dirB,pwmB);

  pinMode(pwmB, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(dirB, OUTPUT);

  Serial.println("Setup Complete");
}


void loop() {
  // put your main code here, to run repeatedly:
  if(PS4.isConnected()) {

    // print_ps4_data();
    int L_Y = dead_zone(PS4.LStickY());
    int R_Y = dead_zone(PS4.RStickY());

    //Serial.printf("%d, %d\n", L_Y, R_Y);

    int left = L_Y/127;
    int right = R_Y/127;

    
    left_motor.driveRPM();
    right_motor.driveRPM();

    
    
  }

  else{
    bot_stop();
    Serial.println("NO PS4");
  }
}