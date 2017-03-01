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

To load package in GAP:

		LoadPackage( "Gurobify");


To read an model file (such as an lp file) to a model do:

		model := GurobiReadModel("model_file_name");

where "model_file_name" is the name of the model file as a string, including the path if it is not located in the current GAP working directory. See gurobi documentation to see which model files it supports.

To optimise the model created in the previous step do:

		GurobiOptimizeModel(model);

This will return the Gurobi status code which indicates the outcome of the optimization. See the Gurobi documentation for more information on their meanings.

To add a constraint to the model do:

		GurobiAddConstraint( model, eqn, sense, rhs);

where eqn is a list indexed by the variables, with the value at that index in the list representing the coefficient of the variable. Sense is either "=", "<" or ">" as a string, recalling that Gurobi does not differentiate between <= and < or >= and >. Lastly, rhs is the value on the right hand side of the expression. For example, to add the constraint

		2 x_1 + 3.5 x_2  - x_3  <= 8.2

we would use the following command

		GurobiAddConstraint(model, [ 2, 3.5, -1 ], "<", 8.2 );

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

For more information on Gurobi parameters, attributes, and status codes, see the following links:

Attributes: http://www.gurobi.com/documentation/7.0/refman/attributes.html

Parameters: http://www.gurobi.com/documentation/7.0/refman/parameters.html

Status codes: https://www.gurobi.com/documentation/7.0/refman/optimization_status_codes.html
