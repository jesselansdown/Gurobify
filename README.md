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
GurobiSolve(file_name);
where file name is the name of the lp file as a string, with the path if it is not in the current GAP working directory