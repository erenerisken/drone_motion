#include <ros/ros.h>
#include <std_srvs/Empty.h>

ros::NodeHandle *nhPtr;

class Color
    {
        public:
            int r,g,b;
            Color(int r, int g, int b) : r(r), g(g), b(b) {}
    };

Color matchColor(std::string colorName)
    {
        if(colorName == "red")
            {
                return Color(255,0,0);
            }
        if(colorName == "green")
            {
                return Color(0,255,0);
            }
        if(colorName == "blue")
            {
                return Color(0,0,255);
            }
        ROS_ERROR_STREAM("Color "<<colorName<<" does not exist!");
        return Color(255,255,255);
    }

void setColor(Color c)
    {
        ros::param::set("background_r", c.r);
        ros::param::set("background_g", c.g);
        ros::param::set("background_b", c.b);
        ros::ServiceClient clearClient = nhPtr->serviceClient<std_srvs::Empty>("/clear");
        std_srvs::Empty srv;
        clearClient.call(srv);
    }

int main(int argc, char* argv[])
    {
        ros::init(argc, argv, "setColor");
        ros::NodeHandle nh;
        nhPtr = &nh;
        ros::service::waitForService("clear");
        ros::Rate rate(2);
        std::string color = "";
        while (ros::ok())
        {
            std::string tmpColor = "";
            if (ros::param::get("color", tmpColor))
            {
                if (color != tmpColor)
                {
                    ROS_INFO_STREAM("Changing color to "<<tmpColor);
                    setColor(matchColor(tmpColor));
                    color = tmpColor;
                }
            }
        }
    }