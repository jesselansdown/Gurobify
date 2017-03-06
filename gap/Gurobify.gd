#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Declarations
#


DeclareCategory( "IsGurobiModel", IsObject );


GurobiObjectFamily := NewFamily( "GurobiObjectFamily" );

BindGlobal("TheTypeGurobiModel", NewType( GurobiObjectFamily, IsGurobiModel ));



#! @Arguments Model, TimeLimit
#! @Returns true
#! @Description
#!	Set a time limit for a Gurobi model.
DeclareOperation( "SetTimeLimit",
	[ IsGurobiModel, IsFloat] );

#! @Arguments Model, BestObjectiveBoundStop
#! @Returns true
#! @Description
#!	Optimisation will terminate if a feasible solution is found with objective value at least as good as BestObjectiveBoundStop.
DeclareOperation( "SetBestObjectiveBoundStop",
	[ IsGurobiModel, IsFloat] );

#! @Arguments Model, CutOff
#! @Returns true
#! @Description
#!	Optimisation will terminate if the objective value is worse than CutOff
DeclareOperation( "SetCutOff",
	[ IsGurobiModel, IsFloat] );