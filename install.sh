#!/bin/sh
gap=../..
while getopts r:g:v: flag
do
    case "${flag}" in
        r) gap=${OPTARG};;
        g) gurobi=${OPTARG};;
        v) version=$OPTARG;;
    esac
done

echo $version > GUROBIVERSION
./autogen.sh
./configure --with-gaproot=${gap} --with-gurobi=${gurobi}
make

echo
echo "Installation attempted with GAP root directory: " $gap
echo "and Gurobi directory: " $gurobi
echo "and Gurobi version:" $version
echo
echo "If these are incorrect, please try again with the following flags: "
echo "-r <path-to-GAP-root> -g <path-to-Gurobi> -v <Gurobi-version>"
echo "(Refer to the documentation for more details)"
echo
