#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <vector>
#include <gazebo_msgs/ModelStates.h>
#include <hectorquad/coordinate.h>
#include <std_srvs/Empty.h>
#include "AStar.hpp"

std::vector<Obstacle*> obstacles;
ros::ServiceClient *clientPtr;

bool readyToPlan = false;

bool startPlanning(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
    {
        ROS_INFO_STREAM("Ready to plan has been called.");
        readyToPlan = true;
        return true;
    }

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

bool quadAvail()
    {
        std::string idle;
        if (!ros::param::get("quadIdle", idle))
        {
            ROS_WARN_STREAM_ONCE("Waiting for quadIdle parameter to be set.");
            return false;
        }
        ROS_WARN_STREAM_ONCE("quadIdle parameter has been set.");
        if (idle == "0")
        {
            return false;
        }
        return true;
    }

void visitObstacles()
    {   
        if(!quadAvail())
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

void prepareMap()
    {
        static bool planned = false;
        if(planned)
            {
                return;
            }
        
        if(!readyToPlan)
            {
                ROS_WARN_STREAM_ONCE("Not ready to go");
                return;
            }
        planned = true;
        ROS_WARN_STREAM("Preparing map");
        AStar::generateMap(obstacles, -10.0, 10.0, -10.0, 10.0, 0.1);
        AStar::printMap();
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "rrtPlanner");
        ros::NodeHandle nh;
        ros::Subscriber sub1 = nh.subscribe("gazebo/model_states", 1000, &updatePositions);
        ros::service::waitForService("quadGoPose");
        ros::ServiceClient client = nh.serviceClient<hectorquad::coordinate>("quadGoPose");
        ros::ServiceServer service = nh.advertiseService("startPlanning", &startPlanning);
        clientPtr = &client;
        while(ros::ok())
            {
                ros::spinOnce();
                //visitObstacles();
                prepareMap();
            }
        clearMemory();
    }