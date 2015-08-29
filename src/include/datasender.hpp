#ifndef DATASENDER_HPP
#define DATASENDER_HPP
#include "sensorlistener.hpp"
#include "multicastserver.hpp"
#include <vector>
#include <string>

class DataSender : public SensorListener
{
private :
	MulticastServer* m_server;
	long int m_timer;
	int m_frequency;
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_accelX;
	float m_accelY;
	float m_accelZ;
	float m_gyroX;
	float m_gyroY;
	float m_gyroZ;
	float m_magnetoX;
	float m_magnetoY;
	float m_magnetoZ;
	float m_dataCount;
public:
	DataSender(std::string host, int port, int frequency);
	~DataSender();
	void onDataReady(std::vector<float> pose, std::vector<float> accel, std::vector<float> gyro, std::vector<float> magneto);
	void startTimer();
};
#endif