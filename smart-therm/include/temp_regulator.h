#ifndef TEMP_REGULATOR_H
#define TEMP_REGULATOR_H
#define TEMP_REGULATOR_HYSTERESIS 0.5f

class TempRegulator {

private:
    const unsigned char relayPin;
    float targetTemp;
    float targetTempUpperLimit;
    float targetTempLowerLimit;
    inline void setRelayState(bool state);

public:
    bool goingUp;

    TempRegulator(unsigned char pin);

    void regulateTemp();

    void setTargetTemp(float &temperature);

};

#endif