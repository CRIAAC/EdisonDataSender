#include <iostream>
#include "include/sensorsdatarecovery.hpp"

SensorsDataRecovery::SensorsDataRecovery(SensorListener* listener){
	m_settings = new RTIMUSettings("RTIMULib");
	m_imu = RTIMU::createIMU(m_settings);
	m_pressure = RTPressure::createPressure(m_settings);
	m_listener = listener;
	
	if ((m_imu == NULL) || (m_imu->IMUType() == RTIMU_TYPE_NULL)) {
		printf("No IMU found\n");
		exit(1);
	}
	
	//  This is an opportunity to manually override any settings before the call IMUInit
	//  set up IMU
	m_imu->IMUInit();
	
	//  this is a convenient place to change fusion parameters
	m_imu->setSlerpPower(0.02);
	m_imu->setGyroEnable(true);
	m_imu->setAccelEnable(true);
	m_imu->setCompassEnable(true);
	
	//  set up pressure sensor
	if (m_pressure != NULL)
		m_pressure->pressureInit();
	
	//  set up for rate timer
	m_rateTimer = m_displayTimer = RTMath::currentUSecsSinceEpoch();
}

SensorsDataRecovery::~SensorsDataRecovery(){
	delete m_imu;
	delete m_pressure;
	delete m_settings;
	delete m_listener;
}

void SensorsDataRecovery::run(){
	while (m_imu->IMURead()) {
		RTIMU_DATA imuData = m_imu->getIMUData();
		//  add the pressure data to the structure
		if (m_pressure != NULL)
			m_pressure->pressureRead(imuData);		
		std::vector<float> pose;
		std::vector<float> accel;
		std::vector<float> gyro;
		std::vector<float> magneto;
		pose.push_back(imuData.fusionPose.x() * RTMATH_RAD_TO_DEGREE);
		pose.push_back(imuData.fusionPose.y() * RTMATH_RAD_TO_DEGREE);
		pose.push_back(imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE);
		accel.push_back(imuData.accel.x());
		accel.push_back(imuData.accel.y());
		accel.push_back(imuData.accel.z());
		gyro.push_back(imuData.gyro.x());
		gyro.push_back(imuData.gyro.y());
		gyro.push_back(imuData.gyro.z());
		magneto.push_back(imuData.compass.x());
		magneto.push_back(imuData.compass.y());
		magneto.push_back(imuData.compass.z());
		m_listener->onDataReady(pose, accel, gyro, magneto);
	}
}

long SensorsDataRecovery::getInterval(){
	return m_imu->IMUGetPollInterval();
}