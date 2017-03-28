#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Declarations
#


DeclareCategory( "IsGurobiModel", IsObject );


GurobiObjectFamily := NewFamily( "GurobiObjectFamily" );

BindGlobal("TheTypeGurobiModel", NewType( GurobiObjectFamily, IsGurobiModel ));


DeclareOperation( "GurobiNewModel",
	[IsList]);



#! @Chapter Using Gurobify
#! @Section Adding and deleting constraints
#! @Arguments Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue[, ConstraintName]
#! @Returns true
#! @Description
#!	Same as below, except that ConstraintRHS value takes an integer value.
DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsInt, IsString]);


#! @Chapter Using Gurobify
#! @Section Adding and deleting constraints
#! @Arguments Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue[, ConstraintName]
#! @Returns true
#! @Description
#!	Adds a constraint to a gurobi model. ConstraintEquation must be a list, 
#!	such that each entry is the coefficient (including $0$ coefficents) of the corresponding variable
#! 	in the constraint equation. The ConstraintSense must be one of "&lt;", "&gt;" or "=",
#!	where Gurobi interprets &lt; as &lt;= and &gt; as &gt;=. The ConstraintRHSValue is the value on the
#!	right hand side of the constraint. A constraint may optionally be given a name, which helps to identify
#!	the constraint if it is to be deleted at some point.
#!	Note that a model must be updated or optimised before any additional constraints become effective.
DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsFloat, IsString] );

DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsFloat]);

DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsInt]);


#! @Chapter Using Gurobify
#! @Section Adding and deleting constraints
#! @Arguments Model, ConstraintEquations, ConstraintSenses, ConstraintRHSValues, ConstraintNames
#! @Returns true
#! @Description
#!	Add multiple constraints to a model at one time. The arguments (except Model)
#!	are lists, such that the i-th entries of each list determine a single constraint in
#!	the same manner as for the operation GurobiAddConstraint.
DeclareOperation( "GurobiAddMultipleConstraints",
	[ IsGurobiModel, IsList, IsList, IsList, IsList] );

