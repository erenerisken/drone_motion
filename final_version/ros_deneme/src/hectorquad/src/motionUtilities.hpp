#ifndef _MOTIONUTILITIES_HPP_
#define _MOTIONUTILITIES_HPP_

#include <ros/ros.h>
#include <iostream>
#include <vector>
#include <geometry_msgs/Vector3.h>
#include <algorithm>

#define FLOAT_EPSILON 0.001
#define EPSILON 0.1
#define QUAD_RADII 1.0
#define CLEARENCE 0.85 //(Coursera: Motion planning lecture)
#define QUAD_COUNT 4
#define COLLISION_COEFF 3
#define SKIP_COEFF 3
#define OBSTACLE_RADIUS 1.0
#define COLLISION_RADIUS 0.8
#define MAX_SPEED 1.3

#define INITIAL_HEIGHT 0.6 //not to hit object while going to start point
#define SLOPE_EPSILON 0.79 //Stable value 0.54 - 30 deg // 1.04 - 60 deg
#define HEIGHT 0.6  //from ground
#define PROBABILITY 0.07 // puts A* magic into RRT*

typedef std::vector<std::vector<int> > Matrix;
typedef std::pair<int, int> intint;

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
            bool operator!=(const Coordinate &rhs)
                {
                    return !(*this == rhs);
                }
            friend std::ostream& operator<<(std::ostream &os, const Coordinate &c)
                {
                    os<<"x = "<<c.x<<" y = "<<c.y<<" z = "<<c.z;
                    return os;
                }
            geometry_msgs::Vector3 getVector3()
                {
                    geometry_msgs::Vector3 ret;
                    ret.x = x;
                    ret.y = y;
                    ret.z = z;
                    return ret;
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

bool equalCoord(Coordinate &a, Coordinate &b)
    {
        return fabs(a.x - b.x) < EPSILON && fabs(a.y - b.y) < EPSILON
                && fabs(a.z - b.z) < EPSILON;
    }
Coordinate getClosestPoint(const Coordinate &p, const Coordinate &a, const Coordinate &b)
    {
        if(equalFloat(b.x, a.x))
            {
                Coordinate ret;
                ret.z = p.z;
                ret.x = b.x;
                ret.y = p.y;
                if (ret.y > std::max(a.y, b.y))
                    {
                        ret.y = std::max(a.y, b.y);
                    }
                if (ret.y < std::min(a.y, b.y))
                    {
                        ret.y = std::min(a.y, b.y);
                    }
                return ret;
            }
        if(equalFloat(b.y, a.y))
            {
                Coordinate ret;
                ret.z = p.z;
                ret.x = p.x;
                ret.y = a.y;
                if (ret.x > std::max(a.x, b.x))
                    {
                        ret.x = std::max(a.x, b.x);
                    }
                if (ret.x < std::min(a.x, b.x))
                    {
                        ret.x = std::min(a.x, b.x);
                    }
                return ret;
            }
        const float m1 = (b.y - a.y) / (b.x - a.x);
        const float m2 = -1.0 / m1;
        const float n1 = b.y - m1 * b.x;
        const float n2 = p.y - m2 * p.x;
        Coordinate ret;
        ret.z = p.z;
        ret.x = (n2 - n1) / (m1 - m2);
        ret.y = m1*ret.x + n1;
        if (ret.y > std::max(a.y, b.y))
            {
                ret.y = std::max(a.y, b.y);
            }
        if (ret.y < std::min(a.y, b.y))
            {
                ret.y = std::min(a.y, b.y);
            }
        if (ret.x > std::max(a.x, b.x))
            {
                ret.x = std::max(a.x, b.x);
            }
        if (ret.x < std::min(a.x, b.x))
            {
                ret.x = std::min(a.x, b.x);
            }
        return ret;
    }

namespace planningUtilities
    {

        bool isLinear(const Coordinate &a, const Coordinate &b, const Coordinate &c)
            {
                double slope1 = a.x == b.x ? -999.0 : (b.y - a.y) / (b.x - a.x);
                double slope2 = b.x == c.x ? -999.0 : (c.y - b.y) / (c.x - b.x);
                return fabs(slope2 - slope1) < SLOPE_EPSILON;
            }
        int dist(intint c1, intint c2)
            {
                //return abs(c1.second - c2.second) + abs(c1.first - c2.first);
                return (int)sqrt(pow((c1.second - c2.second),2) + pow((c1.first - c2.first) ,2));
            }
        std::vector<Coordinate> filterCoordinates(std::vector<Coordinate> &route, std::vector<Obstacle*> *obs)
            {
                //return route;
                std::vector<Coordinate> ret;
                for (size_t i = 0; i < route.size()-1; i++)
                    {
                        ret.push_back(route[i]);
                        for (size_t j = route.size()-1; j > i; j--)
                            {
                                bool clearPath = true;
                                for (auto obstPtr = obs->begin(); obstPtr < obs->end(); obstPtr++)
                                    {
                                        if((*obstPtr)->separates(route[i], route[j]))
                                            {
                                                clearPath = false;
                                                break;
                                            }
                                    }
                                if(!clearPath)
                                    {
                                        continue;
                                    }            
                                ret.push_back(route[j]);
                                i = j;
                                break;                    
                            }
                    }
                return ret;
            }
    }

#endif