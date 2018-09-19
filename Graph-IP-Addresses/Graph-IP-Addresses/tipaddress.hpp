#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace tvv {
	template <typename TNodeType> class IpAddress {
	private:

		// May be used in the future
		enum IPType { v4 = 4, v6 = 6 };

		unsigned int IPv4[4];
		unsigned int IPv6[8];
		unsigned int AdressMask; // Number of bits which equal 1

		/*
			Method which is used to convert IPv4 to IPv6
			and vice versa
		//*/
		bool syncAddresses(IPType);

	public:
		string getIPv4Address();
		string getIPv6Address();
		IpAddress();

	};
}
