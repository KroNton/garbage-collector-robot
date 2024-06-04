#include <Servo.h>

static const int servo1Pin = 6;
static const int servo2Pin = 7;
Servo servo1;
Servo servo2;
void servo_setup()
{

    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
}

void servo_ccw()
{
    servo1.write(90);
    servo2.write(90);
}

void servo_cw()
{
    servo1.write(0);
    servo2.write(0);
}