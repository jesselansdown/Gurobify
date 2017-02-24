# Gurobify
Gurobify provides an interface to Gurobi from GAP.

To install the Gurobify package, move it to the pkg directory of the GAP installation directory.
Inside the Gurobify directory run:
./autogen.sh (Requires AutoTools installed)
./configure --with-gurobi=<gurobi path> [--with-gaproot=<gap path>]
make

To load package in GAP:
LoadPackage( "Gurobify");
To solve an lp file:
GurobiSolve(file_name, TimeLimitOn, TimeLimitValue, ConsoleOutputON);
where file_name is the name of the lp file as a string, with the path if it is not in the current GAP working directory
TimeLimitON is a true false switch to activate the time_limit option of gurobi
TimeLimitValue gives the value of the time limit if TimeLimitON is true
ConsoleOutputOn is a true/false switch to turn the gurobi console logging on or off

Additionally, there is a wrapper function which can be used with options values.
GurobifySolve( file_name : TimeLimit := x, ToConsole := y);
where TimeLimit is an option value that switches on the time limit and sets it to the value x (which must be a double)
ToConsole turns console logging on when y = true