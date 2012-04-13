//
//  Round-trip demonstrator
//
//  While this example runs in a single process, that is just to make
//  it easier to start and stop the example. Each thread has its own
//  context and conceptually acts as a separate process.
//
//    Andreas Hoelzlwimmer <andreas.hoelzlwimmer@fh-hagenberg.at>
//
#include "zmsg.hpp"

#ifdef _WIN32
static unsigned
__stdcall client_task(void *arg) 
#else
static void *
client_task(void *arg) 
#endif
{
    zmq::context_t context (1);
    zmq::socket_t client (context, ZMQ_DEALER);
    client.setsockopt (ZMQ_IDENTITY, "C", 1);
    client.connect ("tcp://localhost:5555");

    std::cout << "Setting up test..." << std::endl;
    s_sleep (100);

    int requests;
    int64_t start;

    std::cout << "Synchronous round-trip test..." << std::endl;
    start = s_clock ();
    for (requests = 0; requests < 10000; requests++) {
        zmsg msg ("HELLO");
        msg.send (client);
        msg.recv (client);
    }
    std::cout << (1000 * 10000) / (int) (s_clock () - start) << " calls/second" << std::endl;

    std::cout << "Asynchronous round-trip test..." << std::endl;
    start = s_clock ();
    for (requests = 0; requests < 100000; requests++) {
        zmsg msg ("HELLO");
        msg.send (client);
    }
    for (requests = 0; requests < 100000; requests++) {
        zmsg msg (client);
    }
    std::cout << (1000 * 100000) / (int) (s_clock () - start) << " calls/second" << std::endl;

    return 0;
}

#ifdef _WIN32
static unsigned
__stdcall worker_task (void *arg) 
#else
static void *
worker_task (void *arg) 
#endif
{
    zmq::context_t context (1);
    zmq::socket_t worker (context, ZMQ_DEALER);
    worker.setsockopt (ZMQ_IDENTITY, "W", 1);
    worker.connect ("tcp://localhost:5556");

    while (1) {
        zmsg msg (worker);
        msg.send (worker);
    }
    return 0;
}

#ifdef _WIN32
static unsigned
__stdcall broker_task (void *arg) 
#else
static void *
broker_task (void *arg) 
#endif
{
    //  Prepare our context and sockets
    zmq::context_t context (1);
    zmq::socket_t frontend (context, ZMQ_ROUTER);
    zmq::socket_t backend  (context, ZMQ_ROUTER);
    frontend.bind ("tcp://*:5555");
    backend.bind  ("tcp://*:5556");

    //  Initialize poll set
    zmq::pollitem_t items [] = {
        { frontend, 0, ZMQ_POLLIN, 0 },
        { backend,  0, ZMQ_POLLIN, 0 }
    };
    while (1) {
        zmq::poll (items, 2, -1);
        if (items [0].revents & ZMQ_POLLIN) {
            zmsg msg (frontend);
            msg.pop_front ();
            msg.push_front ((char *)"W");
            msg.send (backend);
        }
        if (items [1].revents & ZMQ_POLLIN) {
            zmsg msg (backend);
            msg.pop_front ();
            msg.push_front ((char *)"C");
            msg.send (frontend);
        }
    }
    return 0;
}

int main ()
{
    s_version_assert (2, 1);

	auto thr = create_thread(&client_task, 0);
	create_thread(&worker_task, 0);
	create_thread(&broker_task, 0);
    join_thread(thr);
    return 0;
}
