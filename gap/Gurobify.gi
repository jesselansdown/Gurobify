# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Copyright (c) 2017 Jesse Lansdown
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


InstallMethod(GurobiNewModel, "",
	[ IsList] ,
	function(VariableTypes)
		local model;
		model := GUROBINEWMODEL(List(VariableTypes, t -> UppercaseString(t)));
		GurobiUpdateModel(model);
		return model;
	end
);

InstallOtherMethod(GurobiNewModel, "",
	[ IsPosInt, IsString] ,
	function(n, VariableType)
		local model;
		model := GUROBINEWMODEL(List([1 .. n], t -> UppercaseString(VariableType)));
		GurobiUpdateModel(model);
		return model;
	end
);

InstallMethod(GurobiAddConstraint, "",
	[ IsGurobiModel, IsList, IsString, IsFloat, IsString],
	function(Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue, ConstraintName)
		GUROBIADDCONSTRAINT(Model, List(ConstraintEquation, t -> Float(t)), ConstraintSense, ConstraintRHSValue, ConstraintName);
		return true;
	end
);

InstallMethod(GurobiAddConstraint, "",
	[ IsGurobiModel, IsList, IsString, IsInt, IsString],
	function(Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue, ConstraintName)
		GUROBIADDCONSTRAINT(Model, List(ConstraintEquation, t -> Float(t)), ConstraintSense, Float(ConstraintRHSValue), ConstraintName);
		return true;
	end
);

InstallMethod(GurobiAddConstraint, "",
	[ IsGurobiModel, IsList, IsString, IsFloat],
	function(Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue)
		GUROBIADDCONSTRAINT(Model, List(ConstraintEquation, t -> Float(t)), ConstraintSense, ConstraintRHSValue, "UnNamedConstraint");
		return true;
	end
);

InstallMethod(GurobiAddConstraint, "",
	[ IsGurobiModel, IsList, IsString, IsInt],
	function(Model, ConstraintEquation, ConstraintSense, ConstraintRHSValue)
		GUROBIADDCONSTRAINT(Model, List(ConstraintEquation, t -> Float(t)), ConstraintSense, Float(ConstraintRHSValue), "UnNamedConstraint");
		return true;
	end
);

InstallMethod(GurobiAddMultipleConstraints, "",
	[ IsGurobiModel, IsList, IsList, IsList, IsList],
	function(Model, ConstraintEquations, ConstraintSenses, ConstraintRHSValues, ConstraintNames)
		# Add error checks
		local i;
		if Size(ConstraintEquations) <> Size(ConstraintSenses) or Size(ConstraintEquations) <> Size(ConstraintRHSValues)
			or Size(ConstraintEquations) <> Size(ConstraintNames) then
			Print("Error: the ConstraintEquations, ConstraintSenses, Constraint ConstraintRHSValues and ConstraintNames must have the same sizes.");
			return;
		fi;
		for i in [1 .. Size(ConstraintEquations)] do
			GUROBIADDCONSTRAINT(Model, List(ConstraintEquations[i], t -> Float(t)), ConstraintSenses[i],
				Float(ConstraintRHSValues[i]), ConstraintNames[i]);
		od;
		return true;
	end
);

InstallOtherMethod(GurobiAddMultipleConstraints, "",
	[ IsGurobiModel, IsList, IsList, IsList],
	function(Model, ConstraintEquations, ConstraintSenses, ConstraintRHSValues)
		# Add error checks
		local i;
		if Size(ConstraintEquations) <> Size(ConstraintSenses) or Size(ConstraintEquations) <> Size(ConstraintRHSValues) then
			Print("Error: the ConstraintEquations, ConstraintSenses, Constraint ConstraintRHSValues must have the same sizes.");
			return;
		fi;
		for i in [1 .. Size(ConstraintEquations)] do
			GUROBIADDCONSTRAINT(Model, List(ConstraintEquations[i], t -> Float(t)), ConstraintSenses[i],
				Float(ConstraintRHSValues[i]), "UnNamedConstraint");
		od;
		return true;
	end
);

