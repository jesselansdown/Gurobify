# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Copyright (c) 2017 Jesse Lansdown
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.



# Declarations
#


DeclareCategory( "IsGurobiModel", IsObject );


GurobiObjectFamily := NewFamily( "GurobiObjectFamily" );

BindGlobal("TheTypeGurobiModel", NewType( GurobiObjectFamily, IsGurobiModel ));


DeclareOperation( "GurobiNewModel",
	[IsList]);


#! @Chapter Using Gurobify
#! @Section Adding And Deleting Constraints
#! @Arguments Model, ConstraintName
#! @Returns true/false
#! @Description
#!	Deletes all constraints with the name ConstraintName
DeclareOperation("GurobiDeleteConstraintsWithName",
	[IsGurobiModel, IsString]);


#! @Chapter Using Gurobify
#! @Section Adding And Deleting Constraints
#! @Arguments Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue[, ConstraintName]
#! @Returns true
#! @Description
#!	Same as below, except that ConstraintRHS value takes an integer value.
DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsInt, IsString]);


#! @Chapter Using Gurobify
#! @Section Adding And Deleting Constraints
#! @Arguments Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue[, ConstraintName]
#! @Returns true
#! @Description
#!	Adds a constraint to a gurobi model. ConstraintEquation must be a list, 
#!	such that each entry is the coefficient (including $0$ coefficents) of the corresponding variable
#! 	in the constraint equation. The ConstraintSense must be one of "&lt;", "&gt;" or "=",
#!	where Gurobi interprets &lt; as &lt;= and &gt; as &gt;=. The ConstraintRHSValue is the value on the
#!	right hand side of the constraint. A constraint may optionally be given a name, which helps to identify
#!	the constraint if it is to be deleted at some point. If no constraint name is given, then a constraint is
#!	simply assigned the name "UnNamedConstraint".
#!	Note that a model must be updated or optimised before any additional constraints become effective.
DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsFloat, IsString] );

DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsFloat]);

DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsInt]);


#! @Chapter Using Gurobify
#! @Section Adding And Deleting Constraints
#! @Arguments Model, ConstraintEquations, ConstraintSenses, ConstraintRHSValues[, ConstraintNames]
#! @Returns true
#! @Description
#!	Add multiple constraints to a model at one time. The arguments (except Model)
#!	are lists, such that the i-th entries of each list determine a single constraint in
#!	the same manner as for the operation GurobiAddConstraint. ConstraintNames is an optional argument,
#!	and must be given for all constraints, or not at all.
DeclareOperation( "GurobiAddMultipleConstraints",
	[ IsGurobiModel, IsList, IsList, IsList, IsList] );

