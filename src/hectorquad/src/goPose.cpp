#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <hectorquad/coordinate.h>
#include <gazebo_msgs/ModelStates.h>

#define EPSILON 0.1
#define SPEED 1
#define QUADNAME "quadrotor"

class Coordinate
    {
        public:    
            double x,y,z;
            Coordinate(double x, double y, double z) : x(x), y(y), z(z) {}
            Coordinate()
                {
                    x = y = z = 0;
                }
    };

Coordinate coordToGo, currentCoord;

ros::Publisher *pub;

/*
bool equalCoord(Coordinate &a, Coordinate &b)
    {
        return fabs(a.x - b.x) < EPSILON && fabs(a.y - b.y) < EPSILON
                && fabs(a.z - b.z) < EPSILON;
    }
*/
double coeff(double a, double b)
    {
        if(fabs(a-b) < EPSILON)
            return 0;
        else return a < b ? 1.0 : -1.0;
    }

void getPose(const gazebo_msgs::ModelStates &msg)
    {
        int ind;
        for (ind = 0; ind < msg.name.size(); ind++)
        {
            if (msg.name[ind] == QUADNAME)
            {
                break;
            }
        }
        if (ind >= msg.name.size())
        {
            ROS_ERROR_STREAM("Quad not found");
            return;
        }
        static bool init = true;
        ROS_INFO_STREAM_ONCE("Quad is in position " << ind << "of the array.");
        currentCoord.x = msg.pose[ind].position.x;
        currentCoord.y = msg.pose[ind].position.y;
        currentCoord.z = msg.pose[ind].position.z;
        if (init == true)
        {
            init = false;
            coordToGo = currentCoord;
        }
        
        return;
    }

bool serverFunc(hectorquad::coordinate::Request &req, hectorquad::coordinate::Response &res)
{
    ROS_INFO_STREAM("Service has called for x = " << req.x << " y = " << req.y << " z  = " << req.z);
    res.s = "Processed.";
    coordToGo.x = req.x;
    coordToGo.y = req.y;
    coordToGo.z = req.z;
    return true;
}

void goPose()
    {
        geometry_msgs::Twist msg;
        float distanceX = fabs(coordToGo.x - currentCoord.x);
        float distanceY = fabs(coordToGo.y - currentCoord.y);
        float distanceZ = fabs(coordToGo.z - currentCoord.z);
        
        msg.linear.x = SPEED * distanceX / 2 * coeff(currentCoord.x, coordToGo.x);
        msg.linear.y = SPEED * distanceY / 2 * coeff(currentCoord.y, coordToGo.y);
        msg.linear.z = SPEED * distanceZ / 2 * coeff(currentCoord.z, coordToGo.z);
         
        /*
        msg.linear.x = SPEED * distanceX * fabs(distanceX) / 15;
        msg.linear.y = SPEED * distanceY * fabs(distanceY) / 15;
        msg.linear.z = SPEED * distanceZ * fabs(distanceZ) / 15;
        */
        pub->publish(msg);
        return;
    }

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "goPose");
    ros::NodeHandle nh;
    ROS_INFO_STREAM("Advertising service quadGoPose");
    ros::ServiceServer server = nh.advertiseService("quadGoPose", &serverFunc);
    ros::Subscriber sub1 = nh.subscribe("gazebo/model_states", 1, &getPose);
    ros::Publisher pub1 = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    pub = &pub1;
    ROS_INFO_STREAM("Subscribed.");
    while (ros::ok())
    {
        ros::spinOnce();
        goPose();
    }
    
    return 0;
}