

InstallMethod(GurobiNewModel, "",
	[ IsList, IsList] ,
	function(VariableTypes, VariableNames)
		return GUROBINEWMODEL(List(VariableTypes, t -> UppercaseString(t)), VariableNames);
	end
);

InstallMethod(GurobiNewModel, "",
	[ IsList] ,
	function(VariableTypes)
		return GUROBINEWMODEL(List(VariableTypes, t -> UppercaseString(t)), ListWithIdenticalEntries(Size(VariableTypes),0.));
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

InstallMethod(GurobiSolution, "",
	[ IsGurobiModel] ,
	function(model)
		return GurobiAttributeArray(model, "X");
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

	return GurobiAttributeArray(model, "Obj");
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

InstallMethod( GurobiStatus, "",
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

	return GurobiIntegerParameter(model, "TimeLimit");
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

InstallOtherMethod( GurobiSetNumericFocus, "",
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