#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# Reading the declaration part of the package.
#
_PATH_SO:=Filename(DirectoriesPackagePrograms("Gurobify"), "Gurobify.so");
if _PATH_SO <> fail then
    LoadDynamicModule(_PATH_SO);
fi;
Unbind(_PATH_SO);

ReadPackage( "Gurobify", "gap/Gurobify.gd");
