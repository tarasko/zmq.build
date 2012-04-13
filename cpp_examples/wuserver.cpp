//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates
//
//  Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>
//
#include <zmq.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

#ifdef _MSC_VER
# define SNPRINTF _snprintf
#else
# define SNPRINTF snprintf
#endif

inline int within(int num)
{
	if (num > RAND_MAX)
		num = RAND_MAX;
	return (int) ((float) num * rand () / (RAND_MAX + 1.0));
}

int main(int argc, char *argv[]) 
{
	// Make vector of cmdline params.
	vector<const char*> args(argv + 1, argv + argc);

	// If no params are specified use defaults
	if (args.empty())
	{
		args.push_back("tcp://*:5556");
#ifndef _WIN32
		args.push_back("ipc://weather.ipc");
#endif
	}

	cout << "Start weather update server on endpoints:\n";
	for_each(args.begin(), args.end(), [](const char* arg) { cout << "\t" << arg << "\n"; });

	if (1 == argc) 
		cout << "This is default endpoints, override them with command line parameters\n";

    //  Prepare our context and publisher
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
	for_each(args.begin(), args.end(), [&](const char* arg) { publisher.bind(arg); });

    //  Initialize random number generator
    srand ((unsigned) time (NULL));
    while (1) {

        int zipcode, temperature, relhumidity;

        //  Get values that will fool the boss
        zipcode     = within (20000);
        temperature = within (215) - 80;
        relhumidity = within (50) + 10;

        //  Send message to all subscribers
        zmq::message_t message(20);
        SNPRINTF ((char *) message.data(), 20 ,
            "%05d %d %d", zipcode, temperature, relhumidity);
        publisher.send(message);

    }
    return 0;
}