#! @Chapter Using Gurobify
#! @Section Optimizing a model
#! @Arguments Model
#! @Returns Solution
#! @Description
#!	Display the solution found for a successfuly optimised model.
DeclareOperation( "GurobiSolution",
	[ IsGurobiModel] );

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, TimeLimit
#! @Returns true
#! @Description
#!	Set a time limit for a Gurobi model. Note that TimeLimit should be a float, however an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "GurobiSetTimeLimit",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, BestObjectiveBoundStop
#! @Returns true
#! @Description
#!	Optimisation will terminate if a feasible solution is found with objective value at least as good as BestObjectiveBoundStop.
#!	Note that BestObjectiveBoundStop should be a float, however an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "GurobiSetBestObjectiveBoundStop",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, CutOff
#! @Returns true
#! @Description
#!	Optimisation will terminate if the objective value is worse than CutOff.
#!	Note that CutOff should be a float, an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "GurobiSetCutOff",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Adding and modifying objective functions
#! @Arguments Model
#! @Returns true
#! @Description
#!	Sets the model sense to maximise. When the model is optimized, it will try to maximise the objective function.
DeclareOperation("GurobiMaximiseModel",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Adding and modifying objective functions
#! @Arguments Model
#! @Returns true
#! @Description
#!	Sets the model sense to minimise. When the model is optimized, it will try to minimise the objective function.
DeclareOperation("GurobiMinimiseModel",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Adding and modifying objective functions
#! @Arguments Model, ObjectiveValues
#! @Returns true
#! @Description
#!	Set the objective function for a model. ObjectiveValues is a list of coefficients (including $0$ coefficeints)
#!	corresponding to each of the variables
DeclareOperation( "GurobiSetObjectiveFunction",
	[ IsGurobiModel, IsList] );

#! @Chapter Using Gurobify
#! @Section Adding and modifying objective functions
#! @Arguments Model
#! @Returns List of coefficients of the objective function
#! @Description
#!	View the objectivive function for a model.
DeclareOperation( "GurobiObjectiveFunction",
	[ IsGurobiModel] );

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns Number of variables
#! @Description
#!	Returns the number of variables in the model.
DeclareOperation("GurobiNumberOfVariables",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns Number of linear constraints
#! @Description
#!	Returns the number of linear constraints in the model.
DeclareOperation("GurobiNumberOfConstraints",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Optimizing a model
#! @Arguments Model
#! @Returns objective value
#! @Description
#!	Returns the objective value of the current solution.
DeclareOperation("GurobiObjectiveValue",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns objective bound
#! @Description
#!	Returns the best known bound for the model.
DeclareOperation("GurobiObjectiveBound",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns run time of optimization
#! @Description
#!	Returns the wall clock runtime in seconds for the most recent optimization.
DeclareOperation("GurobiRunTime",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Optimizing a model
#! @Arguments Model
#! @Returns objective bound
#! @Description
#!	Returns the optimisation status of the most recent optimization. Refer to the Gurobi documentation for more
#!	on the optimization statuses, or alternatively refer to the Appendix of this manual.
DeclareOperation("GurobiOptimizationStatus",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns numeric focus
#! @Description
#!	Returns the numeric focus value of the model.
DeclareOperation("GurobiNumericFocus",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, NumericFocus
#! @Returns true
#! @Description
#!	Set the numeric focus for a model. Numeric focus must be in the set [0,1,2,3]. A numeric focus of 0 sets the
#!	numeric focus automatically, preferancing speed. Values between 1 and 3 increase the care taken in computations 
#!	as the value increases, but also take longer. The default value is 0.
DeclareOperation( "GurobiSetNumericFocus",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns time limit
#! @Description
#!	Returns the time limit for the model.
DeclareOperation("GurobiTimeLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns cutoff value
#! @Description
#!	Returns the cutoff value for the model.
DeclareOperation("GurobiCutOff",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns best objective bound limit value
#! @Description
#!	Returns the best objective bound limit value for the model.
DeclareOperation("GurobiBestObjectiveBoundStop",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, MIPFocus
#! @Returns true
#! @Description
#!	Set a the MIP focus for a model. The mip focus must be in the set [0,1,2,3], and the default value is 0.
#!	The MIP focus alows you to prioritise finding solutions or proving their optimality. See the Gurobi 
#!	documentation for more information.
DeclareOperation( "GurobiSetMIPFocus",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns MIP focus
#! @Description
#!	Returns the MIP focus value for the model.
DeclareOperation("GurobiMIPFocus",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, BestBdStop
#! @Returns true
#! @Description
#!	Set the best bound stopping value for a model. Terminates opmitzation as soon as the value
#!	of the best bound is determined to be at least as good as the best bound stopping value.
DeclareOperation( "GurobiSetBestBoundStop",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns Best bound stopping value
#! @Description
#!	Returns the best bound stopping value for the model.
DeclareOperation("GurobiBestBoundStop",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, BestBdStop
#! @Returns true
#! @Description
#!	Set the limit for the maximum number of MIP solutions to find.
DeclareOperation( "GurobiSetSolutionLimit",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns solution limit value
#! @Description
#!	Returns the solution limit value for the model.
DeclareOperation("GurobiSolutionLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, IterationLimit
#! @Returns true
#! @Description
#!	Set the limit for the maximum number of simplex iterations performed.
DeclareOperation( "GurobiSetIterationLimit",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns Iteration limit
#! @Description
#!	Returns the iteration limit value for the model.
DeclareOperation("GurobiIterationLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, NodeLimit
#! @Returns true
#! @Description
#!	Set the limit for the maximum number of MIP nodes explored.
DeclareOperation( "GurobiSetNodeLimit",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns Node limit
#! @Description
#!	Returns the node limit value for the model.
DeclareOperation("GurobiNodeLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying attributes and parameters
#! @Arguments Model
#! @Returns 
#! @Description
#!	Returns the names of the variables in the model. The order of the variables acts as the index list for
#!	any lists of variable coefficients, such as in GurobiAddConstraint or GurobiSetObjectiveFunction.
DeclareOperation("GurobiVariableNames",
	[IsGurobiModel]);


#! @Chapter Using Gurobify
#! @Section Creating or reading a model
#! @Arguments VariableTypes[, VariableNames]
#! @Returns A Gurobi model
#! @Description
#!  Creates a gurobi model with variables defined by VariableTypes. VariableTypes must be a list of strings, 
#!	where each entry is the type of the corresponding variable.
#!  Accepted variable types are "CONTINUOUS", "BINARY", "INTEGER", "SEMICONT", or "SEMIINT".
#!  Refer to the Gurobi documentation for more information on the variable types.
#!  Optionally takes the names of the variables as a list of strings.
DeclareOperation( "GurobiNewModel",
	[IsList, IsList]);



#! @Chapter Using Gurobify
#! @Section Creating or reading a model
#! @Arguments Model, VariableNames
#! @Returns true
#! @Description
#!  To do: check that everything is a string
DeclareOperation("GurobiSetVariableNames",
		[IsGurobiModel, IsList]);

