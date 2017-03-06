


InstallMethod(SetTimeLimit, "",
	[ IsGurobiModel, IsFloat] ,
	function(model, TimeLimit)
	
	local edge1, edge2, edge3, edge4, edge5, edge6, vertex1, vertex2, vertex3,
		vertex4, newVerticesOfEdges, newEdgesOfFaces, newVertices, newEdges, newFaces;

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
	function(model, BestObjectiveBoundStop)

	GurobiSetDoubleParameter(model, "CutOff", Float(CutOff));

	return true;
	end
);