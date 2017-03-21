/*
 * Gurobify: Gurobify provides an interface to Gurobi from GAP.
 * Copyright Jesse Lansdown
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




/*
	#! @Chapter Using Gurobify
	#!	@Section Creating or reading a model
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

Obj GUROBINEWMODEL(Obj self, Obj VariableTypes, Obj ObjectiveFunction)
{

	// variable types must be one of GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER, GRB_SEMICONT, or GRB_SEMIINT

    GRBmodel *model = NULL;
    int error = 0;
	error = GRBnewmodel(env, &model, "", 0, NULL, NULL, NULL, NULL, NULL);
		if (error)
	        ErrorMayQuit( "Error: Unable to create new model.", 0, 0 );

    if ( ! IS_PLIST(VariableTypes) || ! IS_PLIST(ObjectiveFunction) )
        ErrorMayQuit( "Error: VariableTypes and ObjectiveFunction msut be lists!", 0, 0 );

    if (LEN_PLIST(VariableTypes) != LEN_PLIST(ObjectiveFunction) )
        ErrorMayQuit( "Error: VariableTypes and ObjectiveFunction must have the same sizes!", 0, 0 );


	int number_of_variables = LEN_PLIST(VariableTypes);
	double obj[number_of_variables];
	char vtype[number_of_variables];

	int length;
	int i;
	for (i = 0; i < number_of_variables; i = i+1){
		if (IS_INTOBJ(ELM_PLIST(ObjectiveFunction, i+1)))
		    ErrorMayQuit( "Error: Objective function must contain doubles.", 0, 0 );
		else
			obj[i] = VAL_MACFLOAT(ELM_PLIST(ObjectiveFunction, i+1));
	
		char *var_type = CSTR_STRING(ELM_PLIST(VariableTypes, i+1));
		length = strlen(var_type);
		if ( length != 10 && length != 6 && length != 7 && length != 8)
		    ErrorMayQuit( "Error: VariableTypes must contain only 'CONTINUOUS', 'BINARY', 'INTEGER', 'SEMICONT', or 'SEMIINT'", 0, 0 );

		if ( strncmp(var_type, "CONTINUOUS", 10) == 0 ){
			vtype[i] = GRB_CONTINUOUS;
		}
		else if ( strncmp(var_type, "BINARY", 6) == 0){
			vtype[i] = GRB_BINARY;
		}
		else if ( strncmp(var_type, "INTEGER", 7) == 0 ){
			vtype[i] = GRB_INTEGER;
		}
		else if ( strncmp(var_type, "SEMIINT", 7) ==0 ){
			vtype[i] = GRB_SEMIINT;
		}
		else if ( strncmp(var_type, "SEMICONT", 8) ==0 ){
			vtype[i] = GRB_SEMICONT;
		}
		else
			ErrorMayQuit( "Error: VariableTypes must contain only 'CONTINUOUS', 'BINARY', 'INTEGER', 'SEMICONT', or 'SEMIINT' ", 0, 0 );
	}		

	error = GRBaddvars(model, number_of_variables, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error)
        ErrorMayQuit( "Error: Unable to add variables.", 0, 0 );

    return NewModel(model);
}

/*
	#! @Chapter Using Gurobify
	#! @Section Optimizing a model
	#! @Arguments Model
	#! @Returns Optimisation status.
	#! @Description
	#!  Takes a Gurobi model and optimises it. Returns the optimisation status code which indicates
	#!	the outcome of the optimisation. A status code of 2 indicates that a feasible solution was found,
	#!	a status code of 3 indicates the model is infeasible. There a number of other status codes.
	#!	Refer to the Gurobi documentation for more information about status codes. The model itself is
	#!	altered to reflect the optimisation, and more information about can be obatained using other functions,
	#!	in particular the GurobiGetAttribute and GurobiGetAttributeArray functions.
	DeclareGlobalFunction("GurobiOptimizeModel");
*/

Obj GurobiOptimizeModel(Obj self, Obj GAPmodel )
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

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error)
        ErrorMayQuit( "Error: unable to obtain optimisation status", 0, 0 );

    return INTOBJ_INT(optimstatus);
}

/*
	#! @Chapter Using Gurobify
	#! @Section Optimizing a model
	#! @Arguments Model
	#! @Returns
	#! @Description
	#!	Reset all information associated with a solution for the model.
	DeclareGlobalFunction("GurobiReset");
*/

