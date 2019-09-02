#include <ros/ros.h>
#include <hectorquad/coordinate.h>
#include <std_srvs/Empty.h>
#include <vector>
#include "motionUtilities.hpp"

std::vector<ros::ServiceClient> clients;

bool serviceCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
    {
        ROS_INFO_STREAM("Multiple Go Pose Called.");
        
        for(auto i = clients.begin(); i < clients.end(); i++)
            {
                std_srvs::EmptyRequest tmpReq;
                std_srvs::EmptyResponse tmpRes;
                i->call(tmpReq, tmpRes);
            }

        ROS_WARN_STREAM("Service is done!");
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "multipleGoPose");
        ros::NodeHandle nh;
        for(size_t i = 1; i <= QUAD_COUNT; i++)
            {
                ros::service::waitForService("uav" + std::to_string(i) + "/startPlanning");
                clients.push_back(nh.serviceClient<std_srvs::Empty>("uav" + std::to_string(i) + "/startPlanning"));
            }
        ros::ServiceServer service = nh.advertiseService("multiGoPose", &serviceCallback);

        ros::spin();
    }