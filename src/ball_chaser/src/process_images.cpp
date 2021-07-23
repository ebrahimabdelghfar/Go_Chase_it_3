#include"ros/ros.h"
#include"sensor_msgs/Image.h"
#include "ball_chaser/DriveToTarget.h"
using namespace ros;
using namespace ball_chaser;
using namespace sensor_msgs;
using namespace std;
ServiceClient client;
void drive_robot(float lin_x, float ang_z)
{
    DriveToTarget srv;
    srv.request.linear_x=lin_x;
    srv.request.angular_z=ang_z;
    if (!client.call(srv)){
       // ROS_ERROR("Failed to call service safe_move");
    }
}
void process_image_callback( const Image img)
{
    int white_pixels=255;
    int left = int(img.width / 3);
    int center = int(2*(img.width) / 3);
    int i=0;
    int steps=0;
    for ( i = 0; i < img.height * img.step; i++) {
        if(img.data[i]==white_pixels&&img.data[i+1]==white_pixels&&img.data[i+2]==white_pixels){
            steps=i % img.step;
            break;
        }
    }
     if ( steps<left )
    {   
        drive_robot(0.0,2.0);
    }
    else if ( left<steps<center) 
    {   
        drive_robot(1.0,0.0);
    }

    else if ( steps>center)
    {   
        drive_robot(0.0,-2.0);
    }

    else
    {   
        drive_robot(0.0,0.0);        
    }
}
int main(int argc, char** argv)
{
    // Initialize the look_away node and create a handle to it
    ros::init(argc, argv, "image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from safe_move
    client = n.serviceClient<DriveToTarget>("/ball_chaser/command_robot");

    ros::Subscriber sub2 = n.subscribe("camera/rgb/image_raw", 10, process_image_callback);
    
    ROS_INFO("Ready to send wheel velocities commands");
    // Handle ROS communication events
    ros::spin();

    return 0;
}