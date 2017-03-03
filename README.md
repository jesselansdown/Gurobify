# Gurobify
Gurobify provides an interface to Gurobi from GAP.

Requirements:
* Gurobi 7.0
* AutoTools 

(AutoTools can be installed on a mac using homebrew: 
	- brew install autoconf
	- brew install automake)

To install the Gurobify package, move it to the pkg directory of the GAP installation directory.
Inside the Gurobify directory run:

* ./autogen.sh (Requires AutoTools installed)

* ./configure --with-gurobi=\<gurobi path> [--with-gaproot=\<gap path>]

* make

* "quit;" | gap.sh makedoc.g

To load package in GAP:

		LoadPackage( "Gurobify");


To read an model file (such as an lp file) to a model do:

		model := GurobiReadModel("model_file_name");

where "model_file_name" is the name of the model file as a string, including the path if it is not located in the current GAP working directory. See gurobi documentation to see which model files it supports.

To create new model, do

		GurobiNewModel( VariableTypes, ObjectiveFunction);

where variable types is a list consisting of "BINARY", "CONTINUOUS", "INTEGER", "SEMICONT", or "SEMIINT", for each variable in the model, and ObjectiveFunction is a list with the coefficients of each variable in the objective function, which must be floats.

For example, to create a model with three binary variables x_1, x_2, x_3 and  objective function

		x_1 + 3 x_2

we would do

		GurobiNewModel( ["BINARY", "BINARY", "BINARY"], [1., 3., 0. ]);

where the model will be set by default to minimise the objective function. This can be changed by changing the ModelSense.

To add a constraint to the model do:

		GurobiAddConstraint( model, eqn, sense, rhs);

where eqn is a list indexed by the variables, with the value at that index in the list representing the coefficient of the variable. Sense is either "=", "<" or ">" as a string, recalling that Gurobi does not differentiate between <= and < or >= and >. Lastly, rhs is the value on the right hand side of the expression. For example, to add the constraint

		2 x_1 + 3.5 x_2  - x_3  <= 8.2

we would use the following command

		GurobiAddConstraint(model, [ 2, 3.5, -1 ], "<", 8.2 );


To optimise the model created in the previous step do:

		GurobiOptimizeModel(model);

This will return the Gurobi status code which indicates the outcome of the optimization. See the Gurobi documentation for more information on their meanings.

To change a parameter of the model do:

		GurobiSetParameter(model, ParameterName, ParameterValue);

where ParameterName is a string corresponding to the parameter, exactly as given in the Gurobi documentation. Parameter value must be either an integer or a double, depending on the parameter being altered.
For example

		GurobiSetParameter(model, "timelimit", 10.0);

would set the time limit to 10 seconds. Note that ParameterName is case insensitive, so "TimeLimit", "tIMeLiMit" and "TIMELIMIT" are all equivalent.

To find the current value of a parameter do

		GurobiGetParameter(model, ParameterName);

Note that Gurobify can only handle parameters which are of int or double type. This is most of the parameters, but there are a few such as "InputFile " which take a string value and are currently not supported.

To change an attribute of the model do:

		GurobiSetAttribute(model, AttributeName, AttributeValue);

where AttributeName is a string, as given in the Gurobi documentation. The AttributeValue must be an integer or a double, depending on the attribute.
For example

		GurobiSetAttribute(model, "ModelSense", -1);

would change the sense of model to Maximise. The default sense is to minimise the objective function.

To find the current value of an attribute do

		GurobiGetAttribute(model, AttributeName);

Only works for int or double attributes.

To obtain an attribute array, do

		GurobiGetAttributeArray(model, AttributeName);

For example, given a model was able to be successfully optimised, the optimal solution can be viewed by:

		GurobiGetAttributeArray(model, "X");

Note that Gurobi does not update immediately, and so it is necessary to either optimize the model, or update the model manually by

		GurobiUpdateModel(model);
		
It is also possible to write a model to a file. The type of file is determined by the suffix of the file name given. For example

		GurobiWriteToFile(model, "file.lp");

would return an lp file. It is also possible to write .mps, .rew, .lp, or .rlp files, or files that write the solution such as .sol. For more, see the Gurobi documentation.


For more information on Gurobi parameters, attributes, and status codes, see the following links:

Attributes: http://www.gurobi.com/documentation/7.0/refman/attributes.html

Parameters: http://www.gurobi.com/documentation/7.0/refman/parameters.html

Status codes: https://www.gurobi.com/documentation/7.0/refman/optimization_status_codes.html
