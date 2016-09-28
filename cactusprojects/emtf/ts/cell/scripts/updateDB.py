from optparse import OptionParser
import subprocess
import re
import time
import sys 
import getpass

"""
A simple helper script that, provided with a new configuration of any
of the four kinds (hw/processor/daq/algo), uploads the corresponding
XML file into the relevant table indexing it with a next version of
key and creates a top level key for this new configuration
"""
# parse the options
parser = OptionParser()
parser.add_option("-c", "--hardware", dest="hardware", default=None,
                  help="read hardware XML configuration from FILE", metavar="FILE")
#parser.add_option("-r", "--runsettings", dest="rs", default=None,
#                  help="read run settings XML configuration from FILE", metavar="FILE")
parser.add_option("-p", "--processor", dest="processor", default=None,
                  help="read MTF7 processor XML configuration from FILE", metavar="FILE")
parser.add_option("-d", "--daq", dest="daq", default=None,
                  help="read AMC13 XML configuration from FILE", metavar="FILE")
parser.add_option("-a", "--algo", dest="algo", default=None,
                  help="read ALGO XML configuration from FILE", metavar="FILE")

(options, args) = parser.parse_args()

# define DB/keys structure
confTable   = {'hardware':'EMTF_HW',   'algo':'EMTF_ALGO',        'processor':'EMTF_INFRA',            'daq':'EMTF_INFRA',             'key':'EMTF_KEYS',       'infraKey':'EMTF_INFRA_KEYS'} #'rs':'EMTF_RS',  
keyTemplate = {'hardware':'EMTF_HW/v', 'algo':'EMTF_ALGO_BASE/v', 'processor':'EMTF_MTF7_base_conf/v', 'daq':'EMTF_AMC13_base_conf/v', 'key':'EMTF_Base_Key/v', 'infraKey':'EMTF_Base_Infra_Key/v'} #'rs':'EMTF_RS/v',

# read-only version of the DB command
sqlplusCmd = ['env', 'LD_LIBRARY_PATH=/opt/xdaq/lib/', '/opt/xdaq/bin/sqlplus', '-S', 'cms_emtf/@cms_omds_lb']
#sqlplusCmd = ['env', 'LD_LIBRARY_PATH=/opt/xdaq/lib/', '/opt/xdaq/bin/sqlplus', '-S', 'cms_trg_l1_subsystem/@int2r']
#if hash( sqlplusCmd[-1] ) != -5508203199304911491:

if hash( sqlplusCmd[-1] ) != -7405555605401402394:
    print 'Do not forget to plug password to this script'
    print 'Exiting.'
    exit(0)

# find all next key versions
keyVersion = {}
for kind,table in confTable.iteritems() :

    query = "select ID from CMS_TRG_L1_CONF.{0} where ID like '{1}%';".format( table, keyTemplate[kind] );
    sqlplus = subprocess.Popen(sqlplusCmd, shell=False, stdout=subprocess.PIPE, stdin=subprocess.PIPE)

    last = -1
    for line in re.split('\n',sqlplus.communicate(query)[0]) :
        if re.match( ''.join( [ keyTemplate[kind], r'(\d+)$' ] ), line ) != None :
            version = re.sub( ''.join( [ keyTemplate[kind],r'(\d+)$']), '\g<1>', line )
#            print line, version
            if last < int(version):
                last = int(version)

    if last >= 0 :
        keyVersion[kind] = last

# what do we have?
print 'Current keys are:\n\n' + '\n'.join( [ kind + ": " + template + str(keyVersion[kind]) for kind,template in keyTemplate.iteritems() ] ) + '\n'
if len(sys.argv)==1:
    print "(no options - doing nothing and exiting)"
    exit(0)

# load in the xmls specified in the script options
payload = {}
for kind,xmlFile in vars(options).iteritems():
    if xmlFile != None:
        with open( xmlFile, 'r' ) as xmlSource:
            payload[kind] = xmlSource.readlines()
            xmlSource.close()


