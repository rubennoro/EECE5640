
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {
    
    //Initializing MPI and values of rank and size, as well as process name for printing distinction
    MPI_Init(&argc, &argv);
    int rank, size;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &name_len);

    //bool max = false;
    //int value = 0;
    
    //Struct initialized with a value and a bool to indicate if maximum 64 had been reached
    struct{
            int value;
            bool max;
    } data;

    //First process
    if (rank == 0) {
        data.value = 1;
        data.max = false;
        std::cout << "Process " << rank << " on node " << hostname << " w/ val " << data.value << std::endl;
        MPI_Send(&data, sizeof(data), MPI_BYTE, (rank + 1)%size, 0, MPI_COMM_WORLD);
    }
    //Run a loop that continues until 0 is reached 
    while(true){
            //MPI process receives the data struct 
            MPI_Recv(&data, sizeof(data), MPI_BYTE, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            //Regular incrementation
            if(data.value < 64 && !data.max){
                    data.value++;
                    //Once top value is reached, set max to true for all next structs
                    if(data.value >= 64){
                            data.max = true;
                    }
            }

            //Decrement the value
            else if(data.max && data.value != -1){
                    data.value -= 2;
            }
            //Print process values as long as they aren't below 0
            if(data.value != -1){
                    std::cout << "Process " << rank << " on node " << hostname << "w/ val " << data.value << std::endl;
            }

            //For negative data, send -1 to the other nodes
            //Shouldn't run unless the bottom conditional is removed
            if(data.value <= 0){
                    data.value = -1;
            }
            MPI_Send(&data, sizeof(data), MPI_BYTE, (rank+ 1) % size, 0, MPI_COMM_WORLD);
            //break out here after sending signal
            if(data.value <= 0){
                    break;
            }
    }
    //Finalize MPI program
    MPI_Finalize();
    return 0;
}