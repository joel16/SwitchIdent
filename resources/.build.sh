#!/bin/sh
set -e
set -x

make clean && make
rm .gitattributes .travis.yml LICENSE Makefile README.md
rm -rf build include resources source SwitchIdent.elf SwitchIdent.nacp SwitchIdent.nso SwitchIdent.pfs0