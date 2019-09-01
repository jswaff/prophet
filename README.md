# prophet4

Prophet4 is, or will be, a chess playing program written in C.  It is the successor to Prophet3, and as the name suggests, the third major rewrite of the Prophet chess engine.


## Design Goals


* Use pure C.  Prophet3 is "nearly C" anyway.  It is not an object oriented program. 
* Modularize the codebase.  Group source files into directories according to functionality.  E.g. movegen, eval, search, etc.
* Break the code down into more source files that are more focused in nature.
* Make better use of static functions.
* Make better use of the const qualifier.
* Improve the documentation.  Each function should have at least a brief description of its purpose, a listing of arguments and return value.
* Improve testing coverage.  In general Prophet3 is well tested, but there are some areas the coverage could improve.  
* Use a proper test harness, e.g. GoogleTest or the like.  The release binary should not contain the test code.  Prophet3 uses assertions exclusively, and all the test code is built into the binary (even though it can't be executed when compiled with the NDEBUG flag).
* Make use of memory leak detectors such as Valgrind on each release.
* Produce a static library containing the move generation, evaluation, and search related functions.  It will not include the opening book related code or the Xboard protocol related code.  The intent is to modularize the core functionality for inclusion in other projects.
* Retire SQLite.  It never felt like a relational database was a good fit for the opening book.  Replace with a Key-Value store type database, possibly LMDB.


## Status

9/24/19 - evaluator complete.  This is a verbatim "migration" of Prophet3's evaluator.  It was a little painful really, because the evaluation routines are clearly a major weakness in the program, but I think it's important to port everything over and run some comparisons before making any changes.

I found the "random move selection" kind of fun, so a command line parameter was created to enable random mode.  Just start the program with the '-r' option.

The next phase of the rewrite will be the search.  Before starting this phase I'm going to try a small side project.  I'm going to add a JNI layer to chess4j to enable it to call native code from Prophet4.  If it works out, this would have some implications for future development.  

1) It may be easier to do some testing from the Java tier where we can use proper mocks without resorting to function pointers (the indirection probably slows the native code down anyway).

2) If it works out really well I may be less inclined to implement some things in C, such as an opening book or PGN facilities.  Possibly not even pondering. 

3) I've had in mind to do some distributed computing work (not to be confused with parallel processing).  Perhaps I could do this from the Java tier where there are more supporting libraries.  

4) What about machine learning experiments?  Best done from a higher level language?

Those are just some thoughts.  Since I'm not on a timeline and not getting paid to do this I have the luxury of playing around a little. :-)


8/30/19 - protocol handler complete.  Enough of the XBoard protocol has been implemented to play a complete game.  Move selection is completely random.  A 20,000 game match was run between identical copies of the program (which didn't take long since it moves instantly), and as predicted the outcome was almost exactly 50%.  Interestingly, 85% of the games resulted in a draw.

The next area of focus will be implementing the evaluation routine, which gives a static analysis of a position.  This will be a direct port of the evaluator from Prophet3.  The evaluation needs a lot of work, but forward progress will come after the rewrite is complete.  Once the evaluator is in place I can make the engine just slightly smarter than a complete random mover.  I'm also considering a JNI integration with chess4j, as a proof of concept for future work.

6/16/19 - move generator complete.  Google Test is being used for the test code.  Many tests have been added to validate all the functionality added thus far, including a wide variety of perft type functions to test move generation in a number of edge cases.  Perft numbers all check out.  Move gen speed is comparable to Prophet3, which is to be expected.  Valgrind has not found any memory leaks.

The next step will be to add the scaffolding for the protocol handler decode-and-dispatch mechanism.  There will be one protocol handler that implements the XBoard protocol, and a smaller handler to add some custom commands.  Following that I will likely move on to the evaluation routines.

