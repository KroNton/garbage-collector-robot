
#include <Arduino.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <servo_control.h>
#include <stepper_control.h>
#include <ir_sensor.h>

#define COMMAND_RATE 5 // hz




void stepperMotorscommandCallback(const std_msgs::String &cmd_msg)
{

  stepper_motors_command = cmd_msg.data;
}

void servoMotorscommandCallback(const std_msgs::String &cmd_msg)
{

  servo_motors_command = cmd_msg.data;
}

ros::NodeHandle nh;

ros::Subscriber<std_msgs::String> servo_motors_sub("/servo_motors_control", &servoMotorscommandCallback);
ros::Subscriber<std_msgs::String> stepper_motors_sub("/stepper_motors_control", &stepperMotorscommandCallback);

std_msgs::Bool ir_msg;
ros::Publisher ir_pub("ir_data", &ir_msg);


void setup()
{

  // Inizialize the ROS node on the Arduino
  nh.initNode();

  servo_setup();
  stepper_setup();
  ir_setup();
  nh.subscribe(servo_motors_sub);
  nh.subscribe(stepper_motors_sub);
}

void loop()
{


  ir_msg=ir_obstacle();

  if (servo_motors_command == "servo_ccw")
  {
    servo_ccw();
  }

  if (servo_motors_command == "servo_cw")
  {
    servo_cw();
  }
  

  if (stepper_motors_command == "stepper_ccw")
  {
    stepper_ccw();
  }

   if (stepper_motors_command == "stepper_cw")
  {
    stepper_cw();
  }



  ir_pub.publish( &ir_msg );



  nh.spinOnce();

  delay(1);

 
}
