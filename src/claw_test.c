#include <kipr/wombat.h>

#define ARM_SERVO 0
#define ARM_SERVO_UP_POS 1250
#define ARM_SERVO_DOWN_POS 2047
#define CLAW_SERVO 3
#define CLAW_SERVO_OPEN_POS 1250
#define CLAW_SERVO_CLOSE_POS 350

static void move_arm_to_position(int servo, int position) {
    int current_position = get_servo_position(servo);
    int movement_direction = current_position > position ? -1 : 1;
    printf("Need to go from %d to %d\n", current_position, position);
    printf("Need to move %d in %d direction\n", abs(position - current_position), movement_direction);
    enable_servo(servo);
    while (current_position != position) {
        current_position += movement_direction * 1;
        set_servo_position(servo, current_position);
        //printf("Moved to %d\n", current_position);
        msleep(2);
    }
    disable_servo(servo);
    printf("Finished movement\n");

}

void raise_arm() {
    move_arm_to_position(ARM_SERVO, ARM_SERVO_UP_POS);
}

void lower_arm() {
    move_arm_to_position(ARM_SERVO, ARM_SERVO_DOWN_POS);
}

void open_claw() {move_arm_to_position(CLAW_SERVO, CLAW_SERVO_OPEN_POS);}
void close_claw() {move_arm_to_position(CLAW_SERVO, CLAW_SERVO_CLOSE_POS);}

int main()
{
    printf("Hello World\n");
    lower_arm();
    open_claw();
    close_claw();
    raise_arm();
    return 0;
}
