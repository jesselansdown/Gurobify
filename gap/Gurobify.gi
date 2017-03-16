

InstallMethod(GetSolution, "",
	[ IsGurobiModel] ,
	function(model)
		return GurobiGetAttributeArray(model, "X");
	end
);

InstallMethod(SetTimeLimit, "",
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


InstallOtherMethod( SetTimeLimit, "",
	[ IsGurobiModel, IsPosInt ] ,
	function(model, TimeLimit)

	GurobiSetDoubleParameter(model, "timelimit", Float(TimeLimit));

	return true;
	end
);


InstallMethod(SetBestObjectiveBoundStop, "",
	[ IsGurobiModel, IsFloat] ,
	function(model, BestObjectiveBoundStop)

		GurobiSetDoubleParameter(model, "bestobjstop", BestObjectiveBoundStop);

		return true;
	end
);


InstallOtherMethod( SetBestObjectiveBoundStop, "",
	[ IsGurobiModel, IsPosInt ] ,
	function(model, BestObjectiveBoundStop)

	GurobiSetDoubleParameter(model, "bestobjstop", Float(BestObjectiveBoundStop));

	return true;
	end
);

InstallMethod(SetCutOff, "",
	[ IsGurobiModel, IsFloat] ,
	function(model, CutOff)
	

		GurobiSetDoubleParameter(model, "CutOff", CutOff);

		return true;
	end
);


InstallOtherMethod( SetCutOff, "",
	[ IsGurobiModel, IsPosInt ] ,
	function(model, CutOff)

	GurobiSetDoubleParameter(model, "CutOff", Float(CutOff));

	return true;
	end
);