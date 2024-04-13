#include "Electrode.h"
#include <QtMath>



Electrode::Electrode(int id) : electrodeId(id), isConnected(false), disconnectedTimer(new QTimer(this)), operationTimer(new QTimer(this)) {
    // Initialize the electrode with the given ID and as disconnected
    // initialize the operation and disconnection timers

    disconnectedTimer->setInterval(3000); // if u change this also cchange the contactlosttimer in Mainwindow
    operationTimer->setInterval(1000);

    connect(operationTimer, &QTimer::timeout, this, &Electrode::runOperation);
    connect(disconnectedTimer, &QTimer::timeout, this, &Electrode::operationEndSequence);
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

     qInfo("operation started for electrode %d ",electrodeId);

 }



 void Electrode::runOperation() {

     if(isConnected){

         disconnectedTimer->stop();

         operationTimeElapsed++; // Increment each second
         int seconds = operationTimeElapsed % 60;

         xGraphForm.append(seconds);
         yGraphForm.append( calculateWaveFormYCoordinate(seconds) );

         qInfo("electrode %d : operation time elapsed =  %d",electrodeId, operationTimeElapsed);

     }
     else{

         //  if disconnected start the disconnect timer, if reconnection doesnt occer the operation will end along with the session

         qInfo("electrode %d is diconnected",electrodeId);
         disconnectedTimer->start();
     }

     if(operationTimeElapsed >= 50){

         operationEndSequence();
     }


 }





 void Electrode::operationEndSequence(){
     // reset all operattion  related  variables and datastructures for the next operrationn

     opFrequency1 = 0 ;
     opFrequency2 = 0;
     opFrequency3 = 0;

     opAmplitude1 = 0;
     opAmplitude2 = 0;
     opAmplitude3 = 0;



     xGraphForm.clear();
     yGraphForm.clear();

     operationTimer->stop();
     disconnectedTimer->stop();

     isConnected = false;

     qInfo("Operation ended for electrode %d",electrodeId);

 }






 double Electrode::calculateWaveFormYCoordinate(int time) {

    wavefunction = opAmplitude1*qSin(opFrequency1*time)
                        + opAmplitude2*qSin(opFrequency2*time) + opAmplitude3*qSin(opFrequency3*time)
                                                                     + opAmplitude1 + opAmplitude2 + opAmplitude3;

    return wavefunction;
 }













