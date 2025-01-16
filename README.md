# Gurobify
Jesse Lansdown (jesse.lansdown@uwa.edu.au)
[View the manual for Gurobify here.](https://www.jesselansdown.com/Gurobify/doc/manual.pdf)

Gurobify provides a [GAP](https://www.gap-system.org) interface to [Gurobi](http://www.gurobi.com).

Gurobify has the following dependencies:
* GAP 4.12 (or later)
* Gurobi 12.0 (or later)
* AutoTools

Autotools may be installed on MacOSX using homebrew with the commands **brew install autoconf** and **brew install automake**.
Gurobi is proprietary software, however it has free academic licences. Please refer to the Gurobi documentation for more information on its licences.

If you want to regenerate the documentation for any reason, then the following will also be required.

* AutoDoc 2020.08.11 (or later)
* GAPDoc 1.6.4 (or later)

To install Gurobify:

1. Download Gurobify and extract it into the pkg directory of the GAP installation directory.
2. Open the terminal in the Gurobify directory.
3. Run the following commands.

		./install.sh -g <gurobi path> -v <Gurobi version> [-r <gap path>]

If you wish to regenerate the documentation do

		gap.sh makedoc.g

To load Gurobify in GAP:

		LoadPackage( "Gurobify" );