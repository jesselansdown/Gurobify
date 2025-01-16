/*
 * Gurobify: Gurobify provides an interface to Gurobi from GAP.
 *
 * Copyright (c) 2017 - 2025 Jesse Lansdown
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "compiled.h"          /* GAP headers */
#include "gurobi_c.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

static GRBenv *env = NULL;
static GRBmodel* current_active_gurobi_model;

void gurobify_signal_handler( int signal ){
	GRBterminate(current_active_gurobi_model);
}

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

	GRBmodel *model = GET_MODEL(o);
	GRBmodel *copy = GRBcopymodel(model);
    return NewModel(copy);
}

void GurobiCleanFunc(Obj o)
{
}

Int GurobiIsMutableObjFuncs(Obj o)
{
    return 0L;
}




/*
	#! @Chapter Using Gurobify
	#!	@Section Creating Or Reading A Model
	#! @Arguments ModelFile
	#! @Returns a Gurobi model.
	#! @Description
	#!  Takes a model file, reads it and creates a Gurobi model from it.
	#!  ModelFile is the name of the file as a string, with the appropriate extension,
	#!  and including the path if the file is not located in the current GAP working directory.
	#!  Gurobi accepts files of type .mps, .rew, .lp, .rlp, .ilp, or .opb.
	#!  Refer to the gurobi documentation for more infomation on which file types can be read.
	DeclareGlobalFunction("GurobiReadModel");
*/

Obj GurobiReadModel(Obj self, Obj ModelFile )
{

    GRBmodel *model = NULL;
    int error = 0;
	//TODO: Check that lp_file is actually a string
	if (! IS_STRING(ModelFile))
        ErrorMayQuit( "Error: File name must be a string.", 0, 0 );

    char *lp_file_name = CSTR_STRING(ModelFile);
    error = GRBreadmodel(env, lp_file_name, &model);
    if (error)
        ErrorMayQuit( "Error: model was not read correctly.", 0, 0 );

    return NewModel(model);
}

/*
  This function is not documented.
  
  Creates a gurobi model with variables defined by VariableTypes and an objective function
  given by ObjectiveFunction. VariableTypes must be a list, with entries indexed by the set
  of variables, and entries corresponding to the type of variable, as a string.
  Accepted variable types are "CONTINUOUS", "BINARY", "INTEGER", "SEMICONT", or "SEMIINT".
  Refer to the Gurobi documentation for more information on the variable types.
  ObjectiveFunction is a list, with entries indexed by the set of variables, where each entry
  corresponds to the coefficient of the variable in the objective function.
  ObjectiveFunction takes only double values.
*/

Obj GUROBINEWMODEL(Obj self, Obj VariableTypes)
{

	// variable types must be one of GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER, GRB_SEMICONT, or GRB_SEMIINT

    GRBmodel *model = NULL;
    int error = 0;
	error = GRBnewmodel(env, &model, "", 0, NULL, NULL, NULL, NULL, NULL);
		if (error)
	        ErrorMayQuit( "Error: Unable to create new model.", 0, 0 );

    if ( ! IS_PLIST(VariableTypes) )
        ErrorMayQuit( "Error: VariableTypes and Varnames msut be lists!", 0, 0 );

	int number_of_variables = LEN_PLIST(VariableTypes);
	char vtype[number_of_variables];

	int i;
	for (i = 0; i < number_of_variables; i = i+1){
	
		if (! IS_STRING(ELM_PLIST(VariableTypes, i+1)))
	        ErrorMayQuit( "Error: Variable types must be strings.", 0, 0 );

		char *var_type = CSTR_STRING(ELM_PLIST(VariableTypes, i+1));

		if ( strcmp(var_type, "CONTINUOUS") == 0 ){
			vtype[i] = GRB_CONTINUOUS;
		}
		else if ( strcmp(var_type, "BINARY") == 0){
			vtype[i] = GRB_BINARY;
		}
		else if ( strcmp(var_type, "INTEGER") == 0 ){
			vtype[i] = GRB_INTEGER;
		}
		else if ( strcmp(var_type, "SEMIINT") ==0 ){
			vtype[i] = GRB_SEMIINT;
		}
		else if ( strcmp(var_type, "SEMICONT") ==0 ){
			vtype[i] = GRB_SEMICONT;
		}
		else
			ErrorMayQuit( "Error: VariableTypes must contain only 'CONTINUOUS', 'BINARY', 'INTEGER', 'SEMICONT', or 'SEMIINT' ", 0, 0 );
	}		

	error = GRBaddvars(model, number_of_variables, 0, NULL, NULL, NULL, NULL, NULL, NULL, vtype, NULL);
    if (error)
        ErrorMayQuit( "Error: Unable to add variables.", 0, 0 );

    return NewModel(model);
}


