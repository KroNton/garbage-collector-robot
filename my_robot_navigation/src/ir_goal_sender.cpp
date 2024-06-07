
#include <ros/ros.h>
#include <string.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>

class IRGoalSender
{

public:
    IRGoalSender() : ac_("move_base", true) // Initialize the action client to "move_base"
    {
        ir_sub_ = nh_.subscribe("/ir_data", 1, &IRGoalSender::irCallback, this);
        stepper_pub_ = nh_.advertise<std_msgs::String>("/stepper_motors_control", 1);
        ROS_INFO("Waiting for move_base action server to start...");
        ac_.waitForServer();
        ROS_INFO("Connected to move_base action server.");
    }

    void irCallback(const std_msgs::Bool::ConstPtr &msg)
    {
        if (msg->data)
        {
            ir_is_true = true;
        }

        else
        {
            ir_is_true = false;
        }
    }

    void robot_run()
    {

        if (ir_is_true&& !motor_flag)
        {
            sendGoal();
            
            motor_flag=true;
        }
        else if ( motor_flag)
        {
            publishStepperControl();
        }
        
        else
        {

            ROS_INFO("wating for IR flag");
        }
    }

private:
    bool ir_is_true=false;
    bool motor_flag= false ;
    bool stepper_moved_once=false ;
    ros::NodeHandle nh_;
    ros::Subscriber ir_sub_;
    ros::Publisher stepper_pub_;
    std_msgs::String msg;
    ros::Time last_detection_time;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac_;

    void sendGoal()
    {
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "odom";
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = 0.0;
        goal.target_pose.pose.position.y = 0.0;
        goal.target_pose.pose.orientation.w = 1.0;

        ROS_INFO("Sending goal to (0,0) in map frame.");
        ac_.sendGoal(goal);

        ac_.waitForResult();
        last_detection_time = ros::Time::now();
        if (ac_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO("The base moved to the goal.");
        else
            ROS_INFO("The base failed to move to the goal for some reason.");
    }

    void publishStepperControl()
    {
        

        ROS_INFO("Publish to /stepper_motor_control.");

        if ((ros::Time::now() - last_detection_time) >= ros::Duration(10) && stepper_moved_once) // 2 minutes = 120 seconds
        {
            msg.data = "stepper_ccw";
            ROS_INFO("%s", msg.data.c_str());
            stepper_pub_.publish(msg);
            motor_flag= false;
        }
        else if (!stepper_moved_once)
        {

            msg.data = "stepper_cw";
            ROS_INFO("%s", msg.data.c_str());
            stepper_pub_.publish(msg);
            stepper_moved_once=true;
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ir_goal_sender");
    IRGoalSender ir_goal_sender;
    ros::Rate loop_rate(10);

    while (ros::ok)
    {
        
        ir_goal_sender.robot_run();
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
