#include "include/multicastserver.hpp"
#include <msgpack.hpp>
#include <iostream>

MulticastServer::MulticastServer(std::string addr, int port){
	m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (m_sockfd < 0){
		perror("socket");
		exit(1);
	}
	bzero(&m_server, sizeof(m_server));
	memset(&m_imreq, 0, sizeof(m_imreq));
	memset(&m_server, 0, sizeof(m_server));
	m_server.sin_family = AF_INET;
	m_server.sin_port = htons(port);
	m_server.sin_addr.s_addr = inet_addr(addr.c_str());
	std::cout << "Joining the multicast " << addr << " on port " << port << std::endl;
	if (bind(m_sockfd, (struct sockaddr *) &m_server, sizeof(m_server)) < 0) {        
        perror("bind");
		exit(1);
    }
	std::string opt = "wlan0";
	if(setsockopt(m_sockfd, SOL_SOCKET, SO_BINDTODEVICE, opt.c_str(), opt.size()) < 0){
		perror("setsockopt");
		exit(1);
	}
	struct ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , opt.c_str() , IFNAMSIZ-1);
	ioctl(m_sockfd, SIOCGIFHWADDR, &ifr);
    unsigned char * tmp = (unsigned char *)ifr.ifr_hwaddr.sa_data;
	char buf [50];
	sprintf (buf, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x" , tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
	m_mac = std::string(buf);
}

void MulticastServer::sendMsg(std::string msg) {
	msg = m_mac + "," + msg;
	if(sendto(m_sockfd,msg.c_str(),msg.size(),0,(struct sockaddr*)&m_server,sizeof(m_server)) < 0){
		perror("sendto");
		exit(1);
	}
}