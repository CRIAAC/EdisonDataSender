#include <iostream>
#include "include/datasender.hpp"
#include "include/sensorsdatarecovery.hpp"

void msleep(long ms);
 
int main()
{
	DataSender* sender = new DataSender("224.52.53.100", 52368, 15);
	SensorsDataRecovery sdr = SensorsDataRecovery(sender);
	sender->startTimer();
	while (1) {
		msleep(sdr.getInterval()*1000);
		sdr.run();
	}
}

void msleep(long ns)
{
    struct timespec wait;
    wait.tv_sec = 0;
    wait.tv_nsec = ns;
    nanosleep(&wait, NULL);
}