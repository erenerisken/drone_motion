#include <ros/ros.h>
#include <hectorquad/coordinate.h>
#include <std_srvs/Empty.h>

ros::ServiceClient *clientPtr1, *clientPtr2, *clientPtr3, *clientPtr4;

bool serviceCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
    {
        ROS_INFO_STREAM("Multiple Go Pose Called.");
        hectorquad::coordinateRequest req1;
        hectorquad::coordinateResponse res1;
        req1.x = 10.0;
        req1.y = 10.0;
        req1.z = 2.0;
        clientPtr1->call(req1, res1);
        
        hectorquad::coordinateRequest req2;
        hectorquad::coordinateResponse res2;
        req2.x = -10.0;
        req2.y = -10.0;
        req2.z = 2.0;
        clientPtr2->call(req2, res2);
        
        hectorquad::coordinateRequest req3;
        hectorquad::coordinateResponse res3;
        req3.x = -10.0;
        req3.y = 10.0;
        req3.z = 2.0;
        clientPtr3->call(req3, res3);
        
        hectorquad::coordinateRequest req4;
        hectorquad::coordinateResponse res4;
        req4.x = 10.0;
        req4.y = -10.0;
        req4.z = 2.0;
        clientPtr4->call(req4, res4);
        ROS_WARN_STREAM("Service is done!");
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "multipleGoPose");
        ros::NodeHandle nh;
        
        ros::service::waitForService("uav1/quadGoPose");
        ros::ServiceClient client1 = nh.serviceClient<hectorquad::coordinate>("uav1/quadGoPose");
        clientPtr1 = &client1;

        ros::service::waitForService("uav2/quadGoPose");
        ros::ServiceClient client2 = nh.serviceClient<hectorquad::coordinate>("uav2/quadGoPose");
        clientPtr2 = &client2;
        
        ros::service::waitForService("uav3/quadGoPose");
        ros::ServiceClient client3 = nh.serviceClient<hectorquad::coordinate>("uav3/quadGoPose");
        clientPtr3 = &client3;
        
        ros::service::waitForService("uav4/quadGoPose");
        ros::ServiceClient client4 = nh.serviceClient<hectorquad::coordinate>("uav4/quadGoPose");
        clientPtr4 = &client4;
    
        ros::ServiceServer service = nh.advertiseService("multiGoPose", &serviceCallback);

        ros::spin();
    }