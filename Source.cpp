#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <map>
#include <functional>
#include <list>
#include <queue>
#include <iomanip>
using namespace std;


//this is the node used to represent a spot in the data structure, it had a destination and a weight, like a vector on a graph
struct adjacency {
    string destination;
    int weight;
};

//for BFS
struct nodeProperties
{
    string color;
    int distance;
    string predecessor;
};

//Read the vertecies)
vector<string> readIn(string filename) 
{
    ifstream myFile(filename);
    string line;
    if (!myFile.is_open()) throw runtime_error("Could not open file");
    vector<string> result;
    while (getline(myFile, line)) {
        result.push_back(line);
    }
    return result;
}

//read the edges
vector<vector<string>> readEdges(const string filename) {
    ifstream inputFile(filename);
    vector <vector<string>> romaniaEdges;
    string line;
    while (getline(inputFile, line)) {
        line.pop_back();
        stringstream ss(line);
        vector<string> result;
        int vectorIndex = 0;
        string colName;
        while (getline(ss, colName, ',')) {
            result.push_back(colName);
            vectorIndex++;
        }
        romaniaEdges.push_back(result);
    }
    return romaniaEdges;
}


//error checking
void Print(vector<vector<string>> const& vec) {
    cout << "\nELEMENTS:\n";
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = 0; j < vec[i].size(); j++)
        {
            cout << vec[i][j] << ' ';
        }
        cout << endl;
    }
    cout << "\n\n";
}

//adds edge to map, takes in the graph and pushes back a node with the info given
void addEdge(map<string, vector<adjacency>>& graph, string source, string dest, string weight) 
{
    adjacency addNode;
    addNode.destination = dest;
    addNode.weight = stoi(weight);
    graph[source].push_back(addNode);
}

//error check
void printVertices(vector<string> vertices)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        cout << vertices[i] << endl;
    }
}

//recursive function to print the path from one point to another
void printPath(map<string, nodeProperties>& graphProperties, string start, string end) {
    if(start == end)
    {
        cout << end << endl;
    }
    else if (graphProperties[end].predecessor == "")
    {
        cout << "no Path" << endl;
    }
    else
    {
        printPath(graphProperties, start, graphProperties[end].predecessor);
        cout << end << " "<<endl;
    }
}

//BFS
void BFS(map<string, nodeProperties>& graphProperties, map<string, vector<adjacency>>& graph, const string& start) {
   
    //initialize Queue
    queue<string> nodeQueue;
    //initializing starting node
    graphProperties[start].color = "gray";
    graphProperties[start].distance = 0;
    graphProperties[start].predecessor = "";
    nodeQueue.push(start);
    //start the queue
    while (!nodeQueue.empty())
    {
        //the top of the queue
        string top = nodeQueue.front();
        //pop from the queue
        nodeQueue.pop();

        //loop through the graph from the begining to the end, and if the next color is white, it will traverse there, turn the color gray, and add in the node to the queue. 
        for (auto itr = graph[top].begin(); itr != graph[top].end(); ++itr)
        {
            if (graphProperties[itr->destination].color == "white")
            {
                graphProperties[itr->destination].color = "gray";
                graphProperties[itr->destination].distance = graphProperties[top].distance + 1;
                graphProperties[itr->destination].predecessor = top;
                nodeQueue.push(itr->destination);
            }

        }
    }
}

//dijkstras
void dijkstra(map <string, vector<adjacency>>& graph, map<string, nodeProperties>& graphProperties, string const& start) {
   //create priority queue
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> priorityQueue;
    
    //initialization
    map<string, bool> processed;
    map<string, int> weight;
    string current;
    //Initialize Single Source
    for (auto itr = graph.begin(); itr != graph.end(); ++itr) {
        priorityQueue.push(make_pair(100000, itr->first));
        processed.emplace(itr->first, false);
        graphProperties[itr->first].predecessor = " ";
        weight.emplace(itr->first, 100000);
    }
    //weight at start is 0
    weight[start] = 0;
    priorityQueue.push(make_pair(0, start));

    //go through the graph, it will travel through the lowest weighted nodes to find the fastest distance between the nodes
    while (!priorityQueue.empty()) {
        current = priorityQueue.top().second;
        priorityQueue.pop();
        string adjNode;
        if (!processed[current]) {
            for (auto itr = graph[current].begin(); itr != graph[current].end(); ++itr) {
                //Relax
                adjNode = itr->destination;
                int currentToAdj = weight[current] + itr->weight;
                if (weight[adjNode] > currentToAdj) {
                    weight[adjNode] = currentToAdj;
                    graphProperties[adjNode].predecessor = current;
                    priorityQueue.push(make_pair(currentToAdj, adjNode));
                }
            }
            processed[current] = true;
        }
    }
}

int main()
{
    string fileName = "C:\\Users\\Thebl\\source\\repos\\315 assignment 3\\RomaniaVertices.txt";
    vector<string> vertices = readIn(fileName);
    vector<vector<string>> romaniaEdges = readEdges("C:\\Users\\Thebl\\source\\repos\\315 assignment 3\\RomaniaEdges.txt");

    //initalize map
    map<string, vector<adjacency>> graph;
    //add in vertices to the graph
    for (int i = 0; i < vertices.size(); i++)
    {
        vector<adjacency> newVec;
        graph.try_emplace(vertices[i], newVec);
    }

    //add edge function, is undirected.
    for (int i = 0; i < romaniaEdges.size(); i++)
    {
        //add edge takes in the graph and places the nodes into it
        addEdge(graph, romaniaEdges[i][0], romaniaEdges[i][1], romaniaEdges[i][2]);
        addEdge(graph, romaniaEdges[i][1], romaniaEdges[i][0], romaniaEdges[i][2]);
    }


    //oh my god im gonna cry... it worked...
    cout << "the printed version of the map is: " << endl << endl;
    for (int i = 0; i < graph.size(); i++)
    {
        cout << vertices[i] << " --> ";
        for (auto itr = graph[vertices[i]].begin(); itr != graph[vertices[i]].end(); ++itr)
        {
            cout << itr->destination << " ";
        }
        cout << "\n";
    }

    //time for BFS!!

    //create properties graph
    map<string, nodeProperties> graphProperties;
    for (auto const& vertices : vertices)
    {
        //node to be added
        nodeProperties addProperties;

        //sets the properties to its defaults;
        addProperties.color = "white";
        addProperties.distance = 100000;
        addProperties.predecessor = "";
        //places the default node into graph, it should have 20;
        graphProperties.try_emplace(vertices, addProperties);
    }
    
    string city1, city2;
    cout << endl << "Breadth First Search" << endl;

    cout << "the path from Arad to Sibiu is: " << endl;
    BFS(graphProperties, graph, "Arad");
    printPath(graphProperties, "Arad", "Sibiu");

    cout << "the path from Arad to Craiova is: " << endl;
    BFS(graphProperties, graph, "Arad");
    printPath(graphProperties, "Arad", "Craiova");

    cout << "the path from Arad to Bucharest is: " << endl;
    BFS(graphProperties, graph, "Arad");
    printPath(graphProperties, "Arad", "Bucharest");

    cout << "dijkstra's algorithm: " << endl;
    cout << "the shortest path from Arad to Bucharest is" << endl<<endl;
    dijkstra(graph,graphProperties, "Arad");
    printPath(graphProperties, "Arad", "Bucharest");
    cout << endl;
    return 0;
}