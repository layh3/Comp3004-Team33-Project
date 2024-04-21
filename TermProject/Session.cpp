#include "Session.h"

Session::Session(int id)
    : sessionId(id) {
    //Protocol stuff just here to make it run, needs to be fixed


    // Constructor implementation could initialize member variables
    intialDominants.resize(ELECTRODE_NUMBER);
    finalDominants.resize(ELECTRODE_NUMBER);

}

Session::~Session() {
    // Destructor implementation, if needed for cleanup
}

double Session::calculateBaseline(QVector<double>& dominants) {
    // Logic to calculate final or intial baseline frequencies

    double total = 0;

    //dominants.size()
    for (int i = 0; i < ELECTRODE_NUMBER; ++i){
        total += dominants.at(i);
    }

    return total/ELECTRODE_NUMBER; // average domiannt freq = baseline
}


const QString& Session::getStartTime() const {
    return startTime;
}

void Session::setStartTime(QString startingTime){
    startTime = startingTime;
}


void Session::setInitialDominantsAndBaseline(QVector<double>& domFrequencies ){

    for (int i = 0; i < ELECTRODE_NUMBER; ++i){
        intialDominants.replace(i, domFrequencies.at(i) );
    }

    intialBaseline = calculateBaseline(domFrequencies);

}


void Session::setFinalDominantsAndBaseline(QVector<double>& domFrequencies){

    for (int i = 0; i < ELECTRODE_NUMBER; ++i){
        finalDominants.replace(i, domFrequencies.at(i) );
    }

    finalBaseline = calculateBaseline(domFrequencies);

}





void Session::toString(){
    qInfo("Session %d : start Time = %s : intial Base: %f : final Base:  %f ",sessionId, startTime.toUtf8().data(), intialBaseline, finalBaseline);
}


QString Session::getInitialBaseline() const {
    return QString::number(intialBaseline, 'f', 2); // Convert double to QString with 2 decimal places
}

QString Session::getFinalBaseline() const {
    return QString::number(finalBaseline, 'f', 2); // Convert double to QString with 2 decimal places
}