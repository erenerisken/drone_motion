#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <gazebo_msgs/ModelStates.h>
#include "Agent.h"
#include "motionUtilities.hpp"
#include <std_msgs/Bool.h>
#include <visualization_msgs/Marker.h>

ros::Publisher *pubPtr, *pubPtr2, *pubPtr3;
RVO::Agent quad("uav", COLLISION_RADIUS);
bool ready = false;
gazebo_msgs::ModelStates neighbors, obstacles;

void visualiseVelocity(const geometry_msgs::Twist &vel)
    {
        if (neighbors.pose.size() <= 0)
            {
                return;
            }  
        
        const geometry_msgs::Point origin = neighbors.pose[0].position;
        visualization_msgs::Marker arrow;
        arrow.type = visualization_msgs::Marker::ARROW;
        arrow.header.frame_id = "/world";
        arrow.color.a = 1.0f;
        arrow.color.r = 1.0f;
        arrow.color.g = 0.0f;
        arrow.color.b = 0.0f;
        arrow.header.stamp = ros::Time::now();
        arrow.ns = "arrows";
        arrow.action = visualization_msgs::Marker::ADD;
        arrow.id = 0;
        arrow.scale.x = 0.2f;
        arrow.scale.y = 0.4f;
        arrow.scale.z = 0.0f;
        arrow.points.push_back(origin);
        geometry_msgs::Point tail;
        tail.x = origin.x + vel.linear.x;
        tail.y = origin.y + vel.linear.y;
        tail.z = origin.z + vel.linear.z;
        arrow.points.push_back(tail);
        pubPtr3->publish(arrow);
    }

void velCallback(const geometry_msgs::Twist &msg)
    {
        if(!ready)
            {
                pubPtr->publish(msg);
            }
        const size_t obstacleCount = obstacles.name.size();
        for (size_t i = 0; i < obstacleCount; i++)
            {
                neighbors.name.push_back(obstacles.name[i]);
                neighbors.twist.push_back(obstacles.twist[i]);
                neighbors.pose.push_back(obstacles.pose[i]);
            }        
        quad.prepareStep(neighbors, msg, neighbors.name.size());
        geometry_msgs::Twist newVel = msg;
        newVel.linear.x = quad.newVelocity_.x_;
        newVel.linear.y = quad.newVelocity_.y_;
        visualiseVelocity(newVel);
        pubPtr->publish(newVel);
    }

void neighborCallback(const gazebo_msgs::ModelStates &msg)
    {
        ready = true;
        neighbors = msg;
        size_t neighborCount = msg.pose.size();
        size_t obstacleCount = obstacles.pose.size();
        if (neighborCount + obstacleCount <= 0)
            {
                return;
            }
        bool col = false;
        geometry_msgs::Vector3 pose;
        pose.x = neighbors.pose[0].position.x;
        pose.y = neighbors.pose[0].position.y;
        pose.z = neighbors.pose[0].position.z;
        for (size_t i = 1; i < neighborCount; i++)
            {        
                geometry_msgs::Vector3 neighborPose;
                neighborPose.x = neighbors.pose[i].position.x;
                neighborPose.y = neighbors.pose[i].position.y;
                neighborPose.z = neighbors.pose[i].position.z;
                if (dist(pose, neighborPose) <= COLLISION_COEFF*(CLEARENCE + QUAD_RADII))
                    {
                        col = true;
                    }
            }
        for (size_t i = 1; i < obstacleCount; i++)
            {        
                geometry_msgs::Vector3 obstaclePose;
                obstaclePose.x = obstacles.pose[i].position.x;
                obstaclePose.y = obstacles.pose[i].position.y;
                obstaclePose.z = obstacles.pose[i].position.z;
                if (dist(pose, obstaclePose) <= COLLISION_COEFF*(CLEARENCE + QUAD_RADII))
                    {
                        col = true;
                    }
            }
        std_msgs::Bool colMsg;
        colMsg.data = col;
        pubPtr2->publish(colMsg);
        return;
    }

void obstaclesCallback(const gazebo_msgs::ModelStates &msg)
    {
        const size_t modelCount = msg.name.size();
        obstacles.name.clear();
        obstacles.pose.clear();
        obstacles.twist.clear();
        for (size_t i = 0; i < modelCount; i++)
            {
                if (isObstacle(msg.name[i]))
                    {
                        obstacles.name.push_back(msg.name[i]);
                        obstacles.twist.push_back(msg.twist[i]);
                        obstacles.pose.push_back(msg.pose[i]);
                    }
            }
        return;
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "avoidCollision");
        ros::NodeHandle nh;
        ros::Subscriber sub = nh.subscribe("cmd_vel_pref", 1, &velCallback);
        ros::Subscriber sub2 = nh.subscribe("neighbors", 1, &neighborCallback);
        ros::Subscriber sub3 = nh.subscribe("model_states", 1, &obstaclesCallback);
        ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
        ros::Publisher pub2 = nh.advertise<std_msgs::Bool>("inCollision", 1);
        ros::Publisher pub3 = nh.advertise<visualization_msgs::Marker>("visualization_marker_vel", 1);
        pubPtr = &pub;
        pubPtr2 = &pub2;
        pubPtr3 = &pub3;
        ros::Rate rate(100);
        while (ros::ok())
        {
            ros::spinOnce();
            rate.sleep();
        }
        
    }