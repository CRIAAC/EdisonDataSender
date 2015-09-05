#include "mraa.hpp"
#include <iostream>
#include <unistd.h>
#include <string>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "SFE_LSM9DS0.h"
#include <chrono>
using namespace std;

int main()
{
	std::string opt = "wlan0";
	struct ifreq ifr;
	int m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , opt.c_str() , IFNAMSIZ-1);
	ioctl(m_sockfd, SIOCGIFHWADDR, &ifr);
  unsigned char * tmp = (unsigned char *)ifr.ifr_hwaddr.sa_data;
	char buf [50];
	sprintf (buf, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x" , tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
	string mac = std::string(buf);
	
  LSM9DS0 *imu;
  imu = new LSM9DS0(0x6B, 0x1D);

  uint16_t imuResult = imu->begin(imu->G_SCALE_500DPS,imu->A_SCALE_8G,imu->M_SCALE_2GS,imu->G_ODR_190_BW_50,imu->A_ODR_100,imu->M_ODR_100);
  imu->setAccelABW(imu->A_ABW_50);
  //uint16_t imuResult = imu->begin();

  bool newAccelData = false;
  bool newMagData = false;
  bool newGyroData = false;
  bool overflow = false;

  // Loop and report data
  while (1)
  {
    // First, let's make sure we're collecting up-to-date information. The
    //  sensors are sampling at 100Hz (for the accelerometer, magnetometer, and
    //  temp) and 95Hz (for the gyro), and we could easily do a bunch of
    //  crap within that ~10ms sampling period.
    while ((newGyroData & newAccelData & newMagData) != true)
    {
      if (newAccelData != true)
      {
        newAccelData = imu->newXData();
      }
      if (newGyroData != true)
      {
        newGyroData = imu->newGData();
      }
      if (newMagData != true)
      {
        newMagData = imu->newMData(); // Temp data is collected at the same
                                      //  rate as magnetometer data.
      } 
    }
  
    newAccelData = false;
    newMagData = false;
    newGyroData = false;
  
    imu->readAccel();
    imu->readMag();
    imu->readGyro();
    imu->readTemp();
  
    unsigned long milliseconds_since_epoch = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout << milliseconds_since_epoch << "," << mac << "," << imu->calcAccel(imu->ax) << "," << imu->calcAccel(imu->ay) << "," << imu->calcAccel(imu->az) << "," << imu->calcGyro(imu->gx) << "," << imu->calcGyro(imu->gy) << "," << imu->calcGyro(imu->gz) << "," << imu->calcMag(imu->mx) << "," << imu->calcMag(imu->my) << "," << imu->calcMag(imu->mz) << "," << endl;
    usleep(1);
  }
  return MRAA_SUCCESS;
}
