/*
 * Gurobify: Gurobify provides an interface to Gurobi from GAP.
 * Jesse Lansdown
 */

#include "src/compiled.h"          /* GAP headers */
#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>


Obj GurobiSolve(Obj self, Obj lp_file, Obj AdditionalConstraintsON, Obj AdditionalConstraintEquations, Obj AdditionalConstraintSense,
					Obj AdditionalConstraintRHSValue, Obj ParameterArguments )
{

	//TODO: Add option of additional constraints
	//TODO: Add numeric focus parameter - infact, all parameters?


//-------------------------------------------------------------------------------------
// Define each of the additional parameters.


	int number_of_arguments = 9;

	if( ! IS_PLIST( ParameterArguments ) ){
        ErrorMayQuit( "Error: additional-parameters is not a list!", 0, 0 );
    }
	int length = LEN_PLIST( ParameterArguments );
    if (length != number_of_arguments){
        ErrorMayQuit("Error: wrong number of additional-parameter!", 0, 0);
    }

    Obj TimeLimitON = ELM_PLIST( ParameterArguments, 1 );
    Obj TimeLimitValue = ELM_PLIST( ParameterArguments, 2 );
    Obj BestObjStopON = ELM_PLIST( ParameterArguments, 3 );
    Obj BestObjStopValue = ELM_PLIST( ParameterArguments, 4 );
    Obj NumericFocusON = ELM_PLIST( ParameterArguments, 5 );
    Obj NumericFocusValue = ELM_PLIST( ParameterArguments, 6 );
    Obj CutOffON = ELM_PLIST( ParameterArguments, 7 );
    Obj CutOffValue = ELM_PLIST( ParameterArguments, 8 );
    Obj ConsoleOutputON = ELM_PLIST( ParameterArguments, 9 );

//-------------------------------------------------------------------------------------
// Set up the environment.

    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    GRBenv *modelenv = NULL;

    int optimstatus;
	double objval;
    int error = 0;

    error = GRBloadenv(&env, NULL);     // We are not interested in a log file, so the second argument of GRBloadenv is NULL
    if (error || env == NULL)
        ErrorMayQuit( "Error: failed to create new environment.", 0, 0 );
    

//-------------------------------------------------------------------------------------
// Enact each of the additional parameters

    if (ConsoleOutputON != True && ConsoleOutputON != False){
    	ErrorMayQuit( "Error: ConsoleOutputON requires a true/false switch.", 0, 0 );
    }
    else{
    	if (ConsoleOutputON != True){
	   		error = GRBsetintparam(env, "LogToConsole", 0);
			   	if (error)
			        ErrorMayQuit( "Error: LogToConsole not updated correctly.", 0, 0 );
    	}
	}

    if ( (TimeLimitON != True) && (TimeLimitON != False) ) {
    	ErrorMayQuit( "Error: TimeLimitON requires a true/false switch.", 0, 0 );
    }
    else{
    	if (TimeLimitON == True ){
    		if (IS_INTOBJ(TimeLimitValue) || ! IS_MACFLOAT(TimeLimitValue)){
    	    	ErrorMayQuit( "Error: TimeLimitValue requires a double.", 0, 0 );
			}
			else{
			   	error = GRBsetdblparam(env, "TimeLimit", VAL_MACFLOAT(TimeLimitValue) );
			   	if (error)
			        ErrorMayQuit( "Error: TimeLimit not updated correctly.", 0, 0 );
			}
    	}
	}

	if ( (BestObjStopON != True) && (BestObjStopON != False) ) {
    	ErrorMayQuit( "Error: BestObjStopON requires a true/false switch.", 0, 0 );
    }
    else{
    	if (BestObjStopON == True ){
    		if (IS_INTOBJ(BestObjStopValue) || ! IS_MACFLOAT(BestObjStopValue)){
    	    	ErrorMayQuit( "Error: BestObjStop requires a double.", 0, 0 );
			}
			else{
			   	error = GRBsetdblparam(env, "BestObjStop", VAL_MACFLOAT(BestObjStopValue) );
			   	if (error)
			        ErrorMayQuit( "Error: BestObjStop not updated correctly.", 0, 0 );
			}
    	}
	}

	if ( (NumericFocusON != True) && (NumericFocusON != False) ) {
    	ErrorMayQuit( "Error: NumericFocusON requires a true/false switch.", 0, 0 );
    }
    else{
    	if (NumericFocusON == True ){
    		if (! IS_INTOBJ(NumericFocusValue) ){
    	    	ErrorMayQuit( "Error: NumericFocus requires a integer.", 0, 0 );
			}
			else{
			   	error = GRBsetintparam(env, "NumericFocus", INT_INTOBJ(NumericFocusValue) );
			   	if (error)
			        ErrorMayQuit( "Error: NumericFocus not updated correctly.", 0, 0 );
			}
    	}
    }

	if ( (CutOffON != True) && (CutOffON != False) ) {
    	ErrorMayQuit( "Error: CutOffON requires a true/false switch.", 0, 0 );
    }
    else{
    	if (CutOffON == True ){
    		if (IS_INTOBJ(CutOffValue) || ! IS_MACFLOAT(CutOffValue)){
    	    	ErrorMayQuit( "Error: CutOffValue requires a double.", 0, 0 );
			}
			else{
			   	error = GRBsetdblparam(env, GRB_DBL_PAR_CUTOFF, VAL_MACFLOAT(CutOffValue) );
			   	if (error)
			        ErrorMayQuit( "Error: Cutoff not updated correctly.", 0, 0 );
			}
    	}
	}

//-------------------------------------------------------------------------------------
// Set up the model


    //TODO: Check that lp_file is actually a string
    char *lp_file_name = CSTR_STRING(lp_file);

    error = GRBreadmodel(env, lp_file_name, &model);


    if (error)
        ErrorMayQuit( "Error: model was not read correctly.", 0, 0 );

//-------------------------------------------------------------------------------------
// Adding additional constraints

	int i;
	int number_of_variables;
	error = GRBgetintattr(model, "NumVars", &number_of_variables);		//TODO: check for errors

	if ( (AdditionalConstraintsON != True) && (AdditionalConstraintsON != False) ) {
    	ErrorMayQuit( "Error: AdditionalConstraintsON requires a true/false switch.", 0, 0 );
    }
	else{
    	if (AdditionalConstraintsON == True ){

			if ( ! IS_PLIST(AdditionalConstraintEquations))
			    ErrorMayQuit( "Error: AdditionalConstraintEquations must be a matrix.", 0, 0 );

    		int constraint_index[number_of_variables];
			double constraint_value[number_of_variables];
			int non_zero_constraints = 0;
			int index = 0;
			int j = 0;
			double rhs;
			double currentVal;
			int currentValAsInt;

		    for (i = 0; i < LEN_PLIST(AdditionalConstraintEquations); i = i+1 ){

				Obj rhsObj = ELM_PLIST(AdditionalConstraintRHSValue, i+1);				
				if (IS_MACFLOAT(rhsObj) ||  IS_INTOBJ(rhsObj) ){
					if (IS_INTOBJ(rhsObj)){
						currentValAsInt = INT_INTOBJ(rhsObj);
						rhs = (double) currentValAsInt;
					}
					else{
						rhs = VAL_MACFLOAT(rhsObj);
					}
				}
				else{
			    	ErrorMayQuit( "Error: AdditionalConstraintRHSValue must be an integer or a double.", 0, 0 );
				}

				Obj CurrentEquation = ELM_PLIST(AdditionalConstraintEquations, i+1);
				if (! IS_PLIST(CurrentEquation))
			    	ErrorMayQuit( "Error: AdditionalConstraintEquations must be a matrix.", 0, 0 );

			    if ( LEN_PLIST(CurrentEquation) != number_of_variables)
			    	ErrorMayQuit( "Error: AdditionalConstraintEquations has incorrect dimensions!", 0, 0 );

		    	non_zero_constraints = 0;
		    	index = 0;
		    	for (j = 0; j < number_of_variables; j = j+1){
					Obj CurrentEntry = ELM_PLIST(CurrentEquation, j+1);
					if ( ! (IS_MACFLOAT(CurrentEntry) ||  IS_INTOBJ(CurrentEntry) ) )
						ErrorMayQuit( "Error: AdditionalConstraintEquations must contain integer or double entries!", 0, 0 );
					else{
						if (IS_INTOBJ(CurrentEntry)){
							currentValAsInt = INT_INTOBJ(CurrentEntry);
							currentVal = (double) currentValAsInt;
						}
						else{
							currentVal = VAL_MACFLOAT(CurrentEntry);
						}
					}

		    		if ( currentVal != 0){
						constraint_index[index] = j;
						constraint_value[index] = currentVal;
						non_zero_constraints = non_zero_constraints + 1;
						index = index + 1;
		    		}

		    	}
		    	if ( strncmp(CSTR_STRING(ELM_PLIST(AdditionalConstraintSense, i+1)), "<", 1) == 0 ){
					error = GRBaddconstr(model, non_zero_constraints , constraint_index, constraint_value, GRB_LESS_EQUAL, rhs, NULL);
						if (error)
							ErrorMayQuit( "Error: unable to add constraint ", 0, 0 );
		    	}
				else if ( strncmp(CSTR_STRING(ELM_PLIST(AdditionalConstraintSense, i+1)), ">", 1) == 0 ){
					error = GRBaddconstr(model, non_zero_constraints , constraint_index, constraint_value, GRB_GREATER_EQUAL, rhs, NULL);
						if (error)
							ErrorMayQuit( "Error: unable to add constraint ", 0, 0 );
				}
				else if ( strncmp(CSTR_STRING(ELM_PLIST(AdditionalConstraintSense, i+1)), "=", 1) == 0 ){
					error = GRBaddconstr(model, non_zero_constraints , constraint_index, constraint_value, GRB_EQUAL, rhs, NULL);
						if (error)
							ErrorMayQuit( "Error: unable to add constraint ", 0, 0 );
				}
				else
					ErrorMayQuit( "Error: wrong type of constraint sense. must be <,> or = ", 0, 0 );
		    }
    	}
	}

//-------------------------------------------------------------------------------------
// Optimise the model

    error = GRBoptimize(model);

    if (error)
        ErrorMayQuit( "Error: model was not able to be optimized", 0, 0 );

//-------------------------------------------------------------------------------------
// Evaluate the outcome of the optimisation

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error)
        ErrorMayQuit( "Error: unable to obtain optimisation status", 0, 0 );
	
	Obj results = NEW_PLIST( T_PLIST , 3);
	SET_LEN_PLIST( results , 3 );
    
    if (optimstatus == GRB_OPTIMAL){
	    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);			//TODO: First check that the optimstatus returned a success
	    if (error)
	        ErrorMayQuit( "Error: unable to obtain optimal value", 0, 0 );

	    double sol[number_of_variables];
	    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, number_of_variables, sol);		//TODO: check for errors
	    
	    Obj solution = NEW_PLIST( T_PLIST , number_of_variables);
		SET_LEN_PLIST( solution , number_of_variables );				// For some reason this gives a warning. 
																		// When number of variables is replaced by
																		// the actual number it gives no warning and
																		// causes no trouble
	    for (i = 0; i < number_of_variables; i = i+1 ){
			SET_ELM_PLIST(solution, i+1, INTOBJ_INT(sol[i]));		//Need to check that it is infact an integer (ie not just for MIPs)
	//		SET_ELM_PLIST(solution, i+1, NEW_MACFLOAT(sol[i]));		//This line gives the solution as doubles, but unnecessary when not a MIP
	    }

		SET_ELM_PLIST(results, 1, INTOBJ_INT(optimstatus));
		SET_ELM_PLIST(results, 2, NEW_MACFLOAT(objval));			//TODO: Check if can return an integer first, otherwise return a float
		SET_ELM_PLIST(results, 3, solution);
	}
	else{
		Obj unsure = NEW_STRING(3);
		C_NEW_STRING_CONST(unsure, "?");
		Obj empty_solution = NEW_PLIST( T_PLIST , 0);
		SET_LEN_PLIST( empty_solution , 0);
		SET_ELM_PLIST(results, 1, INTOBJ_INT(optimstatus));
		SET_ELM_PLIST(results, 2, unsure);			//TODO: Check if can return an integer first, otherwise return a float
		SET_ELM_PLIST(results, 3, empty_solution);
	}
	GRBfreemodel(model);
	GRBfreeenv(env);

    return results;
}



