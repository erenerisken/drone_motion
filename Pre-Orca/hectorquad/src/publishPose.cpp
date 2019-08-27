#include <ros/ros.h>
#include <geometry_msgs/Quaternion.h>
#include <gazebo_msgs/ModelStates.h>
#include <vector>

#define QUAD_COUNT 4

ros::NodeHandle *nhPtr;

class UAV
    {
        public:
            std::string name_;
            ros::Publisher pub_;
            UAV(std::string name) : name_(name)
                {
                    ROS_INFO_STREAM(name + " has been added.");
                    pub_ = nhPtr->advertise<geometry_msgs::Quaternion>(name + "/quadPose", 1);
                }
            UAV() {}
            void publish(const geometry_msgs::Quaternion &msg)
                {
                    pub_.publish(msg);
                }
    };

std::vector<UAV> quads;

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
                for (size_t j = 0; j < QUAD_COUNT; j++)
                    {
                        if(msg.name[*i] == quads[j].name_)
                            {
                                geometry_msgs::Quaternion pose;
                                pose.x = msg.pose[*i].position.x;
                                pose.y = msg.pose[*i].position.y;
                                pose.z = msg.pose[*i].position.z;
                                pose.w = msg.pose[*i].orientation.z;
                                quads[j].publish(pose);
                            }
                    }
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
        ros::spin();
    }