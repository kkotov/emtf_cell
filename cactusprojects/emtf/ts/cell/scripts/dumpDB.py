import re
import os, sys, shutil
import subprocess
"""
A simple helper script that provided with no arguments dumps a list of
top-level keys, and provided with any key from this list as an argument,
dumps a list of sub-keys and saves corresponding configuration to local
files.
"""

# connection string
sqlplusCmd = ['env', 'LD_LIBRARY_PATH=/opt/xdaq/lib/', '/opt/xdaq/bin/sqlplus', '-S', 'cms_trg_r/@cms_omds_lb']

if hash( sqlplusCmd[-1] ) != -2783991022755524048:
    print 'Do not forget to plug password to this script'
    print 'Exiting.'
    exit(0)

# filter out sqlplus formatting
myre = re.compile(r'(ID)|(HW)|(MTF7)|(ALGO)|(AMC13)|(--------------------------------------------------------------------------------)')

# if no arguments are given, query the top level keys only and exit
if len(sys.argv) == 1:
    sqlplus = subprocess.Popen(sqlplusCmd, shell=False, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    print 'No args specified, querying and printing only top-level keys:'
    for line in re.split('\n',sqlplus.communicate('select unique ID from CMS_TRG_L1_CONF.EMTF_KEYS;')[0]):
        if myre.search(line) == None :
            print line
    print 'Pick any of these keys as an argument next time you run this script'
    exit(0)

# if an argument is given query the whole content of the key
key = sys.argv[1]

queryKeys = """
        select
            INFRA_KEYS.MTF7 as MTF7, INFRA_KEYS.AMC13 as AMC13, TOP_KEYS.HW as HW, TOP_KEYS.ALGO as ALGO
        from
            CMS_TRG_L1_CONF.EMTF_INFRA_KEYS INFRA_KEYS
            inner join
            (
            select
                HW, INFRA, ALGO
            from
                CMS_TRG_L1_CONF.EMTF_KEYS
            where
                ID = '{0}'
            ) TOP_KEYS
            on 
                TOP_KEYS.INFRA = INFRA_KEYS.ID
""".format(key)

queryHW    = "select HW.CONF as HW from CMS_TRG_L1_CONF.EMTF_HW HW, ({0}) KEYS where HW.ID = KEYS.HW;".format(queryKeys)
queryMTF7  = "select INFRA_MTF7.CONF as MTF7 from CMS_TRG_L1_CONF.EMTF_INFRA INFRA_MTF7, ({0}) KEYS where INFRA_MTF7.ID = KEYS.MTF7;".format(queryKeys)
queryAMC13 = "select INFRA_AMC13.CONF as AMC13 from CMS_TRG_L1_CONF.EMTF_INFRA INFRA_AMC13, ({0}) KEYS where INFRA_AMC13.ID = KEYS.AMC13;".format(queryKeys)
queryALGO  = "select ALGO.CONF as ALGO from CMS_TRG_L1_CONF.EMTF_ALGO ALGO, ({0}) KEYS where ALGO.ID = KEYS.ALGO;".format(queryKeys)

# write results to the files
batch = {'hw.xml':queryHW, 'mtf7.xml':queryMTF7, 'amc13.xml':queryAMC13, 'algo.xml':queryALGO}

# do the main job here
for fileName,query in batch.iteritems():
    sqlplus = subprocess.Popen(sqlplusCmd, shell=False, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    with open(fileName,'w') as f:
        for line in re.split('\n',sqlplus.communicate('\n'.join(['set linesize 200', 'set longchunksize 200000 long 200000 pages 0',query]))[0]):
            if myre.search(line) == None :
                f.write('\n')
                f.write(line)
        f.close()

sqlplus = subprocess.Popen(sqlplusCmd, shell=False, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
print 'Following sub-keys were queried:'
for line in re.split('\n',sqlplus.communicate(queryKeys+';')[0]):
    print line

print 'Results are saved in ' + ' '.join(batch.keys()) + ' files'
