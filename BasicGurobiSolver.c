// This is intended to become an interface between Gurobi and GAP
//
// Author: Jesse Lansdown
//
// To compile run the following command
// gcc -m64 -g -o BasicGurobiSolver BasicGurobiSolver.c -I/Library/gurobi701/mac64/include/ -L/Library/gurobi701/mac64/lib/ -lgurobi70 -lm

#include "gurobi_c.h"

int main() {

	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int error = 0;
	error = GRBloadenv(&env, NULL);
	error = GRBreadmodel(env, "/Users/jesselansdown/GitHub/Gurobify/lp_file.lp", &model);
	error = GRBoptimize(model);

	return 0;
}