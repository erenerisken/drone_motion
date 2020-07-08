#ifndef _RRT_STAR_HPP_
#define _RRT_STAR_HPP_

#include <ros/ros.h>
#include "motionUtilities.hpp"
#include <stack>

#define REWIRERADIUS 0.45 //fix later
#define MAXLOOP 3000 //100K iteration . dont try this at home 

class RrtStarNode
        {
            public:
                Coordinate coord;
                int parentInd;
                int index;
                double g;
                double lmc;

                RrtStarNode(Coordinate coord, int parentInd, double g, double lmc, int ind):coord(coord), parentInd(parentInd), g(g), lmc(lmc), index(ind){};
                RrtStarNode(const RrtStarNode& rhs) : coord(rhs.coord), parentInd(rhs.parentInd), g(rhs.g), lmc(rhs.lmc), index(rhs.index){};
        };

namespace Rrt_star
    {
        std::vector<Obstacle*> *obs;
        Coordinate start, end;
        double stepSize, xStart, yStart, xEnd, yEnd;
        std::vector<RrtStarNode> startVertices, endVertices;
        int startInd = 1, endInd = 1;
        void init(const Coordinate &startCoord, const Coordinate &endCoord, double step, std::vector<Obstacle*> &obstacles)
            {
                srand(time(NULL));
                start = startCoord;
                end = endCoord;
                obs = &obstacles;
                stepSize = step;
                startVertices.push_back(RrtStarNode(start, -1, 0.0, 0.0, 0)); // -1 means node has no parent
                endVertices.push_back(RrtStarNode(end, -1, 0.0, 0.0, 0));
            }
//----------------------------------TWO TREE VERSION---------------------------------//
        std::vector<Coordinate> rrtGetMap(double xS, double xE, double yS, double yE)
            {
                xStart = xS; yStart = yS; xEnd = xE; yEnd = yE;
                bool finished = false;
                double minCost = 9999;
                int loop = MAXLOOP; 
                int startTreeInd = 0, endTreeInd = 0;

                while(loop > 0)
                    {
                        double randX = xStart + (double)rand() / (double)RAND_MAX * (xEnd - xStart);
                        double randY = yStart + (double)rand() / (double)RAND_MAX * (yEnd - yStart);
                        Coordinate randomPoint(randX,  randY, HEIGHT);
                        int closestStart = 0, closestEnd = 0;
                        RrtStarNode closestStartPoint(startVertices[0]);
                        RrtStarNode closestEndPoint(endVertices[0]);
                        
                        if(loop%100 == 0) ROS_WARN_STREAM("remaining loop #:" + std::to_string(loop));
                        //finding the closest node
                        for(int i = 1; i<startVertices.size(); i++)
                            {
                                if(dist(startVertices[i].coord, randomPoint) < dist(closestStartPoint.coord, randomPoint))
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
                                if(dist(endVertices[i].coord, randomPoint) < dist(closestEndPoint.coord, randomPoint))
                                    {
                                        closestEnd = i;
                                        closestEndPoint.coord = endVertices[i].coord;
                                        closestEndPoint.g = endVertices[i].g;
                                        closestEndPoint.lmc = endVertices[i].lmc;
                                        closestEndPoint.parentInd = endVertices[i].parentInd;
                                    }
                            }
                        
                        //creating new node wrt closest
                        double magStart = std::min(stepSize, dist(closestStartPoint.coord, randomPoint));
                        double magEnd = std::min(stepSize, dist(closestEndPoint.coord, randomPoint));
                        double newStartX = closestStartPoint.coord.x + magStart * (randomPoint.x - closestStartPoint.coord.x) / dist(closestStartPoint.coord, randomPoint);
                        double newStartY = closestStartPoint.coord.y + magStart * (randomPoint.y - closestStartPoint.coord.y) / dist(closestStartPoint.coord, randomPoint);
                        double newEndX = closestEndPoint.coord.x + magEnd * (randomPoint.x - closestEndPoint.coord.x) / dist(closestEndPoint.coord, randomPoint);
                        double newEndY = closestEndPoint.coord.y + magStart * (randomPoint.y - closestEndPoint.coord.y) / dist(closestEndPoint.coord, randomPoint);
                        Coordinate newStart(newStartX, newStartY, HEIGHT);
                        Coordinate newEnd(newEndX, newEndY, HEIGHT);

                        //check for new point
                        bool inBoundStart = false, inBoundEnd = false;
                        for(auto i = obs->begin(); i != obs->end(); i++)
                            {
                                inBoundStart = inBoundStart || (*i)->inObstacle(newStart);
                                inBoundEnd = inBoundEnd || (*i)->inObstacle(newEnd);
                            }
                        
                        RrtStarNode newStartNode(newStart, closestStart, closestStartPoint.g + magStart, 9999, startInd);
                        if(!inBoundStart)
                            {
                                //Reconnecting the graph wrt g values
                                for(int i = 0; i < startVertices.size(); i++)
                                    {
                                        bool isEdgeOK = true;
                                        double distance = dist(startVertices[i].coord, newStart);
                                        if( distance <= REWIRERADIUS && (distance + startVertices[i].g) < newStartNode.g )
                                            {
                                                for(auto j = obs->begin(); j != obs->end(); j++)
                                                    {
                                                        if(!(*j)->separates(newStart, startVertices[i].coord))
                                                            isEdgeOK = false;
                                                    }
                                                if(!isEdgeOK)
                                                    continue;
                                                newStartNode.parentInd = i;
                                                newStartNode.g = distance + startVertices[i].g;
                                            }
                                    }
                                for(int i = 0; i < startVertices.size(); i++)
                                    {
                                        bool isEdgeOK = true;
                                        double distance = dist(startVertices[i].coord, newStart);
                                        if( distance <= REWIRERADIUS && (distance + newStartNode.g) < startVertices[i].g )
                                            {
                                                for(auto j = obs->begin(); j != obs->end(); j++)
                                                    {
                                                        if(!(*j)->separates(newStart, startVertices[i].coord))
                                                           isEdgeOK = false;
                                                    }
                                                if(!isEdgeOK)
                                                    continue;
                                                startVertices[i].parentInd = newStartNode.index;
                                                startVertices[i].g = distance + newStartNode.g;
                                            }
                                    }
                                startVertices.push_back(newStartNode);
                                startInd++;
                            }
                        RrtStarNode newEndNode(newEnd, closestEnd, closestEndPoint.g + magEnd, 9999, endInd);
                        if(!inBoundEnd)
                            {
                                //Reconnecting the graph wrt g values
                                for(int i = 0; i < endVertices.size(); i++)
                                    {
                                        bool isEdgeOK = true;
                                        double distance = dist(endVertices[i].coord, newEnd);
                                        if( distance <= REWIRERADIUS && (distance + endVertices[i].g) < newEndNode.g )
                                            {
                                                for(auto j = obs->begin(); j != obs->end(); j++)
                                                    {
                                                        if(!(*j)->separates(newEnd, endVertices[i].coord))
                                                            isEdgeOK = false;
                                                    }
                                                if(!isEdgeOK)
                                                    continue;
                                                newEndNode.parentInd = i;
                                                newEndNode.g = distance + endVertices[i].g;
                                            }
                                    }
                                //Broadcasting
                                for(int i = 0; i < endVertices.size(); i++)
                                    {
                                        bool isEdgeOK = true;
                                        double distance = dist(endVertices[i].coord, newEnd);
                                        if( distance <= REWIRERADIUS && (distance + newEndNode.g) < endVertices[i].g )
                                            {
                                                for(auto j = obs->begin(); j != obs->end(); j++)
                                                    {
                                                        if(!(*j)->separates(newEnd, endVertices[i].coord))
                                                            isEdgeOK = false;
                                                    }
                                                if(!isEdgeOK)
                                                    continue;
                                                endVertices[i].parentInd = newEndNode.index;
                                                endVertices[i].g = distance + newEndNode.g;
                                            }
                                    }
                                endVertices.push_back(newEndNode);
                                endInd++;
                            }
                        if(inBoundStart || inBoundStart)
                            {
                                loop--;
                                continue;
                            }

                        //shortest path recording
                        if(dist(newStart, newEnd) < stepSize && minCost > (newEndNode.g + newStartNode.g + dist(newStart, newEnd)))
                            {
                                //next 2 line keeps the shortest path's end nodes
                                startTreeInd = startVertices.size()-1;
                                endTreeInd = endVertices.size()-1;
                                minCost = newEndNode.g + newStartNode.g + dist(newStart, newEnd);
                                ROS_WARN_STREAM("New min cost is: " + std::to_string(minCost));
                                ROS_INFO_STREAM("Change made in loop: " + std::to_string(MAXLOOP - loop));
                            }

                        loop--;
                    }
                
                std::stack<Coordinate> s;
                std::vector<Coordinate> ret;
                ROS_WARN_STREAM("The shortest path end node is " + std::to_string(startTreeInd));
                ROS_ERROR_STREAM("The shortest path length is " + std::to_string(startVertices[startTreeInd].g));
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
                ret = planningUtilities::filterCoordinates(ret, obs);
                return ret;
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
                int firstFound = 0;
                bool first = true;
                Coordinate randomPoint(0.0, 0.0, HEIGHT);
                while(loop > 0)
                    {
                        if(loop%100 == 0) ROS_WARN_STREAM("remaining loop #:" + std::to_string(loop));
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
                        RrtStarNode closestStartPoint(startVertices[0]);
                        for(int i = 1; i<startVertices.size(); i++)
                            {
                                if(dist(startVertices[i].coord, randomPoint) < dist(closestStartPoint.coord, randomPoint))
                                    {
                                        closestStart = i;
                                        closestStartPoint.coord = startVertices[i].coord;
                                        closestStartPoint.g =  startVertices[i].g;
                                        closestStartPoint.lmc = startVertices[i].lmc;
                                        closestStartPoint.parentInd = startVertices[i].parentInd;
                                    }
                            }
                        double magStart = std::min(stepSize, dist(closestStartPoint.coord, randomPoint));
                        double newStartX = closestStartPoint.coord.x + magStart * (randomPoint.x - closestStartPoint.coord.x) / dist(closestStartPoint.coord, randomPoint);
                        double newStartY = closestStartPoint.coord.y + magStart * (randomPoint.y - closestStartPoint.coord.y) / dist(closestStartPoint.coord, randomPoint);
                        Coordinate newStart(newStartX, newStartY, HEIGHT);
                        bool inBoundStart = false;
                        for(auto i = obs->begin(); i != obs->end(); i++)
                            {
                                inBoundStart = inBoundStart || (*i)->inObstacle(newStart);
                            }
                        RrtStarNode newStartNode(newStart, closestStart, closestStartPoint.g + magStart, 9999, startInd);
                        if(!inBoundStart)
                            {
                                for(int i = 0; i < startVertices.size(); i++)
                                {
                                    double distance = dist(startVertices[i].coord, newStart);
                                    bool isEdgeOK = true;
                                    if( distance <= REWIRERADIUS && (distance + startVertices[i].g) < newStartNode.g)
                                        {
                                            for(auto j = obs->begin(); j != obs->end(); j++)
                                                {
                                                    if(!(*j)->separates(newStart, startVertices[i].coord))
                                                        isEdgeOK = false;
                                                }
                                            if(!isEdgeOK)
                                                    continue;
                                            newStartNode.parentInd = i;
                                            newStartNode.g = distance + startVertices[i].g;
                                            //ROS_INFO_STREAM("RRT* worked at the loop: " + std::to_string(MAXLOOP - loop));
                                        }
                                }
                                for(int i = 0; i < startVertices.size(); i++)
                                    {
                                        bool isEdgeOK = true;
                                        double distance = dist(startVertices[i].coord, newStart);
                                        if( distance <= REWIRERADIUS && (distance + newStartNode.g) < startVertices[i].g )
                                            {
                                                for(auto j = obs->begin(); j != obs->end(); j++)
                                                    {
                                                        if(!(*j)->separates(newStart, startVertices[i].coord))
                                                            isEdgeOK = false;
                                                    }
                                                if(!isEdgeOK)
                                                    continue;
                                                startVertices[i].parentInd = newStartNode.index;
                                                startVertices[i].g = distance + newStartNode.g;
                                            }
                                    }
                                startInd++;
                                startVertices.push_back(newStartNode);
                            }
                        
                        
                        if(inBoundStart)
                            {
                                loop--;
                                continue;
                            }
                        if(dist(newStart, end) < stepSize * 10 && minCost > (newStartNode.g + dist(newStart, end)))
                            {
                                if(first)firstFound = MAXLOOP - loop, first = false;
                                startTreeInd = startVertices.size()-1;
                                minCost = newStartNode.g + dist(newStart, end);
                                ROS_ERROR_STREAM("New min cost is: " + std::to_string(minCost));
                                ROS_INFO_STREAM("Change made in loop: " + std::to_string(MAXLOOP - loop));
                            }
                        loop--;
                    }
                ROS_WARN_STREAM("loops are done");
                std::stack<Coordinate> s;
                std::vector<Coordinate> ret;
                int cur = startTreeInd;
                ROS_WARN_STREAM("The shortest path end node is " + std::to_string(startTreeInd));
                ROS_WARN_STREAM("The shortest path end node is " + std::to_string(startVertices.size()));
                ROS_ERROR_STREAM("The shortest path length is " + std::to_string(minCost));
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
                ROS_WARN_STREAM("First path is found at: " + std::to_string(firstFound));
                ROS_WARN_STREAM("End point was used as random:" + std::to_string(counter));
                //ROS_WARN_STREAM("Tried but can not wired edges:" + std::to_string(notWired));
                ret = planningUtilities::filterCoordinates(ret, obs);
                return ret;
            }

    }


#endif