#ifndef _ASTAR_HPP_
#define _ASTAR_HPP_

#include <ros/ros.h>
#include <vector>
#include <fstream>
#include "motionUtilities.hpp"

typedef std::vector<std::vector<int> > Matrix;
typedef std::pair<int, int> intint;

namespace AStar
    {
        int n,m;
        intint startPos, destPos;
        Coordinate start, end;
        Matrix map;
        void generateMap(std::vector<Obstacle*> &obstacles, double xStart, double xEnd, double yStart, double yEnd, double stepSize)
            {
                n = (int)((yEnd - yStart)/stepSize);
                m = (int)((xEnd - xStart)/stepSize);
                map.resize(n);
                for (auto i = map.begin(); i != map.end(); i++)
                    {
                        i->resize(m);
                    }
                for (double x = xStart; x < xEnd; x+=stepSize)
                    {
                        for(double y = yStart; y < yEnd; y+=stepSize)
                            {
                                bool cleanPoint = true;
                                for (auto obs = obstacles.begin(); obs != obstacles.end(); obs++)
                                    {
                                        if((*obs)->inObstacle(Coordinate(x,y,0.5)))
                                            {
                                                cleanPoint = false;
                                                break;
                                            }
                                    }
                                if(!cleanPoint)
                                    {
                                        map[(int)((y-yStart) / stepSize)][(int)((x-xStart) / stepSize)] = 1;
                                    }
                            }
                    }
                ROS_INFO_STREAM(n << "x" << m << "map generated");
            }
        void printMap()
            {
                std::ofstream f;
                f.open("/home/burak/Desktop/map.txt");
                f<<n<<" "<<m<<std::endl;
                for (size_t i = 0; i < m; i++)
                {
                    for(size_t j = 0; j<n; j++)
                        {
                            f<<map[i][j]<<" ";
                        }
                    f<<std::endl;
                }
                f<<"0 0"<<std::endl<<n-1<<" "<<m-1<<std::endl;
                f.close();
            }
    }

#endif