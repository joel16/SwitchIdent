#!/bin/sh
set -e
set -x

make clean && make
mv gui/gui.nro ./SwitchIdent_GUI.nro
mv console/console.nro ./SwitchIdent_Console.nro
rm .gitattributes .travis.yml devkitpro-pacman.deb LICENSE Makefile README.md
rm -rf common console gui resources source