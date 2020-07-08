#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <cmath>

#define epsilon 0.01

turtlesim::Pose curPose, initialPose;

void process(const turtlesim::Pose &msg)
    {
        static bool initial = true;
        if (initial)
        {
            initial = false;
            initialPose = msg;
        }
        curPose = msg;
        return;
    }

bool equalAngle(float a, float b)
    {
        return fabs(a-b) < epsilon || fabs(a-b+2*M_PI) < epsilon || fabs(a-b-2*M_PI) < epsilon;
    }

bool equalPose(float a, float b)
    {
        return a-b<5*epsilon && a-b>-5*epsilon;
    }

bool atBase()
    {
        return fabs(curPose.x - initialPose.x) < 10*epsilon && fabs(curPose.y - initialPose.y) < 10*epsilon;
    }

float coeff(float a, float b)
    {
        if(b > a)
            {
                if (b-a < M_PI)
                    {
                        return 1.0f;
                    }
                else return -1.0f;                
            }
        else
            {
                if (a-b < M_PI)
                    {
                        return -1.0f;
                    }
                else return 1.0f;
            }
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "returnBase");
        ros::NodeHandle nh;
        ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_oto", 10);
        ros::Subscriber sub2 = nh.subscribe("turtle1/pose", 1, &process);
        while(ros::ok())
            {
                ros::spinOnce();
                if (atBase())
                {
                    geometry_msgs::Twist msg;
                    msg.angular.z = 0;
                    msg.linear.x = 0;
                    pub.publish(msg);
                    continue;
                }
                
                while (curPose.theta<0)
                {
                    curPose.theta += 2*M_PI;
                }
                
                if (equalPose(curPose.x, initialPose.x) == false)
                    {
                        if (curPose.x < initialPose.x)
                            {
                                if (equalAngle(curPose.theta, 0) == false)
                                {
                                    geometry_msgs::Twist msg;
                                    msg.linear.x = 0;
                                    msg.angular.z = coeff(curPose.theta, 0)*1;
                                    pub.publish(msg);
                                    ROS_INFO_STREAM("Getting theta=0"<<" cur="<<curPose.theta);
                                    continue;
                                }
                                else
                                {
                                    geometry_msgs::Twist msg;
                                    msg.linear.x = 2;
                                    msg.angular.z = 0;
                                    pub.publish(msg);
                                    ROS_INFO_STREAM("Moving Forward");
                                    continue;
                                }
                            }
                        else
                            {
                                if (equalAngle(curPose.theta, M_PI) == false)
                                {
                                    geometry_msgs::Twist msg;
                                    msg.linear.x = 0;
                                    msg.angular.z = 1*coeff(curPose.theta, M_PI);
                                    pub.publish(msg);
                                    ROS_INFO_STREAM("Getting theta="<<M_PI<<" cur="<<curPose.theta);
                                    continue;
                                }
                                else
                                {
                                    geometry_msgs::Twist msg;
                                    msg.linear.x = 2;
                                    msg.angular.z = 0;
                                    pub.publish(msg);
                                    ROS_INFO_STREAM("Moving Forward");
                                    continue;
                                }
                            }
                    }
                    else
                        {
                            if (equalPose(curPose.y, initialPose.y) == false)
                                {
                                    if (curPose.y < initialPose.y)
                                        {
                                            if (equalAngle(curPose.theta, M_PI/2) == false)
                                            {
                                                geometry_msgs::Twist msg;
                                                msg.linear.x = 0;
                                                msg.angular.z = 1*coeff(curPose.theta, M_PI/2);
                                                pub.publish(msg);
                                                ROS_INFO_STREAM("Getting theta="<<M_PI/2<<" cur="<<curPose.theta);
                                                continue;
                                            }
                                            else
                                            {
                                                geometry_msgs::Twist msg;
                                                msg.linear.x = 2;
                                                msg.angular.z = 0;
                                                pub.publish(msg);
                                                ROS_INFO_STREAM("Moving Forward");
                                                continue;
                                            }
                                        }
                                    else
                                        {
                                            if (equalAngle(curPose.theta, 3*M_PI/2) == false)
                                            {
                                                geometry_msgs::Twist msg;
                                                msg.linear.x = 0;
                                                msg.angular.z = 1*coeff(curPose.theta, 3*M_PI/2);
                                                pub.publish(msg);
                                                ROS_INFO_STREAM("Getting theta="<<3*M_PI/2<<" cur="<<curPose.theta);
                                                continue;
                                            }
                                            else
                                            {
                                                geometry_msgs::Twist msg;
                                                msg.linear.x = 2;
                                                msg.angular.z = 0;
                                                pub.publish(msg);
                                                ROS_INFO_STREAM("Moving Forward");
                                                continue;
                                            }
                                        }
                                }
                            else
                            {
                                geometry_msgs::Twist msg;
                                msg.angular.z = 0;
                                msg.linear.x = 0;
                                //pub.publish(msg);
                                continue;
                            }
                            
                        }  
            }
        return 0;
    }