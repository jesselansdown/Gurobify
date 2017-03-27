#! @Chapter Examples
#! @Section Sudoku solver

#! The purpose of the example
#! Statement of the problem
#! The method applied
#! Defining the variables with a view to the constraints (all diff)
#!
#! In this instance we name the variables since that is of use to us

#! @BeginExample
var_names :=[];
#! [  ]
for i in [1 .. 9] do
     for j in [1 .. 9] do
       for k in [1 .. 9] do
         name := Concatenation("x", String(i), String(j), String(k));;
         Add(var_names, name);
       od;
     od;
   od;
#! @EndExample

#! Create the model. We need to tell Gurobi that each variable is binary.

#! @BeginExample
var_types := ListWithIdenticalEntries( Size( var_names ), "Binary" );;
model := GurobiNewModel( var_types, var_names );
#! <object>
#! @EndExample

#! Here we define a few basic functions which are purely for the purpose of this example.
#! Firstly a way to go from a subset of the variables to the corresponding index set.
#! Secondly, a way of going back from the index set to identify the variable name.
#! Lastly, a method of displaying the Sudoku board from the names of the variables which are in the solution set.

#! @BeginExample
ExampleFuncNamesToIndex := function( vari_names, var_included )
     local vars, ind;
     vars := ListWithIdenticalEntries( Size( var_names ), 0 );
     ind := List( var_included, t -> Position( var_names, t ));
     vars{ ind }:=ListWithIdenticalEntries( Size( var_included ), 1 );
     return vars;
end;

ExampleFuncIndexToNames := function( var_names, index_set )
  return Filtered( var_names, t -> index_set[Position( var_names, t )] = 1. );
end;

ExampleFuncDisplaySudoku := function( sol2 )
     local mat_sol, m, i, j, k;
       mat_sol := NullMat(9, 0);;
       for m in sol2 do
         i := EvalString( [m[2]] );
         j := EvalString( [m[3]] );
         k := EvalString( [m[4]] );
         mat_sol[i][j] := k;
       od;
     Display(mat_sol);
end;
#! @EndExample

#! Ensure that a square only takes a single value.

#! @BeginExample
for i in [1 .. 9] do
     for j in [1 .. 9] do
       uniqueness_constr :=[];
         for k in [1 .. 9] do
           name := Concatenation("x", String(i), String(j), String(k));;
           Add(uniqueness_constr, name);
         od;
         constr := ExampleFuncNamesToIndex( var_names, uniqueness_constr );
         GurobiAddConstraint( model, constr , "=", 1 );
     od;
od;
#! @EndExample

#! Ensure that each value occurs exactly once per row.

#! @BeginExample
for i in [1 .. 9] do
     for k in [1 .. 9] do
       row_constr :=[];
       for j in [1 .. 9] do
         name := Concatenation("x", String(i), String(j), String(k));;
         Add(row_constr, name );
       od;
       constr := ExampleFuncNamesToIndex( var_names, row_constr );
       GurobiAddConstraint( model, constr, "=", 1 );
     od;
od;
#! @EndExample

#! Ensure that each value occurs exactly once per column.

#! @BeginExample
for j in [1 .. 9] do
     for k in [1 .. 9] do
       column_constr :=[];
       for i in [1 .. 9] do
         name := Concatenation("x", String(i), String(j), String(k));;
         Add(column_constr, name);
       od;
       constr := ExampleFuncNamesToIndex( var_names, column_constr );
       GurobiAddConstraint(model, constr, "=", 1);
     od;
od;
#! @EndExample

#! Ensure that each value occurs exactly once per sub-square. We start at the top left corner of each square and work our way through them.


#! @BeginExample
starter_points := [ [1,1], [1,4], [1,7], [4,1], [4,4],
[4,7], [7,1], [7,4], [7,7]];;

for m in starter_points do
     for k in [1 .. 9] do
       square_constr := [];
       for i in [0 .. 2] do
         for j in [0 .. 2] do
           name := Concatenation( 
           	  "x", String(m[1] + i), String(m[2] + j), String(k)
           	);;
           Add(square_constr, name);
         od;
       od;
       constr := ExampleFuncNamesToIndex( var_names, square_constr );
       GurobiAddConstraint(model, constr, "=", 1);
     od;
od;
#! @EndExample


#! Now that the lp file will look for solutions that obey the Sudoku rules,
#! we can put in the inital Sudoku configuration by assigning certain entries of the
#! sudoku matrix set values.


#! @BeginExample
starter_squares := ["x112", "x123", "x164", "x217", "x248", "x326",
"x343", "x391", "x411", "x422", "x488", "x516", "x549", "x568", 
"x595", "x625", "x682", "x693", "x719", "x767", "x785", "x865", 
"x894", "x942", "x986", "x998"];;
constr := ExampleFuncNamesToIndex(var_names, starter_squares);;
GurobiAddConstraint( model, constr , "=", Sum( constr ), "StarterSquares");
#! true;
#! @EndExample

#! Now we optimize. Change the solution into the variable names, and then display.

