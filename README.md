![logo](https://raw.githubusercontent.com/wiki/crazyphysicist/cobalt/logo.png "Cobalt")

Cobalt is a header-only C++ library for creating modern CLI applications heavily inspired by the awesome [Cobra](http://github.com/spf13/cobra) go library.

# Overview

Cobalt provides a simple interface to create powerful and modern CLI interfaces similar to git.

It shares with Cobra the following features:
- [x] subcommand-based CLIs: `app server`, `app fetch`, etc.
- [x] Fully POSIX-compliant flags (including short & long versions)
- [x] Nested subcommands
- [x] Global, local and cascading flags
- [x] Intelligent suggestions (`app srver`... did you mean `app server`?)
- [x] Automatic help generation for commands and flags
- [x] Automatic detailed help for `app help [command]`
- [x] Automatic help flag recognition of `-h`, `--help`, etc.
- [x] Command aliases so you can change things without breaking them
- [x] Two ways to declare your application: inside of the main function and via command classes

Applications built with Cobalt commands are designed to be as user-friendly as possible.
Flags can be placed before or after the command (as long as a confusing space isn't provided).
Both short and long flags can be used. Help is automatically generated and available for a specific command using either the help command or the `--help` flag.

Right now, Cobalt is still in an early development stage and it is not recommended to use it in production code,
as important changes to the API are more than likely in the future.

# Concepts

Cobalt uses a structure of commands, flags and arguments.

## Command

Commands build the essential structure of your application. The functionality your application will support
is implemented in commands. A command can contain children and run an action.

A command has the following (simplified) properties:
```cpp
struct Command {
    // The one-line usage message.
    std::string Use;
    // An array of aliases that can be used instead of the first word in use.
    std::vector<std::string> Aliases;
    // The short description shown in the 'help' output.
    std::string Short;
    // The long message shown in the 'help <this-command>' output.
    std::string Long;
    // Examples of how to use the command.
    std::string Example;
    // Is this command deprecated and should print this string when used?
    std::string Deprecated;
    // Is this command hidden and should NOT show up in the list of available commands?
    bool Hidden=false;
};
```
If you implement the application with the inline DSL, you can directly manipulate these properties above to
change the behaviour of your application. They are created via:
```cpp
auto cmd = Cobalt::CreateCommand();
```
A smart pointer to the command is returned that you can use to change the properties.

For an application built with the class DSL you inherit from `Cobalt::Command<Name_Of_Your_Command>` and implement `static` functions
with the name of the property and return the value, i.e.
```cpp
class ServeCommand : public Cobalt::Command<ServeCommand> {
public:
    static std::string Use() {
        return "serve";
    }
};
```

## Flags

Flags give the user additional ways to configure the application. Cobalt supports full POSIX-compliant flags.
A command can define flags that persist through to all children or flags that are only available to that command.

For example, in
```
$ app command --debug --port=8080
```
two flags are set: `debug` mode is turned on and the `port` is set to 8080.

# Getting Started

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
classes. This allows to split the commands into separate headers. While you are free to organize the project
as you like, typically a Cobalt application has the following organization
```
▾ appName/
  ▾ cmd/
      print.hpp
      root.hpp
  main.cpp
```
The echo example from above could be implemented in the following fashion:
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

## Creating your main command

The main command is the root of the command tree in an application.
```cpp
auto root = Cobalt::CreateCommand();
```
or
```cpp
class RootCommand : public Cobalt::Command<RootCommand> {
// ...
};
```

### Adding subcommands

Subcommands are created in the same way as the main command above. Afterwards it
has to be linked to its parent.

Subcommands are added either by using the `AddCommand` function
```cpp
cmd->AddCommand(subcmd);
```
or by specifying its name as template parameter during the declaration of the class:
```cpp
class Command : public Cobalt::Command<Command, SubCommand> {
// ...
};
```
Cobalt will automatically expand this and link the commands properly.

### Executing

For the inline DSL calling the `Execute` method will start the parsing of the arguments:
```cpp
return root->Execute(argc, argv);
```
With the class DSL you simply call the ```Cobalt::Execute``` template class with the name 
of your root command class.
```cpp
return Cobalt::Execute<RootCommand>(argc, argv);
```

## Flags

Flags provide modifiers to control how the action command operates. They can be either local or persistent through
all children commands.

In inline mode, the variables are simply connected to the flags by calling `cmd->LocalFlags.Add` or `cmd->PersistentFlags.Add`
```cpp
int port;
bool debug;

serve->LocalFlags.Add<int>(port, "port", "p", 8080, "The port of the server");
serve->PersistentFlags.Add<bool>(debug, "debug", "d", false, "Debug mode. Print all steps!");
```

When using the class DSL, flags are registered by extending the `RegisterFlags` method
```cpp
class ServeCommand : public Cobalt::Command<ServeCommand> {
public:
    // ...

    void RegisterFlags() {
        // Add local flag
        AddLocalFlag<int>(port, "port", "p", 8080, "The port of the server");

        // Add persistent flag
        AddPersistentFlag<bool>(debug, "debug", "d", false, "Debug mode. Print all steps!");
    }

    int Run(const Cobalt::Arguments& args) {
        // Pseudo code to start the server
        Http::Server server(port);

        // Lookup the persistent flag
        if (Lookup<bool>("debug")) {
            std::cout << "Started server on port " << port << std::endl;
        }
    }
private:
    int port;
    bool debug;
};
```
Here, the flags are private members of the command. For local flags one can simply access the property directly.
In case of persistent flags this is only possible if the flag is defined at this level. To access the persistent
flags of parent commands, use the `Lookup` template function as in the example above.

# Installing

Cobalt is best used in a Cmake project. Usage in a project boils down to a simple

```
> mkdir thirdparty && cd thirdparty
> git submodule add https://github.com/crazyphysicist/cobalt.git
> cp FindCobalt.cmake ../cmake/ && cd ..
```

and adding

```cmake
FIND_PACKAGE(Cobalt REQUIRED)
...
INCLUDE_DIRECTORIES(${COBALT_INCLUDE_DIRS})
```
to your `CMakelists.txt` file.

# Advanced topics

## Pre-/PostRun Hooks

Before and after the run action additional hooks can be registered. These hooks can
be persistent, i.e. they are inherited to subcommands, or local. The order of
execution is

 - Persistent Pre-Run hook
 - Pre-Run hook
 - Run action
 - Post-Run hook
 - Persistent Post-Run hook

In the inline DSL, you simply override the `PersistentPreRun`, `PreRun`, `PostRun` and `PersistentPostRun` functions in the command pointer
with `void(const Arguments& args)` lambdas/functions.

With the class DSL the hooks are implemented as class member functions with the same signature as above, i.e.

```cpp
class Command : public Cobalt::Command<Command> {
public:
    void PersistentPreRun(const Cobalt::Arguments& args) {
        std::cout << "Persistent pre run was called." << std::endl;
    }
};
```

# License

Cobalt is released under the Apache 2.0 licence. See [LICENSE](https://github.com/crazyphysicist/cobalt/blob/master/LICENSE)

Crafted with :heart: and lots of :coffee:.

