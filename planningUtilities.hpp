#ifndef _PLANNINGUTILITIES_HPP_
#define _PLANNINGUTILITIES_HPP_

#include <vector>

#define SLOPE_EPSILON 0.05 //Stable value 0.01

namespace planningUtilities
    {
        bool isLinear(const Coordinate &a, const Coordinate &b, const Coordinate &c)
            {
                double slope1 = a.x == b.x ? -999.0 : (b.y - a.y) / (b.x - a.x);
                double slope2 = b.x == c.x ? -999.0 : (c.y - b.y) / (c.x - b.x);
                return fabs(slope2 - slope1) < SLOPE_EPSILON;
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