#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <hectorquad/coordinate.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Vector3.h>
#include "motionUtilities.hpp"

#define EPSILON 0.15
#define SPEED 1.2
#define QUADNAME "quadrotor"


Coordinate coordToGo, currentCoord;

ros::Publisher *pubPtr1, *pubPtr2;
double yaw;

bool equalCoord(Coordinate &a, Coordinate &b)
    {
        return fabs(a.x - b.x) < EPSILON && fabs(a.y - b.y) < EPSILON
                && fabs(a.z - b.z) < EPSILON;
    }

double coeff(double a, double b)
    {
        if(fabs(a-b) < EPSILON)
            return 0;
        else return a < b ? 1.0 : -1.0;
    }

void getPose(const gazebo_msgs::ModelStates &msg)
    {
        int ind;
        for (ind = 0; ind < msg.name.size(); ind++)
        {
            if (msg.name[ind] == QUADNAME)
            {
                break;
            }
        }
        if (ind >= msg.name.size())
        {
            ROS_ERROR_STREAM("Quad not found");
            return;
        }
        static bool init = true;
        ROS_INFO_STREAM_ONCE("Quad is in position " << ind << "of the array.");
        currentCoord.x = msg.pose[ind].position.x;
        currentCoord.y = msg.pose[ind].position.y;
        currentCoord.z = msg.pose[ind].position.z;
        yaw = msg.pose[ind].orientation.z;
        if (init == true)
        {
            init = false;
            coordToGo = currentCoord;
        }
        
        return;
    }

bool serverFunc(hectorquad::coordinate::Request &req, hectorquad::coordinate::Response &res)
{
    ROS_INFO_STREAM("Service has called for x = " << req.x << " y = " << req.y << " z  = " << req.z);
    res.s = "Processed.";
    coordToGo.x = req.x;
    coordToGo.y = req.y;
    coordToGo.z = req.z;
    return true;
}

void goPose()
    {
        static int paramState = -1;
        if (equalCoord(currentCoord, coordToGo) && paramState != 1)
        {
            ros::param::set("quadIdle", "1");
            paramState = 1;
        }
        else if(paramState != 0)
        {
            paramState = 0;
            ros::param::set("quadIdle", "0");
        }
        
        geometry_msgs::Twist msg;
        float distanceX = fabs(coordToGo.x - currentCoord.x);
        float distanceY = fabs(coordToGo.y - currentCoord.y);
        float distanceZ = fabs(coordToGo.z - currentCoord.z);


        msg.linear.x = SPEED * distanceX / 2 * coeff(currentCoord.x, coordToGo.x);
        msg.linear.y = SPEED * distanceY / 2 * coeff(currentCoord.y, coordToGo.y);
        msg.linear.z = SPEED * distanceZ / 2 * coeff(currentCoord.z, coordToGo.z);
        /*msg.angular.x = 0;
        msg.angular.y = 0;*/
        msg.angular.z = -yaw/8;
        pubPtr1->publish(msg);

        return;
    }

void publishPose()
    {
        geometry_msgs::Vector3 msg;
        msg.x = currentCoord.x;
        msg.y = currentCoord.y;
        msg.z = currentCoord.z;
        pubPtr2->publish(msg);
    }

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "goPose");
    ros::NodeHandle nh;
    ROS_INFO_STREAM("Advertising service quadGoPose");
    ros::ServiceServer server = nh.advertiseService("quadGoPose", &serverFunc);
    ros::Subscriber sub1 = nh.subscribe("gazebo/model_states", 1, &getPose);
    ros::Publisher pub1 = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    ros::Publisher pub2 = nh.advertise<geometry_msgs::Vector3>("quadPose", 1);
    pubPtr1 = &pub1;
    pubPtr2 = &pub2;
    ROS_INFO_STREAM("Subscribed.");
    while (ros::ok())
    {
        ros::spinOnce();
        goPose();
        publishPose();
    }
    
    return 0;
}