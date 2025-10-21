This program answers Question 3 of HW2 for EECE5640.

This program uses a greedy approach to solve the graph coloring problem. 

To Run the Program:
g++ -fopenomp q3.cpp -o q3
time ./q3

Additional Information:
To examine the difference in runtime between number of vertices total and number of adjacent vertices, in int main() choose to uncomment either: 
 //This is the test case for varying # of total vertices, just one adjacent vertex
//t.addAdjacents(i, i+1);

//This is the test case for varying # of adjacent vertices
//for(int j = 1; j <= 8000; j++){
//      int neighbor = (i + j) % vertices;
//      t.addAdjacents(i, neighbor);
//}
        
The top is for one adjacent vertex per vertex, the bottom is for a variable number of adjacent vertices that you can set.

The number of threads is also settable in the program with:
int threads = 10;
t.greedyAlgo(threads);

As is the number of vertices:
int vertices = 80000;
