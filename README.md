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


To read an lp file to a model do:
	model := GurobiReadLP("lp_file_name.lp");
where "lp_file_name.lp" is the name of the lp file as a string, including the path if it is not located in the current GAP working directory.

To optimise the model created in the previous step do:
	GurobiSolveModel(model);

To change a parameter of the model do:
	GurobiSetParameter(model, ParameterName, ParameterValue);
where ParameterName is a string corresponding to the parameter, exactly as given in the Gurobi documentation. Parameter value must be either an integer or a double, depending on the parameter being altered.
For example
	GurobiSetParameter(model, "timelimit", 10.0);
would set the time limit to 10 seconds. Note that ParameterName is case insensitive, so "TimeLimit", "tIMeLiMit" and "TIMELIMIT" are all equivalent.

To find the current value of a parameter do
	GurobiGetParameter(model, ParameterName);

Note that Gurobify can only handle parameters which are of int or double type. This is most of the parameters, but there are a few such as "InputFile " which take a string value and are currently not supported.