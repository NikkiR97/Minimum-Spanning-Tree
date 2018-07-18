#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits.h>

#define V 128

using namespace std;

int city_inc=-1;
int state_inc=-1;
int column_inc=0;

string city_names[128];
string city_states[128];
int city_map [128][128]; //2d array to store the different cities // need to convert to integers

//need adjacency matrix to store distances of cities after it the current city

void initialize_all(){
    for(int k=0; k<128; k++){
        city_names[k]= "";
        city_states[k]= "";
    }
    for(int j=0; j<128; j++){
        for(int k=0; k<128; k++){
            city_map[j][k] = 0;
        }
    }
}

void mirror_elements(){
     for(int i=0; i<128; i++){
         for(int j=0; j<128; j++){
             city_map[i][j] = city_map[j][i];
         }
     }
}

void store(string line){
     //reads from the line that was read
    //one array to store the cities (index zero is first city and moves forward)
    //2D array to store the distances from each of the cities column 0 is for city one and so forth
        //each row is for each city as well

    string name = "";
    string state = "";
    string dist = "";

    int i;
    if(line[0] >= 65 && line[0] <= 90) { //detected a city name
        for (i = 0; i<line.length(); i++){
             if(line[i] == ','){
                 break;
             }
             name += line[i]; //append the string
        }
        city_inc++;
        city_names[city_inc] = name;
        i+=2; //skip comma and space
        for (i; i<line.length(); i++){
            if(line[i] == '['){
                break;
            }
            state += line[i];
        }
        state_inc++;
        city_states[state_inc] = state;

        name = "";
        state = "";
        column_inc=0;// reset to start at first column
    }

    else if(line[0] >= 48 && line[0] <= 57){ //detected a number
        int j=0;

        while(j!=line.length()) {
            for (j; j < line.length(); j++) {
                if (line[j] == ' ') {
                    j++; //skip the space
                    break;
                }
                dist += line[j];
            }
            city_map[city_inc][column_inc] = stoi(dist); //convert to integer and submit into array
            column_inc++;
            dist = "";
        }
    }

}

int min_key(bool visited[], int min_keys[]){
    int min = INT_MAX;
    int min_idx;

    for (int m = 0; m < V; m++){
        if (!visited[m] && min_keys[m] < min) { //if the vertex is not visited and the key is less than the largest number
            min = min_keys[m]; //make that key the new minimum
            min_idx = m; //change the index to have vertex with this new minimum weight
        }
    }

    return min_idx;
}

void min_span_tree(int map[V][V])
{
    //1st) order all the minimum weight edges
    //2nd) have array to store vertices that have been added to the MST (each index refers to an element)
    //3rd) have array to store the MST (indecies of the the vertices)
    //4th) have array to store min edge weight of a vertex
    //5th) choose a vertex with the most minimum key value
    //6th) update the weights for the adjacent vertecies
    //7th) pick the vertex with the smallest key amongst list of adjacent vertecies to visited nodes
    //8th) update the weights for the adjacent vertecies if the new connecting edge is smaller
    //9th) repeat

    int mst[V]; //minimum spanning tree, contents reference the vertex that it is connected to
    int min_weight[V]; //boolean array to store the visited list
    bool added[V]; //boolean array to store the visited list
    int u=0;
    int min=INT_MAX;
    int min_idx;

    for(int i=0; i<V; i++){
        added[i] = false; //initialize
        min_weight[i] = INT_MAX; //initialize
    }

    int mymin = INT_MAX;
    int idx;
    for(int i=2; i<128; i++) {
        if (map[0][i] < mymin) {
            mymin = map[0][i];
            idx = i;
        }
    }

    min_weight[0] = mymin;
    mst[0] = idx;

    for(int i=0; i<V-1; i++){//iterate through all the different vertices by finding min-key V-1 times

        //makes sure only searches from list of non-visited and adjacent vertecies which had min-weight adjusted
        u = min_key(added, min_weight);
//        for (int m = 0; m < V; m++) {
//            if (!added[m] &&
//                min_weight[m] < min) { //if the vertex is not visited and the key is less than the largest number
//                min = min_weight[m]; //make that key the new minimum
//                min_idx = m; //change the index to have vertex with this new minimum weight
//
//            }
//        }
//        u=min_idx;

        added[u] = true; //set newly acquired vertex as visited

        for(int k=0; k<V; k++){ //make comparison with all other vertices
             if(!added[k] && (map[u][k] < min_weight[k])){ //if has not been added to mst and edge connecting vertex u and k is smaller
                 min_weight[k] = map[u][k]; //update the key value (most min weight) of that vertex (k)
                 mst[k] = u; //add this vertex to MST
             }
        }
    }


    int val=0;
    cout << "                   Edge                                                  Weight" << endl;
    for (int i = 0; i < V; i++) {
        cout << setw(15) << city_names[i] << ", " << city_states[i] << " - " << setw(15) << city_names[mst[i]] << ", " << city_states[mst[i]] << "               " << setw(20) << map[i][mst[i]] << endl;
        val+=map[i][mst[i]];
    }

    cout << endl;
    cout << "The TOTAL WEIGHT of all the edges put together are: " << val << endl;

}

int main() {
    initialize_all();

    ifstream ifs_instream;
    //ofstream ifs_outstream;

    //ifs_instream.open("/Users/Nikkitha/Documents/COMPUTER ENGR/CMPE 130/Homework 4/miles.txt");
    ifs_instream.open("miles.txt", ios::app);

    if(ifs_instream.fail())
    {
        cout << "Input file opening failed." << endl;
        exit(1);
    }
    else
    {
       // cout << "File opening didn't fail, you're good to go." << endl;
    }

    string line;

    while(!ifs_instream.eof()) {//;{ //or use ifs_instream.good() or !ifs_instream.eof
        getline(ifs_instream,line);
        if (line[0] == '*') {
            //cout << "Comment line has been skipped in reading from file. " << endl;
        }
        else if (line.length() == 0) {
            //cout << "Skipping an empty line. " << endl;
       }
        else {
            store(line);
        }
        //cout << line << endl;
    }

    ifs_instream.close();

    mirror_elements();


//    for(int i=0; i<128; i++){
//        //if(city_names[i] != '\n')
//        cout << city_names[i] << endl;
//    }
//    cout << endl;
//
//    for(int i=0; i<128; i++){
//        cout << city_states[i] << endl;
//    }
//    cout << endl;
//
//    for(int i=0; i<128; i++){
//        for(int j=0; j<128; j++){
//              cout << city_map [i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;

    min_span_tree(city_map);

    return 0;
}