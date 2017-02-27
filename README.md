# Gurobify
Gurobify provides an interface to Gurobi from GAP.

Requirements:
Gurobi 7.0
AutoTools (Can be installed on a mac using homebrew: 
	brew install autoconf
	brew install automake)

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
				eg [[1,0,1], [0,1,1], [1,1,0]]
			sense is a list of "<", ">" or "=" corresponding to each row of mat eg.
				["=", "=", "<"] (note that gurobi doesn't distinguish between <= and <)
			rhs is a list of the values on the right hand side of the expression for each row of mat
				eg [1,1, 2.5]

	in both instances, it is possible to add as many option variables as desired from the following
	list (written exactly as expressed below).More information in these options can be found in
	the Gurobi documentation
	
		TimeLimit (Takes a floating value. Default is no time limit.)
		BestObjBound (Takes a floating value. Default is no best object bound.)
		NumericFocus (Takes an integer between 0 and 3. Default is 0.)
		Cutoff (Takes a floating value. Default is no cut off.)
		LogToConsole (Takes either true or false. Default is false.)
		
The output is in the form
[ Gurobi optimization status code, objective value, solution ]
Refer to the gurobi documentation for the meanings of the optimization status codes.