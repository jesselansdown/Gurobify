#! @Chapter Getting Started
#! @Section Minimal working example?
#! @BeginExample
model := GurobiNewModel(["BINARY", "BINARY", "BINARY"], [1.,2.,1.]);
#! <object>
GurobiSetIntegerAttribute(model, "ModelSense", -1);
GurobiAddConstraint(model, [2, 2, 2], "<", 6, "Initial Constraint");
GurobiAddConstraint(model, [1, 2, 3], ">", 5, "Initial Constraint"); 
GurobiOptimizeModel(model);
#! 2
GetSolution(model);                                
#! [ 1., 1., 1. ]
GurobiReset(model);
GurobiSetIntegerAttribute(model, "ModelSense", 1);
GurobiOptimizeModel(model);
#! 2
GetSolution(model);                                
#! [ 0., 1., 1. ]
GurobiWriteToFile(model, "test.lp");
re_model := GurobiReadModel("test.lp");
#! <object>
GurobiAddConstraint(re_model, [1, 1, 1], ">", 3, "Other Constraint");
GurobiOptimizeModel(re_model);
#! 2
GetSolution(re_model);
#! [ 1., 1., 1. ]
GurobiAddConstraint(re_model, [0, 1, 1], "<", 1, "Other Constraint");
GurobiOptimizeModel(re_model);
#! 3
GurobiDeleteAllConstraintsWithName(re_model, "Other Constraint");
GurobiOptimizeModel(re_model);
#! 2
GetSolution(re_model);
#! [ 0., 1., 1. ]
SetTimeLimit(re_model, 0.0001);
#! true
GurobiOptimizeModel(re_model); 
#! 9
SetTimeLimit(re_model, 0.01);
#! true
GurobiOptimizeModel(re_model); 
#! 2
#! @EndExample



gap> LoadPackage("gur");
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
Loading  Gurobify 0.1 (Gurobify provides an interface to Gurobi from GAP.)
by Jesse Lansdown (www.jesselansdown.com).
Homepage: https://github.com/jesselansdown/Gurobify/
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
true

