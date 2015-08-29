#ifndef SENSORSDATARECOVERY_H
#define SENSORSDATARECOVERY_H
#include "RTIMULib.h"
#include "sensorlistener.hpp"

class SensorsDataRecovery {
private:
	RTIMUSettings *m_settings;
	RTIMU *m_imu;
	RTPressure *m_pressure;
	uint64_t m_rateTimer;
    uint64_t m_displayTimer;
	SensorListener* m_listener;
public:
	SensorsDataRecovery(SensorListener* m_listener);
	~SensorsDataRecovery();
	void run();
	long getInterval();
};

#endif