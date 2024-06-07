#include "ros/ros.h"
#include "std_msgs/String.h"
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <sstream>
#include <string>

std::string detected_object;
ros::Time last_detection_time;

void darknetCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr &msg)
{
    for (const auto &box : msg->bounding_boxes)
    {
        if (box.Class == "bottle")
        {
            detected_object = "bottle";
            last_detection_time = ros::Time::now();
            ROS_INFO("Detected 'bottle': Probability: %.2f, xmin: %d, ymin: %d, xmax: %d, ymax: %d",
                     box.probability, box.xmin, box.ymin, box.xmax, box.ymax);
        }
    }
}

int main(int argc, char **argv)
{
    // Initialize the ROS system and become a node.
    ros::init(argc, argv, "darknet_motor_controller");
    ros::NodeHandle n;

    // Create a publisher object
    ros::Publisher motor_pub = n.advertise<std_msgs::String>("/servo_motors_control", 1000);

    // Create a subscriber object
    ros::Subscriber darknet_sub = n.subscribe("/darknet_ros/bounding_boxes", 1000, darknetCallback);

    // Loop at 10Hz, publishing messages and processing callbacks
    ros::Rate loop_rate(10);

    while (ros::ok())
    {
        // Create a message object
        std_msgs::String msg;

        if (detected_object == "bottle")
        {
            if ((ros::Time::now() - last_detection_time) >= ros::Duration(120)) // 2 minutes = 120 seconds
            {
                msg.data = "servo_cw";
                ROS_INFO("%s", msg.data.c_str());
                motor_pub.publish(msg);
                detected_object = ""; // Reset detected_object after publishing
            }
            else
            {
                msg.data = "servo_ccw";
                ROS_INFO("%s", msg.data.c_str());
                motor_pub.publish(msg);
            }
        }

        // Process incoming messages
        ros::spinOnce();

        // Sleep to maintain the loop rate
        loop_rate.sleep();
    }

    return 0;
}
