#include <mpi.h>
#include <iostream> 
int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	
	int RankID, ProcessNum, Length;
	char ProcessName[MPI_MAX_PROCESSOR_NAME];
	int Version, SubVersion;

	MPI_Comm_size(MPI_COMM_WORLD, &ProcessNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &RankID);
	MPI_Get_processor_name(ProcessName, &Length);
	MPI_Get_version(&Version, &SubVersion);

	std::cout << "Hello!, I am " << RankID << " of " << ProcessNum << " on " << ProcessName << std::endl;
	std::cout << "My version is " << Version << "." << SubVersion << std::endl;

	MPI_Finalize();
	return 0;
}