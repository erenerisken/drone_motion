#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <vector>
#include <gazebo_msgs/ModelStates.h>
#include <hectorquad/coordinate.h>
#include <geometry_msgs/Vector3.h>
#include <std_srvs/Empty.h>
#include <geometry_msgs/Quaternion.h>
#include "AStar.hpp"
#include "Rrt.hpp"
#include "rrt_star.hpp"
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include "rrt_sharp.hpp"
#include <std_msgs/Bool.h>

enum PlannerType
    {
        RRT, ASTAR, RRTSTAR, RRTSTARONE, RRTSHARP, DIRECT
    };

std::vector<Obstacle*> obstacles;
ros::Publisher *pubPtr;

bool readyToPlan = false, neighborsNearby = false, inCollision = false;
std::vector<Coordinate> route, actualRoute;
Coordinate curQuadPose, destinationCoord;
double destX, destY, startX, startY;
int randomSeed;

PlannerType plannerType = RRTSTARONE;

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
            //ROS_INFO_STREAM(*tmp<<" has been added to list.");
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
        return equalCoord(curQuadPose, destinationCoord);/*
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
        return true;*/
    }

void callService(const Coordinate &c)
    {
        geometry_msgs::Vector3 msg;
        msg.x = c.x;
        msg.y = c.y;
        msg.z = c.z;
        destinationCoord = c;
        pubPtr->publish(msg);
        //ROS_INFO_STREAM("Quad has been directed to "<<c);
    }

void visitPoints(std::vector<Coordinate> &v)
    {
        static bool prevIncollision = false, prevNeighborsNearby = false;
        static int ind = 0;
        if(!neighborsNearby && prevNeighborsNearby)
            {
                size_t start = 0, end = 1;
                for (size_t i = 1; i < v.size() - 1; i++)
                    {
                        if (dist(getClosestPoint(curQuadPose, v[i], v[i+1]), curQuadPose) < dist(getClosestPoint(curQuadPose, v[start], v[end]), curQuadPose))
                            {
                                bool clearPath = true;
                                for (auto obs = obstacles.begin(); obs < obstacles.end(); obs++)
                                    {
                                        if ((*obs)->separates(curQuadPose, getClosestPoint(curQuadPose, v[i], v[i+1])))
                                            {
                                                clearPath = false;
                                                break;
                                            }
                                    }
                                if (clearPath)
                                    {
                                        start = i;
                                        end = i+1;
                                    }
                            }
                    }
                Coordinate closest = getClosestPoint(curQuadPose, v[start], v[end]);
                ind = end;
                prevNeighborsNearby = neighborsNearby;
                prevIncollision = inCollision;
                callService(closest);
                //ROS_INFO_STREAM("Directing to closest point where a = " << startPoint << "\nb = " 
                //                << endPoint << "\np = " << curQuadPose << " closest = " << closest);
                return;
            }
        if(!quadAvail() && !neighborsNearby)
            {
                prevIncollision = inCollision;
                prevNeighborsNearby = neighborsNearby;
                return;
            }
        if (v.size() <= 0)
            {
                prevIncollision = inCollision;
                prevNeighborsNearby = neighborsNearby;
                ROS_WARN_STREAM_ONCE("No route available.");
                return;
            }
        if (neighborsNearby)
            {
                for(; ind < v.size()-1; ind++)
                    if (dist(v[ind], curQuadPose) >= SKIP_COEFF * (QUAD_RADII + CLEARENCE))
                        {
                            //ROS_WARN_STREAM("Changed coordinate to" << v[ind].x << " " << v[ind].y << " " << v[ind].z);
                            break;
                        }            
            }
        callService(v[ind]);
        if(quadAvail())
            ind++;
        if(ind >= v.size())
            ind = v.size()-1;
        prevIncollision = inCollision;
        prevNeighborsNearby = neighborsNearby;
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
                AStar::init(Coordinate(startX,startY,0), Coordinate(destX,destY,0));
                route = AStar::generateMap(obstacles, -10.0, 10.0, -10.0, 10.0, 0.1);
                AStar::printMap();
            }
        else if(plannerType == RRT)
            {
                Rrt::init(Coordinate(startX, startY, 0.4), Coordinate(destX, destY, 0.4), 0.1, obstacles);
                ROS_WARN_STREAM("Map is ready");
                route = Rrt::rrtGetMap(-10.0, 10.0, -10.0, 10.0);
            }
        else if(plannerType == RRTSTAR)
            {
                Rrt_star::init(Coordinate(startX, startY, 0.4), Coordinate(destX, destY, 0.4), 0.1, obstacles);
                ROS_WARN_STREAM("Map is ready");
                route = Rrt_star::rrtGetMap(-10.0, 10.0, -10.0, 10.0);
            }
        else if(plannerType == RRTSTARONE)
            {
                Rrt_star::init(Coordinate(startX, startY, 0.4), Coordinate(destX, destY, 0.4), 0.1, obstacles);
                ROS_WARN_STREAM("Map is ready");
                route = Rrt_star::rrtGetMapOne(-10.0, 10.0, -10.0, 10.0);    
            }
        else if(plannerType == RRTSHARP)
            {
                Rrt_sharp::init(Coordinate(startX, startY, 0.4), Coordinate(destX, destY, 0.4), 0.1, obstacles);
                ROS_WARN_STREAM("Map is ready");
                route = Rrt_sharp::getMap(-10.0, 10.0, -10.0, 10.0);    
            }
        else if(plannerType == DIRECT)
            {
                route.push_back(Coordinate(startX, startY, 0.4));
                route.push_back(Coordinate(destX, destY, 0.4));
                ROS_WARN_STREAM("Map is ready.");
            }
        callService(Coordinate(curQuadPose.x, curQuadPose.y, INITIAL_HEIGHT));
    }

