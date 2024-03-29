//
// Lazy Pirate server
// Binds REQ socket to tcp://*:5555
// Like hwserver except:
// - echoes request as-is
// - randly runs slowly, or exits to simulate a crash.
//
#include "zhelpers.hpp"

int main ()
{
    srand ((unsigned) time (NULL));

    zmq::context_t context(1);
    zmq::socket_t server(context, ZMQ_REP);
    server.bind("tcp://*:5555");

    int cycles = 0;
    while (1) {
        std::string request = s_recv (server);
        cycles++;

        // Simulate various problems, after a few cycles
        if (cycles > 3 && within (3) == 0) {
            std::cout << "I: simulating a crash" << std::endl;
            break;
        }
        else
        if (cycles > 3 && within (3) == 0) {
            std::cout << "I: simulating CPU overload" << std::endl;
            s_sleep (2000);
        }
        std::cout << "I: normal request (" << request << ")" << std::endl;
        s_sleep (1000); // Do some heavy work
        s_send (server, request);
    }
    return 0;
}
