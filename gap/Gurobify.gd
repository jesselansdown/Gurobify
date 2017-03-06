#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Declarations
#


DeclareCategory( "IsGurobiModel", IsObject );


GurobiObjectFamily := NewFamily( "GurobiObjectFamily" );

BindGlobal("TheTypeGurobiModel", NewType( GurobiObjectFamily, IsGurobiModel ));



#! @Arguments Model, TimeLimit
#! @Returns 
#! @Description
#!	Set a time limit for a Gurobi model.
DeclareOperation( "SetTimeLimit",
	[ IsGurobiModel, IsFloat] );

#! @Arguments Model, BestObjectiveBoundStop
#! @Returns 
#! @Description
#!	If a solution is found for which the objective function meets the value of BestObjectiveBoundStop,
#!	then Gurobi will stop optimising.
DeclareOperation( "SetBestObjectiveBoundStop",
	[ IsGurobiModel, IsFloat] );