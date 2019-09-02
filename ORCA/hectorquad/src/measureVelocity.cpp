#include <ros/ros.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Twist.h>

double vx, vy;

void neighborCallback(const gazebo_msgs::ModelStates &msg)
    {
        static auto lastPose = msg.pose[0];
        vx = msg.pose[0].position.x - lastPose.position.x;
        vy = msg.pose[0].position.y - lastPose.position.y;
        ROS_INFO_STREAM("Stated: " << msg.twist[0] << " measured x=" << vx << " y="<<vy);
        lastPose = msg.pose[0];
        return;
    }


int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "measureVelocity");
        ros::NodeHandle nh;
        ros::Subscriber sub1 = nh.subscribe("neighbors", 1, &neighborCallback);
        ros::Rate rate(1);
        while(ros::ok())
            {
                ros::spinOnce();
                rate.sleep();
            }
    }