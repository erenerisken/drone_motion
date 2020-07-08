#ifndef _RRT_SHARP_HPP_
#define _RRT_SHARP_HPP_

#include <queue>
#include <stack>
#include "motionUtilities.hpp"
#include <ros/ros.h>

#define REWIRERADIUS 0.45 //fix later
#define MAXLOOP1 30000 //100K iteration . dont try this at home 


class rrtSharpNode 
    {
        public:
            Coordinate coord;
            int parent;
            int ind;
            double g, lmc, key;

            rrtSharpNode(const Coordinate c, const int p, const double g, const double lmc, const double key,  const int ind) : coord(c), parent(p), g(g), lmc(lmc), key(key),  ind(ind) {}
            rrtSharpNode(const rrtSharpNode& rhs):coord(rhs.coord), parent(rhs.parent), g(rhs.g), lmc(rhs.lmc), key(rhs.key), ind(rhs.ind) {}
            bool operator>(const rrtSharpNode& rhs)const
            {   
                return (this->key > rhs.key);
            }
            bool operator<(const rrtSharpNode &rhs) const 
            {
                return (this->key < rhs.key);
            } 

    };



namespace Rrt_sharp
    {
        std::vector<Obstacle*> *obs;
        Coordinate startPoint, endPoint;
        double stepSize, xStartMap, xEndMap, yStartMap, yEndMap;
        std::vector<rrtSharpNode> vertices;
        std::priority_queue<rrtSharpNode, std::vector<rrtSharpNode>, std::greater<rrtSharpNode>> q;

        void init(const Coordinate &startCoord, const Coordinate &endCoord, double step, std::vector<Obstacle*> &obstacles)
            {
                srand(time(NULL));
                startPoint = startCoord;
                endPoint = endCoord;
                obs = &obstacles;
                stepSize = step;
                Coordinate c1(startPoint.x, startPoint.y, startPoint.z);
                rrtSharpNode startNode(c1, -1, 0.0, 0.0, 0.0, 0);
                vertices.push_back(startNode);
                
            }
        std::vector<Coordinate> getMap(double xS, double xE, double yS, double yE)
            {
                xStartMap = xS; yStartMap = yS; xEndMap = xE; yEndMap = yE;
                bool finished = false;
                double minCost = 9999;
                int loop = MAXLOOP1; 
                int startTreeInd = 0;
                int counter = 0, indCounter = 1; //counts the fact that end point is used for random point
                Coordinate randomPoint(0.0, 0.0, HEIGHT);
            
                while(loop > 0)
                    {
                        if(loop%1000 == 0) ROS_WARN_STREAM("remaining loop #:" + std::to_string(loop));
                        //ROS_WARN_STREAM("remaining loop #:" + std::to_string(loop));
                        // Sampling a point
                        double probability = (double) rand() / (double) RAND_MAX;
                        if(probability < PROBABILITY)
                            {
                                randomPoint.x = endPoint.x;
                                randomPoint.y = endPoint.y;
                                counter++;
                            }
                        else
                            {
                                double randX = xStartMap + (double)rand() / (double)RAND_MAX * (xEndMap - xStartMap);
                                double randY = yStartMap + (double)rand() / (double)RAND_MAX * (yEndMap - yStartMap);
                                randomPoint.x = randX;
                                randomPoint.y = randY;   
                            }
                        // End of Sampling

                        // ------------ Extend phase --------//
                        // Find Closest
                        int closest = 0;
                        rrtSharpNode closestPoint(vertices[0]);
                        for(int i = 1; i<vertices.size(); i++)
                            {
                                if(dist(vertices[i].coord, randomPoint) < dist(closestPoint.coord, randomPoint))
                                    {
                                        closest = i;
                                        closestPoint.coord = vertices[i].coord;
                                        closestPoint.g =  vertices[i].g;
                                        closestPoint.lmc = vertices[i].lmc;
                                        closestPoint.parent = vertices[i].parent;
                                    }
                            }
                        // Steering
                        double mag = std::min(stepSize, dist(closestPoint.coord, randomPoint));
                        double newX = closestPoint.coord.x + mag * (randomPoint.x - closestPoint.coord.x) / dist(closestPoint.coord, randomPoint);
                        double newY = closestPoint.coord.y + mag * (randomPoint.y - closestPoint.coord.y) / dist(closestPoint.coord, randomPoint);
                        Coordinate newNodeCoord(newX, newY, HEIGHT);

                        // Obstacle Check
                        bool inBound = false;
                        for(auto i = obs->begin(); i != obs->end(); i++)
                            {
                                inBound = inBound || (*i)->inObstacle(newNodeCoord);
                            }
                        if(inBound)
                            {
                                loop--;
                                continue;
                            }
                        
                        if(!inBound)
                            {
                                rrtSharpNode newNode(newNodeCoord, closest, INFINITY, closestPoint.g + mag, closestPoint.g + mag + dist(newNodeCoord, endPoint), indCounter);
                                for(int i = 0; i < vertices.size(); i++)
                                {
                                    double distance = dist(vertices[i].coord, newNodeCoord);
                                    bool isEdgeOK = true;
                                    if( distance <= REWIRERADIUS && (distance + vertices[i].g) < newNode.lmc)
                                        {
                                            for(auto j = obs->begin(); j != obs->end(); j++)
                                                {
                                                    if((*j)->separates(newNodeCoord, vertices[i].coord))
                                                        isEdgeOK = false;
                                                }
                                            if(!isEdgeOK)
                                                    continue;
                                            newNode.parent = i;
                                            newNode.lmc = distance + vertices[i].g;
                                        }
                                }
                                vertices.push_back(newNode);
                                indCounter++;
                                q.push(newNode);
                                if(dist(newNodeCoord, endPoint) < stepSize * 10 && minCost > (newNode.lmc + dist(newNodeCoord, endPoint)))
                                    {
                                        startTreeInd = vertices.size()-1;
                                        minCost = newNode.lmc + dist(newNodeCoord, endPoint);
                                        ROS_ERROR_STREAM("New min cost is: " + std::to_string(minCost));
                                        ROS_INFO_STREAM("Change made in loop: " + std::to_string(MAXLOOP1 - loop));
                                    }
                            }
                            //----- End of Extend -----//
                        //----- Replan ------//
                                /*ROS_WARN_STREAM(std::to_string(minCost)); 
                                ROS_WARN_STREAM(std::to_string(q.top().key));
                                ROS_INFO_STREAM(std::to_string(q.size()));*/
                       
                        while(!q.empty() && (q.top()).key < minCost)
                            {
                                
                                rrtSharpNode x(q.top());
                                q.pop();
                                //ROS_ERROR_STREAM("popped at the loop" + std::to_string(loop));
                                //ROS_ERROR_STREAM(std::to_string(x.ind));
                                x.g = x.lmc;
                                
                                for(int i = 0; i < vertices.size(); i++)
                                    {
                                        if(x.ind == vertices[i].ind)
                                            {
                                                //ROS_WARN_STREAM("found");
                                                vertices[i].g = x.g;
                                                break;
                                            }
                                    }
                            
                                for(int i = 0; i < vertices.size(); i++)
                                {
                                    double distance = dist(vertices[i].coord, x.coord);
                                    bool isEdgeOK = true;
                                    if( distance <= REWIRERADIUS && vertices[i].ind != x.ind)
                                        {
                                            for(auto j = obs->begin(); j != obs->end(); j++)
                                                {
                                                    if((*j)->separates(x.coord, vertices[i].coord))
                                                        isEdgeOK = false;
                                                }
                                            if(isEdgeOK && vertices[i].lmc > x.g + dist(vertices[i].coord, x.coord))
                                                {
                                                    vertices[i].lmc = x.g + dist(vertices[i].coord, x.coord);
                                                    vertices[i].parent = x.ind;
                                                    q.push(vertices[i]);
                                                }
                                            
                                        }
                                }
                            }
                        loop--;
                    }
                ROS_WARN_STREAM("loops are done");
                std::stack<Coordinate> s;
                std::vector<Coordinate> ret;
                int cur = startTreeInd;
                ROS_WARN_STREAM("The shortest path end node is " + std::to_string(startTreeInd));
                ROS_ERROR_STREAM("The shortest path length is " + std::to_string(minCost));//care here
                while(cur != -1)
                    {
                        s.push(vertices[cur].coord);
                        cur = vertices[cur].parent;
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
                ret = planningUtilities::filterCoordinates(ret, obs);
                return ret;
            }
    }

#endif