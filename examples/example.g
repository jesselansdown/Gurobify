# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Copyright (c) 2017 Jesse Lansdown
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.




#! @Chapter Getting Started
#! @Section Simple Example

#! The first step in using gurobify is to create a model. To do this, we first need to define the types of variables
#! that are to be used in the model, and we may optionally give them names. In this case we create a model
#! with three binary variables, called $x$, $y$ and $z$. We then define the objective function as $x + 2 y + z$, which we
#! set to be maximised.

#! @BeginExample
model := GurobiNewModel(["BINARY", "BINARY", "BINARY"], ["x", "y", "z"]);
#! <object>
GurobiSetObjectiveFunction( model, [1.,2.,1.] );
#! true
GurobiMaximiseModel(model);
#! true
#! @EndExample

#! Having defined our model, we can now add constraints. To do this, a list of the coefficients for each of the variables is given,
#! along with the sense of the equation (that is, $=$, $&lt;$ or $&gt;$), the value on the right hand side of the constraint,
#! and optionally a name for the constraints.
#! Gurobi does interprets $&lt;$ as $\le$ and $&gt;$ as $\ge$ and does not distinguish between them. Note, however, that gurobify only
#! accepts the form $&lt;$ or $&gt;$.
#! We add the constraints $2x + 2y + 2z \le 6$ and $x + 2y + 3z \ge 5$ to our model. We do not assign them names.

#! @BeginExample
GurobiAddConstraint(model, [2, 2, 2], "<", 6 );
#! true
GurobiAddConstraint(model, [1, 2, 3], ">", 5 );
#! true
#! @EndExample

#! Alternatively, the previous constraints could have been added simultaneously, by containing multiple constraints as entries of a list.

#! @BeginExample
constraints :=  [[2, 2, 2], [1, 2, 3]];
#! [ [ 2, 2, 2 ], [ 1, 2, 3 ] ]
sense := ["<", ">"];
#! [ "<", ">" ]
rhs := [6,5];
#! [ 6, 5 ]
GurobiAddMultipleConstraints( model, constraints, sense, rhs );
#! true
#! @EndExample

#! We can now optimise the model. This will return a Gurobi opimistion status code. More information on the status codes can be
#! found in the Appendix. A status code of $2$ lets us know that the model was successfully optimised. We may then query the model's
#! solution.

#! @BeginExample
GurobiOptimiseModel(model);
#! 2
GurobiSolution(model);
#! [ 1., 1., 1. ]
#! @EndExample

#! In addition to returning the optimisation status upon finishing optimisation, we can query the optimisation status
#! of a model directly at any point in time. It will give the status of the model at the point when it was last optimised.

#! @BeginExample
GurobiOptimisationStatus(model);
#! 2
#! @EndExample

#! We can reset any information found on a model to its pre-optimisation state. If we then check its status it will tell us that the
#! model has been loaded, but no optimisation information is available. 

#! @BeginExample
GurobiReset(model);
#! true
GurobiOptimisationStatus(model);
#! 1
#! @EndExample

#! We can change the objective sense of the model so that Gurobi will look for a solution which minimises the objective function instead.
#! We then optimise the model and, if the optimisation is succesful, retun the solution.

#! @BeginExample
GurobiMinimiseModel(model);
#! true
GurobiOptimiseModel(model);
#! 2
GurobiSolution(model);                                
#! [ 0., 1., 1. ]
#! @EndExample

#! We can write the model to a file so that we may use it later. We need to specify the file name, and the extension of the file name
#! will determine the type of file written. In this case we write the model to an lp file which we call "test.lp".

#! @BeginExample
GurobiWriteToFile(model, "test.lp");
#! true
#! @EndExample

#! It is also possible to read a model directly from a file. In this case it is not necessary to build the model from the ground up.
#! We will read in the model from the "test.lp" file that we created in the previous step.

#! @BeginExample
re_model := GurobiReadModel("test.lp");
#! <object>
#! @EndExample

#! We now add another constraint, $x + y + z &gt; 3$, to the model. Since we may want to remove this constraint later we give
#! it the name "Other Constraint". We optimise the model and since it returns a feasible optimisation status we return the solution.

#! @BeginExample
GurobiAddConstraint(re_model, [1, 1, 1], ">", 3, "Other Constraint");
#! true
GurobiOptimiseModel(re_model);
#! 2
GurobiSolution(re_model);
#! [ 1., 1., 1. ]
#! @EndExample

#! We add another constraint, $y + z &lt; 1$, to the model and also call it "Other Constraint". We optimise the model, and get a
#! status code of $3$, indicating that the model has no feasible solutions.

#! @BeginExample
GurobiAddConstraint(re_model, [0, 1, 1], "<", 1, "Other Constraint");
#! true
GurobiOptimiseModel(re_model);
#! 3
#! @EndExample

#! Since we named the two additional constraints, we can delete them. This makes our model feasible again, and we get the same solution
#! as before.

#! @BeginExample
GurobiDeleteConstraintsWithName(re_model, "Other Constraint");
#! true
GurobiOptimiseModel(re_model);
#! 2
GurobiSolution(re_model);
#! [ 0., 1., 1. ]
#! @EndExample

#! There are many parameters and attributes of Gurobi models which can be set and queried. For example, we may set the time limit
#! to something very small so that Gurobi terminates before finishing optimising. This returns a status code of $9$. We may then
#! change the time limit again to allow Gurobi more time to finish optimising and thus obtain a feasible solution again. Chapter 3
#! documents other parameters and attributes that Gurobify is able to modify.

#! @BeginExample
GurobiSetTimeLimit(re_model, 0.000001);
#! true
GurobiOptimiseModel(re_model); 
#! 9
GurobiSetTimeLimit(re_model, 10);
#! true
GurobiOptimiseModel(re_model);
#! 2
#! @EndExample

#! We can query the model to find out what the current time limit value is set to.

#! @BeginExample
GurobiTimeLimit(re_model);
#! 10.
#! @EndExample

