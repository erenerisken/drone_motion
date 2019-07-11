#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <vector>
#include <gazebo_msgs/ModelStates.h>
#include <hectorquad/coordinate.h>

std::vector<Obstacle*> obstacles;
ros::ServiceClient *clientPtr;

void updatePositions(const gazebo_msgs::ModelStates &msg)
    {
        for (size_t i = 0; i < msg.name.size(); i++)
        {
            bool exist = false;
            if (!isObstacle(msg.name[i]))
            {
                continue;
            }
            Coordinate tmpCoord(msg.pose[i].position.x, msg.pose[i].position.y, msg.pose[i].position.z);
            Cylinder *tmp = new Cylinder(msg.name[i], tmpCoord, 1.0, 2.0);
            for (size_t j = 0; j < obstacles.size(); j++)
            {
                if (*tmp == *obstacles[j])
                {
                    exist = true;
                    break;
                }
            }
            if (exist)
            {
                delete tmp;
                continue;
            }
            ROS_INFO_STREAM(*tmp<<" has been added to list.");
            obstacles.push_back(tmp);
        }
    }

void clearMemory()
    {
        ROS_WARN_STREAM("Clearing memory.");
        /*for (size_t i = obstacles.size() - 1; i >= 0; i--)
        {
            delete obstacles[i];
        }*/
        ROS_WARN_STREAM("Memory has been cleaned.");
    }

void visitObstacles()
    {   
        std::string idle;
        if (!ros::param::get("quadIdle", idle))
        {
            ROS_WARN_STREAM_ONCE("Waiting for quadIdle parameter to be set.");
            return;
        }
        ROS_WARN_STREAM_ONCE("quadIdle parameter has been set.");
        if (idle == "0")
        {
            return;
        }
        if (obstacles.size() <= 0)
        {
            ROS_WARN_STREAM_ONCE("No obstacles available.");
            return;
        }
        static int obstacleInd = 0;
        hectorquad::coordinateRequest req;
        hectorquad::coordinateResponse res;
        req.x = obstacles[obstacleInd]->coord.x;
        req.y = obstacles[obstacleInd]->coord.y;
        req.z = 3.0;
        clientPtr->call(req, res);
        ROS_INFO_STREAM("Quad has been directed to "<<obstacles[obstacleInd++]->coord);
        obstacleInd %= obstacles.size();
        return;
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "rrtPlanner");
        ros::NodeHandle nh;
        ros::Subscriber sub1 = nh.subscribe("gazebo/model_states", 1000, &updatePositions);
        ros::service::waitForService("quadGoPose");
        ros::ServiceClient client = nh.serviceClient<hectorquad::coordinate>("quadGoPose");
        clientPtr = &client;
        while(ros::ok())
            {
                ros::spinOnce();
                visitObstacles();
            }
        clearMemory();
    }