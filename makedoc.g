#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# This file is a script which compiles the package manual.
#
if fail = LoadPackage("AutoDoc", "2016.02.16") then
    Error("AutoDoc version 2016.02.16 or newer is required.");
fi;

AutoDoc( rec( maketest := rec( commands := [ "LoadPackage( \"Gurobify\" );"]), scaffold := true, autodoc := rec( files := [ "doc/Intro.autodoc", "src/Gurobify.c"] ) ) );
