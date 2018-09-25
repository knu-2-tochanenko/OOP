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
		IpAddress(unsigned int, unsigned int, unsigned int, unsigned int);
		//	Constructor for IPv6 format
		IpAddress(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
		// TODO make a function which and use arrays
		IpAddress(unsigned int[]);

		std::string getIPv4Address();
		std::string getIPv6Address();

		//	Generates random IP address (both IPv4 & IPv6)
		static IpAddress* generateRandom();
		/*
		*	Generates random IP address (both IPv4 & IPv6)
		*	in given IpAddress with given mask
		//*/
		static IpAddress* generateRandom(IpAddress, unsigned int);

	};
}
