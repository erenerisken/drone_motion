#ifndef _RRT_STAR_HPP_
#define _RRT_STAR_HPP_

#include "planningUtilities.hpp"
#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <stack>

#define REWIRERADIUS 0.45 //fix later
#define MAXLOOP 30000 //100K iteration . dont try this at home 

class RrtStarNode
        {
            public:
                Coordinate coord;
                int parentInd;
                double g;
                double lmc;

                RrtStarNode(Coordinate coord, int parentInd, double g, double lmc):coord(coord), parentInd(parentInd), g(g), lmc(lmc){};
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
                startVertices.push_back(RrtStarNode(start, -1, 0.0, 0.0)); // -1 means node has no parent
                endVertices.push_back(RrtStarNode(end, -1, 0.0, 0.0));
            }
//----------------------------------TWO TREE VERSION---------------------------------//
        std::vector<Coordinate> rrtGetMap(double xS, double xE, double yS, double yE)
            {
                xStart = xS; yStart = yS; xEnd = xE; yEnd = yE;
                bool finished = false;
                double minCost = 9999;
                int loop = MAXLOOP; 
                int startTreeInd, endTreeInd;

                while(loop > 0)
                    {
                        double randX = xStart + (double)rand() / (double)RAND_MAX * (xEnd - xStart);
                        double randY = yStart + (double)rand() / (double)RAND_MAX * (yEnd - yStart);
                        Coordinate randomPoint(randX,  randY, HEIGHT);
                        int closestStart = 0, closestEnd = 0;
                        RrtStarNode closestStartPoint(startVertices[0].coord, startVertices[0].parentInd, startVertices[0].g, startVertices[0].lmc);
                        RrtStarNode closestEndPoint(endVertices[0].coord, endVertices[0].parentInd, endVertices[0].g, endVertices[0].lmc);
                        
                        //finding the closest node
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
                        
                        //creating new node wrt closest
                        double magStart = std::min(stepSize, planningUtilities::dist(closestStartPoint.coord, randomPoint));
                        double magEnd = std::min(stepSize, planningUtilities::dist(closestEndPoint.coord, randomPoint));
                        double newStartX = closestStartPoint.coord.x + magStart * (randomPoint.x - closestStartPoint.coord.x) / planningUtilities::dist(closestStartPoint.coord, randomPoint);
                        double newStartY = closestStartPoint.coord.y + magStart * (randomPoint.y - closestStartPoint.coord.y) / planningUtilities::dist(closestStartPoint.coord, randomPoint);
                        double newEndX = closestEndPoint.coord.x + magEnd * (randomPoint.x - closestEndPoint.coord.x) / planningUtilities::dist(closestEndPoint.coord, randomPoint);
                        double newEndY = closestEndPoint.coord.y + magStart * (randomPoint.y - closestEndPoint.coord.y) / planningUtilities::dist(closestEndPoint.coord, randomPoint);
                        Coordinate newStart(newStartX, newStartY, HEIGHT);
                        Coordinate newEnd(newEndX, newEndY, HEIGHT);

                        //check for new point
                        bool inBoundStart = false, inBoundEnd = false;
                        for(auto i = obs->begin(); i != obs->end(); i++)
                            {
                                inBoundStart = inBoundStart || (*i)->inObstacle(newStart);
                                inBoundEnd = inBoundEnd || (*i)->inObstacle(newEnd);
                            }
                        
                        RrtStarNode newStartNode(newStart, closestStart, closestStartPoint.g + magStart, 9999);
                        if(!inBoundStart)
                            {
                                //Reconnecting the graph wrt g values
                                for(int i = 0; i < startVertices.size(); i++)
                                    {
                                        bool isEdgeOK = true;
                                        double distance = planningUtilities::dist(startVertices[i].coord, newStart);
                                        if( distance <= REWIRERADIUS && (distance + startVertices[i].g) < newStartNode.g )
                                            {
                                                for(auto j = obs->begin(); j != obs->end(); j++)
                                                    {
                                                        if(does_sep(newStart, startVertices[i].coord, (*j)->coord.x, (*j)->coord.y, 0.5))
                                                        isEdgeOK = false;
                                                    }
                                                if(!isEdgeOK)
                                                    continue;
                                                newStartNode.parentInd = i;
                                                newStartNode.g = distance + startVertices[i].g;
                                            }
                                    }
                                startVertices.push_back(newStartNode);
                            }
                        RrtStarNode newEndNode(newEnd, closestEnd, closestEndPoint.g + magEnd, 9999);
                        if(!inBoundEnd)
                            {
                                //Reconnecting the graph wrt g values
                                for(int i = 0; i < endVertices.size(); i++)
                                    {
                                        bool isEdgeOK = true;
                                        double distance = planningUtilities::dist(endVertices[i].coord, newEnd);
                                        if( distance <= REWIRERADIUS && (distance + endVertices[i].g) < newEndNode.g )
                                            {
                                                for(auto j = obs->begin(); j != obs->end(); j++)
                                                    {
                                                        if(does_sep(newEnd, endVertices[i].coord, (*j)->coord.x, (*j)->coord.y, 0.5))
                                                        isEdgeOK = false;
                                                    }
                                                if(!isEdgeOK)
                                                    continue;
                                                newEndNode.parentInd = i;
                                                newEndNode.g = distance + endVertices[i].g;
                                            }
                                    }
                                endVertices.push_back(newEndNode);
                            }
                        if(inBoundStart || inBoundStart)
                            {
                                continue;
                            }

                        //shortest path recording
                        if(planningUtilities::dist(newStart, newEnd) < stepSize && minCost > (newEndNode.g + newStartNode.g + planningUtilities::dist(newStart, newEnd)))
                            {
                                //next 2 line keeps the shortest path's end nodes
                                startTreeInd = startVertices.size()-1;
                                endTreeInd = endVertices.size()-1;
                                minCost = newEndNode.g + newStartNode.g + planningUtilities::dist(newStart, newEnd);
                                ROS_WARN_STREAM("New min cost is: " + std::to_string(minCost));
                                ROS_INFO_STREAM("Change made in loop: " + std::to_string(MAXLOOP - loop));
                            }

                        loop--;
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
                int loop = MAXLOOP;
                int startTreeInd = 0;
                int counter = 0;
                Coordinate randomPoint(0.0, 0.0, HEIGHT);
                while(loop > 0)
                    {
                        double probability = (double) rand() / (double) RAND_MAX;
                        if(probability < PROBABILITY)
                            {
                                randomPoint.x = end.x;
                                randomPoint.y = end.y;
                                counter++;
                            }
                        else
                            {
                                double randX = xStart + (double)rand() / (double)RAND_MAX * (xEnd - xStart);
                                double randY = yStart + (double)rand() / (double)RAND_MAX * (yEnd - yStart);
                                randomPoint.x = randX;
                                randomPoint.y = randY;   
                            }
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
                                for(int i = 0; i < startVertices.size()-1; i++)
                                {
                                    double distance = planningUtilities::dist(startVertices[i].coord, newStart);
                                    bool isEdgeOK = true;
                                    if( distance <= REWIRERADIUS && (distance + startVertices[i].g) < newStartNode.g)
                                        {
                                            for(auto j = obs->begin(); j != obs->end(); j++)
                                                {
                                                    if(does_sep(newStart, startVertices[i].coord, (*j)->coord.x, (*j)->coord.y, 0.5))
                                                        isEdgeOK = false;
                                                }
                                            if(!isEdgeOK)
                                                    continue;
                                            newStartNode.parentInd = i;
                                            newStartNode.g = distance + startVertices[i].g;
                                            ROS_INFO_STREAM("RRT* worked at the loop: " + std::to_string(MAXLOOP - loop));
                                        }
                                }
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
                    }
                ROS_WARN_STREAM("loops are done");
                std::stack<Coordinate> s;
                std::vector<Coordinate> ret;
                int cur = startTreeInd;
                ROS_WARN_STREAM("The shortest path end node is " + std::to_string(startTreeInd));
                ROS_ERROR_STREAM("The shortest path length is " + std::to_string(startVertices[startTreeInd].g));
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
                ROS_WARN_STREAM("func is done");
                ROS_WARN_STREAM("End point was used as random:" + std::to_string(counter));
                //ROS_WARN_STREAM("Tried but can not wired edges:" + std::to_string(notWired));
                return planningUtilities::filterCoordinates(ret);
            }
        
    }


#endif