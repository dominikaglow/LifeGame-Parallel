
/*
 * LifeParallelImplementation.h
 */

#ifndef LIFEPARALLELIMPLEMENTATION_H_
#define LIFEPARALLELIMPLEMENTATION_H_

#include "Life.h"

class LifeParallelImplementation : public Life
{
private:
    int startR;
    int endR;
    int procRank;
    int numProcs;
    int globalLivingCells;
    int globalPollution;

protected:
    void realStep();

public:
    LifeParallelImplementation();
    void setProcRank();
    void setNumProcs();
    void performSwapTables();
    int numberOfLivingCells();
    double averagePollution();
    void oneStep();
    void beforeFirstStep();
    void afterLastStep();
    int getStartInd();
    int getEndInd();
    int getProcRank();
    void sendCellsToRoot();
    void sendPollutionToRoot();
};

#endif /* LIFEPARALLELIMPLEMENTATION_H_ */
