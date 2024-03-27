#ifndef TREATMENTPROTOCOL_H
#define TREATMENTPROTOCOL_H

class TreatmentProtocol {
private:
    int protocolId;
    float offsetFrequency;

public:
    TreatmentProtocol(int id, float offset); // Constructor with parameters
    ~TreatmentProtocol(); // Destructor

    void executeTreatment();
};

#endif // TREATMENTPROTOCOL_H
