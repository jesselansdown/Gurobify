# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Copyright (c) 2017 Jesse Lansdown
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.



#! @Chapter Examples
#! @Section Sudoku solver

#! To solve a sudoku puzzle, the integers from $1$ to $9$ must be placed in each cell of a $9 \times 9$ grid,
#! such that every number in a column, row, or one of the nine subgrids, is different. A starting configuration
#! is given which must be incorporated into the final solution. In this example we create a model which imposes
#! the sudoku rules as constraints, takes an additional constraint for the starting configuration of the Sudoku
#! puzzle, and then solves the puzzle.
#! <Br/><Br/>
#! To begin with we will define the variables that we will need for our model. Each variable will have a name
#! of the form $x_{ijk}$, where $i$ is the row of the sudoku puzzle, $j$ is the column, and $k$ is the value of
#! the entry defined byt cell $ij$. The variables are binary, since a value of $1$ indicates that the corresponding
#! cell $ij$ has value $k$, and a $0$ indicates that it doesn't have that value.
#! <Br/><Br/>
#! Since the variable names are important to the fomulation of this problem, we must define the variable names.

#! @ExampleSession
#! gap> var_names :=[];
#! [  ]
#! gap> for i in [1 .. 9] do
#! >      for j in [1 .. 9] do
#! >        for k in [1 .. 9] do
#! >          name := Concatenation("x", String(i), String(j), String(k));;
#! >          Add(var_names, name);
#! >        od;
#! >      od;
#! >    od;
#! @EndExampleSession

#! Now create the model. We need to tell Gurobi that each variable is binary.

#! @ExampleSession
#! gap> var_types := ListWithIdenticalEntries( Size( var_names ), "Binary" );;
#! gap> model := GurobiNewModel( var_types, var_names );
#! <object>
#! @EndExampleSession

#! Here we define a few basic functions which are purely for the purpose of this example.
#! Firstly a way to go from the names of a subset of the variables to the corresponding index set.
#! Secondly, a way of going back from the index set to identify the variable name.
#! Lastly, a method of displaying the Sudoku board from the names of the variables which are in the solution set.

#! @ExampleSession
#! gap> ExampleFuncNamesToIndex := function( vari_names, var_included )
#! >      local vars, ind;
#! >      vars := ListWithIdenticalEntries( Size( var_names ), 0 );
#! >      ind := List( var_included, t -> Position( var_names, t ));
#! >      vars{ ind }:=ListWithIdenticalEntries( Size( var_included ), 1 );
#! >      return vars;
#! > end;
#! function( vari_names, var_included ) ... end
#! gap> 
#! ExampleFuncIndexToNames := function( var_names, index_set )
#! >   return Filtered( var_names, t -> index_set[Position( var_names, t )] = 1. );
#! > end;
#! function( var_names, index_set ) ... end
#! gap> 
#! gap> ExampleFuncDisplaySudoku := function( sol2 )
#! >      local mat_sol, m, i, j, k;
#! >        mat_sol := NullMat(9, 0);;
#! >        for m in sol2 do
#! >          i := EvalString( [m[2]] );
#! >          j := EvalString( [m[3]] );
#! >          k := EvalString( [m[4]] );
#! >          mat_sol[i][j] := k;
#! >        od;
#! >      Display(mat_sol);
#! >   end;
#! function( sol2 ) ... end
#! @EndExampleSession

#! Ensure that a square only takes a single value.

#! @ExampleSession
#! gap> for i in [1 .. 9] do
#! >      for j in [1 .. 9] do
#! >        uniqueness_constr :=[];
#! >          for k in [1 .. 9] do
#! >            name := Concatenation("x", String(i), String(j), String(k));;
#! >            Add(uniqueness_constr, name);
#! >          od;
#! >          constr := ExampleFuncNamesToIndex( var_names, uniqueness_constr );
#! >          GurobiAddConstraint( model, constr , "=", 1 );
#! >      od;
#! >   od;
#! @EndExampleSession

#! Ensure that each value occurs exactly once per row.

