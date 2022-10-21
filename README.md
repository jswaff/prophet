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


To elaborate on that last point, although Prophet is a fully functional standalone chess engine, I tend to use it more as a "plug in engine" within chess4j more than I do as a standalone engine.  This hybrid approach allows me to enjoy the benefits of programming non-critical-path functions in a higher level language while still getting the raw speed of native code. Consequently, I have made a decision to NOT build an opening book or pondering support directly into Prophet, as both are handled by chess4j.  (Of course I may change my mind in the future!)


## Installing

To build Prophet, you'll need a C compiler.  I use gcc and 'make'.

If you have the proper build tools, just do:

```
make
./prophet4_3
```

If you want to build the tests, you'll also need g++, and a copy of  Google Test from https://github.com/google/googletest .  Finally, set an environment variable GTEST_DIR to point to the 'googletest' project (we don't need the googlemock stuff).  Then do:

```
make test
./prophet4_test
```

## How Strong is it?

Strong enough to beat most humans, but not that strong as far as chess engines go. (I hope to change this.)

The CCRL Blitz list (https://www.computerchess.org.uk/ccrl/404/) shows Prophet 4.2 to be rated at 2443.  Prophet 4.3 should be around 50 ELO stronger based on the results below:

Rank Name                              Elo    +    - games score oppo. draws
   1 tantabus-2.0.0                     75    3    3 51250   61%    -7   22%
   2 prophet-4.3                        71    3    3 40000   60%    -1   25%
   3 arasan-13.4                        62    3    3 51250   59%    -6   21%
   4 barbarossa-0.6.0                   52    3    3 51250   58%    -5   21%
   5 qapla-0.1.1                        24    3    3 51250   54%    -2   24%
   6 prophet-4.2                        18    3    3 40000   53%    -1   24%
   7 loki-3.5                           16    3    3 51250   52%    -1   23%
   8 myrddin-0.88                       -9    3    3 51250   48%     1   25%
   9 prophet-4.1                       -42    3    3 40000   44%    -1   23%
  10 tjchess-1.3                       -88    3    3 51250   36%     9   22%
  11 jazz-840                         -140    3    3 51250   29%    14   19%
  12 prophet4                         -147    7    7 10000   30%    -1   18%

## Status and Roadmap

4.3:

* Passed pawn by rank (was a single value)
* Non-linear mobility (was a single value)
* Knight outposts
* Trapped bishop penalty

4.2:

* Evaluation terms were tuned using logistic regression with gradient descent (within chess4j)
* Fully implemented tapered evaluation
* Added simple mobility terms for bishop and queen
* Added a pawn hash table


I would still like to improve Prophet's understanding of pawns, but at this point I'm turning my attention to implementing a neural network.

The search is still single threaded.  That is actually a regression of sorts; Prophet 2 was SMP capable (using Young Brothers Wait), but it will come.  I just choose to focus on the eval for a bit first.

You can see the combined Prophet / chess4j backlog here: https://trello.com/b/dhcOEaCO/chess4j-board .

