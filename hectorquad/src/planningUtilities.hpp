#ifndef _PLANNINGUTILITIES_HPP_
#define _PLANNINGUTILITIES_HPP_

#include <vector>

#define INITIAL_HEIGHT 4.0
#define SLOPE_EPSILON 0.54 //Stable value 0.01
#define HEIGHT 0.4

typedef std::vector<std::vector<int> > Matrix;
typedef std::pair<int, int> intint;

namespace planningUtilities
    {
        bool isLinear(const Coordinate &a, const Coordinate &b, const Coordinate &c)
            {
                double slope1 = a.x == b.x ? -999.0 : (b.y - a.y) / (b.x - a.x);
                double slope2 = b.x == c.x ? -999.0 : (c.y - b.y) / (c.x - b.x);
                return fabs(slope2 - slope1) < SLOPE_EPSILON;
            }
        double dist(const Coordinate &c1, const Coordinate &c2)
            {
                return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2) + pow(c1.z - c2.z, 2));
            }
        int dist(intint c1, intint c2)
            {
                //return abs(c1.second - c2.second) + abs(c1.first - c2.first);
                return (int)sqrt(pow((c1.second - c2.second),2) + pow((c1.first - c2.first) ,2));
            }
        std::vector<Coordinate> filterCoordinates(const std::vector<Coordinate> &route)
            {
                if(route.size() < 3)
                    return route;
                std::vector<Coordinate> ret;
                size_t i=0, j=1, k=2;
                for(k = 2; k<route.size(); k++)
                    {
                        if(isLinear(route[i], route[j], route[k]))
                            {
                                j = k;
                            }
                        else
                            {
                                ret.push_back(route[i]);
                                ret.push_back(route[j]);
                                i = j;
                                j = k;
                            }
                    }
                ret.push_back(route[route.size()-1]);
                return ret;
            }
    }

#endif