Obj GUROBISETVARIABLENAMES(Obj self, Obj GAPmodel, Obj VariableNames)
{
	int error = 0;

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

    GRBmodel *model = GET_MODEL(GAPmodel);

   if ( ! IS_PLIST(VariableNames) )
        ErrorMayQuit( "Error: VariableTypes and Varnames msut be lists!", 0, 0 );

	int number_of_variables = LEN_PLIST(VariableNames);
	int i;

	for (i = 0; i < number_of_variables; i = i+1){
		if (! IS_STRING(ELM_PLIST(VariableNames, i+1)))
	        ErrorMayQuit( "Error: Variable names must be strings.", 0, 0 );

		error = GRBsetstrattrelement(model, "VarName", i, CSTR_STRING(ELM_PLIST(VariableNames, i+1)));
	    if (error)
	        ErrorMayQuit( "Error: Unable to set variable names.", 0, 0 );
    }

    return True;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Optimising A Model
	#! @Arguments Model
	#! @Returns Optimisation status code.
	#! @Description
	#!  Takes a Gurobi model and optimises it. Returns the optimisation status code which indicates
	#!	the outcome of the optimisation. A status code of 2 indicates that a feasible solution was found,
	#!	a status code of 3 indicates the model is infeasible. There a number of other status codes.
	#!	Refer to the Gurobi documentation for more information about status codes, or alternatively see the Appendix of this manual.
	#!	The model itself is altered to reflect the optimisation, and more information about it can be obatained using other commands,
	#!	in particular the GurobiSolution and GurobiObjectiveValue functions.
	DeclareGlobalFunction("GurobiOptimiseModel");
*/

Obj GurobiOptimiseModel(Obj self, Obj GAPmodel )
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

    current_active_gurobi_model = model;
    void (*current_signal_handler)(int);
    current_signal_handler = signal(SIGINT,gurobify_signal_handler);
    error = GRBoptimize(model);
    signal(SIGINT,current_signal_handler);

    if (error)
        ErrorMayQuit( "Error: model was not able to be optimised", 0, 0 );

//-------------------------------------------------------------------------------------
// Evaluate the outcome of the optimisation

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error)
        ErrorMayQuit( "Error: unable to obtain optimisation status", 0, 0 );

    return INTOBJ_INT(optimstatus);
}

/*
	#! @Chapter Using Gurobify
	#! @Section Optimising A Model
	#! @Arguments Model
	#! @Returns true
	#! @Description
	#!	Reset all information associated with an optimisation of the model, such as the optimisation status,
	#!	the solution and the objective value.
	DeclareGlobalFunction("GurobiReset");
*/

Obj GurobiReset(Obj self, Obj GAPmodel)
{
	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

    GRBmodel *model = GET_MODEL(GAPmodel);

    int error;
	error = GRBreset(model, 1);
	if (error)
        ErrorMayQuit( "Error: unable to reset model", 0, 0 );	

    return True;	
}


/*
	#! @Chapter Using Gurobify
	#! @Section Modifying Other Attributes And Parameters
	#! @Arguments Model, ParameterName, ParameterValue
	#! @Returns
	#! @Description
	#!	Takes a Gurobi model and assigns a value to a given integer-valued parameter.
	#!	ParameterValue must be a integer value.
	#!	Refer to the Gurobi documentation for a list of parameters and their types.
	DeclareGlobalFunction("GurobiSetIntegerParameter");
*/