#! @BeginExample
GurobiOptimizeModel( model );
sol := GurobiSolution( model );;
sol2 := ExampleFuncIndexToNames( var_names, sol );;
ExampleFuncDisplaySudoku( sol2 );



#! [ [  2,  3,  1,  6,  5,  4,  8,  9,  7 ],
#!   [  7,  9,  4,  8,  1,  2,  5,  3,  6 ],
#!   [  5,  6,  8,  3,  7,  9,  2,  4,  1 ],
#!   [  1,  2,  7,  5,  3,  6,  4,  8,  9 ],
#!   [  6,  4,  3,  9,  2,  8,  7,  1,  5 ],
#!   [  8,  5,  9,  7,  4,  1,  6,  2,  3 ],
#!   [  9,  1,  6,  4,  8,  7,  3,  5,  2 ],
#!   [  3,  8,  2,  1,  6,  5,  9,  7,  4 ],
#!   [  4,  7,  5,  2,  9,  3,  1,  6,  8 ] ]
#! @EndExample

#! At this point we may wish to save the model as an lp file so that other Sudoku problems
#! may be quickly and easily solved in the future.
#! Of course, we do not want to save the starter configuration, only the general Sudoku constraints,
#! and so we must first delete the the constraint "StarterSquares".

#! @BeginExample
GurobiDeleteConstraintsWithName( model, "StarterSquares" );
#! true
GurobiWriteToFile( model, "SudokuSolver.lp" );
#! true
#! @EndExample

#! We can now load the lp file to create a new model with all the generic Sudoku constraints.
#! Assuming we have defined the functions ExampleFuncNamesToIndex, ExampleFuncIndexToNames
#! and ExampleFuncDisplaySudoku as before,
#! we may simply add a new constraint to the model to represent the starting configuration of the
#! Sudoku problem. Assuming we do not remember the variable names or their order,
#! we must first extract this information from the model. We then optimize the model and display
#! the solution as before. Incase we have forgotten the names of the variables, or the order they occur,
#! or simply don't want to reconstruct the var_names list, we can first extract this information directly from the model.

#! @BeginExample
model2 := GurobiReadModel( "SudokuSolver.lp" );
#! <object>

var_names2 := GurobiVariableNames(model2);;

starter_squares := ["x118", "x124", "x132", "x145", "x161", "x219",
 "x337", "x353", "x414", "x425", "x441", "x539", "x544", "x562", 
 "x573", "x669", "x686", "x691", "x754", "x778", "x894", "x947", 
 "x968", "x971", "x985", "x999"];;

constr := ExampleFuncNamesToIndex(var_names2, starter_squares);;
GurobiAddConstraint(model2, constr , "=", Sum( constr ));
#! true
GurobiOptimizeModel(model2);
#! 2
sol := GurobiSolution(model2);;
sol2 := ExampleFuncIndexToNames(var_names2, sol);;
ExampleFuncDisplaySudoku( sol2 );
#! [ [  8,  4,  2,  5,  9,  1,  7,  3,  6 ],
#!   [  9,  3,  1,  6,  2,  7,  5,  4,  8 ],
#!   [  5,  6,  7,  8,  3,  4,  9,  1,  2 ],
#!   [  4,  5,  3,  1,  8,  6,  2,  9,  7 ],
#!   [  6,  1,  9,  4,  7,  2,  3,  8,  5 ],
#!   [  2,  7,  8,  3,  5,  9,  4,  6,  1 ],
#!   [  1,  9,  6,  2,  4,  5,  8,  7,  3 ],
#!   [  7,  8,  5,  9,  1,  3,  6,  2,  4 ],
#!   [  3,  2,  4,  7,  6,  8,  1,  5,  9 ] ]
#! @EndExample

#! What if we removed a initial value from the Sudoku problem? How many solutions would there be?
#! We remove set an entry from the starter configuration and then optimize.
#! We feed this solution back in as a constraint, and then reoptimize. We can repeat this process
#! until we have found all feasible solutions, which will be when the model becomes infeasible.

#! @BeginExample
model3 := GurobiReadModel( "SudokuSolver.lp" );
#! <object>
var_names3 := GurobiVariableNames(model3);;

starter_squares := ["x118", "x124", "x132", "x145", "x161", "x219",
 "x337", "x353", "x414", "x425", "x441", "x539", "x544", "x562", 
 "x573", "x669", "x686", "x691", "x754", "x778", "x894", "x947", 
 "x968", "x971", "x985"];;

constr := ExampleFuncNamesToIndex(var_names3, starter_squares);;
GurobiAddConstraint( model3, constr , "=", Sum( constr ));
#! true
GurobiOptimizeModel( model3 );
#! 2

if GurobiOptimizationStatus( model3 ) = 2 then
	number_of_solutions := 1;
else
	number_of_solutions := 0;
fi;
while GurobiOptimizationStatus( model3 ) = 2 do
	sol := GurobiSolution( model3 );;
	number_of_solutions := number_of_solutions + 1;
	GurobiAddConstraint( model3, sol , "<", 80 );
	GurobiOptimizeModel( model3 );
od;
Print( number_of_solutions, "\n");
#! 67
#! @EndExample