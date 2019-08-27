#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <gazebo_msgs/ModelStates.h>
#include "Agent.h"

ros::Publisher *pubPtr;
RVO::Agent quad("uav");
bool ready = false;
gazebo_msgs::ModelStates neighbors;

void velCallback(const geometry_msgs::Twist &msg)
    {
        if(!ready)
            {
                pubPtr->publish(msg);
            }
        quad.prepareStep(neighbors, msg);
        geometry_msgs::Twist newVel = msg;
        newVel.linear.x = quad.newVelocity_.x_;
        newVel.linear.y = quad.newVelocity_.y_;
        pubPtr->publish(newVel);
    }

void neighborCallback(const gazebo_msgs::ModelStates &msg)
    {
        ready = true;
        neighbors = msg;
        return;
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "avoidCollision");
        ros::NodeHandle nh;
        ros::Subscriber sub = nh.subscribe("cmd_vel_pref", 1, &velCallback);
        ros::Subscriber sub2 = nh.subscribe("neighbors", 1, &neighborCallback);
        ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
        pubPtr = &pub;
        ros::Rate rate(50);
        while (ros::ok())
        {
            ros::spinOnce();
            rate.sleep();
        }
        
    }