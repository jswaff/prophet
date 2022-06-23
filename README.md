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
./prophet4
```

If you want to build the tests, you'll also need g++, and a copy of  Google Test from https://github.com/google/googletest .  Finally, set an environment variable GTEST_DIR to point to the 'googletest' project (we don't need the googlemock stuff).  Then do:

```
make test
./prophet4_test
```

## How Strong is it?

Strong enough to beat most humans, but not that strong as far as chess engines go. (I hope to change this.)

The CCRL Blitz list (https://www.computerchess.org.uk/ccrl/404/) shows Prophet 4.1 to be around 2359.  Prophet 4.2 should be around 50 ELO stronger based on the results below:

|Rank |Name               |Elo    |+    |- |games |score |oppo. |draws  |
|----:|:------------------|------:|----:|-:|-----:|-----:|-----:|-----: |
|   1 |tantabus-2.0.0     | 82    |3    |3 |46250 |  63% |  -10 |  22%  |
|   2 |arasan-13.4        | 67    |3    |3 |46250 |  60% |   -8 |  21%  |
|   3 |barbarossa-0.6.0   | 58    |3    |3 |46250 |  59% |   -7 |  20%  |
|   4 |qapla-0.1.1        | 31    |3    |3 |46250 |  55% |   -4 |  24%  |
|   5 |prophet-4.2        | 24    |3    |3 |40000 |  53% |    5 |  24%  |
|   6 |loki-3.5           | 23    |3    |3 |46250 |  54% |   -3 |  23%  |
|   7 |myrddin-0.88       | -2    |3    |3 |46250 |  50% |    0 |  24%  |
|   8 |prophet-4.1        |-36    |3    |3 |40000 |  44% |    5 |  23%  |
|   9 |tjchess-1.3        |-83    |3    |3 |46250 |  37% |    8 |  21%  |
|  10 |jazz-840          |-134    |3    |3 |46250 |  30% |   14 |  19%  |
|  11 |prophet4          |-141    |7    |7 |10000 |  30% |    5 |  18%  |


## Status and Roadmap

The focus of the 4.2 release was to improve the evaluation.

* Evaluation terms were tuned using logistic regression with gradient descent (within chess4j)
* Fully implemented tapered evaluation
* Added simple mobility terms for bishop and queen
* Added a pawn hash table

I plan to continue to focus on improving the evaluation function for a while, particularly pawns and endgames.  Eventually I'd like to implement a neural network.

The search is still single threaded.  That is actually a regression of sorts; Prophet 2 was SMP capable (using Young Brothers Wait), but it will come.  I just choose to focus on the eval for a bit first.

You can see the combined Prophet / chess4j backlog here: https://trello.com/b/dhcOEaCO/chess4j-board .

