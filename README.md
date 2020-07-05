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


## Status

The rewrite is far enough along now that Prophet4 is a completely functional XBoard compatible chess engine.  Search enhancements are being added and carefully tested.  The engine will be released when it is 100 ELO stronger than its predecessor, Prophet3.

See my website http://jamesswafford.com for the latest updates.
