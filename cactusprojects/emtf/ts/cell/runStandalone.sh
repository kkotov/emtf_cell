#!/bin/bash

HERE=$(python -c "import os.path; print os.path.dirname(os.path.abspath('$BASH_SOURCE'))")
export EMTF_ROOT=${HERE}
export LD_LIBRARY_PATH="${EMTF_ROOT}/lib/linux/x86_64_centos7/"

export SUBSYSTEM_ID=emtf
SUBSYSTEM_CELL_LIB_PATH="$SUBSYSTEM_CELL_LIB_PATH;file:///${EMTF_ROOT}/lib/linux/x86_64_centos7/libemtfcell.so;"

export SUBSYSTEM_CELL_LIB_PATH
export SUBSYSTEM_CELL_CLASS='emtf::Cell'

export SWATCH_DEFAULT_INIT_FILE='${EMTF_ROOT}/exampleP5.xml'
export SWATCH_DEFAULT_GATEKEEPER_XML='${EMTF_ROOT}/emtfTopLevel.xml'
export SWATCH_DEFAULT_GATEKEEPER_KEY='emtf_key'

./runSubsystemCell.sh

