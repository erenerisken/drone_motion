#ifndef _MOTIONUTILITIES_HPP_
#define _MOTIONUTILITIES_HPP_

#include <ros/ros.h>
#include <iostream>
#include <geometry_msgs/Vector3.h>

#define FLOAT_EPSILON 0.001
#define QUAD_RADII 1.0
#define CLEARENCE 0.7 //(Coursera: Motion planning lecture)
#define QUAD_COUNT 4
#define COLLISION_COEFF 3
#define SKIP_COEFF 4
#define OBSTACLE_RADIUS 0.1
#define COLLISION_RADIUS 1.0

bool equalFloat(double a, double b)
    {
        return fabs(a-b) <= FLOAT_EPSILON;
    }

bool isObstacle(const std::string &name)
    {
        if(name == "quadrotor" || name == "ground_plane" || name.substr(0,3) == "uav")
            return false;
        return true;
    }

class Coordinate
    {
        public:    
            double x,y,z;
            Coordinate(double x, double y, double z) : x(x), y(y), z(z) {}
            Coordinate()
                {
                    x = y = z = 0;
                }
            bool operator==(const Coordinate &rhs)
                {
                    return equalFloat(x, rhs.x) && equalFloat(y, rhs.y) && equalFloat(z, rhs.z);
                }
            friend std::ostream& operator<<(std::ostream &os, const Coordinate &c)
                {
                    os<<"x = "<<c.x<<" y = "<<c.y<<" z = "<<c.z;
                    return os;
                }
    };

class Obstacle
    {
        public:
            virtual bool inObstacle(const Coordinate &c) = 0;
            virtual bool separates(const Coordinate &c1, const Coordinate &c2) = 0;
            bool operator==(const Obstacle &rhs)
                {
                    return name == rhs.name && coord == rhs.coord;
                }
            friend std::ostream &operator<<(std::ostream &os, const Obstacle &obs)
                {
                    os<<obs.name<<" "<<obs.coord;
                    return os;
                }
            Coordinate coord;
            std::string name;
    };

class Cylinder : public Obstacle
    {
        public:
            double r, h;
            Cylinder(const std::string &name, const Coordinate &c, double r, double h) : r(r), h(h) 
                {
                    this->name = name;
                    coord = c;
                }
            bool inObstacle(const Coordinate &c)
                {
                    if (c.z > h)
                    {
                        return false;
                    }
                    return sqrt(pow(c.x - coord.x, 2) + pow(c.y - coord.y, 2)) < r + CLEARENCE;
                }
            bool separates(const Coordinate &c1, const Coordinate &c2)
                {
                    //Move everything to frame origined at the center of the cylinder
                    const double ax = c1.x - coord.x;
                    const double ay = c1.y - coord.y;
                    const double bx = c2.x - coord.x;
                    const double by = c2.y - coord.y;
                    const double R = r + CLEARENCE;
                    //set a,b,c of quadratic equation
                    const double a = pow(bx - ax, 2) + pow(by - ay, 2);
                    const double b = 2.0 * (ax * (bx - ax) + ay * (by - ay));
                    const double c = ax * ax + ay * ay - R * R;
                    //get discriminant
                    const double disc = b * b - 4.0 * a * c;
                    if (disc <= 0)
                        {
                            return false;
                        }
                    const double t1 = (-b + sqrt(disc)) / (2.0 * a);
                    const double t2 = (-b - sqrt(disc)) / (2.0 * a);
                    return (t1 >= 0.0 && t1 <= 1.0) || (t2 >= 0.0 && t2 <= 1.0);
                }
    };

double dist(const geometry_msgs::Vector3 &c1, const geometry_msgs::Vector3 &c2)
    {
        return sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y) + (c1.z - c2.z) * (c1.z - c2.z));
    }
double dist(const Coordinate &c1, const Coordinate &c2)
    {
        return sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y) + (c1.z - c2.z) * (c1.z - c2.z));
    }
#endif