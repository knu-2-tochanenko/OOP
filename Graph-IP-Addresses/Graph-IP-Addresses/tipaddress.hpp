#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <random>

namespace tvv {
	class IpAddress {
	private:

		//	May be used in the future
		enum IPType { v4 = 4, v6 = 6 };

		unsigned int IPv4[4];
		unsigned int IPv6[8];
		//unsigned int AdressMask; // Number of bits which equal 1

		//	True if it may be used only as IPv6, else it is false
		bool onlyIpv6;

		/*
		*	Method which is used to convert IPv4 to IPv6
		*	and vice versa
		*	Parameter is referred to which type it is needed
		*	to convert IP address
		*	Result is false, if last 5 octets in IPv6 are not = 0
		*	and WON'T sync it
		//*/
		bool syncAddresses(IPType iptype) {
			if ((iptype == v4) && isConvertable()) {
				IPv4[0] = IPv6[6] >> 4;
				IPv4[1] = IPv6[6] % 256;
				IPv4[2] = IPv6[7] >> 4;
				IPv4[3] = IPv6[7] % 256;
			}
			else {
				IPv6[0] = 0;
				IPv6[1] = 0;
				IPv6[2] = 0;
				IPv6[3] = 0;
				IPv6[4] = 0;
				IPv6[5] = 65535;
				IPv6[6] = (IPv4[0] << 4) + IPv4[1];
				IPv6[7] = (IPv4[2] << 4) + IPv4[3];
			}
		}
		bool isConvertable() {
			if (((this->IPv6[0] + this->IPv6[1] + this->IPv6[2] + this->IPv6[3] + this->IPv6[4]) == 0)
				&& (this->IPv6[5] == 65535))
				return true;
			else
				return false;
		}

	public:
		//	Default constructor
		IpAddress() {

		}
		//	Constructor for IPv4 format
		IpAddress(unsigned int octet0, unsigned int octet1, unsigned int octet2, unsigned int octet3) {
			IPv4[0] = octet0;
			IPv4[1] = octet1;
			IPv4[2] = octet2;
			IPv4[3] = octet3;
			onlyIpv6 = false;
		}
		//	Constructor for IPv6 format
		IpAddress(unsigned int octet0, unsigned int octet1, unsigned int octet2, unsigned int octet3,
			unsigned int octet4, unsigned int octet5, unsigned int octet6, unsigned int octet7) {
			IPv6[0] = octet0;
			IPv6[1] = octet1;
			IPv6[2] = octet2;
			IPv6[3] = octet3;
			IPv6[4] = octet4;
			IPv6[5] = octet5;
			IPv6[6] = octet6;
			IPv6[7] = octet7;
			// may be :/
			onlyIpv6 = true;
		}

		std::string getIPv4Address() {
			return std::to_string(IPv4[0]) + ':' + std::to_string(IPv4[1]) + ':'
				+ std::to_string(IPv4[2]) + ':' + std::to_string(IPv4[3]);
		}
		std::string getIPv6Address() {
			//	TODO make hex to string function
			return std::to_string(IPv6[0]) + ':' + std::to_string(IPv6[1]) + ':'
				+ std::to_string(IPv6[2]) + ':' + std::to_string(IPv6[3]) + ':'
				+ std::to_string(IPv6[4]) + ':' + std::to_string(IPv6[5]) + ':'
				+ std::to_string(IPv6[6]) + ':' + std::to_string(IPv6[7]);
		}

		//	Generates random IP address in IPv4 form
		static IpAddress* generateRandomIPv4() {
			srand(time(NULL));
			IpAddress *newAddress = new IpAddress();
			newAddress->onlyIpv6 = false;
			newAddress->IPv4[0] = rand() % 255;
			newAddress->IPv4[1] = rand() % 255;
			newAddress->IPv4[2] = rand() % 255;
			newAddress->IPv4[3] = rand() % 255;
			return newAddress;
		}
		//	Generates random IP address in IPv6 form
		static IpAddress* generateRandomIPv6() {
			srand(time(NULL));
			IpAddress *newAddress = new IpAddress();
			// may be :/
			newAddress->onlyIpv6 = true;
			newAddress->IPv6[0] = rand() % 255;
			newAddress->IPv6[1] = rand() % 255;
			newAddress->IPv6[2] = rand() % 255;
			newAddress->IPv6[3] = rand() % 255;
			newAddress->IPv6[4] = rand() % 255;
			newAddress->IPv6[5] = rand() % 255;
			newAddress->IPv6[6] = rand() % 255;
			newAddress->IPv6[7] = rand() % 255;
			return newAddress;
		}

		//	TODO write 
		bool checkIfIpIsInSubnet(IpAddress sourceIp, int mask);

		const bool operator== (const IpAddress &secondIp) {
			if (this->onlyIpv6 != secondIp.onlyIpv6)
				return false;
			if (!(this->onlyIpv6))
				if ((this->IPv4[0] == secondIp.IPv4[0])
					&& (this->IPv4[1] == secondIp.IPv4[1])
					&& (this->IPv4[2] == secondIp.IPv4[2])
					&& (this->IPv4[3] == secondIp.IPv4[3]))
					return true;
				else
					return false;
			else
				if ((this->IPv6[0] == secondIp.IPv6[0])
					&& (this->IPv6[1] == secondIp.IPv6[1])
					&& (this->IPv6[2] == secondIp.IPv6[2])
					&& (this->IPv6[3] == secondIp.IPv6[3])
					&& (this->IPv6[4] == secondIp.IPv6[4])
					&& (this->IPv6[5] == secondIp.IPv6[5])
					&& (this->IPv6[6] == secondIp.IPv6[6])
					&& (this->IPv6[7] == secondIp.IPv6[7]))
					return true;
				else return false;
		}
	};
}