Obj GurobiReset(Obj self, Obj GAPmodel)
{
	if (! IS_MODEL(GAPmodel))
        ErrorMayQuit( "Error: Must pass a valid Gurobi model", 0, 0 );

    GRBmodel *model = GET_MODEL(GAPmodel);

    int error;
	error = GRBresetmodel(model);
	if (error)
        ErrorMayQuit( "Error: unable to reset model", 0, 0 );	

    return 0;	
}


/*
	#! @Chapter Using Gurobify
	#! @Section Modifying other attributes and parameters
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
	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;

	if ( ! IS_INTOBJ(ParameterValue) )
		ErrorMayQuit( "Error: Can only set integer parameters.", 0, 0 );

	error = GRBsetintparam(modelenv, CSTR_STRING(ParameterName), INT_INTOBJ(ParameterValue));
    if (error)
        ErrorMayQuit( "Error: Unable to set parameter. Check name and type", 0, 0 );
    
    return 0;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Modifying other attributes and parameters
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
	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;

	if ( ! IS_MACFLOAT(ParameterValue) )
		ErrorMayQuit( "Error: Can only set double parameters.", 0, 0 );

	error = GRBsetdblparam(modelenv, CSTR_STRING(ParameterName), VAL_MACFLOAT(ParameterValue));
    if (error)
        ErrorMayQuit( "Error: Unable to set parameter. Check name and type", 0, 0 );
    
    return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Querying other attributes and parameters
	#! @Arguments Model, ParameterName
	#! @Returns parameter value
	#! @Description
	#!	Takes a Gurobi model and retrieve the value of a integer-valued parameter.
	#!	Refer to the Gurobi documentation for a list of parameters and their types.
	DeclareGlobalFunction("GurobiGetIntegerParameter");
*/

Obj GurobiIntegerParameter( Obj self, Obj GAPmodel, Obj ParameterName )
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;
	int current_int_value;

	error = GRBgetintparam(modelenv, CSTR_STRING(ParameterName), &current_int_value);
	if (error){
		ErrorMayQuit( "Error: Unable to get parameter value. Check parameter type and name.", 0, 0 );
	}
	
	return INTOBJ_INT(current_int_value);
}

/*
	#! @Chapter Using Gurobify
	#! @Section Querying other attributes and parameters
	#! @Arguments Model, ParameterName
	#! @Returns parameter value
	#! @Description
	#!	Takes a Gurobi model and retrieve the value of a double-valued parameter.
	#!	Refer to the Gurobi documentation for a list of parameters and their types.
	DeclareGlobalFunction("GurobiGetDoubleParameter");
*/

Obj GurobiDoubleParameter( Obj self, Obj GAPmodel, Obj ParameterName )
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	GRBenv *modelenv = NULL;
	modelenv = GRBgetenv(model);
	int error;
	double current_double_value;

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

	return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Adding and deleting constraints
	#! @Arguments Model, ConstraintName
	#! @Returns 
	#! @Description
	#!	Deletes all constraints from a model with the name ConstraintName. Returns the updated model.
	DeclareGlobalFunction("GurobiDeleteAllConstraintsWithName");
*/

