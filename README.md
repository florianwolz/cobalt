Cobalt is a header-only C++ library for creating modern CLI applications heavily inspired by the awesome [Cobra](http://github.com/spf13/cobra) go library.

# Overview

Cobalt provides a simple interface to create powerful and modern CLI interfaces similar to git.

It shares with Cobra the following features:
- [x] subcommand-based CLIs: `app server`, `app fetch`, etc.
- [x] Fully POSIX-compliant flags (including short & long versions)
- [x] Nested subcommands
- [x] Global, local and cascading flags
- [x] Intelligent suggestions (`app srver`... did you mean `app server`?)
- [ ] Automatic help generation for commands and flags
- [ ] Automatic detailed help for `app help [command]`
- [ ] Automatic help flag recognition of `-h`, `--help`, etc.
- [x] Command aliases so you can change things without breaking them

Applications built with Cobalt commands are designed to be as user-friendly as possible.
Flags can be placed before or after the command (as long as a confusing space isn't provided).
Both short and long flags can be used. Help is automatically generated and available for a specific command using either the help command or the `--help` flag.

Right now, Cobalt is still in an early development stage and it is not recommended to use it in production code,
as important changes to the API are more than likely in the future.

# Concepts

## Command

## Arguments

## Flags

# Usage

There are two ways Cobalt can be used within projects. For small applications, one can generate the commands
directly inside of the main function.

```cpp
// main.cpp
#include <iostream>
#include <cobalt.hpp>

int main(int argc, char** argv) {
    auto root = Cobalt::CreateCommand();
    root->Use = "echo";

    int times; // Number of times to print the text

    auto cmdPrint = Cobalt::CreateCommand();
    cmdPrint->Use = "print [text to print]";
    cmdPrint->Short = "Print the given text to screen";
    cmdPrint->Long = "Print the given text to screen.";
    cmdPrint->Run = [&](const Cobalt::Arguments& args) -> int {
        for (int i=0; i<times; ++i) {
            for (auto& arg : args) {
                std::cout << arg << " ";
            }
            std::cout << std::endl;
        }
        return 0;
    };

    // Register a flag for the number of times
    cmdPrint->LocalFlags.Add<int>(times, "times", "t", 1, "Number of times to repeat the text");

    // Add the command to the root command
    root->AddCommand(cmdPrint);

    // Parse
    root->Execute(argc, argv);
}
```
For large applications, the commands can be declared inside of separate
classes. This allows to split the commands into separate headers. The echo example from
above could be implemented in the following fashion:
```cpp
// cmd/print.hpp
#include <iostream>
#include <cobalt.hpp>

class PrintCommand : public Cobalt::Command<PrintCommand> {
public:
    static std::string Use() {
        return "print [text to print]";
    }

    static std::string Short() {
        return "Print the given text to screen.";
    }

    static std::string Long() {
        return "Print the given text to screen.";
    }

    void RegisterFlags() {
        AddLocalFlag<int>(times, "times", "t", 1, "Number of times to repeat the text");
    }

    int Run(const Cobalt::Arguments& args) {
        for (int i=0; i<times; ++i) {
            for (auto& arg : args) {
                std::cout << arg << " ";
            }
            std::cout << std::endl;
        }
        return 0;
    }
private:
    int times;
};

// cmd/root.hpp
#include <cobalt.hpp>

#include "print.hpp"

class RootCommand : public Cobalt::Command<RootCommand, PrintCommand> {
public:
    static std::string Use() {
        return "echo";
    }
};

// main.cpp
#include <cobalt.hpp>
#include "cmd/root.hpp"

int main(int argc, char** argv) {
    return Cobalt::Execute<RootCommand>(argc, argv);
}

```

# Installing

Cobalt is best used in a Cmake project. Usage in a project boils down to a simple

```
$ mkdir thirdparty && cd thirdparty
$ git submodule add https://github.com/crazyphysicist/cobalt.git
$ cp FindCobalt.cmake ../cmake/ && cd ..
```

and adding

```cmake
FIND_PACKAGE(Cobalt REQUIRED)
...
INCLUDE_DIRECTORIES(${COBALT_INCLUDE_DIRS})
```
to your `CMakelists.txt` file.

# Copyright

Crafted with :heart: and lots of :coffee:.

