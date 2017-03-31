#
# Gurobify: Gurobify provides an interface to Gurobi from &GAP;.
#
# This file contains package meta data. For additional information on
# the meaning and correct usage of these fields, please consult the
# manual of the "Example" package as well as the comments in its
# PackageInfo.g file.
#
SetPackageInfo( rec(

PackageName := "Gurobify",
Subtitle := "A GAP interface to Gurobi.",
Version := "1.0.0",
Date := "31/03/2017", # dd/mm/yyyy format

Persons := [
  rec(
    IsAuthor := true,
    IsMaintainer := true,
    FirstNames := "Jesse",
    LastName := "Lansdown",
    WWWHome := "http://www.jesselansdown.com",
    Email := "jesse.lansdown@research.uwa.edu.au",
    PostalAddress := Concatenation(
                      "Jesse Lansdown\n",
                      "Lehrstuhl B für Mathematik\n",
                      "RWTH Aachen\n",
                      "Pontdriesch 10 - 16\n",
                      "52062 Aachen\n",
                      "Germany"
                      ),
    Place := "Aachen",
    Institution := "The University of Western Australia/RWTH Aachen University",
  ),
],

#SourceRepository := rec( Type := "TODO", URL := "URL" ),
#IssueTrackerURL := "https://github.com/jesselansdown/Gurobify/issues",
#SupportEmail := "TODO",

PackageWWWHome := "http://jesselansdown.com/Gurobify/",

PackageInfoURL := Concatenation( ~.PackageWWWHome, "PackageInfo.g" ),
README_URL     := Concatenation( ~.PackageWWWHome, "README.md" ),
ArchiveURL     := Concatenation( ~.PackageWWWHome,
                                 "/", ~.PackageName, "-", ~.Version ),

ArchiveFormats := ".tar.gz",

##  Status information. Currently the following cases are recognized:
##    "accepted"      for successfully refereed packages
##    "submitted"     for packages submitted for the refereeing
##    "deposited"     for packages for which the GAP developers agreed
##                    to distribute them with the core GAP system
##    "dev"           for development versions of packages
##    "other"         for all other packages
##
Status := "dev",

AbstractHTML   :=  "",

PackageDoc := rec(
  BookName  := "Gurobify",
  ArchiveURLSubset := ["doc"],
  HTMLStart := "doc/chap0.html",
  PDFFile   := "doc/manual.pdf",
  SixFile   := "doc/manual.six",
  LongTitle := "Gurobify provides an interface to Gurobi from GAP.",
),

Dependencies := rec(
  GAP := ">= 4.8",
  NeededOtherPackages := [ [ "GAPDoc", ">= 1.5" ] ],
  SuggestedOtherPackages := [ ],
  ExternalConditions := [ "Gurobi", "=7.0" ],
),

AvailabilityTest := function()
        return true;
    end,

TestFile := "tst/testall.g",

#Keywords := [ "TODO" ],

AutoDoc := rec(
    TitlePage := rec(
        Copyright := Concatenation("&copyright; 2017 Jesse Lansdown<P/>\n\n",
                    "Gurobify is subject to the terms of the Mozilla ",
                    "Public License, v. 2.0. If a copy of the MPL was not distributed ",
                    "with this file, You can obtain one at https://mozilla.org/MPL/2.0/.\n"),
        Abstract := Concatenation("Gurobify provides an interface to the Gurobi Optimizer software from &GAP;. ",
          "It enables the creation and modification of mixed integer and linear programmming models which can be solved directly ",
          "by Gurobi from within the &GAP; environment."),
        Acknowledgements := Concatenation("I thank Sebastian Gutsche for generously taking the time ",
          "to explain the inner workings of &GAP; and &GAP; packages to me, and for pointing me towards examples. ",
          "I also thank John Bamberg for introducing me to both &GAP; and Gurobi ",
          "and showing me how they can be used to so effectively complement each other. ",
          "I used the AutoDoc<Cite Key=\"autodoc\"/> package to streamline ",
          "the creation of the documentation for this package, ",
          "and PackageMaker<Cite Key=\"packagemaker\"/> to generate a package template. ",
           "I would also like to acknowledge the support of an Australian Government Research Training Program (RTP) Scholarship ",
           "while writing this software.")
    )
),


));


