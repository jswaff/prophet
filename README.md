# prophet

An Xboard compatible chess engine 

## Introduction 

Prophet is a chess playing program written in C.  Technically it's a chess engine, meaning there is no graphical user interface.  To play a game with Prophet, you'll want to install Winboard (Windows) or Xboard (Linux/Mac).  See http://www.gnu.org/software/xboard for details.

Although Prophet is a fully functional chess engine, I tend to use it more as a "plug in engine" within [chess4j](https://github.com/jswaff/chess4j).  Consequently, it does not currently have an opening book or pondering support, as both are handled by chess4j.

## Using a Neural Network

By default, Prophet still uses a hand crafted evaluation.  You can enable a neural network based evaluation using the 'n' command line parameter.

```
./prophet -n nn-24-q.txt 
```

You should see a confirmation that the network was loaded:

```
loading neural network from nn-24-q.txt
```

The supplied network weights were trained using [chess-trainer](https://github.com/jswaff/chess-trainer).  chess-trainer is a Pytorch based trainer I wrote, with some influence from David Carteau's excellent [Cerebrum library](https://github.com/david-carteau/cerebrum).  The inference code within Prophet (particularly the intrinsics) were used almost verbatim.  Thank you David!


## Building from Source

Binaries are provided for Linux and Windows based systems, but if you want to build Prophet from source, you'll need a Linux based environment with gcc/g++ and cmake.  If you have the proper build tools, just do:

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make install
```

If all went well, the 'prophet' and 'prophet_test' binaries will be in the build folder.

You may be able to compile Prophet on other POSIX compliant operating systems, but I haven't tried it.  If you'd like to build a Windows binary, Cygwin does work, but native Windows builds are not currently supported.  I plan to investigate this in the future.

## How Strong is it?

The CCRL Blitz list (https://www.computerchess.org.uk/ccrl/404/) shows Prophet 4.3 to be rated at 2480.  Prophet 5.0 should be around 100 ELO stronger according to my testing. 

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