#! @ExampleSession
#! gap> for i in [1 .. 9] do
#! >      for k in [1 .. 9] do
#! >        row_constr :=[];
#! >        for j in [1 .. 9] do
#! >          name := Concatenation("x", String(i), String(j), String(k));;
#! >          Add(row_constr, name );
#! >        od;
#! >        constr := ExampleFuncNamesToIndex( var_names, row_constr );
#! >        GurobiAddConstraint( model, constr, "=", 1 );
#! >      od;
#! >   od;
#! @EndExampleSession

#! Ensure that each value occurs exactly once per column.

#! @ExampleSession
#! gap> for j in [1 .. 9] do
#! >      for k in [1 .. 9] do
#! >        column_constr :=[];
#! >        for i in [1 .. 9] do
#! >          name := Concatenation("x", String(i), String(j), String(k));;
#! >          Add(column_constr, name);
#! >        od;
#! >        constr := ExampleFuncNamesToIndex( var_names, column_constr );
#! >        GurobiAddConstraint(model, constr, "=", 1);
#! >      od;
#! >   od;
#! @EndExampleSession

#! Ensure that each value occurs exactly once per sub-square. We start at the top left corner of each square
#! and work our way through them.


#! @ExampleSession
#! gap> starter_points := [ [1,1], [1,4], [1,7], [4,1], [4,4],
#! > [4,7], [7,1], [7,4], [7,7]];;
#! gap> for m in starter_points do
#! >      for k in [1 .. 9] do
#! >        square_constr := [];
#! >        for i in [0 .. 2] do
#! >          for j in [0 .. 2] do
#! >            name := Concatenation(
#! >                 "x", String(m[1] + i), String(m[2] + j), String(k)
#! >               );;
#! >            Add(square_constr, name);
#! >          od;
#! >        od;
#! >        constr := ExampleFuncNamesToIndex( var_names, square_constr );
#! >        GurobiAddConstraint(model, constr, "=", 1);
#! >      od;
#! >   od;
#! @EndExampleSession

#! The model now has constraints that will ensure that the Sudoku rules are obeyed.
#! We can put in the inital Sudoku configuration by assigning values to certain entries of the
#! sudoku matrix.


#! @ExampleSession
#! gap> starter_squares := ["x112", "x123", "x164", "x217", "x248", "x326",
#! > "x343", "x391", "x411", "x422", "x488", "x516", "x549", "x568", 
#! > "x595", "x625", "x682", "x693", "x719", "x767", "x785", "x865", 
#! > "x894", "x942", "x986", "x998"];;
#! gap> constr := ExampleFuncNamesToIndex(var_names, starter_squares);;
#! gap> GurobiAddConstraint( model, constr , "=", Sum( constr ), "StarterSquares");
#! true
#! @EndExampleSession

#! Now we optimise. Change the solution into the set of variable names, and then display the solution.

#! @ExampleSession
#! gap> GurobiOptimiseModel( model );
#! 2
#! gap> sol := GurobiSolution( model );;
#! gap> sol2 := ExampleFuncIndexToNames( var_names, sol );;
#! gap>  ExampleFuncDisplaySudoku( sol2 );
#! [ [  2,  3,  1,  6,  5,  4,  8,  9,  7 ],
#!   [  7,  9,  4,  8,  1,  2,  5,  3,  6 ],
#!   [  5,  6,  8,  3,  7,  9,  2,  4,  1 ],
#!   [  1,  2,  7,  5,  3,  6,  4,  8,  9 ],
#!   [  6,  4,  3,  9,  2,  8,  7,  1,  5 ],
#!   [  8,  5,  9,  7,  4,  1,  6,  2,  3 ],
#!   [  9,  1,  6,  4,  8,  7,  3,  5,  2 ],
#!   [  3,  8,  2,  1,  6,  5,  9,  7,  4 ],
#!   [  4,  7,  5,  2,  9,  3,  1,  6,  8 ] ]
#! @EndExampleSession

#! At this point we may wish to save the model as an lp file so that other Sudoku problems
#! may be quickly and easily solved in the future.
#! Of course, we do not want to save the starter configuration, only the general Sudoku constraints,
#! and so we must first delete the the constraint "StarterSquares".

