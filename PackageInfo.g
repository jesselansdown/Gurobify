#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# This file contains package meta data. For additional information on
# the meaning and correct usage of these fields, please consult the
# manual of the "Example" package as well as the comments in its
# PackageInfo.g file.
#
SetPackageInfo( rec(

PackageName := "Gurobify",
Subtitle := "A GAP interface to Gurobi.",
Version := "0.1",
Date := "23/02/2017", # dd/mm/yyyy format

Persons := [
  rec(
    IsAuthor := true,
    IsMaintainer := true,
    FirstNames := "Jesse",
    LastName := "Lansdown",
    WWWHome := "www.jesselansdown.com",
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

PackageWWWHome := "https://github.com/jesselansdown/Gurobify/",

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
        Copyright := Concatenation(
                    "&copyright; 2017 Jesse Lansdown<P/>\n\n",
                    "Licence to come\n"),
        Abstract := "TODO",
        Acknowledgements := "TODO"
    )
),


));


