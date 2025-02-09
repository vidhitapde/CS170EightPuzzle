#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>
using namespace std; 


struct Node
{
    vector <vector <int> > puzzle;
    Node* parent;
    int cost_gn; // cost from starting node
    int cost_hn; 
    int cost_fn; 

    Node(vector <vector <int > > puzzle, Node*parent = nullptr, int gn = 0, int hn = 0)
    : puzzle(puzzle), parent(parent), cost_gn(gn), cost_hn(hn), cost_fn(gn+hn)
    {

    }
    bool operator > (Node & node) const
    {
        return cost_fn > node.cost_fn; 
    }
};

vector <vector <int> > goalState = 
{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

vector <vector <int> > depthLevel2 = 
{
    {1, 2, 3},
    {4, 5, 6},
    {0, 7, 8}
};

vector <vector <int> > depthLevel8 = 
{
    {1, 3, 6},
    {5, 0, 2},
    {4, 7, 8}
};

vector <vector <int> > depthLevel12 = 
{
    {1, 3, 6},
    {5, 0, 7},
    {4, 8, 2}
};

vector <vector <int> > depthLevel16 = 
{
    {1, 6, 7},
    {5, 0, 3},
    {4, 8, 2}
};
vector <vector <int> > depthLevel20 = 
{
    {7, 1, 2},
    {4, 8, 5},
    {6, 3, 0}
};
vector <vector <int> > depthLevel24 = 
{
    {0, 7, 2},
    {4, 6, 1},
    {3, 5, 8}
};

//check where the empty tile is 
pair<int, int > zeroTile(vector <vector <int> > puzzle)
{

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (puzzle[i][j] == 0)
            {
                return make_pair(i,j); 
            }
        }
    }
    return make_pair(-1,-1); 
}

//expands the puzzle, all possible moves left right up down, etc
vector <Node * > expand(Node *node)
{
    vector <vector <int> > puzzle = node->puzzle; 
    vector <Node*> children; 
    pair <int, int> zeroTilePos = zeroTile(puzzle); 
    int x = zeroTilePos.first;
    int y = zeroTilePos.second; 

    //to move it up
    if (x > 0)
    {
        vector <vector <int > > child = puzzle; 
        swap(child[x][y], child[x-1][y]); 
        Node* childrenNode = new Node(child, node, node->cost_gn +1); 
        children.push_back(childrenNode); 
    }
    if (x < 2) // moving it down
    {
        vector <vector <int > > child = puzzle; 
        swap(child[x][y], child[x+1][y]); 
        Node* childrenNode = new Node(child, node, node->cost_gn +1); 
        children.push_back(childrenNode); 
    }
    if(y > 0) // moving it left 
    {
        vector <vector <int > > child = puzzle; 
        swap(child[x][y], child[x][y-1]); 
        Node* childrenNode = new Node(child, node, node->cost_gn +1); 
        children.push_back(childrenNode); 
    }
    if(y < 2) // moving it right
    {
        vector <vector <int > > child = puzzle; 
        swap(child[x][y], child[x][y+1]); 
        Node* childrenNode = new Node(child, node, node->cost_gn +1); 
        children.push_back(childrenNode); 
    }


    return children; 
}

struct CompareNode{
    bool operator()(Node* const & a, Node* const & b)
    {
        return a->cost_fn > b->cost_fn; 
    }

};
//function to check if puzzle reached goal state
bool goalStateReached(vector <vector<int> > puzzle)
{
    return puzzle == goalState; 
}
//function to check whether puzzle has been visited or not
bool visited (vector <vector <int > > &puzzle, vector <vector <vector <int > > >&visitedPuzzles)
{
    for (int i = 0; i < visitedPuzzles.size(); i++)
    {
        if (puzzle == visitedPuzzles[i])
        {
            return true; 
        }
    }
    return false; 
}
int  uniformCostSearch(vector <vector <int > > &puzzle)
{
    return 0; 
}

