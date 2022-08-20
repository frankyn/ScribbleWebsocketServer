#include <iostream>
#include <string>
#include "./common/Logger.h"
#include "./ListenerClass.h"

int main(int argc, char **args) {
    ListenerClass listener;
    std::string console_input;

    // Accept console input
    while (console_input.compare("quit") != 0) {
        std::cout << "Console: ";
        std::cin >> console_input;

        if (console_input.compare("connected") == 0) {
            std::cout << "Connected: " << listener.usersConnected() << std::endl;
        } else if (console_input.compare("channels") == 0) {
            std::cout << "Available Channels:\n"
                      << listener.availableChannels() << std::endl;
        } else if (console_input.compare("help") == 0) {
            std::cout << "Commands:\n"
                      << "help - list of commands\n"
                      << "connected - connected user count\n"
                      << "channels - list of channels\n"
                      << "quit - exit program\n" << std::endl;
        } else if (console_input.compare("quit") != 0) {
            std::cout << "'" << console_input
                      << "' not a valid command. For list of commands type in 'help'"
                      << std::endl;
        }
    }

    // Shutdown Listening server
    std::cout << "Scribble is shutting down." << std::endl;
    listener.setStatus(0);


    return 0;
}

