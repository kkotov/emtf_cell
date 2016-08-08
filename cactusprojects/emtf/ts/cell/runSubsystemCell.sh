#!/bin/bash


##################################################
# 0. CHECK THAT REQUIRED ENV VARIABLES ARE DEFINED

if [ -z "$SWATCH_DEFAULT_INIT_FILE" ]; then
    echo "Need to set SWATCH_DEFAULT_INIT_FILE!"; exit 1
fi

if [ -z "$SWATCH_DEFAULT_INIT_FILE" ]; then
    echo "Need to set SWATCH_DEFAULT_GATEKEEPER_XML!"; exit 1
fi

if [ -z "$SWATCH_DEFAULT_GATEKEEPER_KEY" ]; then
    echo "Need to set SWATCH_DEFAULT_GATEKEEPER_KEY!"; exit 1
fi

if [ -z "$SUBSYSTEM_ID" ]; then
    echo "Need to set SUBSYSTEM_ID!"; exit 1
fi

if [ -z "$SUBSYSTEM_CELL_LIB_PATH" ]; then
    echo "Need to set SUBSYSTEM_CELL_LIB_PATH!"; exit 1
fi

if [ -z "$SUBSYSTEM_CELL_CLASS" ]; then
    echo "Need to set SUBSYSTEM_CELL_CLASS!"; exit 1
fi


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
# 2. SET COMMON ENVIRONMENT VARIABLES

export XDAQ_ROOT=/opt/xdaq
export XDAQ_DOCUMENT_ROOT=${XDAQ_DOCUMENT_ROOT:-/opt/xdaq/htdocs}

export LD_LIBRARY_PATH=/opt/xdaq/lib:/opt/cactus/lib:${LD_LIBRARY_PATH}



#################################################
# 3. CREATE MODIFIED CONFIG FILES UNDER /tmp  (AS NEEDED)

TMP_RUNCELL_DIR=/tmp/${SUBSYSTEM_ID}cell
mkdir -p ${TMP_RUNCELL_DIR}
CONFIGURE_FILE=${TMP_RUNCELL_DIR}/standalone.configure

HERE=$(python -c "import os.path; print os.path.dirname(os.path.abspath('$BASH_SOURCE'))")

MODULE_TAGS=""

for i_LIB_PATH in $(echo "${SUBSYSTEM_CELL_LIB_PATH}" | tr ";" "\n")
do
    echo "Adding module : $i_LIB_PATH"
    MODULE_TAGS="${MODULE_TAGS}<xc:Module>${i_LIB_PATH}</xc:Module>"
done

# sed "s|__HOSTNAME__PORT__|`hostname`:3333|" ${HERE}/../../etc/swatchcell/common.configure > $CONFIGURE_FILE
sed "s|__HOSTNAME__PORT__|`hostname`:3234|" /opt/xdaq/etc/swatchcell/common.configure > $CONFIGURE_FILE
sed -i "s|__SUBSYSTEM_CELL_CLASS__|${SUBSYSTEM_CELL_CLASS}|" ${CONFIGURE_FILE}
sed -i "s|__SUBSYSTEM_ID__|${SUBSYSTEM_ID}|" ${CONFIGURE_FILE}
sed -i "s|__MODULES_TO_LOAD__|${MODULE_TAGS}|" ${CONFIGURE_FILE}


export SWATCH_LOG4CPLUS_CONFIG=log4cplus.properties
# sed "s|__LOGFILE__|${TMP_RUNCELL_DIR}/swatch.log|" ${HERE}/../../etc/swatchcell/log4cplus.properties  >  ${SWATCH_LOG4CPLUS_CONFIG}
# sed "s|__LOGFILE__|${TMP_RUNCELL_DIR}/swatch.log|" /opt/xdaq/etc/swatchcell/log4cplus.properties >  ${SWATCH_LOG4CPLUS_CONFIG}
# sed -i "s|__VERBOSE_LOGFILE__|${TMP_RUNCELL_DIR}/swatch_verbose.log|" ${SWATCH_LOG4CPLUS_CONFIG}
# sed -i "s|__SUBSYSTEM_ID__|${SUBSYSTEM_ID}|" ${SWATCH_LOG4CPLUS_CONFIG}

#################################################
# 4. START THE CELL (interactively)

# PROFILE_FILE=${HERE}/../../etc/swatchcell/common.profile
PROFILE_FILE=/opt/xdaq/etc/swatchcell/common.profile

CMD="/opt/xdaq/bin/xdaq.exe -p 3234 -c ${CONFIGURE_FILE} -e ${PROFILE_FILE}"
if [[ "${RUN_GDB}" -eq 1 ]]; then
    cgdb --args $CMD
else
    # Enable core dumps
    ulimit -S -c unlimited
    # Go!
    $CMD
fi