#! @ExampleSession
#! gap> GurobiDeleteConstraintsWithName( model, "StarterSquares" );
#! true
#! gap> GurobiWriteToFile( model, "SudokuSolver.lp" );
#! true
#! @EndExampleSession

#! We can now load the lp file to create a new model with all the generic Sudoku constraints.
#! Assuming we have defined the functions ExampleFuncNamesToIndex, ExampleFuncIndexToNames
#! and ExampleFuncDisplaySudoku as before,
#! we may simply add a new constraint to the model to represent the starting configuration of the
#! Sudoku problem. In case we do not remember the variable names or their order,
#! we can first extract this information from the model. We then optimise the model and display
#! the solution as before.

#! @ExampleSession
#! gap> model2 := GurobiReadModel( "SudokuSolver.lp" );
#! <object>
#! gap> var_names2 := GurobiVariableNames(model2);;
#! gap> starter_squares := ["x118", "x124", "x132", "x145", "x161", "x219",
#! >  "x337", "x353", "x414", "x425", "x441", "x539", "x544", "x562", 
#! >  "x573", "x669", "x686", "x691", "x754", "x778", "x894", "x947", 
#! >  "x968", "x971", "x985", "x999"];;
#! gap> constr := ExampleFuncNamesToIndex(var_names2, starter_squares);;
#! gap> GurobiAddConstraint(model2, constr , "=", Sum( constr ));
#! true
#! gap> GurobiOptimiseModel(model2);
#! 2
#! gap> sol := GurobiSolution(model2);;
#! gap> sol2 := ExampleFuncIndexToNames(var_names2, sol);;
#! ExampleFuncDisplaySudoku( sol2 );
#! [ [  8,  4,  2,  5,  9,  1,  7,  3,  6 ],
#!   [  9,  3,  1,  6,  2,  7,  5,  4,  8 ],
#!   [  5,  6,  7,  8,  3,  4,  9,  1,  2 ],
#!   [  4,  5,  3,  1,  8,  6,  2,  9,  7 ],
#!   [  6,  1,  9,  4,  7,  2,  3,  8,  5 ],
#!   [  2,  7,  8,  3,  5,  9,  4,  6,  1 ],
#!   [  1,  9,  6,  2,  4,  5,  8,  7,  3 ],
#!   [  7,  8,  5,  9,  1,  3,  6,  2,  4 ],
#!   [  3,  2,  4,  7,  6,  8,  1,  5,  9 ] ]
#! @EndExampleSession

#! What if we removed a initial value from the Sudoku problem? How many solutions would there be?
#! We remove set an entry from the starter configuration and then optimise.
#! We feed this solution back in as a constraint, and then reoptimise. We can repeat this process
#! until we have found all feasible solutions, which will be when the model becomes infeasible.

#! @ExampleSession
#! gap> model3 := GurobiReadModel( "SudokuSolver.lp" );
#! <object>
#! gap> var_names3 := GurobiVariableNames(model3);;
#! gap> starter_squares := ["x118", "x124", "x132", "x145", "x161", "x219",
#! >  "x337", "x353", "x414", "x425", "x441", "x539", "x544", "x562", 
#! >  "x573", "x669", "x686", "x691", "x754", "x778", "x894", "x947", 
#! >  "x968", "x971", "x985"];;
#! gap> constr := ExampleFuncNamesToIndex(var_names3, starter_squares);;
#! gap> GurobiAddConstraint( model3, constr , "=", Sum( constr ));
#! true
#! gap> GurobiOptimiseModel( model3 );
#! 2
#! gap> number_of_solutions := 0;
#! gap> while GurobiOptimisationStatus( model3 ) = 2 do
#! >       sol := GurobiSolution( model3 );;
#! >       number_of_solutions := number_of_solutions + 1;
#! >       GurobiAddConstraint( model3, sol , "<", 80 );
#! >       GurobiUpdateModel( model3 );
#! >       GurobiReset(model3);
#! >       GurobiOptimiseModel( model3 );
#! > od;
#! gap> Print( number_of_solutions, "\n");
#! 66
#! @EndExampleSession