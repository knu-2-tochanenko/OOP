#include "tgraph.hpp"
#include "tipaddress.hpp"
#include <iostream>

//* HERE
bool generateRandom(tvv::Graph<tvv::IpAddress>& graph, int numberOfNodes, int numberOfConnections){
	for (int i = 0; i < numberOfNodes; i++) {
		tvv::IpAddress newAddress;
		newAddress.generateRandomIpV4();
		graph.addElement(newAddress);
	}
	graph.randomlyConnect(numberOfConnections);
	return true;
}
//*/

int main() {
	//* Place one more slash on the beginning of this line in order to check the work of graph. Also make the same on the line with HERE sign
	int numberOfNodes, numberOfConnections;
	tvv::Graph<tvv::IpAddress> graph;
	std::cin >> numberOfNodes >> numberOfConnections;

	generateRandom(graph, numberOfNodes, numberOfConnections);

	graph.getInformationString();
	//*/

	/* Place one more slash on the beginning of this line in order to check the work of checking for subnet
	tvv::IpAddress address;
	address.generateRandomIpV4();
	tvv::IpAddress host;
	host.generateRandomIpV4();

	std::cout << address.checkIfInSubnetIpV4(host, 30) << "\n";
	//*/

	system("pause");
	return 0;
}
