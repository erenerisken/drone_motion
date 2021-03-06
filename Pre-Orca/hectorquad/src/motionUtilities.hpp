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
    };

bool does_sep(Coordinate &p1, Coordinate &p2, double x, double y, double r){
    double m = (p2.y - p1.y)/(p2.x - p1.x);
    double a = ((1+pow(m,2)));
    double b = ((-2*x) + (2*m*p1.y) - (pow(m,2)*p1.x) - m*y);
    double d = (pow(x,2) + pow(p1.y,2) + pow(m,2)*pow(p1.x,2) + pow(y,2) -2*m*p1.x*p1.y -2*p1.y*y +2*p1.y*y + 2*m*p1.x*y - r * r);
    double delta = pow(b , 2) - 4*a*d ;
    double final1,final2;
    if(delta<0) return false;
    else{
        final1=(-b + sqrt(delta))/2*a;
        if(p1.x<p2.x){
            if(p1.x<final1 && final1<p2.x){
                return true;
            }
            else return false;
        }
        else{
            if(p2.x<final1 && final1<1){
                return true;
            }
            else return false;
        }
    }
}
#endif