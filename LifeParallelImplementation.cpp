
/*
 * LifeParallelImplementation.cpp
 *
 */

#include "LifeParallelImplementation.h"
#include <stdlib.h>
#include <mpi.h>
#include <iostream>

using namespace std;

LifeParallelImplementation::LifeParallelImplementation()
{
}

int LifeParallelImplementation::getStartInd(){
    return startR;
}

int LifeParallelImplementation::getEndInd(){
    return endR;
}

void LifeParallelImplementation::setProcRank() {
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
}

int LifeParallelImplementation::getProcRank(){
    return procRank;
}

void LifeParallelImplementation::setNumProcs() {
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
}

void LifeParallelImplementation::performSwapTables()
{
    swapTables();
}

void LifeParallelImplementation::sendCellsToRoot() {
    if (procRank != 0) {
        for (int row = startR; row <= endR; row++) {
            MPI_Send(&cells[row][0], size, MPI_INT, 0, row, MPI_COMM_WORLD);
        }
    }
    else {
        for (int row = endR + 1; row < size - 1; row++) {
            MPI_Recv(&cells[row][0], size, MPI_INT, MPI_ANY_SOURCE, row, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
}

void LifeParallelImplementation::sendPollutionToRoot() {
    if (procRank != 0) {
        for (int row = startR; row <= endR; row++) {
            MPI_Send(&pollution[row][0], size, MPI_INT, 0, row, MPI_COMM_WORLD);
        }
    }
    else {
        for (int row = endR + 1; row < size - 1; row++) {
            MPI_Recv(&pollution[row][0], size, MPI_INT, MPI_ANY_SOURCE, row, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }        
    }
}

void LifeParallelImplementation::realStep() {
    int currentState, currentPollution;

    if(procRank % 2 == 0){
        if(procRank != (numProcs - 1)){
            MPI_Send(&cells[endR][0], size, MPI_INT, procRank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&pollution[endR][0], size, MPI_INT, procRank + 1, 1, MPI_COMM_WORLD);

            MPI_Recv(&cells[endR + 1][0], size, MPI_INT, procRank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&pollution[endR + 1][0], size, MPI_INT, procRank + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if(procRank != 0){
            MPI_Send(&cells[startR][0], size, MPI_INT, procRank - 1, 0, MPI_COMM_WORLD);
            MPI_Send(&pollution[startR][0], size, MPI_INT, procRank - 1, 1, MPI_COMM_WORLD);

            MPI_Recv(&cells[startR - 1][0], size, MPI_INT, procRank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&pollution[startR - 1][0], size, MPI_INT, procRank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else{
        MPI_Recv(&cells[startR - 1][0], size, MPI_INT, procRank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&pollution[startR - 1][0], size, MPI_INT, procRank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if(procRank != (numProcs - 1)){
            MPI_Recv(&cells[endR + 1][0], size, MPI_INT, procRank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&pollution[endR + 1][0], size, MPI_INT, procRank + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Send(&cells[endR][0], size, MPI_INT, procRank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&pollution[endR][0], size, MPI_INT, procRank + 1, 1, MPI_COMM_WORLD);
        }

        MPI_Send(&cells[startR][0], size, MPI_INT, procRank - 1, 0, MPI_COMM_WORLD);
        MPI_Send(&pollution[startR][0], size, MPI_INT, procRank - 1, 1, MPI_COMM_WORLD);
    }

	for (int row = startR; row <= endR; row++) 
    {
        for (int col = 1; col < size_1; col++)
        {
			currentState = cells[row][col];
            currentPollution = pollution[row][col];

            cellsNext[row][col] = rules->cellNextState(currentState, liveNeighbours(row, col),
                                                       currentPollution);
            pollutionNext[row][col] =
                rules->nextPollution(currentState, currentPollution, pollution[row + 1][col] + pollution[row - 1][col] + pollution[row][col - 1] + pollution[row][col + 1],
                                     pollution[row - 1][col - 1] + pollution[row - 1][col + 1] + pollution[row + 1][col - 1] + pollution[row + 1][col + 1]);
        }
    }
}

void LifeParallelImplementation::oneStep()
{
	realStep();
    swapTables();
}


int LifeParallelImplementation::numberOfLivingCells() {
    return sumTable(cells);
}

double LifeParallelImplementation::averagePollution() {
    int globalPollution = sumTable(pollution);
    return (double)globalPollution / size_1_squared / rules->getMaxPollution();
}

void LifeParallelImplementation::beforeFirstStep() {
    setNumProcs();
    setProcRank();

    int rowsPerProc = (size - 2) / numProcs;
    int remainingRows = (size - 2) % numProcs;

    startR = 1 + procRank * rowsPerProc;

    if (procRank < remainingRows) {
        startR += procRank;
        rowsPerProc++;
    }
    else {
        startR += remainingRows;
    }

    endR = startR + rowsPerProc - 1;

    if (endR > size - 1) {
        endR = size - 1;
    }

    for (int row = 0; row < size; row++) {
        MPI_Bcast(&cellsTable()[row][0], size, MPI_INT, 0, MPI_COMM_WORLD);
    }
}

void LifeParallelImplementation::afterLastStep() {

    sendCellsToRoot();
    sendPollutionToRoot();

}