Obj GurobiSetIntegerParameter(Obj self, Obj GAPmodel, Obj ParameterName, Obj ParameterValue)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;

	if ( ! IS_INTOBJ(ParameterValue) )
		ErrorMayQuit( "Error: Can only set integer parameters.", 0, 0 );

	if (! IS_STRING(ParameterName))
        ErrorMayQuit( "Error: ParameterName must be a string.", 0, 0 );

	error = GRBsetintparam(modelenv, CSTR_STRING(ParameterName), INT_INTOBJ(ParameterValue));
    if (error)
        ErrorMayQuit( "Error: Unable to set parameter. Check name and type", 0, 0 );
    
    return 0;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Modifying Other Attributes And Parameters
	#! @Arguments Model, ParameterName, ParameterValue
	#! @Returns
	#! @Description
	#!	Takes a Gurobi model and assigns a value to a given double-valued parameter.
	#!	ParameterValue must be a double value.
	#!	Refer to the Gurobi documentation for a list of parameters and their types.
	DeclareGlobalFunction("GurobiSetDoubleParameter");
*/

Obj GurobiSetDoubleParameter(Obj self, Obj GAPmodel, Obj ParameterName, Obj ParameterValue)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;

	if ( ! IS_MACFLOAT(ParameterValue) )
		ErrorMayQuit( "Error: Can only set double parameters.", 0, 0 );

	if (! IS_STRING(ParameterName))
        ErrorMayQuit( "Error: ParameterName must be a string.", 0, 0 );

	error = GRBsetdblparam(modelenv, CSTR_STRING(ParameterName), VAL_MACFLOAT(ParameterValue));
    if (error)
        ErrorMayQuit( "Error: Unable to set parameter. Check name and type", 0, 0 );
    
    return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, ParameterName
	#! @Returns parameter value
	#! @Description
	#!	Takes a Gurobi model and retrieves the value of a integer-valued parameter.
	#!	Refer to the Gurobi documentation for a list of parameters and their types.
	DeclareGlobalFunction("GurobiIntegerParameter");
*/

Obj GurobiIntegerParameter( Obj self, Obj GAPmodel, Obj ParameterName )
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;
	int current_int_value;

	if (! IS_STRING(ParameterName))
        ErrorMayQuit( "Error: ParameterName must be a string.", 0, 0 );

	error = GRBgetintparam(modelenv, CSTR_STRING(ParameterName), &current_int_value);
	if (error){
		ErrorMayQuit( "Error: Unable to get parameter value. Check parameter type and name.", 0, 0 );
	}
	
	return INTOBJ_INT(current_int_value);
}

/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, ParameterName
	#! @Returns parameter value
	#! @Description
	#!	Takes a Gurobi model and retrieves the value of a double-valued parameter.
	#!	Refer to the Gurobi documentation for a list of parameters and their types.
	DeclareGlobalFunction("GurobiDoubleParameter");
*/

Obj GurobiDoubleParameter( Obj self, Obj GAPmodel, Obj ParameterName )
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;
	double current_double_value;

	if (! IS_STRING(ParameterName))
        ErrorMayQuit( "Error: ParameterName must be a string.", 0, 0 );

	error = GRBgetdblparam(modelenv, CSTR_STRING(ParameterName), &current_double_value);
	if (error){
		ErrorMayQuit( "Error: Unable to get parameter value. Check parameter type and name.", 0, 0 );
	}
	
	return NEW_MACFLOAT(current_double_value);
}



/*
This function is not documented.

	Adds a constraint to a gurobi model. ConstraintEquation must be a list, with entries indexed
	by the variable set, such that each entry is the coefficient of the corresponding variable
 	in the constraint equation. The ConstraintSense must be one of "&lt;", "&gt;" or "=",
	where Gurobi interprets &lt; as &lt;= and &gt; as &gt;=. The ConstraintRHSValue is the value on the
	right hand side of the constraint. A constraint may also be given a name, which helps to identify
	the constraint if it is to be deleted at some point. May also take an empty string "" if no name is needed.
	Note that a model must be updated or optimised before any additional constraints become effective.
*/

