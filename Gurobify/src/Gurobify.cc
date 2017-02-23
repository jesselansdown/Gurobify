/*
 * Gurobify: Gurobify provides an interface to Gurobi from GAP.
 */

// Include gmp.h *before* switching to C mode, because GMP detects when
// compiled from C++ and then does some things differently, which would
// cause an error if called from within extern "C". But libsing.h
// (indirectly) includes gmp.h ...
#include <gmp.h>

extern "C" {
#include "src/compiled.h"          /* GAP headers                */
}


Obj TestCommand(Obj self)
{
    return INTOBJ_INT(42);
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
    return param;
}


typedef Obj (* GVarFunc)(/*arguments*/);

#define GVAR_FUNC_TABLE_ENTRY(srcfile, name, nparam, params) \
  {#name, nparam, \
   params, \
   (GVarFunc)name, \
   srcfile ":Func" #name }

// Table of functions to export
static StructGVarFunc GVarFuncs [] = {
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", TestCommand, 0, ""),
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

extern "C"
StructInitInfo *Init__Dynamic( void )
{
    return &module;
}