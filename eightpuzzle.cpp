#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>
#include <chrono>
using namespace std; 

//the state of the eight puzzle that is desired, the goal state 
vector <vector <int> > goalState = 
{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

struct Node
{
    vector <vector <int> > puzzle; // state of the puzzle (3x3 board)
    Node* parent;
    int cost_gn; // cost from starting node
    int cost_hn; // cost of the heuristic
    int cost_fn; // cost of the overall node  (cost_gn + cost_hn)

    //node constructor
    Node(vector <vector <int > > puzzle, Node*parent = nullptr, int gn = 0, int hn = 0)
    : puzzle(puzzle), parent(parent), cost_gn(gn), cost_hn(hn), cost_fn(gn+hn)
    {

    }
};
//check where the empty tile is in the eight puzzle
pair<int, int > zeroTile(vector <vector <int> > puzzle)
{

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (puzzle[i][j] == 0)
            {
                return make_pair(i,j); //iterates through each row and column to find the empty tile, when found makes a pair of the row and column where it is located
            }
        }
    }
    return make_pair(-1,-1); //if no empty tile is found, returns -1,-1 
}

//expands the puzzle into making children nodes, all possible moves left right up down, etc
vector <Node*> expand(Node *node)
{
    vector <vector <int> > puzzle = node->puzzle;  //finds the current state the puzzle is in 
    vector <Node*> children; //creates a vector of the children nodes
    pair <int, int> zeroTilePos = zeroTile(puzzle); //finds the position of the empty tile in the puzzle using the zeroTile function
    int x = zeroTilePos.first; //row position of the tile []
    int y = zeroTilePos.second; //column position of the tile

    //to move it up cannot move it up if it is in row 0, the top row 
    if (x > 0)
    {
        vector <vector <int > > child = puzzle; //creates a new copy of the puzzle with the changed position of the empty tile 
        swap(child[x][y], child[x-1][y]); //swaps the empty tile with the tile above it
        Node* childrenNode = new Node(child, node, node->cost_gn + 1); //creates a new node with the new state of the puzzle, the parent, and the cost of the move incremented by 1
        children.push_back(childrenNode); // pushes back the new Node into the children vector I have allocated to keep track of the moves 
    }
    if (x < 2) // moving it down, cant move it down if it is already in the bottom row 
    {
        vector <vector <int > > child = puzzle; //creates a new copy of the puzzle with the changed position of the empty tile
        swap(child[x][y], child[x+1][y]); //swaps the empty tile with the tile below it
        Node* childrenNode = new Node(child, node, node->cost_gn + 1);  //creates a new node with the new state of the puzzle, the parent, and the cost of the move incremented by 1
        children.push_back(childrenNode); // pushes back the new Node into the children vector I have allocated to keep track of the moves 
    }
    if(y > 0) // moving it left, cant move it left if its in the left most column  
    {
        vector <vector <int > > child = puzzle; //creates a new copy of the puzzle with the changed position of the empty tile
        swap(child[x][y], child[x][y-1]); //swaps the empty tile with the tile to the left of it
        Node* childrenNode = new Node(child, node, node->cost_gn + 1); //creates a new node with the new state of the puzzle, the parent, and the cost of the move incremented by 1
        children.push_back(childrenNode); // pushes back the new Node into the children vector I have allocated to keep track of the moves 
    }
    if(y < 2) // moving it right, cant move it right if its in the right most column 
    {
        vector <vector <int > > child = puzzle; //creates a new copy of the puzzle with the changed position of the empty tile
        swap(child[x][y], child[x][y+1]); //swaps the empty tile with the tile to the right of it
        Node* childrenNode = new Node(child, node, node->cost_gn + 1); //creates a new node with the new state of the puzzle, the parent, and the cost of the move incremented by 1
        children.push_back(childrenNode); // pushes back the new Node into the children vector I have allocated to keep track of the moves 
    }
    return children; //returns the vector of expanded nodes, the children
}
//struct to compare the cost of the nodes in the priority queue, to ensure the lowest cost node is at the top of the queue, making it a min-heap
struct comparingNode{
    bool operator() (Node *compare1, Node *compare2)
    {
        return compare1->cost_fn > compare2->cost_fn; //returns true if the cost of the first node is greater than that of the second node  
    }
}; 
//function to check if puzzle reached goal state
bool goalStateReached(vector <vector<int> > puzzle)
{
    return puzzle == goalState; //returns true if the puzzle is in the goal state
}
//function to check whether puzzle has been visited or not, passes in the specific puzzle and the vector of visited puzzles 
bool visited (vector <vector <int > > &puzzle, vector <vector <vector <int > > >&visitedPuzzles)
{
    for (int i = 0; i < visitedPuzzles.size(); i++)
    {
        if (puzzle == visitedPuzzles[i]) //if the puzzle is in the visited puzzles vector returns true meaning it has been visited 
        {
            return true; 
        }
    }
    return false; 
}