Obj GUROBIADDCONSTRAINT(Obj self, Obj GAPmodel, Obj AdditionalConstraintEquations, Obj AdditionalConstraintSense,
						Obj AdditionalConstraintRHSValue, Obj ConstraintName)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

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

   	int* constraint_index = (int*) malloc(number_of_variables*sizeof(int));
   	double* constraint_value = (double*) malloc(number_of_variables*sizeof(double));
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

	if (! IS_STRING(AdditionalConstraintSense))
        ErrorMayQuit( "Error: AdditionalConstraintSense must be a string.", 0, 0 );

	char *sense = CSTR_STRING(AdditionalConstraintSense);

	if ( ! (strlen(sense) == 1) ){
		ErrorMayQuit( "Error:  sense must be <,> or = ", 0, 0 );
	}

	if ( strcmp(sense, "<") == 0 ){
		error = GRBaddconstr(model, non_zero_constraints , constraint_index, constraint_value, GRB_LESS_EQUAL, rhs, CSTR_STRING(ConstraintName));
		if (error)
			ErrorMayQuit( "Error: unable to add constraint ", 0, 0 );
	}
	else if ( strcmp(sense, ">" ) == 0 ){
		error = GRBaddconstr(model, non_zero_constraints , constraint_index, constraint_value, GRB_GREATER_EQUAL, rhs, CSTR_STRING(ConstraintName));
		if (error)
			ErrorMayQuit( "Error: unable to add constraint ", 0, 0 );

	}
	else if ( strcmp(sense, "=") == 0 ){
		error = GRBaddconstr(model, non_zero_constraints , constraint_index, constraint_value, GRB_EQUAL, rhs, CSTR_STRING(ConstraintName));
		if (error)
			ErrorMayQuit( "Error: unable to add constraint ", 0, 0 );
	}
	else
			ErrorMayQuit( "Error:  sense must be <,> or = ", 0, 0 );

	free(constraint_index);
	free(constraint_value);

	return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Adding And Deleting Constraints
	#! @Arguments Model, ConstraintName
	#! @Returns true
	#! @Description
	#!	Deletes a single constraint from a model with the name ConstraintName. If multiple constraints have this name,
	#!	then one will be deleted at random.
	DeclareGlobalFunction("GurobiDeleteSingleConstraintWithName");
*/

Obj GurobiDeleteSingleConstraintWithName(Obj self, Obj GAPmodel, Obj ConstraintName)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	int ConstraintNumber;
	int error;
	if (! IS_STRING(ConstraintName))
        ErrorMayQuit( "Error: ConstraintName must be a string.", 0, 0 );

	error = GRBgetconstrbyname(model, CSTR_STRING(ConstraintName), &ConstraintNumber);
	if ( error )
		ErrorMayQuit( "Error: Unable to delete constraint.", 0, 0 );

	// while ( ConstraintNumber != -1 ){
	if (ConstraintNumber != -1)
	 	error = GRBdelconstrs(model, 1, &ConstraintNumber);

	 if ( error )
	 	ErrorMayQuit( "Error: Unable to delete constraint.", 0, 0 );
			
	error = GRBupdatemodel(model);
 	if (error)
 		ErrorMayQuit( "Error: Unable to update model.", 0, 0 );	

	// 	error = GRBgetconstrbyname(model, CSTR_STRING(ConstraintName), &ConstraintNumber);
	// 	if ( error )
	// 		ErrorMayQuit( "Error: Unable to delete constraint.", 0, 0 );
	// }

	return True;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Adding And Deleting Constraints
	#! @Arguments Model, ConstraintName
	#! @Returns true
	#! @Description
	#!	Deletes all constraints from a model which are indexed by the values of ConstraintList.
	#!	Requires an update to the model to take effect.
	DeclareGlobalFunction("GurobiDeleteConstraints");
*/