InstallOtherMethod(GurobiAddMultipleConstraints, "",
	[ IsGurobiModel, IsList, IsString, IsFloat],
	function(Model, ConstraintEquations, ConstraintSense, ConstraintRHSValue)
		# Add error checks
		local senses, rhs;
		senses := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintSense);
		rhs := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintRHSValue);
		GurobiAddMultipleConstraints(Model, ConstraintEquations, senses, rhs);;
		return true;
	end
);

InstallOtherMethod(GurobiAddMultipleConstraints, "",
	[ IsGurobiModel, IsList, IsString, IsInt],
	function(Model, ConstraintEquations, ConstraintSense, ConstraintRHSValue)
		# Add error checks
		local senses, rhs, r;
		r := Float(ConstraintRHSValue);
		senses := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintSense);
		rhs := ListWithIdenticalEntries(Size(ConstraintEquations), r);
		GurobiAddMultipleConstraints(Model, ConstraintEquations, senses, rhs);;
		return true;
	end
);

InstallOtherMethod(GurobiAddMultipleConstraints, "",
	[ IsGurobiModel, IsList, IsString, IsFloat, IsString],
	function(Model, ConstraintEquations, ConstraintSense, ConstraintRHSValue, ConstraintName)
		# Add error checks
		local senses, rhs, names;
		senses := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintSense);
		rhs := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintRHSValue);
		names := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintName);
		GurobiAddMultipleConstraints(Model, ConstraintEquations, senses, rhs, names);;
		return true;
	end
);

InstallOtherMethod(GurobiAddMultipleConstraints, "",
	[ IsGurobiModel, IsList, IsString, IsInt, IsString],
	function(Model, ConstraintEquations, ConstraintSense, ConstraintRHSValue, ConstraintName)
		# Add error checks
		local senses, rhs, names, r;
		r := Float(ConstraintRHSValue);
		senses := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintSense);
		rhs := ListWithIdenticalEntries(Size(ConstraintEquations), r);
		names := ListWithIdenticalEntries(Size(ConstraintEquations), ConstraintName);
		GurobiAddMultipleConstraints(Model, ConstraintEquations, senses, rhs, names);;
		return true;
	end
);

InstallMethod(GurobiSolution, "",
	[ IsGurobiModel] ,
	function(model)
		return GurobiDoubleAttributeArray(model, "X");
	end
);

InstallMethod(GurobiSetTimeLimit, "",
	[ IsGurobiModel, IsFloat] ,
	function(model, TimeLimit)
	
		if TimeLimit <= 0.0 then
			Print("ERROR: Time limit must be a positive value!");
			return fail;
		fi;
		GurobiSetDoubleParameter(model, "timelimit", TimeLimit);

		return true;
	end
);


InstallOtherMethod( GurobiSetTimeLimit, "",
	[ IsGurobiModel, IsPosInt ] ,
	function(model, TimeLimit)

	GurobiSetDoubleParameter(model, "timelimit", Float(TimeLimit));

	return true;
	end
);


InstallMethod(GurobiSetBestObjectiveBoundStop, "",
	[ IsGurobiModel, IsFloat] ,
	function(model, BestObjectiveBoundStop)

		GurobiSetDoubleParameter(model, "bestobjstop", BestObjectiveBoundStop);

		return true;
	end
);


InstallOtherMethod( GurobiSetBestObjectiveBoundStop, "",
	[ IsGurobiModel, IsPosInt ] ,
	function(model, BestObjectiveBoundStop)

	GurobiSetDoubleParameter(model, "bestobjstop", Float(BestObjectiveBoundStop));

	return true;
	end
);

InstallMethod(GurobiSetCutOff, "",
	[ IsGurobiModel, IsFloat] ,
	function(model, CutOff)
	

		GurobiSetDoubleParameter(model, "CutOff", CutOff);

		return true;
	end
);


InstallOtherMethod( GurobiSetCutOff, "",
	[ IsGurobiModel, IsPosInt ] ,
	function(model, CutOff)

	GurobiSetDoubleParameter(model, "CutOff", Float(CutOff));

	return true;
	end
);

