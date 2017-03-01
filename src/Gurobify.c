/*
 * Gurobify: Gurobify provides an interface to Gurobi from GAP.
 * Jesse Lansdown
 */

#include "src/compiled.h"          /* GAP headers */
#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
	// TODO copy the model using gurobi's methods and return
    return o;
}

void GurobiCleanFunc(Obj o)
{
}

Int GurobiIsMutableObjFuncs(Obj o)
{
    return 0L;
}




Obj GurobiReadLP(Obj self, Obj lp_file )
{

    GRBmodel *model = NULL;
    int error = 0;
	//TODO: Check that lp_file is actually a string
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

Obj GurobiAddConstraints(Obj self, Obj GAPmodel, Obj AdditionalConstraintEquations, Obj AdditionalConstraintSense, Obj AdditionalConstraintRHSValue)
{

	GRBmodel *model = GET_MODEL(GAPmodel);

	if ( ! IS_PLIST(AdditionalConstraintEquations))
	    ErrorMayQuit( "Error: AdditionalConstraintEquations must be a list.", 0, 0 );

	int number_of_variables;
	number_of_variables = LEN_PLIST(AdditionalConstraintEquations);

	int error;
	double currentVal;
	int currentValAsInt;
	double rhs;

	if (IS_MACFLOAT(AdditionalConstraintRHSValue) ||  IS_INTOBJ(AdditionalConstraintRHSValue) ){
		if (IS_INTOBJ(AdditionalConstraintRHSValue)){
			currentValAsInt = INT_INTOBJ(AdditionalConstraintRHSValue);
			rhs = (double) currentValAsInt;
		}
		else{
			rhs = VAL_MACFLOAT(AdditionalConstraintRHSValue);
		}
	}
	else{
		ErrorMayQuit( "Error: AdditionalConstraintRHSValue must be an integer or a double.", 0, 0 );
	}

    int constraint_index[number_of_variables];
	double constraint_value[number_of_variables];
	int non_zero_constraints = 0;
	int index = 0;
	int j = 0;

	non_zero_constraints = 0;
	index = 0;
	for (j = 0; j < number_of_variables; j = j+1){
		Obj CurrentEntry = ELM_PLIST(AdditionalConstraintEquations, j+1);
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

	char *sense = CSTR_STRING(AdditionalConstraintSense);

	if ( (! strcmp(sense, "<") && ! strcmp(sense, ">" ) && ! strcmp(sense, "=") ) || ! (strlen(sense) == 1) ){
		ErrorMayQuit( "Error:  sense must be <,> or = ", 0, 0 );
	}
	else{
		error = GRBaddconstr(model, non_zero_constraints , constraint_index, constraint_value, GRB_LESS_EQUAL, rhs, NULL);
		if (error)
			ErrorMayQuit( "Error: unable to add constraint ", 0, 0 );
	}

	return 0;
}


Obj GurobiSetAttribute(Obj self, Obj GAPmodel, Obj AttributeName, Obj AttributeValue)
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;

	if (IS_MACFLOAT(AttributeValue))
		error = GRBsetdblattr(model, CSTR_STRING(AttributeName), VAL_MACFLOAT(AttributeValue));

	if (IS_INTOBJ(AttributeValue))
		error = GRBsetintattr(model, CSTR_STRING(AttributeName), INT_INTOBJ(AttributeValue));

	if ( ! IS_MACFLOAT(AttributeValue) && ! IS_INTOBJ(AttributeValue) )
		ErrorMayQuit( "Error: Can only set integer or double attributes.", 0, 0 );

	if (error)
		ErrorMayQuit( "Error: Unable to set attribute. Check attribute type and name.", 0, 0 );


	return 0;
}

Obj GurobiGetAttribute( Obj self, Obj GAPmodel, Obj AttributeName )
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;
	double current_double_value;
	int current_int_value;
	error = GRBgetdblattr(model, CSTR_STRING(AttributeName), &current_double_value);
	if (error){
		error = GRBgetintattr(model, CSTR_STRING(AttributeName), &current_int_value);
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
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiReadLP, 1, "ModelFile"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSolveModel, 1, "model"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetParameter, 3, "model, ParameterName, ParameterValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiGetParameter, 2, "model, ParameterName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiAddConstraints, 4, "model, ConstraintEquation, ConstraintSense, ConstraintRHS"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetAttribute, 3, "model, AttributeName, AttributeValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiGetAttribute, 2, "model, AttributeName"),

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
	error = GRBsetintparam(env, "LogToConsole", 0);
	if (error)
		ErrorMayQuit( "Error: unable to turn logging off", 0, 0 );

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
