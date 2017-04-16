#include <iostream>

#include "../cobalt.hpp"

class PrintCommand : public Cobalt::Command<PrintCommand> {
public:
    static std::string Use() {
        return "print [text to print]";
    }

    static std::string Short() {
        return "Prints the given text on screen.";
    }

    static std::string Long() {
        return "Prints the given text on screen. \nThis has been done for centuries.";
    }

    static int Run(const Cobalt::Arguments& args) {
        for (int i=0; i<args.size(); ++i) {
            std::cout << args[i] << " ";
        }
        return 0;
    };
};

class EchoCommand : public Cobalt::Command<EchoCommand> {
public:
    static std::string Use() {
        return "echo [text to print]";
    }

    static std::string Short() {
        return "Echo the given text on screen.";
    }

    static std::string Long() {
        return "Echo the given text on screen. \nSimply like print but with the option to repeat yourself like Trump. It's the best, seriously.";
    }

    void RegisterFlags() {
        AddLocalFlag<int>(times, "times", "t", 1, "The number of times to print the text");
    }

    int Run(const Cobalt::Arguments& args) {
        for (int i=0; i<times; ++i) {
            for (int i=0; i<args.size(); ++i) {
                std::cout << args[i] << " ";
            }
            std::cout << std::endl;
        }
        return 0;
    };
private:
    int times;
};

class RootCommand : public Cobalt::Command<RootCommand, PrintCommand, EchoCommand> {
public:
    static std::string Use() {
        return "echooop";
    }
};

int main(int argc, char** argv) {
    return Cobalt::Execute<RootCommand>(argc, argv);
}
