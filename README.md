# prophet

An Xboard compatible chess engine 

## Introduction 

Prophet is a chess playing program written in C.  Technically it's a chess engine, meaning there is no graphical user interface.  To play a game with Prophet, you'll want to install Winboard (Windows) or Xboard (Linux/Mac).  See http://www.gnu.org/software/xboard for details.

## Using a Neural Network

By default, Prophet still uses a handcrafted evaluation.  You can enable a neural network based evaluation using the 'n' command line parameter.

```
./prophet -n nn-52-q.txt 
```

You should see a confirmation that the network was loaded:

```
loading neural network from nn-52-q.txt
```

The supplied network weights were trained using [chess-trainer](https://github.com/jswaff/chess-trainer).  chess-trainer is a Pytorch based trainer I wrote, with some influence from David Carteau's excellent [Cerebrum library](https://github.com/david-carteau/cerebrum).  The inference code within Prophet (particularly the intrinsics) were used almost verbatim.  Thank you David!

## Configuring Hash Tables

You can control the main hash table and pawn hash table sizes (in MB) using the 'h' and 'p' command line parameters.

```
./prophet -h 256 -p 32
```

This sets a 256 MB main hash table and a 32 MB pawn hash table.
Defaults are 64 MB for the main hash table and 8 MB for the pawn hash table.

## Developer API

Public API documentation lives in `docs/API.md`.

## Building from Source

Binaries are provided for Linux and Windows based systems, but if you want to build Prophet from source, you'll need either a Linux environment with gcc/g++ or Windows with MSVC. 

Just follow the standard cmake build process:

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

## How Strong is it?

The CCRL Blitz list (https://www.computerchess.org.uk/ccrl/404/) shows Prophet 5.1 to be rated at 2702.  My testing indicates Prophet 5.2 is 33 elo stronger than 5.1.


## Release Notes

5.2

* Improved neural network
* Native Windows build

5.1

* changed neural network architecture to 2x(768->384)->2
* factored quantization error into network training
* moved several functions that should be internal out of public headers
* lots of code cleanup

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
