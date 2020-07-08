#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <cstdlib>

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "pubvel");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
    srand(time(NULL));
    ros::Rate rate(2);
    while(ros::ok())
    {
        geometry_msgs::Twist msg;
        msg.linear.x = double(rand())/double(RAND_MAX);
        msg.angular.z = double(rand())/double(RAND_MAX) * 2 - 1;
        pub.publish(msg);
        ROS_INFO_STREAM("Published linear = "<<msg.linear.x<<" angular = "<<msg.angular.z);
        rate.sleep();
    }
}