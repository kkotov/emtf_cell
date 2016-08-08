CURRENT_DIR=$(shell pwd)/..

.PHONY : doc
doc :
	-rm -rf html/*
	awk '{sub("OUTPUT_DIRECTORY.*", "OUTPUT_DIRECTORY       = ./"); \
              sub("INPUT .*", "INPUT                  = $(CURRENT_DIR)"); \
              sub("TAGFILES.*","TAGFILES               = ../../busAdapter/doc/busAdapter.tag=../../busAdapter/html ../../generic/doc/generic.tag=../../generic/html") ; \
              print}' Doxyfile > Doxyfile.local
	doxygen Doxyfile.local

	mkdir -p ../../doc/utilities
	cp -r html ../../doc/utilities/.
