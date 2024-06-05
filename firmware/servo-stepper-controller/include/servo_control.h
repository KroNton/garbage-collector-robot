#include <Servo.h>

static const int servo1Pin = 6;
static const int servo2Pin = 7;
Servo servo1;
Servo servo2;

String servo_motors_command = " ";

void servo_setup()
{

    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    servo1.write(0);
    servo2.write(180);
}

void servo_ccw()
{
    servo1.write(180);
    servo2.write(0);
    servo_motors_command = " ";
}

void servo_cw()
{
    servo1.write(0);
    servo2.write(180);
    servo_motors_command = " ";
}