InstallMethod( GurobiMaximiseModel, "",
	[ IsGurobiModel ] ,
	function(model)

	GurobiSetIntegerAttribute(model, "ModelSense", -1);

	return true;
	end
);

InstallMethod( GurobiMinimiseModel, "",
	[ IsGurobiModel ] ,
	function(model)

	GurobiSetIntegerAttribute(model, "ModelSense", 1);

	return true;
	end
);

InstallMethod( GurobiSetObjectiveFunction, "",
	[ IsGurobiModel, IsList ] ,
	function(model, AttributeArray)

	GurobiSetDoubleAttributeArray(model, "Obj", List(AttributeArray, t -> Float(t)));

	return true;
	end
);

InstallMethod( GurobiObjectiveFunction, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleAttributeArray(model, "Obj");
	end
);

InstallMethod( GurobiNumberOfVariables, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerAttribute(model, "NumVars");
	end
);

InstallMethod( GurobiNumberOfConstraints, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerAttribute(model, "NumConstrs");
	end
);


InstallMethod( GurobiObjectiveValue, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleAttribute(model, "ObjVal");
	end
);

InstallMethod( GurobiObjectiveBound, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleAttribute(model, "ObjBound");
	end
);

InstallMethod( GurobiRunTime, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleAttribute(model, "RunTime");
	end
);

InstallMethod( GurobiOptimisationStatus, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerAttribute(model, "Status");
	end
);

InstallMethod( GurobiNumericFocus, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerParameter(model, "NumericFocus");
	end
);


InstallOtherMethod( GurobiSetNumericFocus, "",
	[ IsGurobiModel, IsPosInt ] ,
	function(model, NumericFocus)

	if not NumericFocus in [0,1,2,3] then
		Print("Error: Numeric focus must be an integer between 0 and 3");
		return fail;
	fi;

	GurobiSetIntegerParameter(model, "NumericFocus", NumericFocus);

	return true;
	end
);

InstallMethod( GurobiTimeLimit, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleParameter(model, "TimeLimit");
	end
);

InstallMethod( GurobiCutOff, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleParameter(model, "CutOff");
	end
);

InstallMethod( GurobiBestObjectiveBoundStop, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleParameter(model, "bestobjstop");
	end
);

InstallOtherMethod( GurobiSetMIPFocus, "",
	[ IsGurobiModel, IsInt ] ,
	function(model, MIPFocus)

	if not MIPFocus in [0,1,2,3] then
		Print("Error: MIP focus must be an integer between 0 and 3");
		return fail;
	fi;

	GurobiSetIntegerParameter(model, "MIPFocus", MIPFocus);

	return true;
	end
);

InstallMethod( GurobiMIPFocus, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerParameter(model, "MIPFocus");
	end
);

InstallOtherMethod( GurobiSetBestBoundStop, "",
	[ IsGurobiModel, IsFloat ] ,
	function(model, BestBdStop )

	GurobiSetDoubleParameter(model, "BestBdStop", BestBdStop );

	return true;
	end
);

InstallMethod( GurobiBestBoundStop, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleParameter(model, "BestBdStop");
	end
);

InstallOtherMethod( GurobiSetSolutionLimit, "",
	[ IsGurobiModel, IsInt ] ,
	function(model, SolutionLimit )

	GurobiSetIntegerParameter(model, "SoluitionLimit", SolutionLimit );

	return true;
	end
);

InstallMethod( GurobiSolutionLimit, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerParameter(model, "SolutionLimit");
	end
);

InstallOtherMethod( GurobiSetIterationLimit, "",
	[ IsGurobiModel, IsFloat ] ,
	function(model,  IterationLimit )

	GurobiSetDoubleParameter(model, "IterationLimit",  IterationLimit );

	return true;
	end
);

InstallMethod( GurobiIterationLimit, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleParameter(model, "IterationLimit");
	end
);

InstallOtherMethod( GurobiSetNodeLimit, "",
	[ IsGurobiModel, IsFloat ] ,
	function(model,  NodeLimit )

	GurobiSetDoubleParameter(model, "NodeLimit",  NodeLimit );

	return true;
	end
);

InstallMethod( GurobiNodeLimit, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiDoubleParameter(model, "NodeLimit");
	end
);

