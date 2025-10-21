
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv){
        //Initialize MPI
        MPI_Init(&argc, &argv);

        //Initialize rank and size and hostname for printing clarity
        int rank, size;
        char hostname[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Get_processor_name(hostname, &name_len);
        
        //Starting value
        int value = 0;
        //std::cout << "Rank " << rank << " out of " << size << "processes." <<std::endl << std::flush;
        //Start with first process setting value to 0
        if(rank == 0){

                value = 1;
                std::cout << "Process " << rank << " on node " << hostname << " w/ val " << value << std::en
dl;
                //Send updated value to next process
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        
        }

        //Other processes
        else{
                //Next processes receive the value and increment them
                MPI_Recv(&value, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                value++;
                std::cout << "Process " << rank << " on node " << hostname << " w/ val " << value << std::en
dl;
                //until the process is 63, indicating 64 have run, continue sending updated values
                if(rank < size - 1){
                        MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                }
        }
        
        //Finalize MPI, closing it
        MPI_Finalize();
        return 0;
}