# inserting the payloads into conf tables
for kind,xmlText in payload.iteritems():

    query1 = "insert into CMS_TRG_L1_CONF.{0} (ID,CREATION_DATE,DESCRIPTION,CONF,AUTHOR) values ('{1}',TO_TIMESTAMP_TZ('{2}', 'DD-Mon-RR HH24:MI:SS TZHTZM'),'{3}',{4},'{5}');".format(
                 confTable[kind],
                 ''.join([ keyTemplate[kind], str(keyVersion[kind] + 1)]),
                 time.strftime("%d-%b-%Y %H:%M:%S %z", time.localtime()),
                 'Uploaded with ' + ' '.join(sys.argv),
                 ' || '.join([ "to_clob('"+line+"')" if re.match('\s*$',line) == None else "chr(10)" for line in xmlText ]),
                 getpass.getuser()
             )


    sqlplus = subprocess.Popen(sqlplusCmd, shell=False, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    retval = sqlplus.communicate(query1)[0]
    if '1 row created.' not in retval:
        print 'Failed query:', query1
        exit(0)
    else:
        print 'Successfully inserted new ' + ''.join([ keyTemplate[kind], str(keyVersion[kind] + 1)]) + ' key'


# inserting the keys into key tables

#  intermediate level
if 'daq' in payload or 'processor' in payload:
    middleQuery = "insert into CMS_TRG_L1_CONF.EMTF_INFRA_KEYS (ID,CREATION_DATE,DESCRIPTION,AUTHOR,AMC13,MTF7) values ('{0}',TO_TIMESTAMP_TZ('{1}', 'DD-Mon-RR HH24:MI:SS TZHTZM'),'{2}','{3}'".format(
                      ''.join([ keyTemplate['infraKey'], str(keyVersion['infraKey'] + 1)]),
                      time.strftime("%d-%b-%Y %H:%M:%S %z", time.localtime()),
                      'Uploaded with ' + ' '.join(sys.argv),
                      getpass.getuser(),
                  )

    if 'daq' in payload :
        middleQuery += ",'{0}'".format( ''.join([ keyTemplate['daq'], str(keyVersion['daq'] + 1)]) )
    else :
        middleQuery += ",(select AMC13 from CMS_TRG_L1_CONF.EMTF_INFRA_KEYS where ID='{0}')".format( ''.join([ keyTemplate['infraKey'], str(keyVersion['infraKey'])]) )

    if 'processor' in payload:
        middleQuery += ",'{0}'".format( ''.join([ keyTemplate['processor'], str(keyVersion['processor'] + 1)]) )
    else :
        middleQuery += ",(select MTF7 from CMS_TRG_L1_CONF.EMTF_INFRA_KEYS where ID='{0}')".format( ''.join([ keyTemplate['infraKey'], str(keyVersion['infraKey'])]) )

    middleQuery += ");"

    sqlplus = subprocess.Popen(sqlplusCmd, shell=False, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    retval = sqlplus.communicate(middleQuery)[0]
    if '1 row created.' not in retval:
        print 'Failed query:', middleQuery
        exit(0)
    else:
        print 'Successfully inserted new ' + ''.join([ keyTemplate['infraKey'], str(keyVersion['infraKey'] + 1)]) + ' key'


#  top level
topQuery = "insert into CMS_TRG_L1_CONF.EMTF_KEYS (ID,CREATION_DATE,DESCRIPTION,AUTHOR,ALGO,HW,INFRA) values ('{0}',TO_TIMESTAMP_TZ('{1}', 'DD-Mon-RR HH24:MI:SS TZHTZM'),'{2}','{3}'".format(
               ''.join([ keyTemplate['key'], str(keyVersion['key']+1)]),
               time.strftime("%d-%b-%Y %H:%M:%S %z", time.localtime()),
               'Uploaded with ' + ' '.join(sys.argv),
               getpass.getuser(),
           )

if 'algo' in payload :
    topQuery += ",'{0}'".format( ''.join([ keyTemplate['algo'], str(keyVersion['algo'] + 1)]) )
else :
    topQuery += ",'{0}'".format( ''.join([ keyTemplate['algo'], str(keyVersion['algo'])]) )

if 'hardware' in payload :
    topQuery += ",'{0}'".format( ''.join([ keyTemplate['hardware'], str(keyVersion['hardware'] + 1)]) )
else :
    topQuery += ",'{0}'".format( ''.join([ keyTemplate['hardware'], str(keyVersion['hardware'])]) )

if 'daq' in payload or 'processor' in payload :
    topQuery += ",'{0}'".format( ''.join([ keyTemplate['infraKey'], str(keyVersion['infraKey'] + 1)]) )
else :
    topQuery += ",'{0}'".format( ''.join([ keyTemplate['infraKey'], str(keyVersion['infraKey'])]) )

topQuery += ");"

sqlplus = subprocess.Popen(sqlplusCmd, shell=False, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
retval = sqlplus.communicate(topQuery)[0]
if '1 row created.' not in retval:
    print 'Failed query:', topQuery
    exit(0)
else:
    print 'Successfully inserted new ' + ''.join([ keyTemplate['key'], str(keyVersion['key']+1)]) + ' key'
