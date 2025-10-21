#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class Graph{
        //Attributes for the number of vertices and a vector to keep track of adjacent vertices
        int vertices;
        vector<int> *adjacents;
public:
        //Constructor sets vertices variable and a verts number of adjacnets for each vertex
        Graph(int verts): vertices(verts){
                adjacents = new vector<int>[verts];
        }
        //For new edges, add to the vectors at the given vertices
        void addAdjacents(int vertex, int edge){
                adjacents[vertex].push_back(edge);
                adjacents[edge].push_back(vertex);
        }

        void greedyAlgo(int threads){
                //Initialize an array to store the colored vertices in order
                int coloredVertices[vertices];
                //Let Numbers = Colors, where the next available color is given to a vertex based on the least numbers
                //Colored Vertex should start at 0, the lowest available color
                coloredVertices[0] = 0;
                //Initialize an array to determine if each color is available and to set them to false if they are unavailable
                bool availColors[vertices];

                //Let -1 be an vertex with an unassigned color
                for(int i = 1; i < vertices; i++){
                        coloredVertices[i] = -1;

                }

                //OpenMP directive
                #pragma omp parallel for shared(coloredVertices) num_threads(threads)
                for(int i = 1; i < vertices; i++){
                        //cout << omp_get_thread_num() << endl;
                        bool localAvailColors[vertices];
                        fill(localAvailColors, localAvailColors+vertices, false);
                        //This loops through all the adjacent vertices to see if they have been colored yet, and if they have, then it sets their color value to true to indicate they are unavailable
                        //#pragma omp parallel for
                        for(auto& connected : adjacents[i]){
                                if(coloredVertices[connected] != -1)
                                {

                                        localAvailColors[coloredVertices[connected]] = true;

                                }

                        }
                        //cout << "here" << endl;
                        //Finds the next available color to use
                        int color = 0;
                        while(color < vertices && localAvailColors[color] == true){
                                color++;
                        }
                        //Sets new color
                        #pragma omp critical
                        {
                        coloredVertices[i] = color;
                        }
                        //Loops through the adjacent vertices to this new value back to false, resetted
                        //#pragma omp parallel for
                        for(auto& connected : adjacents[i]){
                                if(coloredVertices[connected] != -1){

                                        localAvailColors[coloredVertices[connected]] = false;

                                }
                        }

                }
                //Printing out vertex results
                //for(int vert = 0; vert < vertices; vert++){
                //        cout << "Vertex " << vert << " has color " << coloredVertices[vert] << endl;
                //}
        }

};
int main(){

        //This value modifies the number of vertices generated
        int vertices = 80000;

        Graph t(vertices);
        for(int i = 0; i < vertices-1; i++){
                //This is the test case for varying # of total vertices, just one adjacent vertex
                t.addAdjacents(i, i+1);

                //This is the test case for varying # of adjacent vertices
                //for(int j = 1; j <= 8000; j++){
                //      int neighbor = (i + j) % vertices;
                //      t.addAdjacents(i, neighbor);
                //}
        }
        int threads = 10;
        t.greedyAlgo(threads);


        return 0;
}
~     