void getQuadPose(const geometry_msgs::Quaternion &msg)
    {
        static int count = 0;
        curQuadPose.x = msg.x;
        curQuadPose.y = msg.y;
        curQuadPose.z = msg.z;
        if (++count % 20 == 0)
            {
                if(actualRoute.size() == 0 || actualRoute[actualRoute.size()-1] != curQuadPose)
                    actualRoute.push_back(curQuadPose);
            }
        return;
    }

void getParameters()
    {
        ROS_INFO_STREAM("Getting parameters.");
        while(!ros::param::get("destX", destX))
            {
                ROS_WARN_STREAM_ONCE("Parameter destX is pending.");
            }
        ROS_WARN_STREAM_ONCE("Parameter destX has been set.");
        while(!ros::param::get("destY", destY))
            {
                ROS_WARN_STREAM_ONCE("Parameter destY is pending.");
            }
        ROS_WARN_STREAM_ONCE("Parameter destY has been set.");
        while(!ros::param::get("startX", startX))
            {
                ROS_WARN_STREAM_ONCE("Parameter startX is pending.");
            }
        ROS_WARN_STREAM_ONCE("Parameter startX has been set.");
        while(!ros::param::get("startY", startY))
            {
                ROS_WARN_STREAM_ONCE("Parameter startY is pending.");
            }
        ROS_WARN_STREAM_ONCE("Parameter startY has been set.");
        while(!ros::param::get("randomSeed", randomSeed))
            {
                ROS_WARN_STREAM_ONCE("Parameter randomSeed is pending.");
            }
        ROS_WARN_STREAM_ONCE("Parameter randomSeed has been set.");
    }

void getNeighborsNearby(const std_msgs::Bool &msg)
    {
        if (msg.data != neighborsNearby)
            {
                ROS_WARN_STREAM("Neighbors are " << (msg.data ? "near" : "away"));
            }
        
        neighborsNearby = msg.data;
        return;
    }
void getInCollision(const std_msgs::Bool &msg)
    {
        inCollision = msg.data;
        return;
    }

