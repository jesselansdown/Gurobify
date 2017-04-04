## This file is automatically generated by AutoDoc.
## Changes will be discarded by the next call of the AutoDoc method.


LoadPackage( "Gurobify" );

AUTODOC_file_scan_list := [ "../PackageInfo.g", "../examples/SudokuExample.g", "../examples/example.g", "../gap/Gurobify.gd", "../gap/Gurobify.gi", "../init.g", "../makedoc.g", "../maketest.g", "../read.g" ];

LoadPackage( "GAPDoc" );

example_tree := ExtractExamples( Directory("./doc/"), "Gurobify.xml", AUTODOC_file_scan_list, 500 );

RunExamples( example_tree, rec( compareFunction := "uptowhitespace" ) );

QUIT;