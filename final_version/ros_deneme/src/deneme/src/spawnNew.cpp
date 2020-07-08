#include <ros/ros.h>
#include <turtlesim/Spawn.h>

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "spawnNew");
        ros::NodeHandle nh;
        std::string spawnName = "";
        ros::Rate rate(2);
        while(!ros::param::get("newTurtleName", spawnName))
            {
                ROS_WARN_STREAM("Waiting for a name to be passed.");
                rate.sleep();
            }
        ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn>("spawn");
        ROS_INFO_STREAM("Spawning "<<spawnName);
        turtlesim::Spawn::Request spawnReq;
        turtlesim::Spawn::Response spawnRes;
        spawnReq.x = 5.0f;
        spawnReq.y = 5.0f;
        spawnReq.theta = 3*M_PI/2;
        spawnReq.name = spawnName;
        ros::service::waitForService("spawn");
        while(spawnClient.call(spawnReq, spawnRes) == false)
            {
                ROS_ERROR_STREAM_ONCE("An error occured while spawning "<<spawnName);
            }
        ROS_INFO_STREAM("Succesfully spawned "<<spawnRes.name);
        return 0;
    }