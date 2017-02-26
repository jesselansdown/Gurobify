#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Implementations
#


InstallMethod( GurobifySolve, "",
	[ IsString] ,
	function( file_name )
		
		local TimeLimitON, TimeLimitValue, ConsoleOutputON, BestObjStopON, BestObjStopValue, NumericFocusON, NumericFocusValue,
				CutOffON, CutOffValue, time_limit, to_console, best_obj_bound, numeric_focus, cut_off, mat, sense, rhs;
		#Set the default values for the other parameters
		mat := [];
		sense := [];
		rhs:= [];

		TimeLimitON := false;
			TimeLimitValue := 10;
		BestObjStopON := false;
			BestObjStopValue := 1;
		NumericFocusON := false;
			NumericFocusValue := 0;
		CutOffON := false;
			CutOffValue := 1;
		ConsoleOutputON := false;

		time_limit := ValueOption("TimeLimit");
		if not time_limit = fail then
			TimeLimitON := true;
			TimeLimitValue := time_limit;
		fi;

		best_obj_bound := ValueOption("BestObjBound");
		if not best_obj_bound = fail then
			BestObjStopON := true;
			BestObjStopValue := best_obj_bound;
		fi;

		numeric_focus := ValueOption("NumericFocus");
		if not numeric_focus = fail then
			NumericFocusON := true;
			NumericFocusValue := numeric_focus;
		fi;

		cut_off := ValueOption("Cutoff");
		if not cut_off = fail then
			CutOffON := true;
			CutOffValue := cut_off;
		fi;

		to_console := ValueOption("LogToConsole");
		if not to_console = fail then
			ConsoleOutputON := to_console;
		fi;

		return GurobiSolve(file_name, false, mat, sense, rhs, [TimeLimitON, TimeLimitValue, BestObjStopON, BestObjStopValue, NumericFocusON, NumericFocusValue,
							CutOffON, CutOffValue, ConsoleOutputON]);

	end
);



InstallOtherMethod( GurobifySolve, "",
	[ IsString, IsList, IsList, IsList] ,
	function( file_name , mat, sense, rhs)
		
		local TimeLimitON, TimeLimitValue, ConsoleOutputON, BestObjStopON, BestObjStopValue, NumericFocusON, NumericFocusValue,
				CutOffON, CutOffValue, time_limit, to_console, best_obj_bound, numeric_focus, cut_off;
		#Set the default values for the other parameters


		TimeLimitON := false;
			TimeLimitValue := 10;
		BestObjStopON := false;
			BestObjStopValue := 1;
		NumericFocusON := false;
			NumericFocusValue := 0;
		CutOffON := false;
			CutOffValue := 1;
		ConsoleOutputON := false;

		time_limit := ValueOption("TimeLimit");
		if not time_limit = fail then
			TimeLimitON := true;
			TimeLimitValue := time_limit;
		fi;

		best_obj_bound := ValueOption("BestObjBound");
		if not best_obj_bound = fail then
			BestObjStopON := true;
			BestObjStopValue := best_obj_bound;
		fi;

		numeric_focus := ValueOption("NumericFocus");
		if not numeric_focus = fail then
			NumericFocusON := true;
			NumericFocusValue := numeric_focus;
		fi;

		cut_off := ValueOption("Cutoff");
		if not cut_off = fail then
			CutOffON := true;
			CutOffValue := cut_off;
		fi;

		to_console := ValueOption("LogToConsole");
		if not to_console = fail then
			ConsoleOutputON := to_console;
		fi;

		return GurobiSolve(file_name, true, mat, sense, rhs, [TimeLimitON, TimeLimitValue, BestObjStopON, BestObjStopValue, NumericFocusON, NumericFocusValue,
							CutOffON, CutOffValue, ConsoleOutputON]);

	end
);



# InstallGlobalFunction( GurobifySolve,
# function( file_name )
	
# 	local TimeLimitON, TimeLimitValue, ConsoleOutputON, BestObjStopON, BestObjStopValue, NumericFocusON, NumericFocusValue,
# 			CutOffON, CutOffValue, time_limit, to_console, best_obj_bound, numeric_focus, cut_off;
# 	#Set the default values for the other parameters
# 	mat := [];
# 	sense := [];
# 	rhs:= [];

# 	TimeLimitON := false;
# 		TimeLimitValue := 10;
# 	BestObjStopON := false;
# 		BestObjStopValue := 1;
# 	NumericFocusON := false;
# 		NumericFocusValue := 0;
# 	CutOffON := false;
# 		CutOffValue := 1;
# 	ConsoleOutputON := false;

# 	time_limit := ValueOption("TimeLimit");
# 	if not time_limit = fail then
# 		TimeLimitON := true;
# 		TimeLimitValue := time_limit;
# 	fi;

# 	best_obj_bound := ValueOption("BestObjBound");
# 	if not best_obj_bound = fail then
# 		BestObjStopON := true;
# 		BestObjStopValue := best_obj_bound;
# 	fi;

# 	numeric_focus := ValueOption("NumericFocus");
# 	if not numeric_focus = fail then
# 		NumericFocusON := true;
# 		NumericFocusValue := numeric_focus;
# 	fi;

# 	cut_off := ValueOption("Cutoff");
# 	if not cut_off = fail then
# 		CutOffON := true;
# 		CutOffValue := cut_off;
# 	fi;

# 	to_console := ValueOption("ToConsole");
# 	if not to_console = fail then
# 		ConsoleOutputON := to_console;
# 	fi;

# 	return GurobiSolve(file_name, false, mat, sense, rhs, [TimeLimitON, TimeLimitValue, BestObjStopON, BestObjStopValue, NumericFocusON, NumericFocusValue,
# 						CutOffON, CutOffValue, ConsoleOutputON]);

# end );

