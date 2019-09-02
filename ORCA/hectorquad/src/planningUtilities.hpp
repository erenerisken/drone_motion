#ifndef _PLANNINGUTILITIES_HPP_
#define _PLANNINGUTILITIES_HPP_

#include <vector>
#include "motionUtilities.hpp"

#define INITIAL_HEIGHT 4.0 //not to hit object while going to start point
#define SLOPE_EPSILON 0.79 //Stable value 0.54 - 30 deg // 1.04 - 60 deg
#define HEIGHT 0.4  //from ground
#define PROBABILITY 0.07 // puts A* magic into RRT*

typedef std::vector<std::vector<int> > Matrix;
typedef std::pair<int, int> intint;

namespace planningUtilities
    {
        //TODO : check for line obstacle collision.(To avoid add edge passes through an object, as rewiring)

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