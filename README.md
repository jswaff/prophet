# prophet

An XBoard compatible chess engine 

## Introduction 

Prophet is a chess playing program written in C.  Technically it's a chess engine, meaning there is no graphical user interface.  To play a game with Prophet, you'll want to install Winboard (Windows) or XBoard (Linux/Mac).  See http://www.gnu.org/software/xboard for details.

Although Prophet is a fully functional standalone chess engine, I tend to use it more as a "plug in engine" within [chess4j](https://github.com/jswaff/chess4j) more than I do as a standalone engine.  This hybrid approach allows me to enjoy the benefits of programming non-critical-path functions in a higher level language while still getting the raw speed of native code. Consequently, I have made a decision to NOT build an opening book or pondering support directly into Prophet, as both are handled by chess4j.  (Of course I may change my mind in the future!)

## Using a Neural Network

By default, Prophet still uses a hand crafted evaluation.  You can enable a neural network based evaluation using the 'n' command line parameter.

```
prophet -n nn-24-q.txt 
```

You should see a confirmation that the network was loaded:

```
loading neural network from nn-24-q.txt
```



## Installing

To build Prophet, you'll need a C compiler.  I use gcc/g++ and 'cmake'.

If you have the proper build tools, just do:

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make install
```

If all went well, the 'prophet' and 'prophet_test' binaries will be in the build folder.


## How Strong is it?

Strong enough to beat most humans, but not that strong as far as chess engines go. (I hope to change this.)

The CCRL Blitz list (https://www.computerchess.org.uk/ccrl/404/) shows Prophet 4.3 to be rated at 2480.  Prophet 5.0 should be around 100 ELO stronger based on the results below:

|Rank |Name               |Elo    |+    |- |games |score |oppo. |draws  |
|----:|:------------------|------:|----:|-:|-----:|-----:|-----:|-----: |
|   1 | tantabus-2.0.0    |   75  |  3  |3 | 51250|  61% |   -7 |   22% |
|   2 | prophet-4.3       |   71  |  3  |3 | 40000|  60% |   -1 |   25% |
|   3 | arasan-13.4       |   62  |  3  |3 | 51250|  59% |   -6 |   21% |
|   4 | barbarossa-0.6.0  |   52  |  3  |3 | 51250|  58% |   -5 |   21% |
|   5 | qapla-0.1.1       |   24  |  3  |3 | 51250|  54% |   -2 |   24% |
|   6 | prophet-4.2       |   18  |  3  |3 | 40000|  53% |   -1 |   24% |
|   7 | loki-3.5          |   16  |  3  |3 | 51250|  52% |   -1 |   23% |
|   8 | myrddin-0.88      |   -9  |  3  |3 | 51250|  48% |    1 |   25% |
|   9 | prophet-4.1       |  -42  |  3  |3 | 40000|  44% |   -1 |   23% |
|  10 | tjchess-1.3       |  -88  |  3  |3 | 51250|  36% |    9 |   22% |
|  11 | jazz-840          | -140  |  3  |3 | 51250|  29% |   14 |   19% |
|  12 | prophet4          | -147  |  7  |7 | 10000|  30% |   -1 |   18% |

## Release Notes

5.0

* NNUE!  Prophet now uses a neural network
* more cleanup - public headers reduced to just those needed to support chess4j
* Some speed optimizations

4.4

* Maintenance release.  Updated build system, switching from plain make to cmake.  Formatted / cleaned up code.

4.3

* Passed pawn by rank (was a single value)
* Non-linear mobility (was a single value)
* Knight outposts
* Trapped bishop penalty

4.2

* Evaluation terms were tuned using logistic regression with gradient descent (within chess4j)
* Fully implemented tapered evaluation
* Added simple mobility terms for bishop and queen
* Added a pawn hash table

## Roadmap

In the short term, I plan to continue experimenting with neural networks and looking for optimizations to offset the speed hit.  Also, I've accumulated a pretty healthy backlog of items to try in the search.  So, there will likely be a number of minor releases in the 5.x line.


You can see the combined Prophet / chess4j backlog here: https://trello.com/b/dhcOEaCO/chess4j-board .

Read about the latest development efforts at https://jamesswafford.dev/ .
