#ifndef _RRT_HPP_
#define _RRT_HPP_

#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <stack>

typedef std::pair<Coordinate, int> RrtNode;

namespace Rrt
    {
        std::vector<Obstacle*> *obs;
        Coordinate start, end;
        double stepSize, xStart, yStart, xEnd, yEnd;
        std::vector<RrtNode> startNodes, endNodes;
        void init(const Coordinate &startCoord, const Coordinate &endCoord, double step, std::vector<Obstacle*> &obstacles)
            {
                srand(time(NULL));
                start = startCoord;
                end = endCoord;
                obs = &obstacles;
                stepSize = step;
                startNodes.push_back(RrtNode(start, -1));
                endNodes.push_back(RrtNode(end, -1));
            }
        std::vector<Coordinate> rrtGetMap(double xS, double xE, double yS, double yE)
            {
                xStart = xS; yStart = yS; xEnd = xE; yEnd = yE;
                bool finished = false;
                while(!finished)
                    {
                        double randX = xStart + (double)rand() / (double)RAND_MAX * (xEnd - xStart);
                        double randY = yStart + (double)rand() / (double)RAND_MAX * (yEnd - yStart);
                        Coordinate randomPoint(randX,  randY, HEIGHT);
                        int closestStart = 0, closestEnd = 0;
                        Coordinate closestStartPoint = startNodes[0].first;
                        Coordinate closestEndPoint = endNodes[0].first;
                        for(int i = 1; i<startNodes.size(); i++)
                            {
                                if(dist(startNodes[i].first, randomPoint) < dist(closestStartPoint, randomPoint))
                                    {
                                        closestStart = i;
                                        closestStartPoint = startNodes[i].first; 
                                    }
                            }
                        for(int i = 1; i<endNodes.size(); i++)
                            {
                                if(dist(endNodes[i].first, randomPoint) < dist(closestEndPoint, randomPoint))
                                    {
                                        closestEnd = i;
                                        closestEndPoint = endNodes[i].first;
                                    }
                            }
                            
                        double magStart = std::min(stepSize, dist(closestStartPoint, randomPoint));
                        double magEnd = std::min(stepSize, dist(closestEndPoint, randomPoint));
                        double newStartX = closestStartPoint.x + magStart * (randomPoint.x - closestStartPoint.x) / dist(closestStartPoint, randomPoint);
                        double newStartY = closestStartPoint.y + magStart * (randomPoint.y - closestStartPoint.y) / dist(closestStartPoint, randomPoint);
                        double newEndX = closestEndPoint.x + magEnd * (randomPoint.x - closestEndPoint.x) / dist(closestEndPoint, randomPoint);
                        double newEndY = closestEndPoint.y + magStart * (randomPoint.y - closestEndPoint.y) / dist(closestEndPoint, randomPoint);
                        Coordinate newStart(newStartX, newStartY, HEIGHT), newEnd(newEndX, newEndY, HEIGHT);
                        bool inBoundStart = false, inBoundEnd = false;
                        for(auto i = obs->begin(); i != obs->end(); i++)
                            {
                                inBoundStart = inBoundStart || (*i)->inObstacle(newStart);
                                inBoundEnd = inBoundEnd || (*i)->inObstacle(newEnd);
                            }
                        if(!inBoundStart)
                            {
                                startNodes.push_back(RrtNode(newStart, closestStart));
                            }
                        if(!inBoundEnd)
                            {
                                endNodes.push_back(RrtNode(newEnd, closestEnd));
                            }
                        if(inBoundEnd || inBoundStart)
                            {
                                continue;
                            }
                        if(dist(newStart, newEnd) < stepSize)
                            {
                                finished = true;
                            }
                        //finished güncelle
                    }
                std::stack<Coordinate> s;
                std::vector<Coordinate> ret;
                int cur = startNodes.size() - 1;
                while(cur != -1)
                    {
                        s.push(startNodes[cur].first);
                        cur = startNodes[cur].second;
                    }
                while(!s.empty())
                    {
                        ret.push_back(s.top());
                        s.pop();
                    }
                cur = endNodes.size() - 1;
                while(cur != -1)
                    {
                        ret.push_back(endNodes[cur].first);
                        cur = endNodes[cur].second;
                    }
                ret[0].z += INITIAL_HEIGHT;
                ret = planningUtilities::filterCoordinates(ret, obs);
                return ret;
            }
        
    }


#endif