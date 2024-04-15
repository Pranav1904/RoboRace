#include <Arduino.h>

#include <PS4Controller.h>

/*Four Wheel Drive Using L298N motor driver and ESP32 */


//MOTOR PIN CONNECTIONS
// PWM PINS
#define pwmA 13
#define pwmB 27

// DIR PINS
#define dirA1 32
#define dirA2 34
#define dirB1 33
#define dirB2 35

int const_limit = 5;

/* Function to stop the Bot*/
void bot_stop() {
  digitalWrite(dirA1, 0);
  digitalWrite(dirA2, 0);
  analogWrite(pwmB, 0);

  digitalWrite(dirB1, 0);
  digitalWrite(dirB2, 0);
  analogWrite(pwmA, 0);
}

/* Function for movement of left track*/
void left_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirA1, 1);
    digitalWrite(dirA2, 0);
   }
   else {
    digitalWrite(dirA1, 0);
    digitalWrite(dirA2, 1);
   }

   if (abs_power*2>240){
      abs_power = 127;
   }
   analogWrite(pwmA, abs_power*2);
   Serial.printf("R_PWM: %d\t", abs_power*2);
}

/* Function for movement of right track*/
void right_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirB1, 1);
    digitalWrite(dirB2, 0);
   }
   else { 
    digitalWrite(dirB1, 0);
    digitalWrite(dirB2, 1);
   }

   if (abs_power*2>240){
      abs_power = 127;
   }
   analogWrite(pwmB, abs_power*2);
   Serial.printf("L_PWM: %d\n", abs_power*2);
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


void setup() {
  // put your setup code here, to run once:
  PS4.begin("9C:B6:D0:90:37:C2");
  Serial.begin(115200);

  pinMode(pwmB, OUTPUT);
  pinMode(pwmA, OUTPUT);

  pinMode(dirA1, OUTPUT);
  pinMode(dirA2, OUTPUT);
  pinMode(dirB1, OUTPUT);
  pinMode(dirB2, OUTPUT);

  Serial.println("Setup Complete");
}


void loop() {
  // put your main code here, to run repeatedly:
  if(PS4.isConnected()) {

    // print_ps4_data();
    int L_Y = dead_zone(PS4.LStickY());
    int R_Y = dead_zone(PS4.RStickY());

    //Serial.printf("%d, %d\n", L_Y, R_Y);

    left_track(R_Y);
    right_track(L_Y);
  }

  else{
    bot_stop();
    Serial.println("NO PS4");
  }
}