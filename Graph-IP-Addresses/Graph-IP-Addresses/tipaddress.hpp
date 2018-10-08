#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <random>

using std::string;

//	Functions to work with hex to int conversations and visa-versa
namespace HEX {
	int toNum(char symbol) {
		if ((symbol >= '0') && (symbol <= '9'))
			return (int)(symbol - '0');
		else if ((symbol >= 'a') && (symbol <= 'f'))
			return (int)(symbol - 'a') + 10;
		else if ((symbol >= 'A') && (symbol <= 'F'))
			return (int)(symbol - 'A') + 10;
		else
			//	Make and exception
			return -1;
	}
	char toChar(int num) {
		if (num < 10)
			return (char)('0' + num);
		else
			return (char)('a' + (num - 10));
	}
	int hexToInt(string number) {
		if (number.size() == 0)
			return 0;
		std::reverse(number.begin(), number.end());
		int numberLength = number.size();
		int resNum = 0;
		for (int i = 0; i < numberLength; i++)
			resNum += toNum(number[i]) * pow(16, i);
		return resNum;
	}
	string intToHex(int num) {
		string resNum;
		int curNum;
		while (num > 0) {
			curNum = num & 15;
			resNum += toChar(curNum);
			num = num >> 4;
		}
		std::reverse(resNum.begin(), resNum.end());
		return resNum;
	}
}

namespace tvv {

	class IpAddress {
	private:
		unsigned int IP[8];

	public:
		IpAddress() {
			generateRandomIpV4();
		}
		IpAddress(unsigned int octet0, unsigned int octet1, unsigned int octet2, unsigned int octet3) {
			IP[0] = 0;
			IP[1] = 0;
			IP[2] = 0;
			IP[3] = 0;
			IP[4] = 0;
			IP[5] = 65535;
			IP[6] = (octet0 << 4) + octet1;
			IP[7] = (octet2 << 4) + octet3;
		}
		IpAddress(string ipAddressString) {
			int octet[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

			int stringLength = ipAddressString.size(),
				curNum = 0,
				octetIterator = 0;
			string curOctet = "";
			//	Divide one string into octets in decimal form
			for (int i = 0; i < stringLength; i++) {
				if (ipAddressString[i] == ':') {
					IP[octetIterator] = HEX::hexToInt(curOctet);
					curOctet = "";
					octetIterator++;
				}
				else curOctet += ipAddressString[i];
			}
			IP[octetIterator] = HEX::hexToInt(curOctet);
			for (int i = 0; i < 8; i++)
				IP[i] = octet[i];
		}

		//	Rewrite address using int values
		bool setIpV4(unsigned int octet0, unsigned int octet1, unsigned int octet2, unsigned int octet3) {
			IP[0] = 0;
			IP[1] = 0;
			IP[2] = 0;
			IP[3] = 0;
			IP[4] = 0;
			IP[5] = 65535;
			IP[6] = (octet0 << 4) + octet1;
			IP[7] = (octet2 << 4) + octet3;
		}
		bool setIpV6(unsigned int octet0, unsigned int octet1, unsigned int octet2, unsigned int octet3,
			unsigned int octet4, unsigned int octet5, unsigned int octet6, unsigned int octet7) {
			IP[0] = octet0;
			IP[1] = octet1;
			IP[2] = octet2;
			IP[3] = octet3;
			IP[4] = octet4;
			IP[5] = octet5;
			IP[6] = octet6;
			IP[7] = octet7;
		}

		const bool operator== (const IpAddress &secondIp) {
			for (int i = 0; i < 8; i++)
				if (this->IP[i] != secondIp.IP[i])
					return false;
			return true;
		}

		//	A function which tells whether this IP is in subnet of specific address
		bool checkIfInSubnetIpV4(IpAddress mainNode, int mask) {
			return checkIfInSubnetIpV6(mainNode, mask + 96);
		}
		bool checkIfInSubnetIpV6(IpAddress mainNode, int mask) {
			int bitIterator;
			for (int i = 0; i < 8; i++) {
				if (this->IP[i] == mainNode.IP[i])
					continue;
				else {
					bitIterator = 1 << 15;
					for (int j = 0; j < 16; j++) {
						//	Check if bits are equal
						if ((this->IP[i] & bitIterator) != (mainNode.IP[i] & bitIterator))
							//	If bits are not equal and in the range of current mask
							if (mask >= (i * 8 + j + 1))
								return false;
							else
								return true;
						bitIterator = bitIterator >> 1;
					}
				}
			}
			return true;
		}

		//	Generate random IP address
		bool generateRandomIpV4() {
			srand(time(NULL));
			this->IP[0] = 0;
			this->IP[1] = 0;
			this->IP[2] = 0;
			this->IP[3] = 0;
			this->IP[4] = 0;
			this->IP[5] = 65535;
			this->IP[6] = rand() % 65535;
			this->IP[7] = rand() % 65535;
			return true;
		}
		bool generateRandomIpV6() {
			srand(time(NULL));
			this->IP[0] = rand() % 65535;
			this->IP[1] = rand() % 65535;
			this->IP[2] = rand() % 65535;
			this->IP[3] = rand() % 65535;
			this->IP[4] = rand() % 65535;
			this->IP[5] = rand() % 65535;
			this->IP[6] = rand() % 65535;
			this->IP[7] = rand() % 65535;
			return true;
		}
	};
}
