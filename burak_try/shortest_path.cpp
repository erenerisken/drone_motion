#include <bits/stdc++.h>

using namespace std;

typedef vector<vector<int> > Matrix;
typedef pair<int, int> intint;

int n,m;
intint startPos, destPos;

void generateMap()
	{
		int sx, sy, dx, dy;
		cin>>n>>m;
		cin>>sx>>sy>>dx>>dy;
		ofstream f;
		f.open("map.txt");
		f<<n<<" "<<m<<endl;
		for(int i = 0; i<n; i++)
			{
				for(int j = 0; j<m; j++)
					{
						if(sy == i && sx == j)
							f<<2;
						else if(dy == i && dx == j)
							f<<3;
						else
							f<<0;
						f<<" ";
					}
				f<<endl;
			}
		f<<sx<<" "<<sy<<endl<<dx<<" "<<dy<<endl;
		f.close();
	}

void printMap(Matrix map)
	{
	 	cout<<n<<" "<<m<<endl;
	 	for(int i = 0; i<n; i++)
	 		{
	 			for(int j = 0; j<m; j++)
	 				cout<<map[i][j]<<" ";
	 			cout<<endl;
	 		}
	}

Matrix getMap()
	{
		ifstream f;
	 	f.open("map.txt");
	 	f>>n>>m;
	 	Matrix map;
	 	map.resize(n);
	 	for(int i = 0; i<n; i++)
	 		for(int j = 0; j<m; j++)
	 			{
	 				int a;
	 				f>>a;
	 				map[i].push_back(a);
	 			}
	 	f>>startPos.first>>startPos.second;
	 	f>>destPos.first>>destPos.second;
	 	f.close();
	 	//cout<<"size = "<<n<<" "<<m<<endl;
		return map;
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

int dist(intint c1, intint c2)
	{
		//return abs(c1.second - c2.second) + abs(c1.first - c2.first);
		return (int)sqrt(pow((c1.second - c2.second),2) + pow((c1.first - c2.first) ,2));
	}

vector<intint> findShortestDjikstra()
	 {
	 	Matrix map = getMap();
		printMap(map);
		priority_queue<intint, vector<intint>, greater<intint> > q; // first = dist, second = nodeIndex
		vector<int> visited;
		visited.resize(n*m);
		vector<intint> minimumDist; // first : dist second : parent
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
				int curDist = q.top().first;
				if(curNode == coordToInd(destPos))
					break;
				visited[curNode] = 1;
				q.pop();
				intint coord = indToCoord(curNode);
				int x = coord.second, y = coord.first;
				vector<intint> adjCoords;
				adjCoords.push_back(intint(y,x-1));
				adjCoords.push_back(intint(y,x+1));
				adjCoords.push_back(intint(y+1,x));
				adjCoords.push_back(intint(y-1,x));
				for(vector<intint>::iterator i = adjCoords.begin(); i!= adjCoords.end(); i++)
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
												q.push(intint(minimumDist[indAdj].first, indAdj));
												visited[indAdj] = 1;
											}
									}
							}
					}
				/*cout<<"Su anki node "<<y<<" "<<x<<endl;
				for(int i = 0; i<n; i++)
					{
						for(int j = 0; j<m; j++)
							printf("%3d ",map[i][j] ? 0 : minimumDist[coordToInd(intint(i,j))].first);
						cout<<endl;
					}
				cout<<"-----------------------------"<<endl;*/
			}
		for(int i = 0; i<n; i++)
			{
				for(int j = 0; j<m; j++)
					printf("%3d ",minimumDist[coordToInd(intint(i,j))].first);
				cout<<endl;
			}
		return minimumDist;
	 }

vector<intint> findShortestAStar()
	 {
	 	Matrix map = getMap();
		printMap(map);
		priority_queue<intint, vector<intint>, greater<intint> > q; // first = dist, second = nodeIndex
		vector<int> visited;
		visited.resize(n*m);
		vector<intint> minimumDist; // first : dist second : parent
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
				vector<intint> adjCoords;
				adjCoords.push_back(intint(y,x-1));
				adjCoords.push_back(intint(y,x+1));
				adjCoords.push_back(intint(y+1,x));
				adjCoords.push_back(intint(y-1,x));

				adjCoords.push_back(intint(y+1,x+1));
				adjCoords.push_back(intint(y+1,x-1));
				adjCoords.push_back(intint(y-1,x+1));
				adjCoords.push_back(intint(y-1,x-1));


				for(vector<intint>::iterator i = adjCoords.begin(); i!= adjCoords.end(); i++)
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
												q.push(intint(minimumDist[indAdj].first + dist(destPos, *i), indAdj));
												visited[indAdj] = 1;
											}
									}
							}
					}
				/*cout<<"Su anki node "<<y<<" "<<x<<" f score = "<<fScore<<endl;
				for(int i = 0; i<n; i++)
					{
						for(int j = 0; j<m; j++)
							printf("%3d ",map[i][j] ? 0 : minimumDist[coordToInd(intint(i,j))].first);
						cout<<endl;
					}
				cout<<"-----------------------------"<<endl;*/
			}
		for(int i = 0; i<n; i++)
			{
				for(int j = 0; j<m; j++)
					printf("%3d ",minimumDist[coordToInd(intint(i,j))].first);
				cout<<endl;
			}
		return minimumDist;
	 }

bool visualize(vector<intint> route, Matrix map)
	{
		int cur = coordToInd(destPos);
		while(cur != 0)
			{
				intint coord = indToCoord(cur);
				map[coord.first][coord.second] = 4;
				cur = route[cur].second;
			}
		printMap(map);
		return true;
	}

int main()
	{
		//generateMap();
		cout<<"Before:"<<endl;
		vector<intint> minDistDjikstra = findShortestDjikstra();
		Matrix map = getMap();
		cout<<"-----------------------------"<<endl;
		cout<<"Djikstra:"<<endl;
		visualize(minDistDjikstra, map);
		vector<intint> minDistAStar = findShortestAStar();
		cout<<"A*:"<<endl;
		visualize(minDistAStar, map);
		
	}
