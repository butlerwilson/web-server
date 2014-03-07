#ifndef _INFOIP_H_
#define _INFOIP_H_

#include <ifaddrs.h>

// get current connect ip
extern void get_current_ip(char *addrbuff)
{
	struct ifaddrs *ifaddr = NULL;
	void *tmp = NULL;

	getifaddrs(&ifaddr);

	while (ifaddr) {
		if (ifaddr->ifa_addr->sa_family == AF_INET) { 
		    tmp=&((struct sockaddr_in*)ifaddr->ifa_addr)->sin_addr;
		    inet_ntop(AF_INET, tmp, addrbuff, INET_ADDRSTRLEN);
		    if (strcmp(ifaddr->ifa_name, "wlan0") == 0) {
			break;
		}
		}  else { 
		    tmp=&((struct sockaddr_in*)ifaddr->ifa_addr)->sin_addr;
		    inet_ntop(AF_INET6, tmp,addrbuff,INET6_ADDRSTRLEN);
		}
	
		ifaddr = ifaddr->ifa_next;		
	}
}

#endif
