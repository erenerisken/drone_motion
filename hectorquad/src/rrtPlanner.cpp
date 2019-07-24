#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <vector>
#include <gazebo_msgs/ModelStates.h>
#include <hectorquad/coordinate.h>
#include <std_srvs/Empty.h>
#include <geometry_msgs/Vector3.h>
#include "AStar.hpp"
#include "Rrt.hpp"
#include "planningUtilities.hpp"
#include "rrt_star.hpp"

enum PlannerType
    {
        RRT, ASTAR, RRTSTAR, RRTSTARONE
    };

std::vector<Obstacle*> obstacles;
ros::ServiceClient *clientPtr;

bool readyToPlan = false;
std::vector<Coordinate> route;
Coordinate curQuadPose;

PlannerType plannerType = RRTSTAR
;

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
            Cylinder *tmp = new Cylinder(msg.name[i], tmpCoord, 0.5, 2.0);
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

void callService(const Coordinate &c)
    {
        hectorquad::coordinateRequest req;
        hectorquad::coordinateResponse res;
        req.x = c.x;
        req.y = c.y;
        req.z = c.z;
        clientPtr->call(req, res);
        ROS_INFO_STREAM("Quad has been directed to "<<c);
    }

void visitPoints(std::vector<Coordinate> &v)
    {   
        if(!quadAvail())
            {
                return;
            }
        if (v.size() <= 0)
            {
                ROS_WARN_STREAM_ONCE("No route available.");
                return;
            }
        static int ind = 0;
        callService(v[ind]);
        ind++;
        ind %= v.size();
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
        if(plannerType == ASTAR)
            {
                AStar::init(Coordinate(-10.0,-10.0,0), Coordinate(9.5,9.5,0));
                route = AStar::generateMap(obstacles, -10.0, 10.0, -10.0, 10.0, 0.1);
                AStar::printMap();
            }
        else if(plannerType == RRT)
            {
                Rrt::init(Coordinate(-10.0, -10.0, 0.4), Coordinate(10.0, 10.0, 0.4), 0.1, obstacles);
                ROS_WARN_STREAM("Map is ready");
                route = Rrt::rrtGetMap(-10.0, 10.0, -10.0, 10.0);
            }
        else if(plannerType == RRTSTAR)
            {
                Rrt_star::init(Coordinate(-10.0, -10.0, 0.4), Coordinate(10.0, 10.0, 0.4), 0.1, obstacles);
                ROS_WARN_STREAM("Map is ready");
                route = Rrt_star::rrtGetMap(-10.0, 10.0, -10.0, 10.0);
            }
        else if(plannerType == RRTSTARONE)
            {
                Rrt_star::init(Coordinate(-10.0, -10.0, 0.4), Coordinate(10.0, 10.0, 0.4), 0.1, obstacles);
                ROS_WARN_STREAM("Map is ready");
                route = Rrt_star::rrtGetMapOne(-10.0, 10.0, -10.0, 10.0);    
            }
        callService(Coordinate(curQuadPose.x, curQuadPose.y, INITIAL_HEIGHT));
    }

void getQuadPose(const geometry_msgs::Vector3 &msg)
    {
        curQuadPose.x = msg.x;
        curQuadPose.y = msg.y;
        curQuadPose.z = msg.z;
        return;
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "rrtPlanner");
        ros::NodeHandle nh;
        ros::Subscriber sub1 = nh.subscribe("gazebo/model_states", 1000, &updatePositions);
        ros::Subscriber sub2 = nh.subscribe("quadPose", 1, &getQuadPose);
        ros::service::waitForService("quadGoPose");
        ros::ServiceClient client = nh.serviceClient<hectorquad::coordinate>("quadGoPose");
        ros::ServiceServer service = nh.advertiseService("startPlanning", &startPlanning);
        clientPtr = &client;
        while(ros::ok())
            {
                ros::spinOnce();
                prepareMap();
                if (route.size() > 0)
                {
                    visitPoints(route);
                }
            }
        clearMemory();
    }