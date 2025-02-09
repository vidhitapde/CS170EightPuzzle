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
vector <vector <int> > puzzle; 
vector <vector <int> > goalState = 
{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

vector <vector <int> > trivial = 
{
    {1, 2, 3},
    {4, 0, 6},
    {7, 5, 8}
};

vector <vector <int> > veryEasy = 
{
    {1, 3, 6},
    {5, 0, 7},
    {4, 8, 2}
};

vector <vector <int> > easy = 
{
    {1, 2, 3},
    {5, 0, 6},
    {4, 7, 8}
};

vector <vector <int> > doable = 
{
    {1, 6, 7},
    {5, 0, 3},
    {4, 8, 2}
};

vector <vector <int> > oh_boy = 
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
//uniform cost search
void general_search(vector <vector <int> > puzzle, function <int(vector < vector <int > > &) > search)
{
    priority_queue<Node*, vector<Node*>, CompareNode > frontier;
    vector < vector < vector <int > > > visitedPuzzles; 

    Node *root = new Node(puzzle,nullptr,0,search(puzzle)); 
    frontier.push(root);

    int numOfExpandedNodes = 0; 


    while(!frontier.empty())
    {

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

            while(!frontier.empty())
            {
                delete frontier.top();
                frontier.pop();
            } 
            delete currentPuzzle;
            
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
            for (Node* child : children)
            {
                if(!visited(child->puzzle, visitedPuzzles))
                {
                    child->cost_hn = search(child-> puzzle);
                    child->cost_fn = child->cost_gn + child->cost_hn;
                    frontier.push(child); 
                }
                else{
                    delete child; 
                }

            }
            delete currentPuzzle; 
        }
    cout << " No Solution " << endl; 
    while(!frontier.empty())
    {
        delete frontier.top();
        frontier.pop(); 
    }


}




int main()
{

    cout << "Welcome to my 8 Puzzle Solver" << endl; 

    cout << "Would you like to use a Default Puzzle or Create your own Puzzle? Type \"1\" for Default or \"2\" for Create your own Puzzle" << endl;

    int puzzleChoice; 

    if(cin >> puzzleChoice)
    {
        if (puzzleChoice == 1)
        {
            cout << "Default Puzzle Selected " << endl;
            int puzzleNum; 

            cout << "Please select a puzzle to solve from trivial (1), veryEasy (2), easy (3), doable (4), oh_boy (5) " << endl; 

            if(cin >> puzzleNum)
            {
                if (puzzleNum == 1)
                {
                    cout << "Trivial Puzzle Selected " << endl; 
                    puzzle = trivial; 
                }
                else if (puzzleNum == 2)
                {
                    cout << "Very Easy Puzzle Selected " << endl; 
                    puzzle = veryEasy; 
                }
                else if (puzzleNum == 3)
                {
                    cout << "Easy Puzzle Selected " << endl; 
                    puzzle = easy; 
                }
                else if (puzzleNum == 4)
                {
                    cout << "Do Able Puzzle Selected " << endl; 
                    puzzle = doable; 
                }
                else if (puzzleNum == 5)
                {
                    cout << "Oh Boy Puzzle Selected " << endl; 
                    puzzle = oh_boy; 
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
            int size;
            vector <vector <int> > puzzle(size, vector <int> (size)); 
            cout << "Enter the size of your puzzle"  << endl; 
            cin >> size; 
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
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
        }
        else
        {
            cout << "Invalid Choice " << endl; 
        }
    }

    cout << "Find the empty Tile " << endl;
    cout << "Empty Tile is at: " << zeroTile(puzzle).first << " " << zeroTile(puzzle).second << endl;
    return 0;
}








