#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

bool newKey, newAuto;

geometry_msgs::Twist keyMsg, autoMsg;

void getKey(const geometry_msgs::Twist &msg)
    {
        ROS_INFO_STREAM("Key received.");
        newKey = true;
        keyMsg = msg;
    }

void getAuto(const geometry_msgs::Twist &msg)
    {
        newAuto = true;
        autoMsg = msg;
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "regulateCmd");
        ros::NodeHandle nh;
        ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
        ros::Subscriber sub1 = nh.subscribe("turtle1/cmd_key", 1000, &getKey);
        ros::Subscriber sub2 = nh.subscribe("turtle1/cmd_oto", 10, &getAuto);
        ros::Rate rate(20);
        int i=0;
        while(ros::ok())
            {
                newKey = false;
                newAuto = false;
                ros::spinOnce();
                if (newKey == true)
                {
                    ROS_INFO_STREAM("Getting keyboard!");
                    pub.publish(keyMsg);
                    rate.sleep();
                    i = 20;
                    continue;
                }
                if (newAuto == true)
                {
                    if (i>0)
                    {
                        i--;
                        rate.sleep();
                        continue;
                    }
                    //ROS_INFO_STREAM("Getting auto i = "<<i);
                    pub.publish(autoMsg);
                }
            }
    }