#ifndef _ASTAR_HPP_
#define _ASTAR_HPP_

#include <ros/ros.h>
#include <vector>
#include <fstream>
#include "motionUtilities.hpp"
#include <queue>
#include <stack>


namespace AStar
    {
        int n,m;
        double step;
        intint startPos, destPos;
        Coordinate start, end;
        Matrix map;
        std::vector<Coordinate> findShortestAStar(double xStart, double xEnd, double yStart, double yEnd, double stepSize);
        void init(const Coordinate &startCoord, const Coordinate &endCoord)
            {
                start = startCoord;
                end = endCoord;
            }
        std::vector<Coordinate> generateMap(std::vector<Obstacle*> &obstacles, double xStart, double xEnd, double yStart, double yEnd, double stepSize)
            {
                step = stepSize;
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
                startPos.first = (start.y - yStart) / stepSize;
                startPos.second = (start.x - xStart) / stepSize;
                destPos.first = (end.y - yStart) / stepSize;
                destPos.second = (end.x - xStart) / stepSize;
                ROS_INFO_STREAM(n << "x" << m << "map generated for xs = " << xStart << " ys = " << yStart << " xe = " << xEnd << " ye = " << yEnd);
                std::vector<Coordinate> route = findShortestAStar(xStart, xEnd, yStart, yEnd, stepSize);
                route = planningUtilities::filterCoordinates(route, &obstacles);
                ROS_INFO_STREAM("Size of route is " << route.size());
                return route;
            }
        void printMap()
            {
                std::ofstream f;
                f.open("/home/$USER/Desktop/map.txt");
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
        intint indToCoord(int ind)
            {
                return intint(ind / m, ind % m);
            }

        int coordToInd(intint coord)
            {
                return m*coord.first + coord.second;
            }

        bool inBound(intint coord)
            {
                return coord.first >= 0 && coord.second >= 0 &&
                        coord.first<n && coord.second < m;
            }

        
        std::vector<intint> findShortestAStarCore()
            {
                std::priority_queue<intint, std::vector<intint>, std::greater<intint> > q; // first = dist, second = nodeIndex
                std::vector<int> visited;
                visited.resize(n*m);
                std::vector<intint> minimumDist; // first : dist second : parent
                minimumDist.resize(n*m);
                for(int i = 0; i<n*m; i++)
                    minimumDist[i].first = -1;
                int startNode = coordToInd(startPos);
                int destNode = coordToInd(destPos);
                minimumDist[startNode].first = 0;
                q.push(intint(0, startNode));
                while(!q.empty())
                    {
                        int curNode = q.top().second;
                        int curDist = minimumDist[curNode].first;
                        int fScore = q.top().first;
                        if(curNode == coordToInd(destPos))
                            break;
                        visited[curNode] = 1;
                        q.pop();
                        intint coord = indToCoord(curNode);
                        int x = coord.second, y = coord.first;
                        std::vector<intint> adjCoords;
                        adjCoords.push_back(intint(y,x-1));
                        adjCoords.push_back(intint(y,x+1));
                        adjCoords.push_back(intint(y+1,x));
                        adjCoords.push_back(intint(y-1,x));

                        adjCoords.push_back(intint(y+1,x+1));
                        adjCoords.push_back(intint(y+1,x-1));
                        adjCoords.push_back(intint(y-1,x+1));
                        adjCoords.push_back(intint(y-1,x-1));


                        for(std::vector<intint>::iterator i = adjCoords.begin(); i!= adjCoords.end(); i++)
                            {
                                if(inBound(*i) && map[i->first][i->second] != 1)
                                    {
                                        int indAdj = coordToInd(*i);
                                        if(minimumDist[indAdj].first == -1 || curDist + 1 < minimumDist[indAdj].first)
                                            {
                                                minimumDist[indAdj].first = curDist + 1;
                                                minimumDist[indAdj].second = curNode;
                                                if(!visited[indAdj])
                                                    {
                                                        //cout<<i->first<<" "<<i->second<<" ekleniyor dist = "<<dist(destPos, *i)<<endl;
                                                        q.push(intint(minimumDist[indAdj].first + planningUtilities::dist(destPos, *i), indAdj));
                                                        visited[indAdj] = 1;
                                                    }
                                            }
                                    }
                            }
                    }
                return minimumDist;
            }
        std::vector<Coordinate> findShortestAStar(double xStart, double xEnd, double yStart, double yEnd, double stepSize)
            {
                std::vector<intint> minimumDist = findShortestAStarCore();
                std::vector<Coordinate> route;
                int cur = minimumDist[coordToInd(destPos)].second;
                std::stack<int> routeStack;
                while(cur != 0)
                    {
                        routeStack.push(cur);
                        cur = minimumDist[cur].second;
                    }
                double h = 4.0;
                while(!routeStack.empty())
                    {
                        cur = routeStack.top();
                        routeStack.pop();
                        intint coord2 = indToCoord(cur);
                        Coordinate tmp(xStart + stepSize * coord2.second, yStart + stepSize * coord2.first, h + HEIGHT);
                        h = 0;
                        route.push_back(tmp);
                    }
                return route;
            }
    }

#endif