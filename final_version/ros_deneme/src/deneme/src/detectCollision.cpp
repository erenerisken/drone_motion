#include <ros/ros.h>
#include <turtlesim/Pose.h>
#include <std_srvs/Empty.h>

#define epsilon 0.1

turtlesim::Pose pose1, pose2;

void getPose1(const turtlesim::Pose &msg)
    {
        pose1 = msg;
    }
void getPose2(const turtlesim::Pose &msg)
    {
        pose2 = msg;
    }

bool collided()
    {
        return fabs(pose1.x - pose2.x) < epsilon && fabs(pose1.y - pose2.y) < epsilon;
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "detectCol");
        ros::NodeHandle nh;
        std::string turtleName = "";
        while (ros::param::get("newTurtleName", turtleName) == false)
        {
            ROS_WARN_STREAM_ONCE("Waiting for second turtle to be initialized");
        }
        ROS_INFO_STREAM("Turtle "<<turtleName<<" has been initialized and started to being tracked.");
        ros::Subscriber sub2 = nh.subscribe(turtleName + "/pose", 1, &getPose2);
        ros::Subscriber sub1 = nh.subscribe("turtle1/pose", 1, &getPose1);
        ros::ServiceClient client = nh.serviceClient<std_srvs::Empty>(turtleName + "/reverseDirect");
        ros::service::waitForService(turtleName + "/reverseDirect");
        ros::Rate rate(2);
        while (ros::ok())
        {
            ros::spinOnce();
            if (collided())
            {
                ROS_WARN_STREAM("Collided. Reversing "<<turtleName);
                std_srvs::Empty::Request req;
                std_srvs::Empty::Response res;
                while (client.call(req, res) == false)
                {
                    ROS_WARN_STREAM("Waiting for response");
                    rate.sleep();
                }
                ROS_INFO_STREAM("Reversed "<<turtleName);
                for (int i = 0; i < 6; i++)
                {
                    rate.sleep();
                }
            }
            
        }
        
    }