Obj TestCommandWithParams(Obj self, Obj param, Obj param2)
{
    /* simply return the first parameter */
    if( ! IS_PLIST( param ) ){
        ErrorMayQuit( "Param is not a list!", 0, 0 );
    }
    if( ! IS_INTOBJ(param2) ){
        ErrorMayQuit( "Param2 is not an integer!", 0, 0);
    }
    int index = INT_INTOBJ(param2);
    int length = LEN_PLIST( param );
    if (index > length){
        ErrorMayQuit("Param2 is larger than the size of param!", 0, 0);
    }
    Obj a = ELM_PLIST( param, index );
    int b = INT_INTOBJ(a);
    Obj return_val = INTOBJ_INT( b );

    return return_val;
}


typedef Obj (* GVarFunc)(/*arguments*/);

#define GVAR_FUNC_TABLE_ENTRY(srcfile, name, nparam, params) \
  {#name, nparam, \
   params, \
   (GVarFunc)name, \
   srcfile ":Func" #name }

// Table of functions to export
static StructGVarFunc GVarFuncs [] = {
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSolve, 6, "lp_file,  TimeLimitON, TimeLimitValue, CutOffON, CutOffValue, ConsoleOutputON"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", TestCommandWithParams, 2, "param, param2"),

  { 0 } /* Finish with an empty entry */

};

/******************************************************************************
*F  InitKernel( <module> )  . . . . . . . . initialise kernel data structures
*/
static Int InitKernel( StructInitInfo *module )
{
    /* init filters and functions                                          */
    InitHdlrFuncsFromTable( GVarFuncs );

    /* return success                                                      */
    return 0;
}

/******************************************************************************
*F  InitLibrary( <module> ) . . . . . . .  initialise library data structures
*/
static Int InitLibrary( StructInitInfo *module )
{
    /* init filters and functions */
    InitGVarFuncsFromTable( GVarFuncs );

    /* return success                                                      */
    return 0;
}

/******************************************************************************
*F  InitInfopl()  . . . . . . . . . . . . . . . . . table of init functions
*/
static StructInitInfo module = {
 /* type        = */ MODULE_DYNAMIC,
 /* name        = */ "Gurobify",
 /* revision_c  = */ 0,
 /* revision_h  = */ 0,
 /* version     = */ 0,
 /* crc         = */ 0,
 /* initKernel  = */ InitKernel,
 /* initLibrary = */ InitLibrary,
 /* checkInit   = */ 0,
 /* preSave     = */ 0,
 /* postSave    = */ 0,
 /* postRestore = */ 0
};

StructInitInfo *Init__Dynamic( void )
{
    return &module;
}