#! @Chapter Using Gurobify
#! @Section Optimising A Model
#! @Arguments Model
#! @Returns Solution
#! @Description
#!	Display the solution found for a successfuly optimised model. Note that if a solution has not been optimised, is infeasible, or
#!	the optimisation was not completed, then this will return an error. Thus it is advisable to first check the optimisation status.
DeclareOperation( "GurobiSolution",
	[ IsGurobiModel] );

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, TimeLimit
#! @Returns true
#! @Description
#!	Set a time limit for a Gurobi model. Note that TimeLimit should be a float, however an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "GurobiSetTimeLimit",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, BestObjectiveBoundStop
#! @Returns true
#! @Description
#!	Optimisation will terminate if a feasible solution is found with objective value at least as good as BestObjectiveBoundStop.
#!	Note that BestObjectiveBoundStop should be a float, however an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "GurobiSetBestObjectiveBoundStop",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, CutOff
#! @Returns true
#! @Description
#!	Optimisation will terminate if the objective value is worse than CutOff.
#!	Note that CutOff should be a float, an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "GurobiSetCutOff",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Adding And Modifying Objective Functions
#! @Arguments Model
#! @Returns true
#! @Description
#!	Sets the model sense to maximise. When the model is optimised, it will try to maximise the objective function.
DeclareOperation("GurobiMaximiseModel",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Adding And Modifying Objective Functions
#! @Arguments Model
#! @Returns true
#! @Description
#!	Sets the model sense to minimise. When the model is optimised, it will try to minimise the objective function.
DeclareOperation("GurobiMinimiseModel",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Adding And Modifying Objective Functions
#! @Arguments Model, ObjectiveValues
#! @Returns true
#! @Description
#!	Set the objective function for a model. ObjectiveValues is a list of coefficients (including $0$ coefficeints)
#!	corresponding to each of the variables
DeclareOperation( "GurobiSetObjectiveFunction",
	[ IsGurobiModel, IsList] );

#! @Chapter Using Gurobify
#! @Section Adding And Modifying Objective Functions
#! @Arguments Model
#! @Returns List of coefficients of the objective function
#! @Description
#!	View the objective function for a model.
DeclareOperation( "GurobiObjectiveFunction",
	[ IsGurobiModel] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns Number of variables
#! @Description
#!	Returns the number of variables in the model.
DeclareOperation("GurobiNumberOfVariables",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns Number of linear constraints
#! @Description
#!	Returns the number of linear constraints in the model.
DeclareOperation("GurobiNumberOfConstraints",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Optimising A Model
#! @Arguments Model
#! @Returns objective value
#! @Description
#!	Returns the objective value of the current solution.
DeclareOperation("GurobiObjectiveValue",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns objective bound
#! @Description
#!	Returns the best known bound on the objective value of the model.
DeclareOperation("GurobiObjectiveBound",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns run time of optimisation
#! @Description
#!	Returns the wall clock runtime in seconds for the most recent optimisation.
DeclareOperation("GurobiRunTime",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Optimising A Model
#! @Arguments Model
#! @Returns optimisation status code
#! @Description
#!	Returns the optimisation status code of the most recent optimisation. Refer to the Gurobi documentation for more
#!	on the optimisation statuses, or alternatively refer to the Appendix of this manual.
DeclareOperation("GurobiOptimisationStatus",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns numeric focus
#! @Description
#!	Returns the numeric focus value of the model. The numeric focus is a value in the set [0,1,2,3]. A numeric focus of $0$ sets the
#!	numeric focus automatically, preferencing speed. Values between 1 and 3 increase the care taken in computations 
#!	as the value increases, but also take longer. The default value is 0.
DeclareOperation("GurobiNumericFocus",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, NumericFocus
#! @Returns true
#! @Description
#!	Set the numeric focus for a model. Numeric focus must be in the set [0,1,2,3]. A numeric focus of $0$ sets the
#!	numeric focus automatically, preferencing speed. Values between 1 and 3 increase the care taken in computations 
#!	as the value increases, but also take longer. The default value is 0.
DeclareOperation( "GurobiSetNumericFocus",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns time limit
#! @Description
#!	Returns the time limit for the model. The default value is infinity.
DeclareOperation("GurobiTimeLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns cutoff value
#! @Description
#!	Returns the cutoff value for the model. Optimisation will terminate if the objective value is worse than CutOff.
#!	The default value is infinity for minimisation, and negative infinity for maximisation.
DeclareOperation("GurobiCutOff",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns best objective bound limit value
#! @Description
#!	Returns the best objective bound limit value for the model.
#!	Optimisation will terminate if a feasible solution is found with objective value at least as good as the best objective bound.
#!	The default value is negative infinity.
DeclareOperation("GurobiBestObjectiveBoundStop",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, MIPFocus
#! @Returns true
#! @Description
#!	Set the MIP focus for a model. The mip focus must be in the set [0,1,2,3], and the default value is 0.
#!	The MIP focus alows you to prioritise finding solutions or proving their optimality. See the Gurobi 
#!	documentation for more information.
DeclareOperation( "GurobiSetMIPFocus",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns MIP focus
#! @Description
#!	Returns the MIP focus value for the model. The mip focus must be in the set [0,1,2,3], and the default value is 0.
#!	The MIP focus alows you to prioritise finding solutions or proving their optimality.  See the Gurobi 
#!	documentation for more information.
DeclareOperation("GurobiMIPFocus",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, BestBdStop
#! @Returns true
#! @Description
#!	Set the best bound stopping value for a model. Terminates optimisation as soon as the value
#!	of the best bound is determined to be at least as good as the best bound stopping value. Default value is infinity.
DeclareOperation( "GurobiSetBestBoundStop",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns Best bound stopping value
#! @Description
#!	Returns the best bound stopping value for the model. Optimisation terminates as soon as the value
#!	of the best bound is determined to be at least as good as the best bound stopping value. Default value is infinity.
DeclareOperation("GurobiBestBoundStop",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, BestBdStop
#! @Returns true
#! @Description
#!	Set the limit for the maximum number of MIP solutions to find.
#!	Default value is $2,000,000,000$.
DeclareOperation( "GurobiSetSolutionLimit",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns solution limit value
#! @Description
#!	Returns the solution limit value for the model. This value limits the maximum number of MIP solutions that will be found.
#!	Default value is $2,000,000,000$.
DeclareOperation("GurobiSolutionLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, IterationLimit
#! @Returns true
#! @Description
#!	Set the limit for the maximum number of simplex iterations performed. Default value is infinity.
DeclareOperation( "GurobiSetIterationLimit",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns Iteration limit
#! @Description
#!	Returns the iteration limit value for the model, which limits the number of simplex iterations performed during optimisation.
#!	Default value is infinity.
DeclareOperation("GurobiIterationLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, NodeLimit
#! @Returns true
#! @Description
#!	Set the limit for the maximum number of MIP nodes explored.
#!	The default value is infinity.
DeclareOperation( "GurobiSetNodeLimit",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns Node limit
#! @Description
#!	Returns the node limit value for the model, which limits the number of MIP nodes explored during optimisation.
#!	The default value is infinity.
DeclareOperation("GurobiNodeLimit",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns 
#! @Description
#!	Returns the names of the variables in the model. This list acts as the index set for
#!	any lists of variable coefficients, such as in GurobiAddConstraint or GurobiSetObjectiveFunction.
DeclareOperation("GurobiVariableNames",
	[IsGurobiModel]);


#! @Chapter Using Gurobify
#! @Section Creating Or Reading A Model
#! @Arguments VariableTypes[, VariableNames]
#! @Returns A Gurobi model
#! @Description
#!  Creates a gurobi model with variables defined by VariableTypes. VariableTypes must be a list of strings, 
#!	where each entry is the type of the corresponding variable.
#!  Accepted variable types are "CONTINUOUS", "BINARY", "INTEGER", "SEMICONT", or "SEMIINT". The variable types are not case sensitive.
#!  Refer to the Gurobi documentation for more information on the variable types.
#!  Optionally takes the names of the variables as a list of strings.
DeclareOperation( "GurobiNewModel",
	[IsList, IsList]);



#! @Chapter Using Gurobify
#! @Section Creating Or Reading A Model
#! @Arguments Model, VariableNames
#! @Returns true
#! @Description
#!  Assigns each variable a new name from a list of names. The names must be strings. 
DeclareOperation("GurobiSetVariableNames",
		[IsGurobiModel, IsList]);



#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, Switch
#! @Returns true
#! @Description
#!  Turns console logging on or off. If Switch is true the output of Gurobi will be printed to the screen,
#!	and if it is false the output will be supressed. The default for Gurobify is to supress the output.
DeclareOperation("GurobiSetLogToConsole",
		[IsGurobiModel, IsBool]);



#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns true/false
#! @Description
#!	Returns true if the Gurobi output is set to display to the screen, and false if the output is supressed.
#!	Gurobify suppresses the output by default.
DeclareOperation("GurobiLogToConsole",
	[IsGurobiModel]);

#! @Chapter Using Gurobify
#! @Section Optimising A Model
#! @Arguments Model, Size
#! @Returns Set of all solutions.
#! @Description
#!	This function finds all possible solutions of a given size, for a model with only binary variables.
#!	Takes a Gurobi model and repeatedly optimises it, each time adding the previous solution as a
#!	constraint so that it isn't found again. This continues until all solutions are found, and
#!	then they are returned as a set. During the process the number of found solutions is displayed.
#!	Note:
#!		- Only for models where every variable is a binary variable.
#!		- Only finds solution sets of a given size.
DeclareOperation("GurobiFindAllBinarySolutions",
	[IsGurobiModel, IsPosInt]);

#! @Chapter Using Gurobify
#! @Section Optimising A Model
#! @Arguments Model, Size, Group
#! @Returns Set of all solutions.
#! @Description
#!	This function finds all possible solutions of a given size, for a model with only binary variables.
#!	Same as above, except that it also takes a permutation group acting on the index set of variables.
#!	Instead of finding all solutions directly, the group is used to find the orbit of each new
#!	solution, and these are then all returned at the end, and used as constraints until then.
#!	An option value may also be given which will only return the representatives of each orbit of the
#!	solutions. Hence it returns all the unique solutions up to equivalence under the group.
#!	This saves on memory, and the remaing solutions may be refound by generating the
#!	orbit under the group. To invoke this option place a colon after the group argument and then put
#!	representatives:=true so for example GurobiFindAllSolutions(model, size, gp : representatives:=true);
DeclareOperation("GurobiFindAllBinarySolutions",
	[IsGurobiModel, IsPosInt, IsGroup]);

#! @Chapter Using Gurobify
#! @Section Additional Functionality
#! @Arguments IndexSet, NumberOfIndices
#! @Returns Characterisitc vector
#! @Description
#!	Takes a list of integers which form a subset of the set [1 .. n], where n is the second argument,
#!	and converts the set of indices to its characteristic vector. For example, if n = 5, the set
#!	[1,3] would be converted to [1, 0, 1, 0, 0]. It is useful to be able to convert between the two,
#!	since Gurobify always takes the characteristic vector (for example when taking constraints),
#!	yet the set of indices is generally more helpful for the user.
DeclareOperation("IndexSetToCharacteristicVector",
	[IsList, IsPosInt]);

#! @Chapter Using Gurobify
#! @Section Additional Functionality
#! @Arguments CharacteristicVector
#! @Returns Characterisitc vector
#! @Description
#!	Takes a characteristic vector and returns the set of indices corresponding to it. This reverses the
#!	process which occurs with IndexSetToCharacteristicVector. It is particularly useful to convert the
#!	output of a Gurobi solution back in terms of the variables. For example, the characteristic vector
#!	[1, 0, 1, 0, 0] would return the index set [1,3]. Note that since the function expects a characteristic 
#!	vector it doesn't account for any weightings, and is only interested in whether or not the corresponding 
#!	index is present, and as such it rounds each entry to the nearest integer and checks that it is non-zero.
#!	Hence it is particularly suitable for use with binary variables. 
DeclareOperation("CharacteristicVectorToIndexSet",
	[IsList]);


#! @Chapter Using Gurobify
#! @Section Additional Functionality
#! @Arguments Subset, FullSet
#! @Returns Characterisitc vector
#! @Description
#!	Takes a subset of some set, and returns the characteristic vector where the entries of the characteristic
#!	vector are indexed by the full set. For example, the subset ["c"] of ["a", "c", "n", "q"] would give the 
#!	characteristic vector [0, 1, 0, 0]. This removes the need to first find the index set of the subset.
DeclareOperation("SubsetToCharacteristicVector",
	[IsList, IsList]);

#! @Chapter Using Gurobify
#! @Section Additional Functionality
#! @Arguments CharacteristicVector
#! @Returns Characterisitc vector
#! @Description
#!	Takes a characteristic vector and some set which it takes to be indexing the entries of the characteristic 
#!	vector. It then returns the subset of the full set corresponding to the non-zero entries of the characteristic 
#!	vector. This is the reverse process to SubsetToCharacteristicVector. Note again that the characteristic vector 
#!	is rounded to an integer before being compared to 0. As an example, the characteristic vector [0, 1, 0, 0] with 
#!	the set ["a", "c", "n", "q"] would return ["c"]. This removes the need to first return an index set before 
#!	finding the subset.
DeclareOperation("CharacteristicVectorToSubset",
	[IsList, IsList]);


#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns 
#! @Description
#!	Returns the types of the variables in the model.
DeclareOperation("GurobiVariableTypes",
	[IsGurobiModel]);


#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, MethodType
#! @Returns true
#! @Description
#!	Set the method used to solve a model. 
#!	-1=automatic (this is the default),
#!	0=primal simplex,
#!	1=dual simplex,
#!	2=barrier,
#!	3=concurrent,
#!	4=deterministic concurrent,
#!	5=deterministic concurrent simplex.
#!	See the Gurobi documentation for more details.
DeclareOperation( "GurobiSetMethod",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns MethodType
#! @Description
#!	Returns the method used to solve a model. See the Gurobi documentation for more details.
DeclareOperation("GurobiMethod",
	[IsGurobiModel]);




#! @Chapter Using Gurobify
#! @Section Modifying Attributes And Parameters
#! @Arguments Model, ThreadCount
#! @Returns true
#! @Description
#!	Set the number of threads Gurobi is allowed to use. The default value is 0,
#!	which will use as many cores as it wants. See the Gurobi documentation for more details.
DeclareOperation( "GurobiSetThreads",
	[ IsGurobiModel, IsInt] );

#! @Chapter Using Gurobify
#! @Section Querying Attributes And Parameters
#! @Arguments Model
#! @Returns ThreadCount
#! @Description
#!	Returns number of threads Gurobi is allowed to use. See the Gurobi documentation for more details.
DeclareOperation("GurobiThreads",
	[IsGurobiModel]);
