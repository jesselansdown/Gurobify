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

GurobifySolve( file_name : OptionVariable1 := 7; OptionVariable2 := 3; OptionVariable4 := 100.9);

	where file_name is the name of the lp_file (and path if not in the current GAP working directory)

GurobifySolve( file_name, mat, sense, rhs : OptionVariable1 := 7; OptionVariable2 := 3; OptionVariable4 := 100.9);

	where	mat is a matrix of additional constraints
			sense is "<", ">" or "=" corresponding to each row of mat
			rhs is the value on the right hand side of the expression for each row of mat

	in both instances, it is possible to add as many option variables as desired from the following list (written exactly as expressed below).
	More information in these options can be found in the Gurobi documentation
	
		TimeLimit (Takes a floating value. Default is no time limit.)
		BestObjBound (Takes a floating value. Default is no best object bound.)
		NumericFocus (Takes an integer between 0 and 3. Default is 0.)
		LogToConsole (Takes either true or false. Default is false.)