#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Reading the implementation part of the package.
#
ReadPackage( "Gurobify", "gap/Gurobify.gi");

# Print the Gurobi version upon loading package
Print("Gurobify is running Gurobi Optimizer version ", String(GurobiVersion()), ".\n");