void visualise(ros::Publisher &marker_pub, ros::Publisher &markerArray_pub)
    {
        static visualization_msgs::MarkerArray obsArray;
        static visualization_msgs::Marker points, line_strip, cylinder;
        static bool init = true;
        if(init)
            {
                init = false;
                points.type =  visualization_msgs::Marker::POINTS;
                line_strip.type = visualization_msgs::Marker::LINE_STRIP;
                cylinder.type = visualization_msgs::Marker::CYLINDER;
                line_strip.color.r = float(rand()) / float(RAND_MAX);
                line_strip.color.g = float(rand()) / float(RAND_MAX);
                line_strip.color.b = float(rand()) / float(RAND_MAX);
            }
        cylinder.header.frame_id = points.header.frame_id = line_strip.header.frame_id = "/world";
        cylinder.header.stamp = points.header.stamp = line_strip.header.stamp = ros::Time::now();
        cylinder.ns = "cylinders";
        points.ns = line_strip.ns = "points_and_lines";
        cylinder.action = points.action = line_strip.action = visualization_msgs::Marker::ADD;
        cylinder.pose.orientation.w = points.pose.orientation.w = line_strip.pose.orientation.w = 1.0;

        points.id = 0;
        line_strip.id = 1;

        cylinder.scale.x = 1.0;
        cylinder.scale.y = 1.0;
        cylinder.scale.z = 2.0;

        points.scale.x = 0.2;
        points.scale.y = 0.2;

        line_strip.scale.x = 0.1;

        cylinder.color.r = 1.0f;
        cylinder.color.g = 0.0f;
        cylinder.color.b = 0.0f;
        cylinder.color.a = 1.0;

        points.color.r = line_strip.color.r;
        points.color.g = line_strip.color.g;
        points.color.b = line_strip.color.b;
        points.color.a = 1.0;

        line_strip.color.a = 1.0;
            
        obsArray.markers.resize(obstacles.size()+2);
        for(int i = 0; i < obstacles.size(); i++)
            {
                cylinder.id = 2 + i;
                cylinder.pose.position.x = obstacles[i]->coord.x;
                cylinder.pose.position.y = obstacles[i]->coord.y;
                cylinder.pose.position.z = obstacles[i]->coord.z;
                obsArray.markers[i] = cylinder;
            }
        cylinder.pose.position.x = curQuadPose.x;
        cylinder.pose.position.y = curQuadPose.y;
        cylinder.pose.position.z = curQuadPose.z;
        cylinder.color.r = 0;
        cylinder.color.g = 0;
        cylinder.color.b = 1.0f;
        cylinder.scale.x = 0.6f;
        cylinder.scale.y = 0.6f;
        cylinder.scale.z = 0.5f;
        cylinder.id = obstacles.size() + 2;
        obsArray.markers[obstacles.size()] = cylinder;
        //---
        cylinder.pose.position.x = destinationCoord.x;
        cylinder.pose.position.y = destinationCoord.y;
        cylinder.pose.position.z = destinationCoord.z;
        cylinder.color = line_strip.color;
        cylinder.scale.x = 0.3f;
        cylinder.scale.y = 0.3f;
        cylinder.scale.z = 0.3f;
        cylinder.id = obstacles.size() + 3;
        obsArray.markers[obstacles.size()+1] = cylinder;
        markerArray_pub.publish(obsArray);
        if (route.size() > 0)
            {
                points.points.clear();
                line_strip.points.clear();
                for(int i = 0; i < route.size(); i++)
                    {
                        geometry_msgs::Point p;
                        p.x = route[i].x;
                        p.y = route[i].y;
                        p.z = route[i].z;
                        line_strip.points.push_back(p);
                    }
                for(int i = 0; i < actualRoute.size(); i++)
                    {
                        geometry_msgs::Point p;
                        p.x = actualRoute[i].x;
                        p.y = actualRoute[i].y;
                        p.z = actualRoute[i].z;
                        points.points.push_back(p);
                    }
                marker_pub.publish(points);
                marker_pub.publish(line_strip);
            }
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "rrtPlanner");
        ros::NodeHandle nh;
        ros::Subscriber sub1 = nh.subscribe("model_states", 1, &updatePositions);
        ros::Subscriber sub2 = nh.subscribe("quadPose", 1, &getQuadPose);
        ros::Subscriber sub3 = nh.subscribe("neighborsNearby", 1, &getNeighborsNearby);
        ros::Subscriber sub4 = nh.subscribe("inCollision", 1, &getInCollision);
        //ros::service::waitForService("quadGoPose");
        ros::Publisher pub = nh.advertise<geometry_msgs::Vector3>("quadGoPose", 1);
        ros::ServiceServer service = nh.advertiseService("startPlanning", &startPlanning);
        pubPtr = &pub;
        ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 1);
        ros::Publisher markerArray_pub = nh.advertise<visualization_msgs::MarkerArray>("visualization_marker_array",1);
        getParameters();
        srand(randomSeed);
        ros::Rate rate(100);
        //ROS_FATAL_STREAM(getClosestPoint(Coordinate(0.0,2.0,0.0), Coordinate(-4.0, 0.0, 0.0), Coordinate(8.0, 1.0, 0.0)));
        //ROS_WARN_STREAM("A random number is " << rand() << " seed is " << randomSeed);
        while(ros::ok())
            {
                ros::spinOnce();
                prepareMap();
                visualise(marker_pub, markerArray_pub);
                visitPoints(route);
                rate.sleep();
            }
        clearMemory();
    }