Obj GurobiDeleteConstraintsWithName(Obj self, Obj GAPmodel, Obj ConstraintName)
{

	GRBmodel *model = GET_MODEL(GAPmodel);
	int ConstraintNumber;
	int error;
	error = GRBgetconstrbyname(model, CSTR_STRING(ConstraintName), &ConstraintNumber);
	if ( error )
		ErrorMayQuit( "Error: Unable to delete constraint.", 0, 0 );

	while ( ConstraintNumber != -1 ){

		error = GRBdelconstrs(model, 1, &ConstraintNumber);
		if ( error )
			ErrorMayQuit( "Error: Unable to delete constraint.", 0, 0 );
			
		error = GRBupdatemodel(model);
		if (error)
			ErrorMayQuit( "Error: Unable to update model.", 0, 0 );	

		error = GRBgetconstrbyname(model, CSTR_STRING(ConstraintName), &ConstraintNumber);
		if ( error )
			ErrorMayQuit( "Error: Unable to delete constraint.", 0, 0 );
	}

	return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Modifying other attributes and parameters
	#! @Arguments Model, AttributeName, AttributeValue
	#! @Returns
	#! @Description
	#!	Takes a Gurobi model and assigns a value to a given integer-valued attribute.
	#!	AttributeValue must be a double value
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiSetIntegerAttribute");
*/
Obj GurobiSetIntegerAttribute(Obj self, Obj GAPmodel, Obj AttributeName, Obj AttributeValue)
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;

	if ( ! IS_INTOBJ(AttributeValue) )
		ErrorMayQuit( "Error: Can only set integer-valued attributes. Check attribute type and name.", 0, 0 );

	if (IS_INTOBJ(AttributeValue)){
		error = GRBsetintattr(model, CSTR_STRING(AttributeName), INT_INTOBJ(AttributeValue));
    	if (error)
        	ErrorMayQuit( "Error: Unable to set attribute.", 0, 0 );
   	}

	return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Modifying other attributes and parameters
	#! @Arguments Model, AttributeName, AttributeValue
	#! @Returns
	#! @Description
	#!	Takes a Gurobi model and assigns a value to a given double-valued attribute.
	#!	AttributeValue must be a double value
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiSetDoubleAttribute");
*/

Obj GurobiSetDoubleAttribute(Obj self, Obj GAPmodel, Obj AttributeName, Obj AttributeValue)
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;

	if ( ! IS_MACFLOAT(AttributeValue) )
		ErrorMayQuit( "Error: Can only set double-valued attributes. Check attribute type and name.", 0, 0 );

	if (IS_MACFLOAT(AttributeValue)){
		error = GRBsetdblattr(model, CSTR_STRING(AttributeName), VAL_MACFLOAT(AttributeValue));
		if (error)
        	ErrorMayQuit( "Error: Unable to set attribute.", 0, 0 );
	}

	return 0;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Querying other attributes and parameters
	#! @Arguments Model, AttributeName
	#! @Returns attibute value
	#! @Description
	#!	Takes a Gurobi model and retrieve the value of an integer-valued attribute.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiGetIntegerAttribute");
*/

Obj GurobiIntegerAttribute( Obj self, Obj GAPmodel, Obj AttributeName )
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;
	int current_int_value;

	error = GRBgetintattr(model, CSTR_STRING(AttributeName), &current_int_value);
	if (error){
		ErrorMayQuit( "Error: Unable to get attribute value. Check attribute type and name.", 0, 0 );
	}

	return INTOBJ_INT(current_int_value);
}

/*
	#! @Chapter Using Gurobify
	#! @Section Querying other attributes and parameters
	#! @Arguments Model, AttributeName
	#! @Returns attibute value
	#! @Description
	#!	Takes a Gurobi model and retrieve the value of a double-valued attribute.
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiGetDoubleAttribute");
*/

Obj GurobiDoubleAttribute( Obj self, Obj GAPmodel, Obj AttributeName )
{
	GRBmodel *model = GET_MODEL(GAPmodel);
	int error;
	double current_double_value;

	error = GRBgetdblattr(model, CSTR_STRING(AttributeName), &current_double_value);
	if (error){
		ErrorMayQuit( "Error: Unable to get attribute value. Check attribute type and name.", 0, 0 );
	}

	return NEW_MACFLOAT(current_double_value);
}


/*
	#! @Chapter Using Gurobify
	#! @Section Querying other attributes and parameters
	#! @Arguments Model, AttributeName
	#! @Returns attibute array
	#! @Description
	#!	Takes a Gurobi model and retrieve an attribute array.
	#!	Can only get value of attributes arrays which take integer or double values,
	#!	Refer to the Gurobi documentation for a list of attributes and their types.
	DeclareGlobalFunction("GurobiGetAttributeArray");
*/

Obj GurobiAttributeArray( Obj self, Obj GAPmodel, Obj AttributeName)
{
	GRBmodel *model = GET_MODEL(GAPmodel);

	int i;
	int error;
	int number_of_variables;
    error = GRBgetintattr(model, "NumVars", &number_of_variables);
    	if (error)
	        ErrorMayQuit( "Error: unable to obtain number of variables", 0, 0 );

	double sol[number_of_variables];
	error = GRBgetdblattrarray(model, CSTR_STRING(AttributeName), 0, number_of_variables, sol);		//TODO: check for errors
	    if (error){
			int solInt[number_of_variables];
			error = GRBgetintattrarray(model, CSTR_STRING(AttributeName), 0, number_of_variables, solInt);
				if (error){
					ErrorMayQuit( "Error: Unable to get attribute array. Check attribute type and name.", 0, 0 );
				}
				else{
					Obj solution = NEW_PLIST( T_PLIST , number_of_variables);
					SET_LEN_PLIST( solution , number_of_variables );				// For some reason this gives a warning. 
																		// When number of variables is replaced by
																		// the actual number it gives no warning and
																		// causes no trouble
					for (i = 0; i < number_of_variables; i = i+1 ){
					SET_ELM_PLIST(solution, i+1, INTOBJ_INT(solInt[i]));		//Need to check that it is infact an integer (ie not just for MIPs)
					//	SET_ELM_PLIST(solution, i+1, NEW_MACFLOAT(sol[i]));		//This line gives the solution as doubles, but unnecessary when not a MIP
					}
				return solution;
				}
	    }
	    else{
			Obj solution = NEW_PLIST( T_PLIST , number_of_variables);
			SET_LEN_PLIST( solution , number_of_variables );				// For some reason this gives a warning. 
																		// When number of variables is replaced by
																		// the actual number it gives no warning and
																		// causes no trouble
			for (i = 0; i < number_of_variables; i = i+1 ){
			//SET_ELM_PLIST(solution, i+1, INTOBJ_INT(sol[i]));		//Need to check that it is infact an integer (ie not just for MIPs)
				SET_ELM_PLIST(solution, i+1, NEW_MACFLOAT(sol[i]));		//This line gives the solution as doubles, but unnecessary when not a MIP
			}

			return solution;
		}

}



