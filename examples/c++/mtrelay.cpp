//
//  Multithreaded relay in C++
//
// Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>


#include "zhelpers.hpp"


//  Step 1 pushes one message to step 2

#ifdef _WIN32
static unsigned
__stdcall step1 (void *arg) 
#else
static void *
step1 (void *arg) 
#endif
{
	
	zmq::context_t * context = static_cast<zmq::context_t*>(arg);
	
	//  Signal downstream to step 2
	zmq::socket_t sender (*context, ZMQ_PAIR);
	sender.connect("inproc://step2");

	s_send (sender, "");

	return (NULL);
}

//  Step 2 relays the signal to step 3

#ifdef _WIN32
static unsigned
__stdcall step2 (void *arg) 
#else
static void *
step2 (void *arg) 
#endif
{
	zmq::context_t * context = static_cast<zmq::context_t*>(arg);
	
    //  Bind to inproc: endpoint, then start upstream thread
	zmq::socket_t receiver (*context, ZMQ_PAIR);
    receiver.bind("inproc://step2");

	create_thread(&step1, context);

    //  Wait for signal
    s_recv (receiver);

    //  Signal downstream to step 3
    zmq::socket_t sender (*context, ZMQ_PAIR);
    sender.connect("inproc://step3");
    s_send (sender, "");

    return (NULL);
}

//  Main program starts steps 1 and 2 and acts as step 3

int main () 
{	
	zmq::context_t context(1);

    //  Bind to inproc: endpoint, then start upstream thread
    zmq::socket_t receiver (context, ZMQ_PAIR);
    receiver.bind("inproc://step3");

	create_thread(&step2, &context);

    //  Wait for signal
    s_recv (receiver);
    
    std::cout << "Test successful!" << std::endl;

    return 0;
}
