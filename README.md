Cobalt is a header-only C++ library for creating modern CLI applications heavily inspired by the awesome Cobra go library.

# Overview

Cobalt provides a simple interface to create powerful and modern CLI interfaces similar to git.

It shares with Cobra the following features:
* subcommand-based CLIs: `app server`, `app fetch`, etc.
* Fully POSIX-compliant flags (including short & long versions)
* Nested subcommands
* Global, local and cascading flags
* Intelligent suggestions (`app srver`... did you mean `app server`?)
* Automatic help generation for commands and flags
* Automatic detailed help for `app help [command]`
* Automatic help flag recognition of `-h`, `--help`, etc.
* Command aliases so you can change things without breaking them

Applications built with Cobalt commands are designed to be as user-friendly as possible.
Flags can be placed before or after the command (as long as a confusing space isn't provided).
Both short and long flags can be used. Help is automatically generated and available for a specific command using either the help command or the `--help` flag.

# Concepts

# Installing

Cobalt is best used in a Cmake project. Usage in a project boils down to a simple

   > mkdir thirdparty && cd thirdparty
   > git submodule add https://github.com/crazyphysicist/cobalt.git
   > cp FindCobalt.cmake ../cmake/ && cd ..

and adding

   FIND_PACKAGE(Cobalt REQUIRED)
   ...
   INCLUDE_DIRECTORIES(${COBALT_INCLUDE_DIRS})

to your `CMakelists.txt` file.