//general_search algorithm to solve the eight puzzle, passes in the puzzle state, and the function that will called to calculate the heuristic value h(n)
void general_search(vector <vector <int> > puzzle, function <int(vector < vector <int > > &) > algorithmSearch)
{
    auto startTimer = chrono::high_resolution_clock::now(); //starts the timer to calculate the time it takes to solve the algorithm  
    priority_queue<Node*, vector<Node*>, comparingNode > frontier; //priority queue, min-heap, stores the nodes in order of cost 
    vector < vector < vector <int > > > visitedPuzzles;  //vector of visited puzzles 

    Node *root = new Node(puzzle,nullptr,0,algorithmSearch(puzzle)); //creates the root node with the initial state of the puzzle, the parent being nullptr, the cost of the move g(n), and the heuristic value h(n)
    frontier.push(root); //pushes the root to the priotity queue

    int numOfExpandedNodes = 0; //counter keeping track of number of nodes expanded 
    int maxqueueSize = 0; //  keep track of max size of the queue  


    while(!frontier.empty()) //while the queue is not empty
    {
        maxqueueSize = max(maxqueueSize, (int)frontier.size());  //updates the max queue size to be the max between the current max queue size and the size of the queue

        Node* currentPuzzle = frontier.top(); //top of the node in the queue is acquired 
        frontier.pop(); //pops the top node from the queue

        if (visited(currentPuzzle->puzzle,visitedPuzzles)) //if the puzzle has already been visited them it will be deleted from the queue, and the loop will continue
        {
            delete currentPuzzle;
            continue; 
        }

        if (goalStateReached(currentPuzzle->puzzle)) //if the goal state is reached, the algorithm solved the puzzle 
        {
            auto endTimer = chrono::high_resolution_clock::now();
            chrono::duration<double> timeTaken = endTimer - startTimer;  //calculates the total time taken to solve the puzzle 
            cout << "Goal State Reached! Puzzle is solved! : " << endl; 
            cout << " Solution Depth:  " << currentPuzzle->cost_gn << endl; //prints the depth of the solution
            cout << " Number of Nodes Expanded: " << numOfExpandedNodes << endl;  //prints the number of expanded nodes 
            cout << " Maximum Queue Size: " << maxqueueSize << endl; //prints the max queue size 

            cout << "Time Taken: " << timeTaken.count() << " seconds" << endl;  //prints the time taken to solve the puzzle 

            return;  //exits the function
        }

        cout << "Best state to expand with g(n) = " << currentPuzzle->cost_gn // prints the best state to expand with the cost of the move and the heuristic value
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
        visitedPuzzles.push_back(currentPuzzle->puzzle); //pushes the current puzzle into the visited puzzles vector 
        numOfExpandedNodes++; //increments the number of expanded nodes 
        vector <Node*> children = expand(currentPuzzle); //expands the current puzzle into children nodes
        for (int i = 0; i < children.size(); i++)
        {
            if(!visited(children[i]->puzzle, visitedPuzzles)) //if the children nodes haven't been visited yet
            {
                children[i]->cost_hn = algorithmSearch(children[i]-> puzzle); //calculates the heuristic value of the children nodes
                children[i]->cost_fn = children[i]->cost_gn + children[i]->cost_hn; //calculates the cost of the children nodes
                frontier.push(children[i]);  //pushes the children nodes into the queue
            }
        }
    }
    cout << "No Solution " << endl;  //if the queue is empty and no solution is found, print out no solution
}

int uniformCostSearch(vector <vector <int > > &puzzle) //uniform cost search function, returns 0 as the heuristic value is 0 h(n) = 0
{
    return 0; 
}

int misplacedTileHeuristic(vector <vector <int > > &puzzle) // misplaced tile heuristic, calculates the number of misplaced tiles in the puzzle which represents the heuristic cost h(n)
{
    int hn = 0; 

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if((puzzle[i][j] != goalState[i][j]) && (puzzle[i][j] != 0)) //if it doesn't match the goal state and it  is not an empty tile then it is a misplaced tile 
            {
                hn++; 
            }
        }
    }
    return hn; 
}
int manhattanDistanceHeuristic(vector <vector <int > > &puzzle) //manhattan distance heuristic, calculates the manhattan distance of the tiles from their goal position, the distance = h(n) the cost of the heuristic 
{
    int hn = 0; 
    int goaliVal = 0; 
    int goaljVal = 0; 
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if((puzzle[i][j] != goalState[i][j]) && (puzzle[i][j] != 0)) //if it doesn't match the goal state and it is not an empty tile then it is a misplaced tile
            {
                for (int goali = 0; goali < 3; goali++)
                {
                    for (int goalj = 0; goalj < 3; goalj++)
                    {
                        if ((puzzle[i][j] != goalState[i][j]) && (puzzle[i][j] != 0)) 
                        {
                            goaliVal = goali; //finds the goal position of the tile 
                            goaljVal = goalj; //finds the goal position of the tile
                            break;
                        }
                    }
                }
                int xabs = abs(i - goaliVal); //calculates the absolute value of the difference between the current position and the goal position in the rows
                int yabs = abs(j - goaljVal); //calculate the absolute value of the difference between the current position and the goal position in the columms
                hn += xabs + yabs; //updates the manhattan distance of the tile calculating the sum of the absolute value of the differences in the rows and columns between the current State position and the goal state position
            }

        }
    }
    return hn; //returns the manhatta distance of the puzzle 
}



