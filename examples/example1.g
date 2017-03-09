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
GurobiGetAttributeArray(model, "X");                                
#! [ 1., 1., 1. ]
GurobiReset(model);
GurobiSetIntegerAttribute(model, "ModelSense", 1);
GurobiOptimizeModel(model);
#! 2
GurobiGetAttributeArray(model, "X");                                
#! [ 0., 1., 1. ]
#! @EndExample
