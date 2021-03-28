/* Talha Çomak
 * 150160726 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Graph {
    int V;
    vector <vector <int> > adj;
    vector <pair <int, int> > elements;
public:
    void optimize(int V){
        this->V = V;
        adj.resize(V);
    }
    vector <int> &allPaths(int node){
        return adj[node];
    }
    void addEdge(int v, int f){
        adj[v].push_back(f);
    }
    void add_node(int i, int j){
        elements.push_back(make_pair(i, j));
    }
    int findNodeNum(int i, int j){
        for(int e=0; i<V; e++){
            if(elements[e].first == i && elements[e].second == j) return e;
        }
        return -1; // not found
    }
    char movement(int s, int e){
        if(elements[e].first > elements[s].first) return 'D';

        else if(elements[e].first == elements[s].first){
            if(elements[e].second > elements[s].second) return 'R';
            else if(elements[e].second == elements[s].second) return 'P';
            else return 'L';
        }
        else return 'U';

    }
};


class agent {
    int end_node;
    int current_node;
    vector <int> path;
public:
    int get_current(){
        return current_node;
    }
    int get_end(){
        return end_node;
    }
    vector <int> &getPath(){
        return path;
    }
    void set_end(int e){
        end_node = e;
    }
    void set_current(int n){
        current_node = n;
    }
    void set_path(){
        path.push_back(current_node);
    }
    int get_path_len(){
        return (int) path.size();
    }
};

struct moves{
    vector <agent> agents;
    vector <int> states;
    bool visited;
    void paths(){
        int size = (int)agents.size();
        for(int i=0; i<size; i++){
            agents[i].set_path();
        }
        visited = false;
    }
    void printOut(Graph &gr, string fileName){
        ofstream out(fileName);
        int size = agents[0].get_path_len();
        out<<size- 1<< " ";
        int size2 = agents.size();
        out<<size2<<endl;
        for(int i=0; i < size-1; i++){
            for(int j=0; j<size2; j++){
                int s = agents[j].getPath()[i];
                int e = agents[j].getPath()[i+1];
                char move = gr.movement(s, e);
                out<< move;
            }
            out << endl;
        }
    }
};

int theNode;

void allPossibleActions(bool &founded, int &counter, int i, int pacman, Graph &gr, vector <agent> agents,
                        vector <int> states, vector <moves> *levelAgents, int current_level, vector <int> stack) {
    bool isSame = false;
    int current = agents[i].get_current();
    int first_loc = current;
    int cant_go = -1;
    if(states[current] != 2){
        if (i + 1 != pacman) {
            allPossibleActions(founded, counter, i + 1, pacman, gr, agents, states, levelAgents, current_level,stack);
            if(founded) return;
        }
        else {
            if(stack.empty()){
                for(int level=0; level<=current_level; level++){
                    int p;
                    int levelSize = (int) levelAgents[level].size();
                    for(p=0; p < levelSize; p++){
                        moves m = levelAgents[level][p];
                        int q;
                        for(q = 0; q<pacman; q++){
                            if(m.agents[q].get_current() != agents[q].get_current()) {
                                break;
                            }
                        }
                        if(q == pacman) {
                            isSame = true;
                            break;
                        }
                    }
                    if(isSame) break;
                }
                if(!isSame){
                    moves* move = new moves;
                    move->states = states;
                    move->agents = agents;
                    move->paths();
                    levelAgents[current_level].push_back(*move);
                    counter++;

                    //cout << "counter: " << counter << " ag1: " << agents[0].get_current() << " " << agents[1].get_current() << " " << agents[2].get_current() << endl;

                    for(int k = 0; k<pacman; k++){
                        if(agents[k].get_current() != agents[k].get_end()) break;
                        if(k == pacman -1) founded = true;
                    }
                    if(founded) {
                        theNode = (int) levelAgents[current_level].size()-1;
                        return;
                    }
                }
            }

        }
    }
    else{
        for(int k = 0; k<(int)stack.size(); k+=2){
            if(stack[k] == current) {
                cant_go = stack[k+1];
                stack.erase(stack.begin()+k, stack.begin()+k+2);
                break;
            }
        }
    }

    vector<int> actions = gr.allPaths(current);
    for (int j = 0; j < (int) actions.size(); j++) {
        isSame = false;
        bool pushed = false;
        current = agents[i].get_current();
        int next = actions[j];
        if (states[next] != 2 && next != cant_go) {
            agents[i].set_current(next);
            states[current]--;
            states[next]++;
            if(states[next] == 2) {
                stack.push_back(next);
                stack.push_back(first_loc);
                pushed = true;
            }
            if (i + 1 != pacman) {
                allPossibleActions(founded, counter, i + 1, pacman, gr, agents, states, levelAgents, current_level, stack);
                if(founded) return;
            }
            else {
                if(stack.empty()){
                    for(int level=0; level<=current_level; level++){
                        int p;
                        int levelSize = (int) levelAgents[level].size();
                        for(p=0; p < levelSize; p++){
                            moves m = levelAgents[level][p];
                            int q;
                            for(q = 0; q<pacman; q++){
                                if(m.agents[q].get_current() != agents[q].get_current()) {
                                    break;
                                }
                            }
                            if(q == pacman) {
                                isSame = true;
                                break;
                            }
                        }
                        if(isSame) break;
                    }
                    if(!isSame){
                        moves* move = new moves;
                        move->states = states;
                        move->agents = agents;
                        move->paths();
                        levelAgents[current_level].push_back(*move);
                        counter++;

                        //cout << "counter: " << counter << " ag1: " << agents[0].get_current() << " " << agents[1].get_current() << " " << agents[2].get_current() << endl;

                        for(int k = 0; k<pacman; k++){
                            if(agents[k].get_current() != agents[k].get_end()) break;
                            if(k == pacman -1) founded = true;
                        }
                        if(founded) {
                            theNode = (int) levelAgents[current_level].size()-1;
                            return;
                        }
                    }
                }
            }
            agents[i].set_current(first_loc);
            states[current]++;
            states[next]--;
            if(pushed) {
                stack.erase(stack.end()-1);
                stack.erase(stack.end()-1);
            }
        }
    }
}

int main(int argv, char* argc[]){
    if (argc[1] == NULL) {
        cout << "No parameter!" << endl;
        return 1;
    }

    string out_name;
    ifstream file(argc[1]);
    if(argc[2]) out_name = argc[2];
    else out_name = "output.txt";

    int row, col, pacman;
    file >> row;
    file >> col;
    file >> pacman;

    vector <vector <string> > grid(row);
    moves move;
    move.agents.resize(pacman);
    string str;
    int node_count = 0;
    Graph gr;

    for(int i=0; i<row; i++){   // reading file
        for(int j=0; j<col; j++){
            file >> str;
            grid[i].push_back(str);
            if(str != "W"){
                gr.add_node(i, j);
                if(str[0] == 'A') {
                    move.states.push_back(1);
                    move.agents[((int)str[1]) - 49].set_current(node_count); //Agent starts 1 but we get agents start from 0
                }
                else {
                    move.states.push_back(0);
                }
                if(str[0] == 'G') move.agents[((int)str[1]) - 49].set_end(node_count); //Goals starts 1 but we get goals start from 0
                node_count++;
            }
        }
    }
    vector <moves> *levelAgents;
    levelAgents = new vector <moves> [50]; // max 50 hamlede biter
    move.paths();
    levelAgents[0].push_back(move);

    gr.optimize(node_count);
    for(int i=0; i<row; i++){ // creating graph
        for(int j=0; j<col; j++){
            if(grid[i][j] != "W"){
                int theNodeNum = gr.findNodeNum(i, j);
                if(i > 1) { // up
                    if(grid[i-1][j] != "W"){
                        int destination = gr.findNodeNum(i-1, j);
                        if(destination > -1){
                            gr.addEdge(theNodeNum, destination);
                        }
                    }
                }
                if(j > 1) { // left
                    if(grid[i][j-1] != "W"){
                        int destination = gr.findNodeNum(i, j-1);
                        if(destination > -1){
                            gr.addEdge(theNodeNum, destination);
                        }
                    }
                }
                if(i < row-2) { // down
                    if(grid[i+1][j] != "W"){
                        int destination = gr.findNodeNum(i+1, j);
                        if(destination > -1){
                            gr.addEdge(theNodeNum, destination);
                        }
                    }
                }
                if(j < col-2) { // right
                    if(grid[i][j+1] != "W"){
                        int destination = gr.findNodeNum(i, j+1);
                        if(destination > -1){
                            gr.addEdge(theNodeNum, destination);
                        }
                    }
                }
            }
        }
    }

    theNode = -1;

    int current_level = 1;
    int last_count = 0;
    int counter = 1;
    bool founded = false;
    int k, expanded_counter = 0;

    bool dfsOrBfs = false;
    if(argv == 3) {
        if(argc[3][0] == 'b' || argc[3][0] == 'B') dfsOrBfs = true;
    }

    if(!dfsOrBfs){
        while(true){
            int size = (int)levelAgents[current_level-1].size();
            moves m;
            for(k=0; k<size; k++){
                if(!levelAgents[current_level-1][k].visited) {
                    levelAgents[current_level-1][k].visited = true;
                    m = levelAgents[current_level-1][k];
                    break;
                }
            }
            if(k == size) {
                current_level--;
                continue;
            }
            vector <int> stack;
            allPossibleActions(founded, counter, 0, pacman, gr, m.agents, m.states, levelAgents, current_level, stack);
            expanded_counter += k;
            //cout <<" current level: " << current_level<< " counter: " << counter << endl;
            //if(current_level == 2) break; // for debug
            if(theNode != -1){
                expanded_counter++;
                break;
            }
            current_level++;
        }
        if(theNode != -1){
            levelAgents[current_level][theNode].printOut(gr, out_name);
            cout  << "number of generated nodes: " << counter << endl << "number of expanded nodes: " << expanded_counter << endl;
        }
    }
    else{
        for(int i=1; i<=50; i++){
            for(k = 0; k < (int)levelAgents[current_level-1].size(); k++) {
                moves m = levelAgents[current_level-1][k];
                vector <int> stack;
                allPossibleActions(founded, counter, 0, pacman, gr, m.agents, m.states, levelAgents, current_level, stack);
                if(theNode != -1) break;
                last_count++;
            }
            expanded_counter += k;
            //cout <<" current level: " << current_level<< " counter: " << counter << endl;
            //if(current_level == 2) break; // for debug
            if(theNode != -1) {
                expanded_counter++;
                break;
            }
            current_level++;
        }
        if(theNode != -1){
            levelAgents[current_level][theNode].printOut(gr, out_name);
            cout  << "number of generated nodes: " << counter << endl << "number of expanded nodes: " << expanded_counter << endl;
        }
    }

    return 0;

}
