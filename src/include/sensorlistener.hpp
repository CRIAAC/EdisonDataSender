#ifndef SENSORLISTENER_HPP
#define SENSORLISTENER_HPP
#include <vector>
class SensorListener
{
    public:
        virtual ~SensorListener() {}
        virtual void onDataReady(std::vector<float> pose,std::vector<float> accel,std::vector<float> gyro,std::vector<float> magneto) = 0;
};
#endif