//hardcoded puzzles to test the algorithm 
vector <vector <int> > depthLevel2 = 
{
    {1, 2, 3},
    {4, 5, 6},
    {0, 7, 8}
};
vector <vector <int> > depthLevel4 = 
{
    {1, 2, 3},
    {5, 0, 6},
    {4, 7, 8}
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

// main function to run the eight puzzle solver 
int main()
{
    cout << endl; 
    cout << "Welcome to my 8 Puzzle Solver" << endl; 
    cout << endl; 

    cout << "Would you like to use a Default Puzzle or Create your own Puzzle? Type \"1\" for Default or \"2\" for Create your own Puzzle" << endl;

    int puzzleChoice; //variable to store the user choice 
    vector<vector<int> > puzzle; //vector to store the puzzle


    if(cin >> puzzleChoice) //gets user input in which puzzle they would like to choose (default of create their own)
    {
        if (puzzleChoice == 1) //if user chooses 1 the default puzzle 
        {
            cout << "Default Puzzle Selected " << endl;
            int puzzleNum; 
            cout << endl; 

            cout << "Please select a puzzle (#) to solve from: depthLevel2 (1), depthLevel4 (2), depthLevel8 (3), depthLevel12 (4), depthLevel16 (5), depthLevel20 (6), depthLevel24 (7) " << endl; 

            if(cin >> puzzleNum)
            {
                if (puzzleNum == 1) //if user chooses 1 the depthLevel2 puzzle
                {
                    cout << "DepthLevel2 Puzzle Selected " << endl;
                    puzzle = depthLevel2;  
                }
                else if (puzzleNum == 2) //if user chooses 2 the depthLevel4 puzzle
                {
                    cout << "DepthLevel4 Selected " << endl; 
                    puzzle = depthLevel4; 
                }
                else if (puzzleNum == 3)
                {
                    cout << "DepthLevel8 Selected " << endl; //if user chooses 3 the depthLevel8 puzzle
                    puzzle = depthLevel8; 
                }
                else if (puzzleNum == 4) //if user chooses 4 the depthLevel12 puzzle
                {
                    cout << "DepthLevel12 Selected " << endl; 
                    puzzle = depthLevel12; 
                }
                else if (puzzleNum == 5) //if user chooses 5 the depthLevel16 puzzle
                {
                    cout << "DepthLevel16 Puzzle Selected " << endl; 
                    puzzle = depthLevel16;
                }
                else if (puzzleNum == 6) //if user chooses 6 the depthLevel20 puzzle
                {
                    cout << "DepthLevel20 Puzzle Selected " << endl;  
                    puzzle = depthLevel20; 
                }
                else if (puzzleNum == 7)//if user chooses 7 the depthLevel24 puzzle
                {
                    cout << "DepthLevel24 Puzzle Selected " << endl; 
                    puzzle = depthLevel24;
                }
                else
                {
                    cout << "Invalid Puzzle Choice " << endl; //if none choses an invalid choice is made 
                }
            }
        } 
        else if (puzzleChoice == 2) //if the users chooses 2 they create their own puzzle 
        {
            cout << "Create your own Puzzle " << endl;

            puzzle.resize(3,vector<int>(3)); //resizes puzzle to be a 3x3 board      
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cin >> puzzle[i][j]; //user inputs the values of the puzzle 
                }
            }
        }
        else
        {
            cout << "Invalid Choice " << endl; 
        }
    }
    cout << endl; 
    cout << "Choose your algorithm # for solving the 8 puzzle" << endl; //asks the user to choose the algorithm they would like to use to solve the eight puzzle  
    cout << "Type \"1.\" for Uniform Cost Search " << endl; 
    cout << "Type \"2.\" for A* with Misplaced Tile Heuristic " << endl; 
    cout << "Type \"3.\" for A* with Manhattan Distance Heuristic " << endl; 

    int algorithmNum; 

    if (cin >> algorithmNum)
    {
        cout << endl; 
        if (algorithmNum == 1) //if user input is 1, the uniform cost search is used to solve the puzzle
        {
            cout << "Solving the 8-Puzzle using Uniform Cost Search ..." << endl; 
            general_search(puzzle, uniformCostSearch);
        }
        else if (algorithmNum == 2)//if user input is 2, the misplace tile heuristic  is used to solve the puzzle
        {
            cout << "Solving the 8-Puzzle using A* with Misplaced Tile Heuristic..." << endl;
            general_search(puzzle, misplacedTileHeuristic); 
        }
        else if (algorithmNum == 3) //if user input is 3, the manhattan distance heuristic  is used to solve the puzzle
        {
            cout << "Solving the 8-Puzzle using A* with Manhattan Distance Heuristic..." << endl; 
            general_search(puzzle, manhattanDistanceHeuristic);
        }
        else
        {
            cout << "Invalid Choice " << endl; 
        }
    }
    return 0;
}