/*
	#! @Chapter Using Gurobify
	#! @Section Modifying other attributes and parameters
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
	GRBmodel *model = GET_MODEL(GAPmodel);

	if (! IS_PLIST(GAParray) )
    	ErrorMayQuit( "Error: The attribute takes a list.", 0, 0 );

	int error;
	int length;
	length = LEN_PLIST(GAParray);
	double vals[length];
	int i;
	for (i = 0; i < length; i = i+1 ){
		if (! IS_MACFLOAT(ELM_PLIST(GAParray, i+1))){
	    	ErrorMayQuit( "Error: Attibute must be a list of floats.", 0, 0 );		
		}
		else{
			vals[i] = VAL_MACFLOAT(ELM_PLIST(GAParray,i+1));
		}
	}
	
	error = GRBsetdblattrarray(model, CSTR_STRING(AttributeName), 0, length, vals);
	if (error)
    	ErrorMayQuit( "Error: Unable to set attribute array.", 0, 0 );

	return 0;
}

/*
	#! @Chapter Using Gurobify
	#! @Section Other
	#! @Arguments Model, FileName
	#! @Returns
	#! @Description
	#!  Takes a model and writes it to a file. File type written is determined by the FileName suffix.
	#!	File types include .mps, .rew, .lp, .rlp, .ilp, .sol, or .prm
	#!  Refer to the gurobi documentation for more infomation on which file types can be read.
	DeclareGlobalFunction("GurobiWriteToFile");
*/

Obj GurobiWriteToFile(Obj self, Obj GAPmodel, Obj FileName)
{
// Note that the file suffix determines what sort of file is written. Valid sufixes are .mps, .rew, .lp, or .rlp, and a number
// of others depending on what is to be written.
	GRBmodel *model = GET_MODEL(GAPmodel);
	char *file_name = CSTR_STRING(FileName);

	int error = GRBwrite(model, file_name);
	if (error)
		ErrorMayQuit( "Error: Unable to write model.", 0, 0 );

	return 0;
}


/*
	#! @Chapter Using Gurobify
	#! @Section Other
	#! @Arguments Model
	#! @Returns
	#! @Description
	#!  Takes a model and updates it. Changes to parameters or constraints are not processed
	#!	until the model is either updated or optimised. 
	DeclareGlobalFunction("GurobiUpdateModel");
*/

Obj GurobiUpdateModel(Obj self, Obj GAPmodel){

	GRBmodel *model = GET_MODEL(GAPmodel);
	int error = GRBupdatemodel(model);
	if (error)
		ErrorMayQuit( "Error: Unable to update model.", 0, 0 );

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
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiReadModel, 1, "ModelFile"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GUROBINEWMODEL, 2, "VariableTypes, ObjectiveFunction"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiOptimizeModel, 1, "model"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiReset, 1, "model"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetIntegerParameter, 3, "model, ParameterName, ParameterValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetDoubleParameter, 3, "model, ParameterName, ParameterValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiIntegerParameter, 2, "model, ParameterName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDoubleParameter, 2, "model, ParameterName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GUROBIADDCONSTRAINT, 5, "model, ConstraintEquation, ConstraintSense, ConstraintRHS, ConstraintName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDeleteConstraintsWithName, 2, "model, ConstraintName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetIntegerAttribute, 3, "model, AttributeName, AttributeValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetDoubleAttribute, 3, "model, AttributeName, AttributeValue"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiIntegerAttribute, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiDoubleAttribute, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiAttributeArray, 2, "model, AttributeName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiWriteToFile, 2, "model, FileName"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiUpdateModel, 1, "model"),
    GVAR_FUNC_TABLE_ENTRY("Gurobify.c", GurobiSetDoubleAttributeArray, 3, "model, AttributeName, AttributeArray"),

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
