//
//  Weather update client in C++
//  Connects SUB socket to tcp://localhost:5556
//  Collects weather updates and finds avg temp in zipcode
//
//  Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>
//
#include <zmq.hpp>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main (int argc, char *argv[])
{
	const char* server = (1 == argc) ? "tcp://localhost:5556" : argv[1];

    //  Socket to talk to server
    cout << "Collecting updates from weather server: " << server << endl;
	if (1 == argc) 
		cout << "This is default server, you can override it with command line parameter\n";

	zmq::context_t context (1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect(server);

    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = "10001 ";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    //  Process 100 updates
    int update_nbr;
    long total_temp = 0;
    for (update_nbr = 0; update_nbr < 100; update_nbr++) 
	{
        zmq::message_t update;
        int zipcode, temperature, relhumidity;

        subscriber.recv(&update);

        std::istringstream iss(static_cast<char*>(update.data()));
        iss >> zipcode >> temperature >> relhumidity ;

		cout << "Got record for 10001 index: " << static_cast<char*>(update.data()) << endl;

        total_temp += temperature;
    }
    std::cout     << "Average temperature for zipcode '"<< filter
                <<"' was "<<(int) (total_temp / update_nbr) <<"F"
                << std::endl;
    return 0;
}