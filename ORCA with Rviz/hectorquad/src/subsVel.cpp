#include <ros/ros.h>
#include <geometry_msgs/Vector3Stamped.h>

void velocityCallback(const geometry_msgs::Vector3Stamped &msg)
    {
        ROS_INFO_STREAM(msg);
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "subsVel");
        ros::NodeHandle nh;
        ros::Subscriber sub1 = nh.subscribe("/uav2/fix_velocity", 1, &velocityCallback);
        ros::spin();
    }