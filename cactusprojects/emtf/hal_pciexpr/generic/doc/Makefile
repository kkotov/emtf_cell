CURRENT_DIR=$(shell pwd)/..

.PHNY : doc
doc :
	-rm -rf html/*
	awk '{sub("OUTPUT_DIRECTORY.*", "OUTPUT_DIRECTORY       = ./"); \
              sub("INPUT .*", "INPUT                  = $(CURRENT_DIR)"); \
              sub("TAGFILES.*","TAGFILES               = ../../busAdapter/doc/busAdapter.tag=../../busAdapter/html ../../utilities/doc/utilities.tag=../../utilities/html") ; \
              print}' Doxyfile > Doxyfile.local
	doxygen  Doxyfile.local
	mkdir -p ../../doc/generic
	cp -r html ../../doc/generic/.