InstallMethod( GurobiVariableNames, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiStringAttributeArray(model, "varname");
	end
);


InstallMethod(GurobiNewModel, "",
	[ IsList, IsList] ,
	function(VariableTypes, variablenames)
		local model;
		model := GUROBINEWMODEL(List(VariableTypes, t -> UppercaseString(t)));
		GurobiSetVariableNames(model, variablenames);
		GurobiUpdateModel(model);
	return model;
	end
);

InstallMethod(GurobiSetVariableNames, "",
	[ IsGurobiModel, IsList] ,
	function(model, variablenames)
		GUROBISETVARIABLENAMES(model, variablenames);
		GurobiUpdateModel(model);
	return true;
	end
);

InstallMethod(GurobiSetLogToConsole, "",
	[ IsGurobiModel, IsBool] ,
	function(model, switch)
		if switch then
			GurobiSetIntegerParameter(model, "LogToConsole", 1);
		else
			GurobiSetIntegerParameter(model, "LogToConsole", 0);
		fi;
	return true;
	end
);

InstallMethod(GurobiLogToConsole, "",
	[ IsGurobiModel ] ,
	function(model )
		if GurobiIntegerParameter(model, "LogToConsole") = 1 then
			return true;
		else
			return false;
		fi;
	end
);

InstallMethod(GurobiDeleteConstraintsWithName, "",
		[IsGurobiModel, IsString],
	function(model, ConstraintName )
		local n, indices, i;
		n := GurobiNumberOfConstraints(model);
		indices :=[];
		for i in [0 .. n-1] do
			if GurobiStringAttributeElement(model, i, "ConstrName") = ConstraintName then
				Add(indices, i);
			fi;
		od;
		GurobiDeleteConstraints(model, indices);
		GurobiUpdateModel(model);
	return true;
	end
);

InstallMethod(GurobiFindAllBinarySolutions, "",
		[IsGurobiModel, IsPosInt],

	function(model, size)
		local good, result, sol, count, i;
		if Set(GurobiVariableTypes(model)) <> [ "B" ] then
			Print("Error: Model must only have binary variables.\n");
			return fail;
		fi;
		Print("Solutions found so far: 0\r");
		GurobiAddConstraint(model, ListWithIdenticalEntries(GurobiNumberOfVariables(model),1) , "=", size, "FindAllSolutionsSizeConstr");
		good:=[];
		GurobiSetTimeLimit(model, 100000000);
		result := GurobiOptimiseModel(model);
		if result = 9 then
			Print("timed out");
			return fail;
		fi;
		if result = 2 then
			count:=1;
		fi;
		while result = 2 do
			sol := GurobiSolution(model);
			sol := List(sol, t -> Int(Round(t)));
			Add(good, sol);
			GurobiAddConstraint(model, sol, "<",size-1, "FindAllSolutionsConstr");
			GurobiUpdateModel(model);
			GurobiReset(model);
			result := GurobiOptimiseModel(model);
			if result = 9 then
				Print("timed out");
				return fail;
			fi;
			count:=count+1;
			if result = 2 then
				Print("Solutions found so far: ", count, "\r");
			fi;
		od;
		if GurobiOptimisationStatus(model) <> 3 then
			Print("\nWarning! Optimisation terminated with status code: ", GurobiOptimisationStatus(model), "\n");
		fi;
		Print("\n");
		GurobiDeleteConstraintsWithName(model, "FindAllSolutionsConstr");
		GurobiDeleteConstraintsWithName(model, "FindAllSolutionsSizeConstr");
		return good;
	end
);

