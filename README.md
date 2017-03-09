# Gurobify
Gurobify provides an interface to Gurobi from GAP.
[View the manual here.](https://github.com/jesselansdown/Gurobify/blob/master/doc/manual.pdf) Please note that this is a beta version of Gurobify and the documentation and features are constantly being updated.

Requirements:
* Gurobi 7.0
* AutoTools 

(AutoTools can be installed on a mac using homebrew: 
	- brew install autoconf
	- brew install automake)

To install the Gurobify package, move it to the pkg directory of the GAP installation directory.
Inside the Gurobify directory run:

* ./autogen.sh (Requires AutoTools installed)

* ./configure --with-gurobi=\<gurobi path> [--with-gaproot=\<gap path>]

* make

To regenerate the documentation do

* gap.sh makedoc.g

To load package in GAP:

		LoadPackage( "Gurobify");
