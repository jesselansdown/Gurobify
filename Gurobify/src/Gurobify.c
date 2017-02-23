/*
 * Gurobify: Gurobify provides an interface to Gurobi from GAP.
 * Jesse Lansdown
 */

#include "src/compiled.h"          /* GAP headers */
#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>


Obj GurobiSolve(Obj self, Obj lp_file)
{

	//TODO: Add option of suppressing the output to the screen during optimization
	//TODO: Return the actual solution to the problem
	//TODO: Add option of additional constraints
	//TODO: Add time limit paramter
	//TODO: Add numeric focus parameter - infact, all parameters?
	

    GRBenv *env = NULL;
    GRBmodel *model = NULL;

    int optimstatus;
	double objval;
    int error = 0;

    //TODO: Check that lp_file is actually a string
    char *lp_file_name = CSTR_STRING(lp_file);

    error = GRBloadenv(&env, NULL);     // We are not interested in a log file, so the second argument of GRBloadenv is NULL
    if (error || env == NULL) {
        ErrorMayQuit( "Error: failed to create new environment.", 0, 0 );
    }

    error = GRBreadmodel(env, lp_file_name, &model);

    if (error)
        ErrorMayQuit( "Error: model was not read correctly.", 0, 0 );    

    error = GRBoptimize(model);

    if (error)
        ErrorMayQuit( "Error: model was not able to be optimized", 0, 0 );

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error)
        ErrorMayQuit( "Error: unable to obtain optimisation status", 0, 0 );

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);			//TODO: First check that the optimstatus returned a success
    if (error)
        ErrorMayQuit( "Error: unable to obtain optimal value", 0, 0 );


	Obj results = NEW_PLIST( T_PLIST , 2);
	SET_LEN_PLIST( results , 2 );
	SET_ELM_PLIST(results, 1, INTOBJ_INT(optimstatus));
	SET_ELM_PLIST(results, 2, INTOBJ_INT(objval));			//TODO: This should be a float?
  
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
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSolve, 1, "lp_file"),
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
