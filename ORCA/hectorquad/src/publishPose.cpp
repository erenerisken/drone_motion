#include <ros/ros.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Quaternion.h>
#include <gazebo_msgs/ModelStates.h>
#include <vector>
#include "motionUtilities.hpp"

ros::NodeHandle *nhPtr;

class UAV;
void velocityCallback(const geometry_msgs::Vector3Stamped &msg);
int getQuadIndex(const std::string &name);

std::vector<UAV> quads;
class UAV
    {
        public:
            std::string name_;
            ros::Publisher pub_, pub2_, pub3_;
            ros::Subscriber sub_;
            geometry_msgs::Vector3 vel_;
            geometry_msgs::Pose pose_;
            UAV(std::string name) : name_(name)
                {
                    ROS_INFO_STREAM(name + " has been added.");
                    pub_ = nhPtr->advertise<geometry_msgs::Quaternion>(name + "/quadPose", 1);
                    pub2_ = nhPtr->advertise<gazebo_msgs::ModelStates>(name + "/neighbors", 1);
                    pub3_ = nhPtr->advertise<gazebo_msgs::ModelStates>(name + "/model_states", 1);
                    sub_ = nhPtr->subscribe(name + "/fix_velocity", 1, &velocityCallback);
                }
            
            UAV() {}

            void publishPose(const geometry_msgs::Quaternion &msg)
                {
                    pub_.publish(msg);
                }
            
            void publishNeighbors()
                {
                    const int ind = getQuadIndex(name_);
                    gazebo_msgs::ModelStates msg;
                    geometry_msgs::Twist tmp;

                    msg.name.push_back(name_);
                    msg.pose.push_back(pose_);
                    tmp.linear = vel_;
                    msg.twist.push_back(tmp);

                    if (ind == -1)
                        {
                            ROS_FATAL_STREAM("An error occured in drone list.");
                        }
                    for (size_t i = 0; i < QUAD_COUNT; i++)
                        {
                            if(i == ind)
                                {
                                    continue;
                                }
                            msg.name.push_back(quads[i].name_);
                            msg.pose.push_back(quads[i].pose_);
                            tmp.linear = quads[i].vel_;
                            msg.twist.push_back(tmp);
                        }
                    pub2_.publish(msg);
                }
    };

int getQuadIndex(const std::string &name)
    {
        const int n = quads.size();
        for (int i = 0; i < n; i++)
            {
                if (name == quads[i].name_)
                {
                    return i;
                }
            }
        return -1;
    }

void velocityCallback(const geometry_msgs::Vector3Stamped &msg)
    {
        std::string name = msg.header.frame_id.substr(0,4);
        //ROS_INFO_STREAM(name + " received.");
        const int ind = getQuadIndex(name);
        if (ind == -1)
            {
                ROS_ERROR_STREAM_ONCE(name + " not found in list.");
                return;
            }
        quads[ind].vel_ = msg.vector;
    }

void getPoses(const gazebo_msgs::ModelStates &msg)
    {
        std::vector<int> indexes;
        const int modelCount = msg.name.size();
        for (size_t i = 0; i < modelCount; i++)
            {
                if (msg.name[i].substr(0,3) == "uav")
                    {
                        indexes.push_back(i);
                        //ROS_INFO_STREAM("A quad " << msg.name[i].substr(0,3) << " is found in "<<i);
                    }
            }
        for (auto i = indexes.begin(); i < indexes.end(); i++)
            {
                const int ind = getQuadIndex(msg.name[*i]);
                if(ind == -1)
                    {
                        continue;
                    }
                geometry_msgs::Quaternion pose;
                pose.x = msg.pose[*i].position.x;
                pose.y = msg.pose[*i].position.y;
                pose.z = msg.pose[*i].position.z;
                pose.w = msg.pose[*i].orientation.z;
                quads[ind].pose_ = msg.pose[*i];
                quads[ind].publishPose(pose);
            }
        for(auto i = quads.begin(); i < quads.end(); i++)
            {
                i->pub3_.publish(msg);
            }
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "publishPose");
        ros::NodeHandle nh;
        nhPtr = &nh;
        ros::Subscriber sub = nh.subscribe("gazebo/model_states", 1, &getPoses);
        for (size_t i = 1; i <= QUAD_COUNT; i++)
            {
                quads.push_back(UAV("uav" + std::to_string(i)));
            }
        ros::Rate rate(100);
        while(ros::ok())
            {
                ros::spinOnce();
                for (auto i = quads.begin(); i < quads.end(); i++)
                {
                    i->publishNeighbors();
                }
                rate.sleep();
            }
    }