CURRENT_DIR=$(shell pwd)/..

.PHONY : doc
doc :
	-rm -rf html/*
	awk '{sub("OUTPUT_DIRECTORY.*", "OUTPUT_DIRECTORY       = ./"); \
              sub("INPUT .*", "INPUT                  = $(CURRENT_DIR)"); \
              sub("TAGFILES.*","TAGFILES               = ../../generic/doc/generic.tag=../../generic/html ../../utilities/doc/utilities.tag=../../utilities/html"); \
              print}' Doxyfile > Doxyfile.local
	doxygen  Doxyfile.local
	mkdir -p ../../doc/busAdapter
	cp -r html ../../doc/busAdapter/.
