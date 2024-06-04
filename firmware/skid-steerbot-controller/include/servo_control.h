#include <Servo.h>

static const int servoPin = 25;

Servo servo1;

void servo_setup() {

    servo1.attach(servoPin);
}

void servo_ccw() {
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
}


void servo_cw() {
    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
}