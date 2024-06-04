#include <ros.h>
#include <string.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <motor_control.h>
#include <mpu6050.h>
#include <servo_control.h>
#include <stepper_control.h>

#define COMMAND_RATE 5 // hz

double radius = 0.1;

double ly = 0.18;
double lx = 0.1275;
std::string motors_command;
double lower_speed_limit = -10;
double upper_speed_limit = 10;

double lower_pwm_limit = -255;
double upper_pwm_limit = 255;

static unsigned long prev_control_time = 0;
static unsigned long now = 0;
static unsigned long lastTime = 0;


void commandCallback(const geometry_msgs::Twist &cmd_msg)
{

  Vx = cmd_msg.linear.x;
  Vy = cmd_msg.linear.y;
  Wz = cmd_msg.angular.z;
}

void MotorscommandCallback(const std_msgs::String &cmd_msg)
{

  motors_command = cmd_msg.data;
}

ros::NodeHandle nh;
ros::Publisher imu_pub("imu_raw", &floatArrayMsg);
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &commandCallback);
ros::Subscriber<std_msgs::String> motors_sub("/motors_control", &MotorscommandCallback);


void FourMecanumKinematic()
{
  // forward kinematics of a Four Mecanum Wheeled Mobile Robot
  w_fl = (1 / radius) * (Vx - Vy - (lx + ly) * Wz);
  w_fr = (1 / radius) * (Vx + Vy + (lx + ly) * Wz);
  w_rl = (1 / radius) * (Vx + Vy - (lx + ly) * Wz);
  w_rr = (1 / radius) * (Vx - Vy + (lx + ly) * Wz);

  pwm_fl = map(w_fl, lower_speed_limit, upper_speed_limit, lower_pwm_limit, upper_pwm_limit);
  pwm_fr = map(w_fr, lower_speed_limit, upper_speed_limit, lower_pwm_limit, upper_pwm_limit);
  pwm_rl = map(w_rl, lower_speed_limit, upper_speed_limit, lower_pwm_limit, upper_pwm_limit);
  pwm_rr = map(w_rr, lower_speed_limit, upper_speed_limit, lower_pwm_limit, upper_pwm_limit);

  // Serial.print("front right pwm: ");
  // Serial.println(pwm_fr);

  // Serial.print("front left pwm: ");
  // Serial.println(pwm_fl);

  motor_frontleft.setSpeed(pwm_fl);  // PWM 1A = Pin 10, PWM 1B = Pin 11.
  motor_frontright.setSpeed(pwm_fr); // PWM 2A = Pin 8, PWM 2B = Pin 9.

  motor_backleft.setSpeed(pwm_rl);  // PWM 1A = Pin 10, PWM 1B = Pin 11.
  motor_backright.setSpeed(pwm_rr); // PWM 2A = Pin 8, PWM 2B = Pin 9.
}


void setup()
{

  // Inizialize the ROS node on the Arduino
  nh.initNode();
  motorsSetup();
  servo_setup();
  stepper_setup();
  // Serial.begin(9600);

  // Inform ROS that this node will subscribe to messages on a given topic
  nh.subscribe(sub);
  nh.subscribe(motors_sub);
  nh.advertise(imu_pub);
  setupIMU();
}

void loop()
{

  if (motors_command == "servo_ccw")
  {
    servo_ccw();
  }

  if (motors_command == "servo_cw")
  {
    servo_cw();
  }

  if (motors_command == "stepper_ccw")
  {
    stepper_ccw();
  }

  if (motors_command == "stepper_cw")
  {
    stepper_cw();
  }

  FourMecanumKinematic();
  publishIMU();

  imu_pub.publish(&floatArrayMsg);

  nh.spinOnce();

  delay(1);

  // delay(2000);  // Wait for 2 seconds
}