Obj GurobiDeleteConstraints(Obj self, Obj GAPmodel, Obj ConstraintList)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);

	if (! IS_PLIST(ConstraintList) )
        ErrorMayQuit( "Error: Must pass a list of positions", 0, 0 );

	int length;
	length = LEN_PLIST(ConstraintList);
	int constr_index[length];
	int i;

	for (i = 0; i < length; i = i+1 ){
		if (! IS_INTOBJ(ELM_PLIST(ConstraintList, i+1))){
	    	ErrorMayQuit( "Error: Position must be an integer.", 0, 0 );		
		}
		else{
			constr_index[i] = INT_INTOBJ(ELM_PLIST(ConstraintList,i+1));
		}
	}


    int error;
    error = GRBdelconstrs(model, length, constr_index);
	if ( error )
		ErrorMayQuit( "Error: Unable to delete constraint.", 0, 0 );

	return True;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Modifying Other Attributes And Parameters
	#! @Arguments Model, AttributeName, AttributeValue
	#! @Returns
	#! @Description
	#!	Takes a Gurobi model and assigns a value to a given integer-valued attribute.
	#!	AttributeValue must be a double value.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiSetIntegerAttribute");
*/
Obj GurobiSetIntegerAttribute(Obj self, Obj GAPmodel, Obj AttributeName, Obj AttributeValue)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;

	if ( ! IS_INTOBJ(AttributeValue) )
		ErrorMayQuit( "Error: Can only set integer-valued attributes. Check attribute type and name.", 0, 0 );

	if (IS_INTOBJ(AttributeValue)){

		if (! IS_STRING(AttributeName))
	        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

		error = GRBsetintattr(model, CSTR_STRING(AttributeName), INT_INTOBJ(AttributeValue));
    	if (error)
        	ErrorMayQuit( "Error: Unable to set attribute.", 0, 0 );
   	}

	return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Modifying Other Attributes And Parameters
	#! @Arguments Model, AttributeName, AttributeValue
	#! @Returns
	#! @Description
	#!	Takes a Gurobi model and assigns a value to a given double-valued attribute.
	#!	AttributeValue must be a double value.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiSetDoubleAttribute");
*/

Obj GurobiSetDoubleAttribute(Obj self, Obj GAPmodel, Obj AttributeName, Obj AttributeValue)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;

	if ( ! IS_MACFLOAT(AttributeValue) )
		ErrorMayQuit( "Error: Can only set double-valued attributes. Check attribute type and name.", 0, 0 );

	if (IS_MACFLOAT(AttributeValue)){

		if (! IS_STRING(AttributeName))
	        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

		error = GRBsetdblattr(model, CSTR_STRING(AttributeName), VAL_MACFLOAT(AttributeValue));
		if (error)
        	ErrorMayQuit( "Error: Unable to set attribute.", 0, 0 );
	}

	return 0;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, AttributeName
	#! @Returns attribute value
	#! @Description
	#!	Takes a Gurobi model and retrieves the value of an integer-valued attribute.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiIntegerAttribute");
*/

Obj GurobiIntegerAttribute( Obj self, Obj GAPmodel, Obj AttributeName )
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;
	int current_int_value;

	if (! IS_STRING(AttributeName))
        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBgetintattr(model, CSTR_STRING(AttributeName), &current_int_value);
	if (error){
		ErrorMayQuit( "Error: Unable to get attribute value. Check attribute type and name.", 0, 0 );
	}

	return INTOBJ_INT(current_int_value);
}


/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, position, AttributeName
	#! @Returns attribute value
	#! @Description
	#!	Takes a Gurobi model and retrieves the value of a string attribute element at a given position.
	#!	For example to get the names of constraints with "ConstrName".
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiStringAttributeElement");
*/

