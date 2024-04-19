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


int Electrode::getId(){ return electrodeId; }  // their id is also their index

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


     opFrequency1 = getRandomDouble(1,4);     // delta band
     opFrequency2 = getRandomDouble(4,8);     // theta band
     opFrequency3 = getRandomDouble(12,30);   // beta band

     opAmplitude1 = getRandomDouble(0,5);
     opAmplitude1 = getRandomDouble(0,5);
     opAmplitude1 = getRandomDouble(0,5);

     operationTimeElapsed = 0;

     operationTimer->start();

     if(inDisplay){ qInfo("operation started for electrode %d ",electrodeId); }

     //calculate and send intial dominant frequency
     currentDominatFrequency = calculateDominant();
     emit dominantFrequency(electrodeId, currentDominatFrequency);

 }



 void Electrode::runOperation() {

     if(isConnected){

         disconnectedTimer->stop();

         operationTimeElapsed++; // Increment each second
         int seconds = operationTimeElapsed % 60;

         // check time and apply treatment here?
         applyTreatment();

         xGraphForm.append(seconds);
         yGraphForm.append( calculateWaveFormYCoordinate(seconds) );

         if(inDisplay){ qInfo("electrode %d : operation time elapsed =  %d", electrodeId, operationTimeElapsed); }

     }
     else{

         //  if disconnected start the disconnect timer, if reconnection doesnt occer the operation will end along with the session

         if(inDisplay){ qInfo("electrode %d is diconnected",electrodeId); }

         disconnectedTimer->start();
     }

     if(operationTimeElapsed >= 50){ // operation ends wwith this condition

         //calculate and send final dominant frequency if connected
         if(isConnected){

             currentDominatFrequency = calculateDominant();
             emit dominantFrequency(electrodeId, currentDominatFrequency);

         }

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

     if(inDisplay){ qInfo("Operation ended for electrode %d",electrodeId); }

 }



 void Electrode::endOperation(){
     operationEndSequence();
 }






 void Electrode::applyTreatment(){

     // define ur treatment here, also sync the mainwindow ui lights while this happens

     // the goal is to make the current ui graph gradually change its shape over time, but u should doeble check the specs
     // my idea was to just calculate dominant frequency addd thhe offsset of 5 hertz
     // then add this new nnumber to all the  frequency bands in this class
     // while printing the relevant info in terminal
     // reapeat this 4 times/rounds at 4 different time intervals - if statemnts maybe?



     // the rest of the my algorithm wwill automatically calulate y coorordinates with ur defined treatment adjustment as it uses the frequency bands in its calculation

 }







 double Electrode::getRandomDouble(double min, double max){
     return min + QRandomGenerator::global()->generateDouble() * (max - min);
 };





 double Electrode::calculateDominant(){

     if(inDisplay){ qInfo("calculating dominant frequency for electrode %d", electrodeId); }

     double fd = (opFrequency1*opAmplitude1 + opFrequency2*opAmplitude2 + opFrequency3*opAmplitude3) / (opAmplitude1 + opAmplitude2 + opAmplitude3);

     return fd;
 }







 double Electrode::calculateWaveFormYCoordinate(int time) {

    wavefunction = opAmplitude1*qSin(opFrequency1*time)
                        + opAmplitude2*qSin(opFrequency2*time) + opAmplitude3*qSin(opFrequency3*time)
                                                                     + opAmplitude1 + opAmplitude2 + opAmplitude3;

    return wavefunction;
 }













