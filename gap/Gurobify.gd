#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Declarations
#


DeclareCategory( "IsGurobiModel", IsObject );


GurobiObjectFamily := NewFamily( "GurobiObjectFamily" );

BindGlobal("TheTypeGurobiModel", NewType( GurobiObjectFamily, IsGurobiModel ));


#! @Chapter Using Gurobify
#! @Section Creating or reading a model
#! @Arguments VariableTypes, ObjectiveFunction
#! @Returns A Gurobi model
#! @Description
#!  Creates a gurobi model with variables defined by VariableTypes and an objective function
#!  given by ObjectiveFunction. VariableTypes must be a list, with entries indexed by the set
#!  of variables, and entries corresponding to the type of variable, as a string.
#!  Accepted variable types are "CONTINUOUS", "BINARY", "INTEGER", "SEMICONT", or "SEMIINT".
#!  Refer to the Gurobi documentation for more information on the variable types.
#!  ObjectiveFunction is a list, with entries indexed by the set of variables, where each entry
#!  corresponds to the coefficient of the variable in the objective function.
DeclareOperation( "GurobiNewModel",
	[IsList, IsList]);

#! @Chapter Using Gurobify
#! @Section Adding and deleting constraints
#! @Arguments Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue, ConstraintName
#! @Returns true
#! @Description
#!	Adds a constraint to a gurobi model. ConstraintEquation must be a list, with entries indexed
#!	by the variable set, such that each entry is the coefficient of the corresponding variable
#! 	in the constraint equation. The ConstraintSense must be one of "&lt;", "&gt;" or "=",
#!	where Gurobi interprets &lt; as &lt;= and &gt; as &gt;=. The ConstraintRHSValue is the value on the
#!	right hand side of the constraint. A constraint may also be given a name, which helps to identify
#!	the constraint if it is to be deleted at some point.
#!	Note that a model must be updated or optimised before any additional constraints become effective.
DeclareOperation( "GurobiAddConstraint",
	[ IsGurobiModel, IsList, IsString, IsFloat, IsString] );

#! @Chapter Using Gurobify
#! @Section Optimizing a model
#! @Arguments Model
#! @Returns Solution
#! @Description
#!	Display the solution found for a successfuly optimised model.
DeclareOperation( "GetSolution",
	[ IsGurobiModel] );

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, TimeLimit
#! @Returns true
#! @Description
#!	Set a time limit for a Gurobi model. Note that TimeLimit should be a float, however an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "SetTimeLimit",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, BestObjectiveBoundStop
#! @Returns true
#! @Description
#!	Optimisation will terminate if a feasible solution is found with objective value at least as good as BestObjectiveBoundStop.
#!	Note that BestObjectiveBoundStop should be a float, however an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "SetBestObjectiveBoundStop",
	[ IsGurobiModel, IsFloat] );

#! @Chapter Using Gurobify
#! @Section Modifying attributes and parameters
#! @Arguments Model, CutOff
#! @Returns true
#! @Description
#!	Optimisation will terminate if the objective value is worse than CutOff.
#!	Note that CutOff should be a float, an integer value can be given
#!	which will be automatically converted to a float.
DeclareOperation( "SetCutOff",
	[ IsGurobiModel, IsFloat] );