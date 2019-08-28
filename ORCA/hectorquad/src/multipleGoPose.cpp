#include <ros/ros.h>
#include <hectorquad/coordinate.h>
#include <std_srvs/Empty.h>

ros::ServiceClient *clientPtr1, *clientPtr2, *clientPtr3, *clientPtr4;

bool serviceCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
    {
        ROS_INFO_STREAM("Multiple Go Pose Called.");
        
        std_srvs::EmptyRequest tmpReq;
        std_srvs::EmptyResponse tmpRes;
        clientPtr1->call(tmpReq,tmpRes);
        clientPtr2->call(tmpReq,tmpRes);
        clientPtr3->call(tmpReq,tmpRes);
        clientPtr4->call(tmpReq,tmpRes);

        ROS_WARN_STREAM("Service is done!");
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "multipleGoPose");
        ros::NodeHandle nh;
        
        ros::service::waitForService("uav1/startPlanning");
        ros::ServiceClient client1 = nh.serviceClient<std_srvs::Empty>("uav1/startPlanning");
        clientPtr1 = &client1;

        ros::service::waitForService("uav2/startPlanning");
        ros::ServiceClient client2 = nh.serviceClient<std_srvs::Empty>("uav2/startPlanning");
        clientPtr2 = &client2;
        
        ros::service::waitForService("uav3/startPlanning");
        ros::ServiceClient client3 = nh.serviceClient<std_srvs::Empty>("uav3/startPlanning");
        clientPtr3 = &client3;
        
        ros::service::waitForService("uav4/startPlanning");
        ros::ServiceClient client4 = nh.serviceClient<std_srvs::Empty>("uav4/startPlanning");
        clientPtr4 = &client4;
    
        ros::ServiceServer service = nh.advertiseService("multiGoPose", &serviceCallback);

        ros::spin();
    }