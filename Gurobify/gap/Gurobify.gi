#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Implementations
#
InstallGlobalFunction( GurobifySolve,
function( file_name )
	
	local TimeLimitON, TimeLimitValue, ConsoleOutputON, time_limit, to_console;
	#Set the default values for the other parameters
	TimeLimitON := false;
		TimeLimitValue := 10;
	ConsoleOutputON := false;



	time_limit := ValueOption("TimeLimit");
	if not time_limit = fail then
		TimeLimitON := true;
		TimeLimitValue := time_limit;
	fi;

	to_console := ValueOption("ToConsole");
	if not to_console = fail then
		ConsoleOutputON := to_console;
	fi;


	return GurobiSolve(file_name, TimeLimitON, TimeLimitValue, ConsoleOutputON);

end );

