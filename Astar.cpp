/* Talha Çomak
 * 150160726 */

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

class Graph {
    int V;
    vector <vector <int> > adj;
    vector <int> prevList;
    vector <pair <int, int> > elements;
public:
    void optimize(int V){
        this->V = V;
        adj.resize(V);
        prevList.resize(V);
    }
    int give_row(int node_count){
        return elements[node_count].first;
    }
    int give_col(int node_count){
        return elements[node_count].second;
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
    int start_node;
    int end_node;
    int current_node;
public:
    int get_current(){
        return current_node;
    }
    int get_end(){
        return end_node;
    }
    int get_start(){
        return start_node;
    }
    void set_start(int s){
        start_node = s;
        current_node = s;
    }
    void set_end(int e){
        end_node = e;
    }
    void set_current(int n){
        current_node = n;
    }
};

struct moves{
    vector <agent> agents;
    vector <int> states;
    int counter;
    int total_h;
    int total_f;
};

class agentGraph {
    int V;
    vector <vector <int> > adj;
    vector <int> reverse;
    vector <int> toPrint;
    vector <int> prevList;
    vector <moves> elements;
public:
    agentGraph(int V){
        this->V = V;
        adj.resize(V);
        prevList.resize(V);
    }
    void optimize(int V){
        this->V = V;
        adj.resize(V);
        prevList.resize(V);
    }
    void addEdge(int v, int f){
        adj[v].push_back(f);
        reverse.push_back(v);
    }
    void add_node(moves m){
        elements.push_back(m);
    }
    void findPath(int dest){
        toPrint.push_back(dest);
        dest = prevList[dest];
        while(dest != -1){
            toPrint.push_back(dest);
            dest = prevList[dest];
        }
    }
    int pathLen(int dest){
        int len = 0;
        while (dest != 0){
            len++;
            dest = reverse[dest-1];
            if(dest == 0) break;
        }
        return len;
    }
    void printOut(Graph &gr, string fileName){
        ofstream out(fileName);
        int size = (int) toPrint.size()-1;
        out<<size<< " ";
        int size2 = (int)elements[toPrint[0]].agents.size();
        out<<size2<<endl;
        for(int i=size; i > 0; i--){
            for(int j=0; j<size2; j++){
                int s = elements[toPrint[i]].agents[j].get_current();
                int e = elements[toPrint[i-1]].agents[j].get_current();
                char move = gr.movement(s, e);
                out<< move;
            }
            out << endl;
        }
        toPrint.clear();
    }
    bool isAnySame(vector <agent> &agents){
        bool isSame = false;
        for(auto it = elements.begin(); it != elements.end(); it++){
            moves m = *it;
            int q;
            for(q = 0; q<(int)agents.size(); q++){
                if(m.agents[q].get_current() != agents[q].get_current()) {
                    break;
                }
            }
            if(q == (int)agents.size()) {
                isSame = true;
                break;
            }
        }
        return isSame;
    }
    void BFS(int source, int dest){
        int size = (int)elements.size();
        bool* discovered = new bool[size];
        for(int i = 0; i < size; i++)
            discovered[i] = false;
        discovered[source] = true;
        vector <int> L;
        L.push_back(source);

        prevList.clear();
        toPrint.clear();
        prevList.resize(V);
        prevList[source] = -1; // no previous node
        int i = 0;

        while((int)L.size() > i){
            source = L[i];

            for (int j = 0; j < (int)adj[source].size(); ++j){
                int x = adj[source][j];
                if (!discovered[x])
                {
                    discovered[x] = true;
                    L.push_back(x);
                    prevList[x] = source;
                    if (x == dest) {
                        return;
                    }
                }
            }
            i+=1;
        }
    }
};

int calculate_h(int current, int start, int end, Graph gr){
    int distance_e = abs(gr.give_row(current)-gr.give_row(end));
    distance_e += abs(gr.give_col(current) - gr.give_col(end));

    return distance_e;
}

void sort(deque <moves> &vec){
    bool sorted = false;
    int n = vec.size();
    for (int i = n; i > 1 && !sorted; i--) {
        sorted = true;
        for (int j = 1; j < i; j++) {
            if (vec[j].total_f < vec[j - 1].total_f) {
                swap(vec[j], vec[j - 1]);
                sorted = false;
            }
            else if (vec[j].total_f == vec[j - 1].total_f){
                if(vec[j].total_h < vec[j-1].total_h){
                    swap(vec[j], vec[j - 1]);
                    sorted = false;
                }
            }
        }
    }
}

int theNode;

void allPossibleActions(bool &founded, int &counter, int i, int pacman, Graph &gr, vector <agent> agents, vector <int> states, int parent_count,
                        agentGraph &agentsGraph, vector <int> stack, deque <moves> &li) {
    bool isSame = false;
    int current = agents[i].get_current();
    int first_loc = current;
    int cant_go = -1;
    if(states[current] != 2){
        if (i + 1 != pacman) {
            allPossibleActions(founded, counter, i + 1, pacman, gr, agents, states, parent_count, agentsGraph, stack, li);
            if(founded) return;
        }
        else {
            if(stack.empty()){
                isSame = agentsGraph.isAnySame(agents);
                if(!isSame){
                    moves* move = new moves;
                    move->states = states;
                    move->agents = agents;
                    move->counter = counter;
                    agentsGraph.add_node(*move);
                    agentsGraph.optimize(counter+1);
                    agentsGraph.addEdge(parent_count, counter);
                    int max_h = 0;
                    for(int p=0; p<pacman; p++){
                        int h = calculate_h(agents[p].get_current(), agents[p].get_start(), agents[p].get_end(), gr);
                        if(h > max_h) max_h = h;
                    }
                    int g = agentsGraph.pathLen(counter);
                    move->total_f = g + max_h;
                    move->total_h = max_h;
                    li.push_front(*move);
                    sort(li);
                    counter++;
                    //cout << g << " ";
                    //cout << "counter: " << counter << " ag1: " << agents[0].get_current() << " " << agents[1].get_current() << endl;

                    for(int k = 0; k<pacman; k++){
                        if(agents[k].get_current() != agents[k].get_end()) break;
                        if(k == pacman -1) founded = true;
                    }
                    if(founded) {
                        theNode = counter-1;
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
                allPossibleActions(founded, counter, i + 1, pacman, gr, agents, states, parent_count, agentsGraph, stack, li);
                if(founded) return;
            }
            else {
                if(stack.empty()){
                    isSame = agentsGraph.isAnySame(agents);
                    if(!isSame){
                        moves* move = new moves;
                        move->states = states;
                        move->agents = agents;
                        move->counter = counter;
                        agentsGraph.add_node(*move);
                        agentsGraph.optimize(counter+1);
                        agentsGraph.addEdge(parent_count, counter);
                        int max_h = 0;
                        for(int p=0; p<pacman; p++){
                            int h = calculate_h(agents[p].get_current(), agents[p].get_start(), agents[p].get_end(), gr);
                            if(h > max_h) max_h = h;
                        }
                        int g = agentsGraph.pathLen(counter);
                        move->total_f = g + max_h;
                        move->total_h = max_h;
                        li.push_front(*move);
                        sort(li);
                        counter++;
                        //cout << g << " ";
                        //cout << "counter: " << counter << " ag1: " << agents[0].get_current() << " " << agents[1].get_current() << endl;

                        for(int k = 0; k<pacman; k++){
                            if(agents[k].get_current() != agents[k].get_end()) break;
                            if(k == pacman -1) founded = true;
                        }
                        if(founded) {
                            theNode = counter-1;
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
                    move.agents[((int)str[1]) - 49].set_start(node_count); //Agent starts 1 but we get agents start from 0
                }
                else {
                    move.states.push_back(0);
                }
                if(str[0] == 'G') move.agents[((int)str[1]) - 49].set_end(node_count); //Goals starts 1 but we get goals start from 0
                node_count++;
            }
        }
    }
    gr.optimize(node_count);

    move.counter = 0;
    agentGraph agents(1);
    agents.optimize(1);
    agents.add_node(move);

    deque <moves> li;
    int max_h = 0;
    for(int p=0; p<pacman; p++){
        int h = calculate_h(move.agents[p].get_current(), move.agents[p].get_start(), move.agents[p].get_end(), gr);
        if(h > max_h) max_h = h;
    }
    int g = 0;
    move.total_f = g + max_h;
    move.total_h = max_h;
    li.push_front(move);

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

    int counter = 1;
    bool founded = false;
    int expanded_counter = 0;

    while(!li.empty()){
        moves m = li[0];
        li.pop_front();
        vector <int> stack;
        allPossibleActions(founded, counter, 0, pacman, gr, m.agents, m.states, m.counter, agents, stack, li);
        expanded_counter ++;
        //cout << "counter: " << counter << "\n";
        if(theNode != -1) break;
    }

    if(theNode != -1){
        agents.BFS(0, theNode);
        agents.findPath(theNode);
        agents.printOut(gr, out_name);
        cout  << "number of generated nodes: " << counter << endl << "number of expanded nodes: " << expanded_counter << endl;
    }

    return 0;

}
