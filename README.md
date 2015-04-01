# SupportCPP
Some support functions and classes in C++. Mostly are written in pure STL, so they are Windows and Linux compatible, using any standard C++ compiler.
I use these classes a lot, because I've never found good implementations for them. Here are some descriptions:

## TDateTime
Class using pure STL to manage date and/or time. It's a wrapper for <ctime>, that uses a very odd structure. This classes can use timestamp and Julian calendar as well (the Julian calendar is used by Excel).

## TYearMonth
Class that gives a year/month type. It's pretty simple, but it has incremental operators for months scanning, so it comes to hand in data mining.

## TMultiFit
This is a multivariate linear fitting tool. I how that GSL has an implementation but it's based in bayesian distributions, which is not 100% true. For nonlinear series, it's better to use a nonlinear solver for the least squares method.
Please note that this class uses NLOpt (http://ab-initio.mit.edu/wiki/index.php/NLopt) as the solver. So you'll need to install it before using TMultiFit.

## TConfigFile
Class to use Linux-style configuration files. It can read and write files that names variables as VarName = VarValue, even if VarValue has spaces and other strange chars. Also accepts comments in lines starting with '#'.

## TPrecisionTimer
It's not actually a very "useful" class, but it helps to benchmarking parts of the code. Sometimes it needs a little tweaking to work in multiple OSes, but I pretend to add macros to help compiling it. It returns a double precision with seconds (and therefore miliseconds) of a piece of code.

## Friends
Various functions to help manipulate containers and strings. Usually I don't include it fully in projects, but only the functions used. Also, note that some functions in Friends are copied to the classes, to avoid dependancy.