InstallMethod(GurobiFindAllBinarySolutions, "",
		[IsGurobiModel, IsPosInt, IsGroup],

	function(model, size, gp)
		local good, result, sol, count, solution_orbits, i, representatives, all;
		representatives := ValueOption("representatives");
		if Set(GurobiVariableTypes(model)) <> [ "B" ] then
			Print("Error: Model must only have binary variables.\n");
			return fail;
		fi;
		GurobiAddConstraint(model, ListWithIdenticalEntries(GurobiNumberOfVariables(model),1) , "=", size, "FindAllSolutionsSizeConstr");
		Print("Solutions found so far: 0\c");
		good:=[];
		GurobiSetTimeLimit(model, 100000000);
		result := GurobiOptimiseModel(model);
		if result = 9 then
			Print("timed out");
		fi;
		if result = 2 then
			count:=1;
			all := 1;
			if representatives = true then
				Print("\b",1, " (", 1, ")\c");
			else
				Print("\b",1, "\c");
			fi;
		fi;
		while result = 2 do
			sol := GurobiSolution(model);
			sol := List(sol, t -> Int(Round(t)));
			solution_orbits := Orbit(gp, sol, Permuted);;
			if representatives = true then
				Add(good, sol);
				for i in [1 .. Size(String(count))+Size(String(all))+3] do
					Print("\b");
				od;
				all:=all+Size(solution_orbits);
				count:=Size(good);
				Print(count, " (", all, ")\c");
			else
				good:=Concatenation(good, solution_orbits);;
				for i in [1 .. Size(String(count))] do
					Print("\b");
				od;
				count:=Size(good);
				Print(count, "\c");
			fi;
			for sol in solution_orbits do
				GurobiAddConstraint(model, sol, "<", size-1, "FindAllSolutionsConstr");
			od;
			GurobiUpdateModel(model);
			GurobiReset(model);
			result := GurobiOptimiseModel(model);
			if result = 9 then
				Print("timed out");
				return fail;
			fi;
		od;
		Print("\n");
		if GurobiOptimisationStatus(model) <> 3 then
			Print("\nWarning! Optimisation terminated with status code: ", GurobiOptimisationStatus(model), "\n");
		fi;
		GurobiDeleteConstraintsWithName(model, "FindAllSolutionsConstr");
		GurobiDeleteConstraintsWithName(model, "FindAllSolutionsSizeConstr");
		return good;
	end
);


InstallMethod(IndexSetToCharacteristicVector, "",
		[IsList, IsPosInt],

	function(indexset, n)
		local charvec;
		charvec:=ListWithIdenticalEntries(n, 0);
		charvec{indexset}:= ListWithIdenticalEntries(Size(indexset), 1);
		return charvec;
	end
);

InstallMethod(CharacteristicVectorToIndexSet, "",
		[IsList],

	function(charvec)
		local indexset;
		indexset:=Filtered([1 .. Size(charvec)], t -> Int(Round(Float(charvec[t]))) <> 0);
		return indexset;
	end
);


InstallMethod(SubsetToCharacteristicVector, "",
		[IsList, IsList],

	function(indexset, actualset)
		local indexset2;
		indexset2 := List(indexset, t -> Position(actualset, t));
		return IndexSetToCharacteristicVector(indexset2, Size(actualset));
	end
);

InstallMethod(CharacteristicVectorToSubset, "",
		[IsList, IsList],

	function(charvec, actualset)
		local indexset, subset;
		indexset:= CharacteristicVectorToIndexSet(charvec);
		subset := actualset{indexset};
		return subset;
	end
);


InstallMethod( GurobiVariableTypes, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiCharAttributeArray(model, "VType");
	end
);


InstallOtherMethod( GurobiSetMethod, "",
	[ IsGurobiModel, IsInt ] ,
	function(model, MethodType)

	if not MethodType in [-1, 1,2,3,4,5] then
		Print("Error: MethodType must be an integer in [-1,1,2,3,4,5]");
		return fail;
	fi;

	GurobiSetIntegerParameter(model, "method", MethodType);

	return true;
	end
);

InstallMethod( GurobiMethod, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerParameter(model, "method");
	end
);

InstallOtherMethod( GurobiSetThreads, "",
	[ IsGurobiModel, IsInt ] ,
	function(model, ThreadCount)

	if ThreadCount < 0 then
		Print("Error: ThreadCount must be non-negative integer");
		return fail;
	fi;

	GurobiSetIntegerParameter(model, "threads", ThreadCount);

	return true;
	end
);

InstallMethod( GurobiThreads, "",
	[ IsGurobiModel ] ,
	function(model)

	return GurobiIntegerParameter(model, "threads");
	end
);
