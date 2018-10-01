#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace tvv {
	class IpAddress {
	private:

		//	May be used in the future
		enum IPType { v4 = 4, v6 = 6 };

		unsigned int IPv4[4];
		unsigned int IPv6[8];
		unsigned int AdressMask; // Number of bits which equal 1

		/*
		*	Method which is used to convert IPv4 to IPv6
		*	and vice versa
		*	Parameter is referred to which type it is needed
		*	to convert IP address
		*	Result is false, if last 5 octets in IPv6 are not = 0
		*	and WON'T sync it
		//*/
		bool syncAddresses(IPType);

	public:
		//	Default constructor
		IpAddress();
		//	Constructor for IPv4 format
		IpAddress(unsigned int octet0, unsigned int octet1, unsigned int octet2, unsigned int octet3) {
			IPv4[0] = octet0;
			IPv4[1] = octet1;
			IPv4[2] = octet2;
			IPv4[3] = octet3;
		}
		//	Constructor for IPv6 format
		IpAddress(unsigned int octet0, unsigned int octet1, unsigned int octet2,
			unsigned int octet3, unsigned int octet4, unsigned int octet5) {
			IPv6[0] = octet0;
			IPv6[1] = octet1;
			IPv6[2] = octet2;
			IPv6[3] = octet3;
			IPv6[4] = octet4;
			IPv6[5] = octet5;
		}

		std::string getIPv4Address() {
			return to_string(IPv4[0]) + ':' + to_string(IPv4[1]) + ':'
				+ to_string(IPv4[2]) + ':' + to_string(IPv4[3]);
		}
		std::string getIPv6Address() {
			//	TODO make hex to string function
			return to_string(IPv6[0]) + ':' + to_string(IPv6[1]) + ':'
				+ to_string(IPv6[2]) + ':' + to_string(IPv6[3]) + ':'
				+ to_string(IPv6[4]) + ':' + to_string(IPv6[5]);
		}

		//	Generates random IP address in IPv4 form
		static IpAddress* generateRandomIPv4() {
			IpAddress *newAddress = new IpAddress();
			newAddress->
		}
		//	Generates random IP address in IPv6 form
		static IpAddress* generateRandomIPv6();

		/*
		*	Generates random IP address (both IPv4 & IPv6)
		*	in given IpAddress with given mask
		//*/
		static IpAddress* generateRandom(IpAddress, unsigned int);

	};
}
