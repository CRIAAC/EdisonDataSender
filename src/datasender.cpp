#include "include/datasender.hpp"
#include <iostream>
#include <sstream>

long int getTimeSinceEpoch();

DataSender::DataSender(std::string host, int port, int frequency){
	m_server = new MulticastServer(host, port);
	m_frequency = frequency;
	m_dataCount = 0.0f;
}

void DataSender::onDataReady(std::vector<float> pose, std::vector<float> accel, std::vector<float> gyro, std::vector<float> magneto){
	
	m_yaw += pose[0];
	m_pitch += pose[1];
	m_roll += pose[2];
	
	m_accelX += accel[0];
	m_accelY += accel[1];
	m_accelZ += accel[2];
	
	m_gyroX += gyro[0];
	m_gyroY += gyro[1];
	m_gyroZ += gyro[2];
	
	m_magnetoX += magneto[0];
	m_magnetoY += magneto[1];
	m_magnetoZ += magneto[2];
	
	m_dataCount++;
	
	long int now = getTimeSinceEpoch();
	float diff = now - m_timer;
	if(diff >= m_frequency){
		std::ostringstream sstm;
		sstm << (m_yaw/m_dataCount) << "," << (m_pitch/m_dataCount) << "," << (m_roll/m_dataCount) << "," << (m_accelX/m_dataCount) << "," << (m_accelY/m_dataCount) << "," << (m_accelZ/m_dataCount) << "," << (m_gyroX/m_dataCount) << "," << (m_gyroY/m_dataCount) << "," << (m_gyroZ/m_dataCount) << "," << (m_magnetoX/m_dataCount) << "," << (m_magnetoY/m_dataCount) << "," << (m_magnetoZ/m_dataCount);
		m_dataCount = m_yaw = m_pitch = m_roll = m_accelX = m_accelY = m_accelZ = m_gyroX = m_gyroY = m_gyroZ = m_magnetoX = m_magnetoY = m_magnetoZ = 0.0f;
		m_server->sendMsg(sstm.str());
		m_timer = getTimeSinceEpoch();
	}
}

void DataSender::startTimer(){
	m_timer = getTimeSinceEpoch();
}

DataSender::~DataSender(){
	delete m_server;
}

long int getTimeSinceEpoch(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}