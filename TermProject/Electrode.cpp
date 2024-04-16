#include "Electrode.h"
#include <QtMath>



Electrode::Electrode(int id) : electrodeId(id), isConnected(false), operationTimer(new QTimer(this)) {
    // Initialize the electrode with the given ID and as disconnected
    // initialize the operaation timer

    operationTimer->setInterval(1000);
    connect(operationTimer, &QTimer::timeout, this, &Electrode::runOperation);

}

Electrode::~Electrode() {
    // Cleanup tasks if necessary
}

bool Electrode::checkConnection() {
    // Implement the logic to check if the electrode is properly connected
    return isConnected;
}

void Electrode::connectElectrode() {
    // Logic to connect the electrode
    isConnected = true;
}

void Electrode::disconnectElectrode() {
    // Logic to disconnect the electrode
    isConnected = false;
}






 void Electrode::initiateOperation() {


     opFrequency1 = getRandomDouble(1,4);
     opFrequency2 = getRandomDouble(4,8);
     opFrequency3 = getRandomDouble(12,30);

     opAmplitude1 = getRandomDouble(0,5);
     opAmplitude1 = getRandomDouble(0,5);
     opAmplitude1 = getRandomDouble(0,5);

     operationTimeElapsed = 0;

     operationTimer->start();

 }



 void Electrode::runOperation() {

     if(isConnected){
         operationTimeElapsed++; // Increment each second
         int seconds = operationTimeElapsed % 60;

         xGraphForm.append(seconds);
         yGraphForm.append( calculateWaveFormYCoordinate(seconds) );
     }
     else{
         printf("electrode %d is diconnected",electrodeId);
         operationTimer->stop();
     }

     if(operationTimeElapsed > 50){
         operationTimer->stop();
     }


 }




 double Electrode::calculateWaveFormYCoordinate(int time) {

    wavefunction = opAmplitude1*qSin(opFrequency1*time)
                        + opAmplitude2*qSin(opFrequency2*time) + opAmplitude3*qSin(opFrequency3*time)
                                                                     + opAmplitude1 + opAmplitude2 + opAmplitude3;

    return wavefunction;
 }













