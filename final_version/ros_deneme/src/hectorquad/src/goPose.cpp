#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <hectorquad/coordinate.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Vector3.h>
#include "motionUtilities.hpp"

#define SPEED 1.0
#define QUADNAME "quadrotor"


Coordinate coordToGo, currentCoord;

ros::Publisher *pubPtr1;
double yaw;


double coeff(double a, double b)
    {
        if(fabs(a-b) < EPSILON)
            return 0;
        else return a < b ? 1.0 : -1.0;
    }

void getPose(const geometry_msgs::Quaternion &msg)
    {
        static bool init = true;
        //ROS_INFO_STREAM_ONCE("Quad is in position " << ind << "of the array.");
        currentCoord.x = msg.x;
        currentCoord.y = msg.y;
        currentCoord.z = msg.z;
        yaw = msg.w;
        if (init == true)
            {
                init = false;
                coordToGo = currentCoord;
            }
        return;
    }

void serverFunc(const geometry_msgs::Vector3 &msg)
    {
        //ROS_INFO_STREAM("Service has called for x = " << req.x << " y = " << req.y << " z  = " << req.z);
        coordToGo.x = msg.x;
        coordToGo.y = msg.y;
        coordToGo.z = msg.z;
        return;
    }

void goPose()
    {
        /*static int paramState = -1;
        if (equalCoord(currentCoord, coordToGo) && paramState != 1)
            {
                //ros::param::set("quadIdle", "1");
                paramState = 1;
            }
        else if(paramState != 0)
            {
                paramState = 0;
                //ros::param::set("quadIdle", "0");
            }*/
        
        geometry_msgs::Twist msg;
        float distanceX = fabs(coordToGo.x - currentCoord.x);
        float distanceY = fabs(coordToGo.y - currentCoord.y);
        float distanceZ = fabs(coordToGo.z - currentCoord.z);


        msg.linear.x = SPEED * distanceX  * coeff(currentCoord.x, coordToGo.x);
        msg.linear.y = SPEED * distanceY  * coeff(currentCoord.y, coordToGo.y);
        msg.linear.z = SPEED * distanceZ  * coeff(currentCoord.z, coordToGo.z);

        float magnitude = sqrt(msg.linear.x * msg.linear.x + msg.linear.y * msg.linear.y);
        if(magnitude > MAX_SPEED)
            {
                msg.linear.x *= MAX_SPEED / magnitude;
                msg.linear.y *= MAX_SPEED / magnitude;
            }
        /*msg.angular.x = 0;
        msg.angular.y = 0;*/
        msg.angular.z = -yaw/8;
        pubPtr1->publish(msg);

        return;
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "goPose");
        ros::NodeHandle nh;
        ROS_INFO_STREAM("Advertising service quadGoPose");
        ros::Subscriber sub2 = nh.subscribe("quadGoPose", 1, &serverFunc);
        ros::Subscriber sub1 = nh.subscribe("quadPose", 1, &getPose);
        ros::Publisher pub1 = nh.advertise<geometry_msgs::Twist>("cmd_vel_pref", 1);
        pubPtr1 = &pub1;
        ROS_INFO_STREAM("Subscribed.");
        ros::Rate rate(100);
        while (ros::ok())
            {
                ros::spinOnce();
                goPose();
                rate.sleep();
            }
        
        return 0;
    }