Obj GurobiStringAttributeElement( Obj self, Obj GAPmodel, Obj position, Obj AttributeName )
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

    if (! IS_INTOBJ(position) )
        ErrorMayQuit( "Error: Must pass an integer as the position", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;
	int i;
	i = INT_INTOBJ(position);
	char *cname;

	if (! IS_STRING(AttributeName))
        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBgetstrattrelement(model, CSTR_STRING(AttributeName), i, &cname);
	if (error){
		ErrorMayQuit( "Error: Unable to get attribute value. Check attribute type and name.", 0, 0 );
	}

	Obj name;
    name = MakeString(cname);

	return name;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, AttributeName
	#! @Returns attribute value
	#! @Description
	#!	Takes a Gurobi model and retrieves the value of a double-valued attribute.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiDoubleAttribute");
*/

Obj GurobiDoubleAttribute( Obj self, Obj GAPmodel, Obj AttributeName )
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;
	double current_double_value;

	if (! IS_STRING(AttributeName))
        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBgetdblattr(model, CSTR_STRING(AttributeName), &current_double_value);
	if (error){
		ErrorMayQuit( "Error: Unable to get attribute value. Check attribute type and name.", 0, 0 );
	}

	return NEW_MACFLOAT(current_double_value);
}



/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, AttributeName
	#! @Returns attribute array
	#! @Description
	#!	Takes a Gurobi model and retrieves an attribute array.
	#!	Can only get values of attributes arrays which take integer values.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiIntegerAttributeArray");
*/

Obj GurobiIntegerAttributeArray( Obj self, Obj GAPmodel, Obj AttributeName)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);

	int i;
	int error;
	int number_of_variables;
    error = GRBgetintattr(model, "NumVars", &number_of_variables);
    	if (error)
	        ErrorMayQuit( "Error: unable to obtain number of variables", 0, 0 );

	int* sol = (int*) malloc(number_of_variables*sizeof(int));

	if (! IS_STRING(AttributeName))
	    ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBgetintattrarray(model, CSTR_STRING(AttributeName), 0, number_of_variables, sol);		//TODO: check for errors
	    if (error)
			ErrorMayQuit( "Error: Unable to get attribute array. Check attribute type and name.", 0, 0 );
	
	Obj solution = NEW_PLIST( T_PLIST , number_of_variables);
	for (i = 0; i < number_of_variables; i = i+1 ){
				ASS_LIST(solution, i+1, INTOBJ_INT(sol[i]));
	}
	free(sol);
	return solution;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, AttributeName
	#! @Returns attribute array
	#! @Description
	#!	Takes a Gurobi model and retrieves an attribute array.
	#!	Can only get values of attributes arrays which take double values.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiDoubleAttributeArray");
*/

Obj GurobiDoubleAttributeArray( Obj self, Obj GAPmodel, Obj AttributeName)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);

	int i;
	int error;
	int number_of_variables;
        error = GRBgetintattr(model, "NumVars", &number_of_variables);
    	if (error)
	        ErrorMayQuit( "Error: unable to obtain number of variables", 0, 0 );

/*	double sol[number_of_variables]; */
	double* sol = (double*) malloc(number_of_variables*sizeof(double));

	if (! IS_STRING(AttributeName))
        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBgetdblattrarray(model, CSTR_STRING(AttributeName), 0, number_of_variables, sol);		//TODO: check for errors
	    if (error)
			ErrorMayQuit( "Error: Unable to get attribute array. Check attribute type and name.", 0, 0 );
	
	Obj solution = NEW_PLIST( T_PLIST , number_of_variables);
	for (i = 0; i < number_of_variables; i = i+1 ){
				ASS_LIST(solution, i+1, NEW_MACFLOAT(sol[i]));
	}
	free(sol);
	return solution;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, AttributeName
	#! @Returns attribute array
	#! @Description
	#! @Description
	#!	Takes a Gurobi model and retrieves an attribute array.
	#!	Can only get values of attributes arrays which have string values.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
		DeclareGlobalFunction("GurobiStringAttributeArray");