int misplacedTileHeuristic(vector <vector <int > > &puzzle)
{
    int hn = 0; 

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if((puzzle[i][j] != goalState[i][j]) && (puzzle[i][j] != 0))
            {
                hn++; 
            }
        }
    }
    return hn; 
}
int manhattanDistanceHeuristic(vector <vector <int > > &puzzle)
{
    int hn = 0; 
    int goaliVal = 0; 
    int goaljVal = 0; 
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if((puzzle[i][j] != goalState[i][j]) && (puzzle[i][j] != 0))
            {
                for (int goali = 0; goali < 3; goali++)
                {
                    for (int goalj = 0; goalj < 3; goalj++)
                    {
                        if ((puzzle[i][j] != goalState[i][j]) && (puzzle[i][j] != 0))
                        {
                            goaliVal = goali; 
                            goaljVal = goalj; 
                            break;
                        }
                    }
                }
                int xabs = abs(i - goaliVal); 
                int yabs = abs(j - goaljVal); 
                hn += xabs + yabs; 
            }

        }
    }
    return hn; 
}
//uniform cost search
void general_search(vector <vector <int> > puzzle, function <int(vector < vector <int > > &) > search)
{
    priority_queue<Node*, vector<Node*>, CompareNode > frontier;
    vector < vector < vector <int > > > visitedPuzzles; 

    Node *root = new Node(puzzle,nullptr,0,search(puzzle)); 
    frontier.push(root);

    int numOfExpandedNodes = 0; 
    int maxqueueSize = 0; 


    while(!frontier.empty())
    {
        maxqueueSize = max(maxqueueSize, (int)frontier.size()); 

        Node* currentPuzzle = frontier.top(); 
        frontier.pop();

        if (visited(currentPuzzle->puzzle,visitedPuzzles))
        {
            delete currentPuzzle;
            continue; 
        }

        if (goalStateReached(currentPuzzle->puzzle))
        {
            cout << "Goal State Reached! Puzzle is solved! :) " << endl; 
            cout << " Solution Depth:  " << currentPuzzle->cost_gn << endl;
            cout << " Number of Nodes Expanded: " << numOfExpandedNodes << endl; 
            cout << " Maximum Queue Size: " << maxqueueSize << endl; 
            return; 
        }

        cout << "Best state to expand with g(n) = " << currentPuzzle->cost_gn
        << " and h(n) = " << currentPuzzle->cost_hn << " is: " << endl; 

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                cout << currentPuzzle->puzzle[i][j] << " "; 
            }
            cout << endl; 
        }
        cout << endl;    
            visitedPuzzles.push_back(currentPuzzle->puzzle);
            numOfExpandedNodes++; 
            vector <Node*> children = expand(currentPuzzle); 
            for (int i = 0; i < children.size(); i++)
            {
                if(!visited(children[i]->puzzle, visitedPuzzles))
                {
                    children[i]->cost_hn = search(children[i]-> puzzle);
                    children[i]->cost_fn = children[i]->cost_gn + children[i]->cost_hn;
                    frontier.push(children[i]); 
                }

            }
        }
        
        cout << " No Solution " << endl; 
}




int main()
{

    cout << "Welcome to my 8 Puzzle Solver" << endl; 

    cout << "Would you like to use a Default Puzzle or Create your own Puzzle? Type \"1\" for Default or \"2\" for Create your own Puzzle" << endl;

    int puzzleChoice; 
    vector<vector<int> > puzzle;


    if(cin >> puzzleChoice)
    {
        if (puzzleChoice == 1)
        {
            cout << "Default Puzzle Selected " << endl;
            int puzzleNum; 

            cout << "Please select a puzzle to solve from depthLevel2 (1), depthLevel8 (2), depthLevel12 (3), depthLevel16 (4), depthLevel20 (5), depthLevel24 (6) " << endl; 

            if(cin >> puzzleNum)
            {
                if (puzzleNum == 1)
                {
                    cout << "DepthLevel2 Puzzle Selected " << endl;
                    puzzle = depthLevel2;  
                }
                else if (puzzleNum == 2)
                {
                    cout << "DepthLevel8 Selected " << endl; 
                    puzzle = depthLevel8; 
                }
                else if (puzzleNum == 3)
                {
                    cout << "DepthLevel12 Selected " << endl; 
                    puzzle = depthLevel12; 
                }
                else if (puzzleNum == 4)
                {
                    cout << "DepthLevel16 Puzzle Selected " << endl; 
                    puzzle = depthLevel16; 
                }
                else if (puzzleNum == 5)
                {
                    cout << "DepthLevel20 Puzzle Selected " << endl; 
                    puzzle = depthLevel20; 
                }
                else if (puzzleNum == 6)
                {
                    cout << "DepthLevel24 Puzzle Selected " << endl; 
                    puzzle = depthLevel24; 
                }
                else
                {
                    cout << "Invalid Puzzle Choice " << endl; 
                }
            }
        }
        else if (puzzleChoice == 2)
        {
            cout << "Create your own Puzzle " << endl;

            puzzle.resize(3,vector<int>(3));     
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cin >> puzzle[i][j];
                }
            }
        }
        else
        {
            cout << "Invalid Choice " << endl; 
        }
    }
    
    cout << "Choose your algorithm for solving the 8 puzzle" << endl; 
    cout << "Type \"1\" for Uniform Cost Search " << endl; 
    cout << "Type \"2\" for A* with Misplaced Tile Heuristic " << endl; 
    cout << "Type \"3\" for A* with Manhattan Tile Heuristic " << endl; 

    int algorithmNum; 

    if (cin >> algorithmNum)
    {
        if (algorithmNum == 1)
        {
            cout << "Solving the 8-Puzzle using Uniform Cost Search ..." << endl; 
            general_search(puzzle, uniformCostSearch);
        }
        else if (algorithmNum == 2)
        {
            cout << "Solving the 8-Puzzle using A* with Misplaced Tile Heuristic..." << endl;
            general_search(puzzle, misplacedTileHeuristic); 
        }
        else if (algorithmNum == 3)
        {
            cout << "Solving the 8-Puzzle using A* with Manhattan Tile Heuristic..." << endl; 
            general_search(puzzle, manhattanDistanceHeuristic);
        }
        else
        {
            cout << "Invalid Choice " << endl; 
        }
    }
    return 0;
}
