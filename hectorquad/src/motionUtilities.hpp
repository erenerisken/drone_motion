#ifndef _MOTIONUTILITIES_HPP_
#define _MOTIONUTILITIES_HPP_

#include <ros/ros.h>
#include <iostream>

#define FLOAT_EPSILON 0.001
#define CLEARENCE 0.7 //(Coursera: Motion planning lecture)

bool equalFloat(double a, double b)
    {
        return fabs(a-b) <= FLOAT_EPSILON;
    }

bool isObstacle(const std::string &name)
    {
        if(name == "quadrotor" || name == "ground_plane")
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
        private:
            double r, h;
        public:
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
    };

#endif