*/
Obj GurobiStringAttributeArray( Obj self, Obj GAPmodel, Obj AttributeName)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);

	int i;
	int error;
	int number_of_variables;
    error = GRBgetintattr(model, "NumVars", &number_of_variables);
    	if (error)
	        ErrorMayQuit( "Error: unable to obtain number of variables", 0, 0 );

	char * attrvals[number_of_variables];
	
	if (! IS_STRING(AttributeName))
        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBgetstrattrarray(model, CSTR_STRING(AttributeName), 0, number_of_variables, attrvals );
    if (error)
		ErrorMayQuit( "Error: Unable to get attribute array. Check attribute type and name.", 0, 0 );

	Obj solution = NEW_PLIST( T_PLIST , number_of_variables);

	for (i = 0; i < number_of_variables; i = i+1 ){
		Obj name;
        name = MakeString(attrvals[i]);
		ASS_LIST(solution, i+1, name);
	}
	return solution;

}



/*
	#! @Chapter Using Gurobify
	#! @Section Modifying Other Attributes And Parameters
	#! @Arguments Model, AttributeName, AttributeValueArray
	#! @Returns
	#! @Description
	#!	Takes a Gurobi model and assigns a value to a given attribute which takes an array of floats.
	#!	AttributeValue must be an array of floats.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiSetDoubleAttributeArray");
*/
Obj GurobiSetDoubleAttributeArray(Obj self, Obj GAPmodel, Obj AttributeName, Obj GAParray)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);

	if (! IS_PLIST(GAParray) )
    	ErrorMayQuit( "Error: The attribute takes a list.", 0, 0 );

	int error;
	int length;
	length = LEN_PLIST(GAParray);
	double* vals = (double*) malloc(length*sizeof(double));
	int i;
	for (i = 0; i < length; i = i+1 ){
		if (! IS_MACFLOAT(ELM_PLIST(GAParray, i+1))){
	    	ErrorMayQuit( "Error: attribute must be a list of floats.", 0, 0 );		
		}
		else{
			vals[i] = VAL_MACFLOAT(ELM_PLIST(GAParray,i+1));
		}
	}

	if (! IS_STRING(AttributeName))
        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBsetdblattrarray(model, CSTR_STRING(AttributeName), 0, length, vals);
	if (error)
    	ErrorMayQuit( "Error: Unable to set attribute array.", 0, 0 );
	free(vals);
	
	return 0;
}



/*
	#! @Chapter Using Gurobify
	#! @Section Querying Other Attributes And Parameters
	#! @Arguments Model, AttributeName
	#! @Returns attribute array
	#! @Description
	#! @Description
	#!	Takes a Gurobi model and retrieves an attribute array.
	#!	Can only get values of attributes arrays which have char values.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
		DeclareGlobalFunction("GurobiCharAttributeArray");
*/
Obj GurobiCharAttributeArray( Obj self, Obj GAPmodel, Obj AttributeName)
{

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);

	int i;
	int error;
	int number_of_variables;
    error = GRBgetintattr(model, "NumVars", &number_of_variables);
    	if (error)
	        ErrorMayQuit( "Error: unable to obtain number of variables", 0, 0 );

	char* attrvals = (char*) malloc(number_of_variables*sizeof(char));
	
	if (! IS_STRING(AttributeName))
        ErrorMayQuit( "Error: AttributeName must be a string.", 0, 0 );

	error = GRBgetcharattrarray(model, CSTR_STRING(AttributeName), 0, number_of_variables, attrvals );
    if (error)
		ErrorMayQuit( "Error: Unable to get attribute array. Check attribute type and name.", 0, 0 );

	Obj solution = NEW_PLIST( T_PLIST , number_of_variables);

	for (i = 0; i < number_of_variables; i = i+1 ){
		Obj name;
		name = NEW_STRING(1);
		SET_LEN_STRING(name,1);
		COPY_CHARS(name,&attrvals[i],1);
		ASS_LIST(solution, i+1, name);
	}
	free(attrvals);
	
	return solution;

}



/*
	#! @Chapter Using Gurobify
	#! @Section Creating Or Reading A Model
	#! @Arguments Model, FileName
	#! @Returns true
	#! @Description
	#!  Takes a model and writes it to a file. File type written is determined by the FileName suffix.
	#!	File types include .mps, .rew, .lp, .rlp, .ilp, .sol, or .prm.
	#!  Refer to the gurobi documentation for more infomation on which file types can be written.
	DeclareGlobalFunction("GurobiWriteToFile");
*/

