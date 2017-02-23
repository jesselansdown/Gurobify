// This is intended to become an interface between Gurobi and GAP
//
// Author: Jesse Lansdown
//
// To compile run the following command
// g++ -m64 -g -o BasicGurobiSolver BasicGurobiSolver.cpp -I /Library/gurobi701/mac64/include/ /Library/gurobi701/mac64/lib/libgurobi_c++.a /Library/gurobi701/mac64/lib/libgurobi70.so -lpthread -lm

#include "gurobi_c++.h"
using namespace std;

int main() {
	GRBEnv env = GRBEnv();
	GRBModel model = GRBModel(env, "lp_file.lp");
	model.optimize();
	cout << "Optimal solution: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	return 0;
}