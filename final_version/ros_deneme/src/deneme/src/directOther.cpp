#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_srvs/Empty.h>

ros::Publisher *pubPtr;

float multip = 1.0;

bool changeDirect(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
    {
        ROS_WARN_STREAM("Changing direction.");
        multip*=-1.0;
        return true;
    }

void velReceived(const geometry_msgs::Twist &msg)
    {
        geometry_msgs::Twist tmpMsg = msg;
        tmpMsg.linear.x*=multip;
        tmpMsg.angular.z*=multip;
        pubPtr->publish(tmpMsg);
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "directOther");
        ros::NodeHandle nh;
        ros::Subscriber sub = nh.subscribe("turtle1/cmd_vel", 1000, &velReceived);
        std::string turtleName;
        while (ros::param::get("newTurtleName", turtleName) == false)
        {
            ROS_WARN_STREAM("Waiting for turtle name...");
        }
        ros::Publisher pub = nh.advertise<geometry_msgs::Twist>(turtleName + "/cmd_vel", 1000);
        ros::ServiceServer server = nh.advertiseService(turtleName + "/reverseDirect", &changeDirect);
        pubPtr = &pub;
        ros::spin();
    }