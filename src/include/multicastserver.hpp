#ifndef MULTICASTSERVER_HPP
#define MULTICASTSERVER_HPP
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>

class MulticastServer {
private:
	struct sockaddr_in m_server;
	struct ip_mreq m_imreq;
	int m_sockfd;
	int m_port;
	std::string m_addr;
	std::string m_mac;
public:
	MulticastServer(std::string addr, int port);
	void sendMsg(std::string msg);
};
#endif