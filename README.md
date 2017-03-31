# Gurobify
Jesse Lansdown (jesse.lansdown@research.uwa.edu.au)

Gurobify provides a [GAP](https://www.gap-system.org) interface to [Gurobi](http://www.gurobi.com).
[View the manual for Gurobify here.](https://github.com/jesselansdown/Gurobify/blob/master/doc/manual.pdf)

Gurobify has the following dependencies:
* GAP 4.8 (or later)
* Gurobi 7.0
* AutoTools

Autotools may be installed on MacOSX using homebrew with the commands **brew install autoconf** and **brew install automake**.
Gurobi is proprietary software, however it has free academic licences. Please refer to the Gurobi documentation for more information on its licences.

If you want to regenerate the documentation for any reason, then the following will also be required.

* AutoDoc 2016.03.08 (or later)
* GAPDoc 1.5 (or later)

To install Gurobify:

1. Download Gurobify and extract it into the pkg directory of the GAP installation directory.
2. Open the terminal in the Gurobify directory.
3. Run the following commands.

		./autogen.sh (Requires AutoTools installed)
		./configure --with-gurobi=\<gurobi path> [--with-gaproot=\<gap path>]
		make

If you wish to regenerate the documentation do

		gap.sh makedoc.g

To load Gurobify in GAP:

		LoadPackage( "Gurobify" );