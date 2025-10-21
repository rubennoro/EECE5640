#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &name_len);
    //bool max = false;
    //int value = 0;
    struct{
	    int value;
	    bool max;
    } data;

    if (rank == 0) {

        data.value = 1;
	data.max = false;
        std::cout << "Process " << rank << " on node " << hostname << " w/ val " << data.value << std::endl;

        MPI_Send(&data, sizeof(data), MPI_BYTE, (rank + 1)%size, 0, MPI_COMM_WORLD);
    }
     
    while(true){
	    MPI_Recv(&data, sizeof(data), MPI_BYTE, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	    //Break out if the received signal means 0 has been printed
	    
	    if(data.value < 64 && !data.max){
		    data.value++;
		    if(data.value >= 64){
			    data.max = true;
		    }
	    }
	    else if(data.max && data.value != -1){
		    data.value -= 2;
	    
	    }
	    if(data.value != -1){
		    std::cout << "Process " << rank << " on node " << hostname << "w/ val " << data.value << std::endl;
	    }
	    //For negative data, send -1 to the other nodes
	    if(data.value <= 0){
		    data.value = -1;
	    }

	    MPI_Send(&data, sizeof(data), MPI_BYTE, (rank+ 1) % size, 0, MPI_COMM_WORLD);

	    //break out here after sending signal
	    if(data.value <= 0){
		    break;
	    }
    }

    MPI_Finalize();

    return 0;

}
