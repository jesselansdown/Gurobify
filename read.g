#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Reading the implementation part of the package.
#
ReadPackage( "Gurobify", "gap/Gurobify.gi");

# Print the Gurobi version upon loading package
PackageInfo("Gurobify")[1].PackageWWWHome:=Concatenation(PackageInfo("Gurobify")[1].PackageWWWHome, "\n     Currently running Gurobi Optimizer ", String(GurobiVersion()[1]), ".", String(GurobiVersion()[2]), ".", String(GurobiVersion()[3]));;