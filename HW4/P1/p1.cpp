#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv){
	MPI_Init(&argc, &argv);

	int rank, size;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int name_len;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname, &name_len);

	int value = 0;
	//std::cout << "Rank " << rank << " out of " << size << "processes." <<std::endl << std::flush;
	if(rank == 0){
		value = 1;
		std::cout << "Process " << rank << " on node " << hostname << " w/ val " << value << std::endl;
		MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		
	}
	else{

		
		MPI_Recv(&value, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		value++;
		std::cout << "Process " << rank << " on node " << hostname << " w/ val " << value << std::endl;
		if(rank < size - 1){
			MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();


	return 0;
}
