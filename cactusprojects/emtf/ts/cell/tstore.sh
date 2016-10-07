#!/bin/bash

#################################################
# 1. PARSE SCRIPT ARGUMENTS 

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

RUN_GDB=0

while getopts ":g" opt; do
case $opt in
    g)
        echo "-g was triggered, Parameter: $OPTARG" >&2
        RUN_GDB=1
        ;;
    \?)
        echo "Invalid option: -$OPTARG" >&2
        exit 1
        ;;
esac
done


##################################################
# 2. SET ENVIRONMENT VARIABLES

export XDAQ_ROOT=/opt/xdaq
export XDAQ_DOCUMENT_ROOT=/opt/xdaq/htdocs

export LD_LIBRARY_PATH=/opt/xdaq/lib:/opt/cactus/lib:${LD_LIBRARY_PATH}
#export LD_LIBRARY_PATH=/nfshome0/kkotov/emtf_swatch_control_software/cactuscore/swatchcell/framework/lib/linux/x86_64_slc6:/nfshome0/kkotov/emtf_swatch_control_software/cactuscore/candela/lib/linux/x86_64_slc6:/opt/xdaq/lib:/opt/cactus/lib:$LD_LIBRARY_PATH

HERE=$(python -c "import os.path; print os.path.dirname(os.path.abspath('$BASH_SOURCE'))")

# SWATCH_ROOT=${HERE}/../../../swatch
SWATCH_ROOT=/opt/cactus/include/swatch
export LD_LIBRARY_PATH=${SWATCH_ROOT}/logger/lib:${SWATCH_ROOT}/core/lib:${SWATCH_ROOT}/processor/lib:${SWATCH_ROOT}/system/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${SWATCH_ROOT}/amc13/lib:${SWATCH_ROOT}/mp7/lib:${LD_LIBRARY_PATH}

export swatchexample_ROOT="../../../../cactuscore/swatchcell/"
export swatchframework_ROOT=${swatchexample_ROOT}/../framework

export LD_LIBRARY_PATH=${swatchframework_ROOT}/lib/linux/x86_64_slc6:${LD_LIBRARY_PATH}

export PWD_PATH=/home/emtfts/cactus_dev/cactusprojects/emtf/ts/cell
#################################################
# 3. START THE CELL (interactively)

mkdir -p /tmp/swatchcell/
CONFIGURE_FILE=/tmp/common.configure

sed "s/__HOSTNAME__PORT__/`hostname`:3234/" ./common.configure > $CONFIGURE_FILE

CMD="/opt/xdaq/bin/xdaq.exe -p 1972 -c ${CONFIGURE_FILE}"
if [[ "${RUN_GDB}" -eq 1 ]]; then
    gdb --args $CMD
else
    $CMD
fi