Obj GurobiWriteToFile(Obj self, Obj GAPmodel, Obj FileName)
{
// Note that the file suffix determines what sort of file is written. Valid sufixes are .mps, .rew, .lp, or .rlp, and a number
// of others depending on what is to be written.

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

   	GRBmodel *model = GET_MODEL(GAPmodel);

	if (! IS_STRING(FileName))
        ErrorMayQuit( "Error: FileName must be a string.", 0, 0 );

	char *file_name = CSTR_STRING(FileName);

	int error = GRBsetstrattr(model, "ModelName", "created with Gurobify - A GAP interface to Gurobi Optimizer.");
	if (error)
		ErrorMayQuit( "Error: Unable to set model name.", 0, 0 );

	error = GRBwrite(model, file_name);
	if (error)
		ErrorMayQuit( "Error: Unable to write model.", 0, 0 );

	return True;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Optimising A Model
	#! @Arguments Model
	#! @Returns true
	#! @Description
	#!  Takes a model and updates it. Changes to a model (such as changes to parameters or constraints) are not processed
	#!	until the model is either updated or optimised. 
	DeclareGlobalFunction("GurobiUpdateModel");
*/

Obj GurobiUpdateModel(Obj self, Obj GAPmodel){

	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

	GRBmodel *model = GET_MODEL(GAPmodel);
	int error = GRBupdatemodel(model);
	if (error)
		ErrorMayQuit( "Error: Unable to update model.", 0, 0 );

	return True;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Other
	#! @Arguments 
	#! @Returns [ major_version, minor_version, technical_version ]
	#! @Description
	#!	This function returns a list [ major_version, minor_version, technical_version ] which
	#!	indicates the Gurobi library version. For example, if using Gurobi Version 7.5.1, then
	#!	GurobiVersion() would return [ 7, 5, 1 ].
	DeclareGlobalFunction("GurobiVersion");
*/

Obj GurobiVersion(Obj self){
	int major, minor, technical;
	GRBversion(&major, &minor, &technical);
	Obj version = NEW_PLIST( T_PLIST , 3);
	ASS_LIST( version, 1 , INTOBJ_INT(major));
	ASS_LIST( version , 2, INTOBJ_INT(minor));
	ASS_LIST( version, 3, INTOBJ_INT(technical));

	return version;
}

typedef Obj (* GVarFunc)(/*arguments*/);

#define GVAR_FUNC_TABLE_ENTRY(srcfile, name, nparam, params) \
  {#name, nparam, \
   params, \
   (GVarFunc)name, \
   srcfile ":Func" #name }

// Table of functions to export
static StructGVarFunc GVarFuncs [] = {
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiReadModel, 1, "ModelFile"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GUROBINEWMODEL, 1, "VariableTypes"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GUROBISETVARIABLENAMES, 2, "model, VariableNames"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiOptimiseModel, 1, "model"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiReset, 1, "model"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetIntegerParameter, 3, "model, ParameterName, ParameterValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetDoubleParameter, 3, "model, ParameterName, ParameterValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiIntegerParameter, 2, "model, ParameterName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDoubleParameter, 2, "model, ParameterName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GUROBIADDCONSTRAINT, 5, "model, ConstraintEquation, ConstraintSense, ConstraintRHS, ConstraintName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDeleteSingleConstraintWithName, 2, "model, ConstraintName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetIntegerAttribute, 3, "model, AttributeName, AttributeValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetDoubleAttribute, 3, "model, AttributeName, AttributeValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiIntegerAttribute, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDoubleAttribute, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiIntegerAttributeArray, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDoubleAttributeArray, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiStringAttributeArray, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiCharAttributeArray, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiWriteToFile, 2, "model, FileName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiUpdateModel, 1, "model"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetDoubleAttributeArray, 3, "model, AttributeName, AttributeArray"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiStringAttributeElement, 3, "model, position, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDeleteConstraints, 2, "model, ConstraintList"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiVersion, 0, ""),

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
