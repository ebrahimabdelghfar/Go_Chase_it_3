
#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<ball_chaser/DriveToTarget.h>
ros::Publisher motor_command_publisher;
bool handle_motor(ball_chaser::DriveToTarget::Request& req,ball_chaser::DriveToTarget::Response& res){
    geometry_msgs::Twist motor_command;
    motor_command.linear.x=req.linear_x;
    motor_command.angular.z=req.angular_z;
    motor_command_publisher.publish(motor_command);
    res.msg_feedback= "linear_x : " + std::to_string(req.linear_x) + " , angular_z: " + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);
    return true;
}



int main(int argc ,char** argv){
    ros::init(argc,argv,"drive");
    ros::NodeHandle n;
    motor_command_publisher=n.advertise<geometry_msgs::Twist>("cmd_vel",10);
    ros::ServiceServer service = n.advertiseService("ball_chaser/command_robot", handle_motor);
    ros::spin();
    return 0;
}