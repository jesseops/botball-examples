/*
 *   Botball Straight Line Example
 */

// Include the KIPR Wombat library for robot control functions
#include <kipr/wombat.h>
#include <stdlib.h>

// Motor ports: change these if your robot uses different ports
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 3

// How fast the robot moves (change this number to go faster or slower)
#define DEFAULT_VELOCITY 900

// Wheel and motor settings (measure these for your own robot!)
#define WHEEL_CIRCUMFERENCE_CM 22.1 // How far the wheel goes in one turn
#define TICKS_PER_ROTATION 1820     // How many ticks for one full wheel turn

// Calculate how many ticks for each centimeter
const float TICKS_PER_CM = TICKS_PER_ROTATION / WHEEL_CIRCUMFERENCE_CM;

// Pick one motor as the "standard" (left)
// Adjust the right motor speed to help the robot go straight
const float RIGHT_MOTOR_ADJUST = 0.97; // Change this if your robot curves

// Move straight for a certain distance (in centimeters)
void move_straight(int velocity, float distance_cm) {
    int direction = (distance_cm >= 0) ? 1 : -1;
    int target_ticks = (int)(direction * distance_cm * TICKS_PER_CM);

    printf("Requested movement: direction=%d ticks=%d\n", direction, target_ticks);

    // Reset the counters
    clear_motor_position_counter(LEFT_MOTOR);
    clear_motor_position_counter(RIGHT_MOTOR);

    // Start moving both motors
    motor(LEFT_MOTOR, velocity * direction);
    motor(RIGHT_MOTOR, (int)(velocity * RIGHT_MOTOR_ADJUST) * direction);

    // Keep going until we've moved far enough
    while (direction * get_motor_position_counter(LEFT_MOTOR) < target_ticks) {
        msleep(50); // Check every 50ms
    }

    // Stop the motors
    ao();
}

int main() {
    // Move forward 50 cm
    printf("Moving forward\n");
    move_straight(DEFAULT_VELOCITY, 50);

    // Wait 2 seconds
    msleep(2000);

    // Move backward 50 cm
    printf("Moving back to start\n");
    move_straight(DEFAULT_VELOCITY, -50);

    return 0;
}