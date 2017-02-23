#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# This file runs package tests. It is also referenced in the package
# metadata in PackageInfo.g.
#
LoadPackage( "Gurobify" );

TestDirectory(DirectoriesPackageLibrary( "Gurobify", "tst" ),
  rec(exitGAP := true));

FORCE_QUIT_GAP(1); # if we ever get here, there was an error
