#include <ros/ros.h>
#include <gazebo_msgs/SpawnModel.h>
#include <geometry_msgs/Pose.h>
#include <stdlib.h>
int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "spawnBox");
        ros::NodeHandle nh;
        system("rosrun gazebo_ros spawn_model -urdf -file /box.urdf -model box1 -x 1.0 -y 1.0 -z 1.0");
        system("rosrun gazebo_ros spawn_model -urdf -file /box.urdf -model box2 -x 1.0 -y 3.0 -z 1.0");
    }