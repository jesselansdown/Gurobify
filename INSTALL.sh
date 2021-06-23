#!/bin/sh
gap=../..
while getopts r:g: flag
do
    case "${flag}" in
        r) gap=${OPTARG};;
        g) gurobi=${OPTARG};;
    esac
done

./autogen.sh
./configure --with-gaproot=${gap} --with-gurobi=${gurobi}
./autogen.sh
./configure --with-gaproot=${gap} --with-gurobi=${gurobi}
make

echo
echo "Installation attempted with GAP root directory: "
echo $gap
echo "and Gurobi directory: "
echo $gurobi
echo
echo "If these are incorrect, please try again with the following flags: "
echo "-r <path-to-GAP-root> -g <path-to-Gurobi>"
echo "(Refer to the documentation for more details)"
echo