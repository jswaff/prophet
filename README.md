# prophet

An XBoard compatible chess engine 

## Introduction 

Prophet is a chess playing program written in C.  Technically it's a chess engine, meaning there is no graphical user interface.  To play a game with Prophet, you'll want to install Winboard (Windows) or XBoard (Linux/Mac).  See http://www.gnu.org/software/xboard for details.


## Design Goals

This codebase is the third major rewrite of Prophet.  The first verson was started in 2000.  The second version (first rewrite) was started sometime around 2007, and the last version (second rewrite) around 2011.  The last version of the codebase (https://github.com/jswaff/prophet3) was retired in 2018.  Some design goals for this codebase were:

* Use pure C.  
* Modularize.  Group source files into directories according to functionality.  E.g. movegen, eval, search, etc.
* Break the code down into more source files that are more focused in nature.
* Make better use of static functions.
* Make better use of the const qualifier.
* Improve the documentation.  Each function should have at least a brief description of its purpose, a listing of arguments and return value.
* Improve unit testing.  In general Prophet3 is well tested, but there are some areas the coverage could improve.  
* Use a proper test harness, e.g. GoogleTest or the like.  The release binary should not contain the test code.  Prophet3 uses assertions exclusively, and all the test code is built into the binary (even though it can't be executed when compiled with the NDEBUG flag).
* Make use of memory leak detectors such as Valgrind on each release.
* Produce a static library containing the move generation, evaluation, and search related functions.  The intent is to modularize the core functionality for inclusion in other projects.


To elaborate on that last point, although Prophet is a fully functional standalone chess engine, I tend to use it more as a "plug in engine" within chess4j more than I do as a standalone engine.  This hybrid approach allows me to enjoy the benefits of programming non-critical-path functions in a higher level language while still getting the raw speed of native code. Consequently, I have made a decision to NOT build an opening book or pondering support directly into Prophet, as both are handled by chess4j.  


## Installing

To build Prophet, you'll need a C compiler.  I use gcc and 'make'.

If you have the proper build tools, just do:

```
make
./prophet4
```

If you want to build the tests, you'll also need g++, and a copy of  Google Test from https://github.com/google/googletest .  Finally, set an environment variable GTEST_DIR to point to the 'googletest' project (we don't need the googlemock stuff).  Then do:

```
make test
./prophet4_test
```

## How Strong is it?

Strong enough to beat most humans, but not that strong as far as chess engines go. (I hope to change this.)

I estimate a 64 bit build would land somewhere in the 2200-2300 range on the CCRL list.


## Status and Roadmap

The "great rewrite", which started in 2019, is finally complete! This version is approximately 75 ELO stronger than P3, even though hardly anything new has been introduced.  That's mainly due to careful testing along the way.

I plan to focus on improving the evaluation function for a while.  At some point I'd like to try my hand at implementing a neural network, but in the short term I want to improve the "Hand Crafted Evaluation".  The first step in doing that will be to add some automated tuning using logistic regression ("Texel Tuning").  Once the existing weights are optimized, I can think of several new terms that need to be added.  Prophet doesn't understand pawns very well, and king safety is crude.  It lacks knowledge of even basic endgames.

The search is still single threaded.  That is actually a regression of sorts -- Prophet 2 was SMP capable (using Young Brother's Wait), but it will come.  I just choose to focus on the eval for a bit first.

