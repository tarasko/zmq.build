/*
    Multithreaded Hello World server in C
*/

#include "zhelpers.hpp"

#ifdef _WIN32
static unsigned __stdcall worker_routine (void *arg) 
#else
static void* worker_routine (void *arg) 
#endif
{
    zmq::context_t *context = (zmq::context_t *) arg;

    zmq::socket_t socket (*context, ZMQ_REP);
    socket.connect ("inproc://workers");

    while (true) {
        //  Wait for next request from client
        zmq::message_t request;
        socket.recv (&request);
        std::cout << "Received request: [" << (char*) request.data() << "]" << std::endl;

        //  Do some 'work'
        s_sleep (1000);

        //  Send reply back to client
        zmq::message_t reply (6);
        memcpy ((void *) reply.data (), "World", 6);
        socket.send (reply);
    }
    return (NULL);
}


int main ()
{
    //  Prepare our context and sockets
    zmq::context_t context (1);
    zmq::socket_t clients (context, ZMQ_ROUTER);
    clients.bind ("tcp://*:5555");
    zmq::socket_t workers (context, ZMQ_DEALER);
    workers.bind ("inproc://workers");

    //  Launch pool of worker threads
    for (int thread_nbr = 0; thread_nbr != 5; thread_nbr++) 
	{
		create_thread(&worker_routine, &context);
    }
    //  Connect work threads to client threads via a queue
    zmq::device (ZMQ_QUEUE, clients, workers);
    return 0;
}
    
