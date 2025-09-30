#include <kipr/wombat.h>


// -------- Ports --------
#define LEFT_MOTOR  0
#define RIGHT_MOTOR 3
#define SERVO_ARM   0


// -------- Motion constants (CALIBRATE THESE) --------
#define TICKS_PER_INCH   250    // encoder ticks per inch (calibrate!)
#define TURN_90_TICKS    900    // ticks for ~90° turn (calibrate!)
#define TWIST_TICKS      TURN_90_TICKS / 3    // ticks for small wiggle
#define DRIVE_POWER      60
#define TURN_POWER       60


// -------- Servo positions (CALIBRATE THESE) --------
#define SERVO_UP_POS     200
#define SERVO_DOWN_POS   1200




void set_motors(int l, int r){
   motor(LEFT_MOTOR, l);
   motor(RIGHT_MOTOR, r);
}


void reset_mpc(void){
   cmpc(LEFT_MOTOR);
   cmpc(RIGHT_MOTOR);
}


int left_ticks(void){  return gmpc(LEFT_MOTOR); }
int right_ticks(void){ return gmpc(RIGHT_MOTOR); }


void servo_down(void){
   set_servo_position(SERVO_ARM, SERVO_DOWN_POS);
   msleep(300);
}
void servo_up(void){
   set_servo_position(SERVO_ARM, SERVO_UP_POS);
   msleep(300);
}


// -------- Motion --------


// Drive straight for N inches using *left* wheel only
void drive_inches(float inches, int power){
   int target = (int)(inches * TICKS_PER_INCH);
   reset_mpc();
   set_motors(power, power);
   while (left_ticks() < target){
       msleep(10);
   }
   ao();
}


// In-place left turn using left wheel ticks
void turn_left_ticks(int ticks, int power){
   reset_mpc();
   set_motors(-power, power);
   while (-left_ticks() < ticks){
       msleep(10);
   }
   ao();
}


// In-place right turn using right wheel ticks
void turn_right_ticks(int ticks, int power){
   reset_mpc();
   set_motors(power, -power);
   while (-right_ticks() < ticks){
       msleep(10);
   }
   ao();
}


// Twist = small left then small right
void twist_once(void){
   turn_left_ticks(TWIST_TICKS, TURN_POWER);
   turn_right_ticks(TWIST_TICKS, TURN_POWER);
}


// -------- Routes --------


// Route 1: forward 2 ft, lower servo, twist 5 times
void route_1(void){
   drive_inches(24.0, DRIVE_POWER);
   servo_down();
   for (int i=0; i<5; i++) {
       printf("Twist number %d\n", i);
       twist_once();
   }
}


// Route 2: forward 10", left turn, forward 4 ft, lower & raise servo
void route_2(void){
   drive_inches(10.0, DRIVE_POWER);
   turn_left_ticks(TURN_90_TICKS, TURN_POWER);
   drive_inches(48.0, DRIVE_POWER);
   servo_down();
   msleep(400);
   servo_up();
}


// -------- Main --------
#define RUN_ROUTE 1  // set to 1 or 2
#define RUN_CALIBRATION 0  // Set to 1 to drive forward 12 inches


int main(void){
   printf("Starting route %d\n", RUN_ROUTE);
   enable_servos();
   servo_up(); // start up
  
   if (RUN_CALIBRATION == 1) {
       printf("Performing calibration, measure actual distance and adjust TICKS_PER_INCH.\n");
       drive_inches(12.0, DRIVE_POWER);
   } else {
      
       if (RUN_ROUTE == 1) {
           route_1();
       } else if (RUN_ROUTE == 2) {
           route_2();
       } else {
           printf("Set RUN_ROUTE to 1 or 2.\n");
       }
   }
  
   ao();
   disable_servos();
   printf("Done.\n");
   return 0;
}
