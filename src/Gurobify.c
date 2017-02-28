/*
 * Gurobify: Gurobify provides an interface to Gurobi from GAP.
 * Jesse Lansdown
 */

#include "src/compiled.h"          /* GAP headers */
#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>


static GRBenv *env = NULL;


Obj TheTypeGurobiModel;

void SET_MODEL(Obj o, GRBmodel* p) {
    ADDR_OBJ(o)[0] = (Obj)p;
}

GRBmodel* GET_MODEL(Obj o) {
    return (GRBmodel*)(ADDR_OBJ(o)[0]);
}

#define IS_MODEL(o) (TNUM_OBJ(o) == T_GUROBI)

UInt T_GUROBI = 0;

Obj NewModel(GRBmodel* C)
{
    Obj o;
    o = NewBag(T_GUROBI, 1 * sizeof(Obj));
    SET_MODEL(o, C);
    return o;
}

/* Free function */
void GurobiFreeFunc(Obj o)
{
    GRBfreemodel(GET_MODEL(o));
}

/* Type object function for the object */
Obj GurobiTypeFunc(Obj o)
{
    return TheTypeGurobiModel;
}

Obj GurobiCopyFunc(Obj o, Int mut)
{
    // Cone objects are mathematically immutable, so
    // we don't need to do anything,
    return o;
}

void GurobiCleanFunc(Obj o)
{
}

Int GurobiIsMutableObjFuncs(Obj o)
{
    // Cone objects are mathematically immutable.
    return 0L;
}




Obj GurobiReadLP(Obj self, Obj lp_file )
{

//-------------------------------------------------------------------------------------
// Set up the environment.

    GRBmodel *model = NULL;
    int error = 0;

    error = GRBloadenv(&env, NULL);     // We are not interested in a log file, so the second argument of GRBloadenv is NULL
    if (error || env == NULL)
        ErrorMayQuit( "Error: failed to load the environment.", 0, 0 );

//-------------------------------------------------------------------------------------
// Set up the model

	//TODO: Check that lp_file is actually a string AND ALL OTHER ERROR CHECKS IN THIS SECTION
    char *lp_file_name = CSTR_STRING(lp_file);
    error = GRBreadmodel(env, lp_file_name, &model);
    if (error)
        ErrorMayQuit( "Error: model was not read correctly.", 0, 0 );

    return NewModel(model);
}



Obj GurobiSolveModel(Obj self, Obj GAPmodel )
{

//-------------------------------------------------------------------------------------
// Set up the environment.

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

    GRBmodel *model = GET_MODEL(GAPmodel);

    int optimstatus;
	double objval;
	int i;
    int error = 0;

    error = GRBloadenv(&env, NULL);     // We are not interested in a log file, so the second argument of GRBloadenv is NULL
    if (error || env == NULL)
        ErrorMayQuit( "Error: failed to load the environment.", 0, 0 );

//-------------------------------------------------------------------------------------
// Optimise the model

    error = GRBoptimize(model);

    if (error)
        ErrorMayQuit( "Error: model was not able to be optimized", 0, 0 );

//-------------------------------------------------------------------------------------
// Evaluate the outcome of the optimisation
    int number_of_variables;
    error = GRBgetintattr(model, "NumVars", &number_of_variables);        //TODO: check for errors

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

    return results;
}


Obj GurobiSetParameter(Obj self, Obj GAPmodel, Obj ParameterName, Obj ParameterValue)
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;

	if (IS_MACFLOAT(ParameterValue))
		error = GRBsetdblparam(modelenv, CSTR_STRING(ParameterName), VAL_MACFLOAT(ParameterValue));

	if (IS_INTOBJ(ParameterValue))
		error = GRBsetintparam(modelenv, CSTR_STRING(ParameterName), INT_INTOBJ(ParameterValue));

	if ( ! IS_MACFLOAT(ParameterValue) && ! IS_INTOBJ(ParameterValue) )
		ErrorMayQuit( "Error: Can only set integer or double parameters.", 0, 0 );

	if (error)
		ErrorMayQuit( "Error: Unable to set parameter. Check parameter type and name.", 0, 0 );


	return 0;
}

Obj GurobiGetParameter( Obj self, Obj GAPmodel, Obj ParameterName )
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;
	double current_double_value;
	int current_int_value;
	error = GRBgetdblparam(modelenv, CSTR_STRING(ParameterName), &current_double_value);
	if (error){
		error = GRBgetintparam(modelenv, CSTR_STRING(ParameterName), &current_int_value);
		if (error){
			ErrorMayQuit( "Error: Unable to get parameter value. Check parameter type and name.", 0, 0 );
		}
		else{
			return INTOBJ_INT(current_int_value);
		}
	}
	else{
		return NEW_MACFLOAT(current_double_value);
	}

	ErrorMayQuit( "Error: Unable to get parameter value. Check parameter type and name.", 0, 0 );
	return 0;
}



typedef Obj (* GVarFunc)(/*arguments*/);

#define GVAR_FUNC_TABLE_ENTRY(srcfile, name, nparam, params) \
  {#name, nparam, \
   params, \
   (GVarFunc)name, \
   srcfile ":Func" #name }

// Table of functions to export
static StructGVarFunc GVarFuncs [] = {
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiReadLP, 1, "lp_file,  TimeLimitON, TimeLimitValue, CutOffON, CutOffValue, ConsoleOutputON"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSolveModel, 1, "lp_file,  TimeLimitON, TimeLimitValue, CutOffON, CutOffValue, ConsoleOutputON"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetParameter, 3, "lp_file,  TimeLimitON, TimeLimitValue, CutOffON, CutOffValue, ConsoleOutputON"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiGetParameter, 2, "lp_file,  TimeLimitON, TimeLimitValue, CutOffON, CutOffValue, ConsoleOutputON"),

  { 0 } /* Finish with an empty entry */

};

/******************************************************************************
*F  InitKernel( <module> )  . . . . . . . . initialise kernel data structures
*/
static Int InitKernel( StructInitInfo *module )
{
    /* init filters and functions                                          */
    InitHdlrFuncsFromTable( GVarFuncs );

	
    InitCopyGVar( "TheTypeGurobiModel", &TheTypeGurobiModel );

    int error = 0;
    error = GRBloadenv(&env, NULL);     // We are not interested in a log file, so the second argument of GRBloadenv is NULL
    if (error || env == NULL)
        ErrorMayQuit( "Error: failed to create new environment.", 0, 0 );
    /* return success*/


	T_GUROBI = RegisterPackageTNUM("GurobiModel", GurobiTypeFunc);

    InitMarkFuncBags(T_GUROBI, &MarkNoSubBags);
    InitFreeFuncBag(T_GUROBI, &GurobiFreeFunc);

    CopyObjFuncs[ T_GUROBI ] = &GurobiCopyFunc;
    CleanObjFuncs[ T_GUROBI ] = &GurobiCleanFunc;
	IsMutableObjFuncs[ T_GUROBI ] = &GurobiIsMutableObjFuncs;

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
