

#define stepPin 3
#define dirPin 4

#define stepper_steps 2000
#define stepper_delay 700
String stepper_motors_command = " ";
void stepper_setup()
{
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void stepper_ccw()
{
  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x < stepper_steps; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepper_delay); // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepper_delay);
  }
  stepper_motors_command = " ";
}

void stepper_cw()
{
  digitalWrite(dirPin, LOW); // Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for (int x = 0; x < stepper_steps; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepper_delay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepper_delay);
  }
  stepper_motors_command = " ";
}