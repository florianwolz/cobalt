#include <iostream>

#include "../cobalt.hpp"

int main(int argc, char** argv) {
    // Create the main command of your executable
    auto cobalt = Cobalt::CreateCommand();
    cobalt->Use = "echo";

    // Create the print command
    auto cmdPrint = Cobalt::CreateCommand();
    cmdPrint->Use = "print [text to print]";
    cmdPrint->Short = "Prints the given text on screen.";
    cmdPrint->Long  = "Prints the given text on screen. \nThis has been done for centuries.";
    cmdPrint->Run = [](const Cobalt::Arguments& args) {
        for (int i=0; i<args.size(); ++i) {
            std::cout << args[i] << " ";
        }
        return 0;
    };

    int times=0;

    // Create the echo command. In principle the same command as print, except for the fact that...
    auto cmdEcho = Cobalt::CreateCommand();
    cmdEcho->Use = "echo [text to print]";
    cmdEcho->Short = "Echo the given text on screen.";
    cmdEcho->Long  = "Echo the given text on screen. \nSimply like print but with the option to repeat yourself like Trump. It's the best, seriously.";
    cmdEcho->Run = [&](const Cobalt::Arguments& args) {
        for (int i=0; i<times; ++i) {
            for (int i=0; i<args.size(); ++i) {
                std::cout << args[i] << " ";
            }
            std::cout << std::endl;
        }
        return 0;
    };

    cmdEcho->PersistentFlags.Add<int>(times, "times", "t", 1, "The number of times to print the text");

    cobalt->AddCommand(cmdPrint);
    cobalt->AddCommand(cmdEcho);

    cobalt->Execute(argc, argv);
}
