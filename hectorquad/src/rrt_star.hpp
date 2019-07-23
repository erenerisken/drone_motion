#ifndef _RRT_STAR_HPP_
#define _RRT_STAR_HPP_

#include "planningUtilities.hpp"
#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <stack>

#define RADIUS 1 //fix later
#define MAXLOOP 1000

class RrtStarNode
        {
            public:
                Coordinate coord;
                int parentInd;
                double g;
                double lmc;

                RrtStarNode(Coordinate coord, int parentInd, double g, double lmc):coord(coord), parentInd(parentInd), g(g), lmc(lmc){};
                /*RrtStarNode* operator= (const RrtStarNode &rhs)
                {
                    RrtStarNode temp(Coordinate(0.0,0.0,0.0), 0, 0.0, 0.0);
                    temp.coord = rhs.coord;
                    temp.g = rhs.g;
                    temp.lmc = rhs.lmc;
                    temp.parentInd = rhs.parentInd;

                    return &temp;
                }*/
        };

namespace Rrt_star
    {
        std::vector<Obstacle*> *obs;
        Coordinate start, end;
        double stepSize, xStart, yStart, xEnd, yEnd;
        std::vector<RrtStarNode> startVertices, endVertices;
        void init(const Coordinate &startCoord, const Coordinate &endCoord, double step, std::vector<Obstacle*> &obstacles)
            {
                srand(time(NULL));
                start = startCoord;
                end = endCoord;
                obs = &obstacles;
                stepSize = step;
                startVertices.push_back(RrtStarNode(startCoord, -1, 0, 0)); // -1 means node has no parent
                endVertices.push_back(RrtStarNode(endCoord, -1, 0, 0));
            }
        std::vector<Coordinate> rrtGetMap(double xS, double xE, double yS, double yE)
            {
                xStart = xS; yStart = yS; xEnd = xE; yEnd = yE;
                bool finished = false;
                double minCost = 9999;
                int loop = MAXLOOP; //100K iteration . dont try this at home 
                int startTreeInd, endTreeInd;
                while(loop > 0)
                    {
                        double randX = xStart + (double)rand() / (double)RAND_MAX * (xEnd - xStart);
                        double randY = yStart + (double)rand() / (double)RAND_MAX * (yEnd - yStart);
                        Coordinate randomPoint(randX,  randY, HEIGHT);
                        int closestStart = 0, closestEnd = 0;
                        RrtStarNode closestStartPoint(startVertices[0].coord, startVertices[0].parentInd, startVertices[0].g, startVertices[0].lmc);
                        RrtStarNode closestEndPoint(endVertices[0].coord, endVertices[0].parentInd, endVertices[0].g, endVertices[0].lmc);
                        for(int i = 1; i<startVertices.size(); i++)
                            {
                                if(planningUtilities::dist(startVertices[i].coord, randomPoint) < planningUtilities::dist(closestStartPoint.coord, randomPoint))
                                    {
                                        closestStart = i;
                                        closestStartPoint.coord = startVertices[i].coord;
                                        closestStartPoint.g =  startVertices[i].g;
                                        closestStartPoint.lmc = startVertices[i].lmc;
                                        closestStartPoint.parentInd = startVertices[i].parentInd;
                                    }
                            }
                        for(int i = 1; i<endVertices.size(); i++)
                            {
                                if(planningUtilities::dist(endVertices[i].coord, randomPoint) < planningUtilities::dist(closestEndPoint.coord, randomPoint))
                                    {
                                        closestEnd = i;
                                        closestEndPoint.coord = endVertices[i].coord;
                                        closestEndPoint.g = endVertices[i].g;
                                        closestEndPoint.lmc = endVertices[i].lmc;
                                        closestEndPoint.parentInd = endVertices[i].parentInd;
                                    }
                            }
                        double magStart = std::min(stepSize, planningUtilities::dist(closestStartPoint.coord, randomPoint));
                        double magEnd = std::min(stepSize, planningUtilities::dist(closestEndPoint.coord, randomPoint));
                        double newStartX = closestStartPoint.coord.x + magStart * (randomPoint.x - closestStartPoint.coord.x) / planningUtilities::dist(closestStartPoint.coord, randomPoint);
                        double newStartY = closestStartPoint.coord.y + magStart * (randomPoint.y - closestStartPoint.coord.y) / planningUtilities::dist(closestStartPoint.coord, randomPoint);
                        double newEndX = closestEndPoint.coord.x + magEnd * (randomPoint.x - closestEndPoint.coord.x) / planningUtilities::dist(closestEndPoint.coord, randomPoint);
                        double newEndY = closestEndPoint.coord.y + magStart * (randomPoint.y - closestEndPoint.coord.y) / planningUtilities::dist(closestEndPoint.coord, randomPoint);
                        Coordinate newStart(newStartX, newStartY, HEIGHT);
                        Coordinate newEnd(newEndX, newEndY, HEIGHT);
                        bool inBoundStart = false, inBoundEnd = false;
                        for(auto i = obs->begin(); i != obs->end(); i++)
                            {
                                inBoundStart = inBoundStart || (*i)->inObstacle(newStart);
                                inBoundEnd = inBoundEnd || (*i)->inObstacle(newEnd);
                            }
                        RrtStarNode newStartNode(newStart, closestStart, closestStartPoint.g + magStart, 9999);
                        if(!inBoundStart)
                            {
                                startVertices.push_back(newStartNode);
                            }
                        RrtStarNode newEndNode(newEnd, closestEnd, closestEndPoint.g + magEnd, 9999);
                        if(!inBoundEnd)
                            {
                                endVertices.push_back(newEndNode);
                            }
                        if(inBoundStart || inBoundStart)
                            {
                                continue;
                            }
                        if(planningUtilities::dist(newStart, newEnd) < stepSize && minCost > (newEndNode.g + newStartNode.g + planningUtilities::dist(newStart, newEnd)))
                            {
                                startTreeInd = startVertices.size()-1;
                                endTreeInd = endVertices.size()-1;
                                minCost = newEndNode.g + newStartNode.g + planningUtilities::dist(newStart, newEnd);
                                ROS_WARN_STREAM("New min cost is: " + std::to_string(minCost));
                                ROS_INFO_STREAM("Change made in loop: " + std::to_string(MAXLOOP - loop));
                            }
                        loop--;
                        /*
                        for(auto i = startVertices.begin(); i != startVertices.end(); i++)
                            {

                            }*/
                        //finished güncelle
                    }
                
                std::stack<Coordinate> s;
                std::vector<Coordinate> ret;

                int cur = startTreeInd;
                while(cur != -1)
                    {
                        s.push(startVertices[cur].coord);
                        cur = startVertices[cur].parentInd;
                    }
                while(!s.empty())
                    {
                        ret.push_back(s.top());
                        s.pop();
                    }
                cur = endTreeInd;
                 while(cur != -1)
                    {
                        ret.push_back(endVertices[cur].coord);
                        cur = endVertices[cur].parentInd;
                    }
                ret[0].z += INITIAL_HEIGHT;
                return planningUtilities::filterCoordinates(ret);
            }

    //--------------------------- ONE TREE VERSION -----------------------------------//

    std::vector<Coordinate> rrtGetMapOne(double xS, double xE, double yS, double yE)
            {
                xStart = xS; yStart = yS; xEnd = xE; yEnd = yE;
                bool finished = false;
                double minCost = 9999;
                int loop = MAXLOOP; //100K iteration . dont try this at home 
                int startTreeInd;
                while(loop > 0)
                    {
                        double randX = xStart + (double)rand() / (double)RAND_MAX * (xEnd - xStart);
                        double randY = yStart + (double)rand() / (double)RAND_MAX * (yEnd - yStart);
                        Coordinate randomPoint(randX,  randY, HEIGHT);
                        int closestStart = 0;
                        RrtStarNode closestStartPoint(startVertices[0].coord, startVertices[0].parentInd, startVertices[0].g, startVertices[0].lmc);
                        for(int i = 1; i<startVertices.size(); i++)
                            {
                                if(planningUtilities::dist(startVertices[i].coord, randomPoint) < planningUtilities::dist(closestStartPoint.coord, randomPoint))
                                    {
                                        closestStart = i;
                                        closestStartPoint.coord = startVertices[i].coord;
                                        closestStartPoint.g =  startVertices[i].g;
                                        closestStartPoint.lmc = startVertices[i].lmc;
                                        closestStartPoint.parentInd = startVertices[i].parentInd;
                                    }
                            }
                        double magStart = std::min(stepSize, planningUtilities::dist(closestStartPoint.coord, randomPoint));
                        double newStartX = closestStartPoint.coord.x + magStart * (randomPoint.x - closestStartPoint.coord.x) / planningUtilities::dist(closestStartPoint.coord, randomPoint);
                        double newStartY = closestStartPoint.coord.y + magStart * (randomPoint.y - closestStartPoint.coord.y) / planningUtilities::dist(closestStartPoint.coord, randomPoint);
                        Coordinate newStart(newStartX, newStartY, HEIGHT);
                        bool inBoundStart = false;
                        for(auto i = obs->begin(); i != obs->end(); i++)
                            {
                                inBoundStart = inBoundStart || (*i)->inObstacle(newStart);
                            }
                        RrtStarNode newStartNode(newStart, closestStart, closestStartPoint.g + magStart, 9999);
                        if(!inBoundStart)
                            {
                                startVertices.push_back(newStartNode);
                            }
                        if(inBoundStart)
                            {
                                continue;
                            }
                        if(planningUtilities::dist(newStart, end) < stepSize * 10 && minCost > (newStartNode.g + planningUtilities::dist(newStart, end)))
                            {
                                startTreeInd = startVertices.size()-1;
                                minCost = newStartNode.g + planningUtilities::dist(newStart, end);
                                ROS_WARN_STREAM("New min cost is: " + std::to_string(minCost));
                                ROS_INFO_STREAM("Change made in loop: " + std::to_string(MAXLOOP - loop));
                            }
                        loop--;
                        /*
                        for(auto i = startVertices.begin(); i != startVertices.end(); i++)
                            {

                            }*/
                        //finished güncelle
                    }
                
                std::stack<Coordinate> s;
                std::vector<Coordinate> ret;
                int cur = startTreeInd;
                while(cur != -1)
                    {
                        s.push(startVertices[cur].coord);
                        cur = startVertices[cur].parentInd;
                    }
                while(!s.empty())
                    {
                        ret.push_back(s.top());
                        s.pop();
                    }
                ret[0].z += INITIAL_HEIGHT;
                return planningUtilities::filterCoordinates(ret);
            }
        
    }


#endif