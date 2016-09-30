from optparse import OptionParser
import subprocess
import re
import time
import sys 
import getpass

"""
A simple helper script that, provided with new configurations of any
of the four kinds (hw/processor/daq/algo), uploads the corresponding
XML files into the relevant table indexing it with a next version of
key and creates the top-level key for this new configuration.

Running it with no arguments or with the key prototype (-k) argument
does not modify the DB and can be used to monitor the keys.

The prinout always starts with "top" version of the keys that will
be incremented and assigned to the new configuration whenever the
later is requested to be uploaded to the DB.

If optional key prototype (-k) is provided, the keys found in the
prototype will printed and, if upload is requested, used in the new
version of the key to aggregate the unchanged configurations.

If key prototype is absent, the latest top-level key is used as
the prototype.
"""

# parse the options
parser = OptionParser()
parser.add_option( "-c",
                   "--hardware",
                   dest="hardware",
                   default=None,
                   help="read hardware XML configuration from FILE", 
                   metavar="FILE"
                 )
parser.add_option( "-p",
                   "--processor",
                   dest="processor",
                   default=None,
                   help="read MTF7 processor XML configuration from FILE",
                   metavar="FILE"
                 )
parser.add_option( "-d",
                   "--daq",
                   dest="daq",
                   default=None,
                   help="read AMC13 XML configuration from FILE",
                   metavar="FILE"
                 )
parser.add_option( "-a",
                   "--algo",
                   dest="algo",
                   default=None,
                   help="read ALGO XML configuration from FILE",
                   metavar="FILE"
                 )
parser.add_option( "-k",
                   "--key",
                   dest="key",
                   default=None,
                   help="prototype system key that we use to build a new key",
                   metavar="KEY"
                 )

(options, args) = parser.parse_args()

# define DB/keys structure
confTable   = {
                'key'      : 'EMTF_KEYS',
                'infraKey' : 'EMTF_INFRA_KEYS',
                'hardware' : 'EMTF_HW',
                'algo'     : 'EMTF_ALGO',
                'processor': 'EMTF_INFRA',
                'daq'      : 'EMTF_INFRA'
              } #'rs':'EMTF_RS',  
keyTemplate = {
                'key'      : 'EMTF_Base_Key/v',
                'infraKey' : 'EMTF_Base_Infra_Key/v',
                'hardware' : 'EMTF_HW/v',
                'algo'     : 'EMTF_ALGO_BASE/v',
                'processor': 'EMTF_MTF7_base_conf/v',
                'daq'      : 'EMTF_AMC13_base_conf/v'
              } #'rs':'EMTF_RS/v',

# command line DB client
sqlplusCmd = [ 'env',
               'LD_LIBRARY_PATH=/opt/xdaq/lib/',
               '/opt/xdaq/bin/sqlplus',
               '-S',
               'cms_emtf/@cms_omds_lb'
             ]
#sqlplusCmd = [ 'env',
#               'LD_LIBRARY_PATH=/opt/xdaq/lib/',
#               '/opt/xdaq/bin/sqlplus',
#               '-S',
#               'cms_trg_l1_subsystem/password2015@int2r'
#             ]
#if hash( sqlplusCmd[-1] ) != -5508203199304911491:

if hash( sqlplusCmd[-1] ) != -7405555605401402394:
    print 'Do not forget to plug password to this script'
    print 'Exiting.'
    exit(0)


# find all next key versions
keyVersion = {}
for kind,table in confTable.iteritems() :

    query = "select ID from CMS_TRG_L1_CONF.{0} where ID like '{1}%';".format(
              table,
              keyTemplate[kind]
            );
    sqlplus = subprocess.Popen( sqlplusCmd,
                                shell  = False,
                                stdout = subprocess.PIPE,
                                stdin  = subprocess.PIPE
                              )

    last = -1
    for line in re.split( '\n', sqlplus.communicate(query)[0] ) :

        if re.match( keyTemplate[kind] + r'(\d+)$', line ) != None :

            version = re.sub( keyTemplate[kind] + r'(\d+)$', '\g<1>', line )
#            print line, version
            last = int(version) if last < int(version) else last

    if last >= 0 :
        keyVersion[kind] = last


# check if we've found everything we need
if 'algo' not in keyVersion :
    print "No match for the 'algo' key template: " + keyTemplate['algo']
    exit(0)

if 'hardware' not in keyVersion :
    print "No match for the 'hardware' key template: " + keyTemplate['hardware']
    exit(0)

if 'processor' not in keyVersion :
    print "No match for the 'processor' key template: "+keyTemplate['processor']
    exit(0)

if 'daq' not in keyVersion :
    print "No match for the 'daq' key template: " + keyTemplate['daq']
    exit(0)


# what do we have for the current key versions?
print 'Current "highest" versions of the keys are:\n' 
for kind in ['key', 'infraKey', 'hardware', 'processor', 'daq', 'algo'] :
    print ("%10s" % kind) + ": " + keyTemplate[kind] + str(keyVersion[kind])
print ""

# no options - doing nothing and exiting
if len(sys.argv) == 1:
    print "(no options - doing nothing and exiting)"
    exit(0)


# load in the xmls specified in the script options
payload = {}
prototypeKey = keyTemplate['key'] + str(keyVersion['key']) # choose last as default

for kind,argument in vars(options).iteritems() :

    if kind == 'key' :

        prototypeKey = argument

        # print sub keys from the prototype key
        myre = re.compile(r'(INFRAKEY)|(HW)|(PROC)|(ALGO)|(DAQ)|(-{80})')

        query = """select
                       TOP_KEYS.INFRA as INFRAKEY,
                       INFRA_KEYS.MTF7 as PROC,
                       INFRA_KEYS.AMC13 as DAQ,
                       TOP_KEYS.HW as HW,
                       TOP_KEYS.ALGO as ALGO
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
                           TOP_KEYS.INFRA = INFRA_KEYS.ID;""".format(prototypeKey)

        sqlplus = subprocess.Popen( sqlplusCmd,
                                    shell  = False,
                                    stdout = subprocess.PIPE,
                                    stdin  = subprocess.PIPE
                                  )

        retval = sqlplus.communicate(query)[0]

        print 'Keys in the chosen ' + prototypeKey + ' prototype key:\n' 
        for line in re.split('\n',retval):
            if len(line) and myre.match(line) == None :
                print line
        print ""

    elif argument != None :

        with open( argument, 'r' ) as xmlSource:
            payload[kind] = xmlSource.readlines()
            xmlSource.close()


if len(payload) == 0 :
    print "(nothing to do)"
    exit(0)


# insert the payloads into conf tables
for kind,xmlText in payload.iteritems():

    query1 = """insert into CMS_TRG_L1_CONF.{0}
                    (ID, CREATION_DATE, DESCRIPTION, CONF, AUTHOR)
                values
                    ('{1}',
                     TO_TIMESTAMP_TZ('{2}', 'DD-Mon-RR HH24:MI:SS TZHTZM'),
                     '{3}',
                      {4},
                     '{5}');""".format(
                 confTable[kind],
                 keyTemplate[kind] + str(keyVersion[kind] + 1),
                 time.strftime( "%d-%b-%Y %H:%M:%S %z", time.localtime() ),
                 'Uploaded with ' + ' '.join(sys.argv),
                 ' || '.join([
                                 "to_clob('" + line + "')"
                                 if re.match( '\s*$', line ) == None
                                 else "chr(10)"
                                 for line in xmlText
                             ]),
                 getpass.getuser()
             )

    sqlplus = subprocess.Popen( sqlplusCmd,
                                shell  = False,
                                stdout = subprocess.PIPE,
                                stdin  = subprocess.PIPE
                              )

    retval = sqlplus.communicate(query1)[0]
    if '1 row created.' not in retval:
        print 'Failed query:', query1
        exit(0)
    else:
        print ('Successfully inserted new '
               + keyTemplate[kind]
               + str(keyVersion[kind] + 1)
               + ' key')

# insert the keys into key tables

#  intermediate INFRA_KEYS level for 'daq' and 'processor'
if 'daq' in payload or 'processor' in payload:
    # start building a query updating the EMTF_INFRA_KEYS table
    query = """insert into CMS_TRG_L1_CONF.EMTF_INFRA_KEYS
                   (ID,CREATION_DATE,DESCRIPTION,AUTHOR,AMC13,MTF7)
               values
                   ('{0}',
                    TO_TIMESTAMP_TZ('{1}', 'DD-Mon-RR HH24:MI:SS TZHTZM'),
                    '{2}',
                    '{3}'""".format(
                 keyTemplate['infraKey'] + str(keyVersion['infraKey'] + 1),
                 time.strftime("%d-%b-%Y %H:%M:%S %z", time.localtime()),
                 'Uploaded with ' + ' '.join(sys.argv),
                 getpass.getuser(),
            )

    if 'daq' in payload :
        # create a new (next) version
        query += ",'"
        query += keyTemplate['daq'] + str(keyVersion['daq'] + 1)
        query += "'"
    else :
        # inherit reference version from the prototype key
        query += """,(select
                          INFRA_KEYS.AMC13
                      from
                          CMS_TRG_L1_CONF.EMTF_INFRA_KEYS INFRA_KEYS,
                          CMS_TRG_L1_CONF.EMTF_KEYS         TOP_KEYS
                      where INFRA_KEYS.ID = TOP_KEYS.INFRA
                      and     TOP_KEYS.ID = '{0}')""".format(prototypeKey)

    if 'processor' in payload:
        # create a new (next) version
        query += ",'"
        query += keyTemplate['processor'] + str(keyVersion['processor'] + 1)
        query += "'"
    else :
        # inherit reference version from the prototype key
        query += """,(select
                          INFRA_KEYS.MTF7
                      from
                          CMS_TRG_L1_CONF.EMTF_INFRA_KEYS INFRA_KEYS,
                          CMS_TRG_L1_CONF.EMTF_KEYS         TOP_KEYS
                      where INFRA_KEYS.ID = TOP_KEYS.INFRA
                      and     TOP_KEYS.ID = '{0}')""".format(prototypeKey)

    query += ");"

    sqlplus = subprocess.Popen( sqlplusCmd,
                                shell  = False,
                                stdout = subprocess.PIPE,
                                stdin  = subprocess.PIPE
                              )

    retval = sqlplus.communicate(query)[0]

    if '1 row created.' not in retval:
        print 'Failed query:', query
        exit(0)
    else:
        print ('Successfully inserted new '
               + keyTemplate['infraKey']
               + str(keyVersion['infraKey'] + 1)
               + ' key')

#  top level
query = """insert into CMS_TRG_L1_CONF.EMTF_KEYS
               (ID,CREATION_DATE,DESCRIPTION,AUTHOR,ALGO,HW,INFRA)
           values
               ('{0}',
                TO_TIMESTAMP_TZ('{1}', 'DD-Mon-RR HH24:MI:SS TZHTZM'),
                '{2}',
                '{3}'""".format(
             keyTemplate['key'] + str(keyVersion['key'] + 1),
             time.strftime("%d-%b-%Y %H:%M:%S %z", time.localtime()),
             'Uploaded with ' + ' '.join(sys.argv),
             getpass.getuser()
        )

if 'algo' in payload :
    # create a new (next) version
    query += ",'"
    query += keyTemplate['algo'] + str(keyVersion['algo'] + 1)
    query += "'"
else :
    # inherit reference version from the prototype key
    query += """,(select
                      ALGO
                  from
                      CMS_TRG_L1_CONF.EMTF_KEYS
                  where
                      ID = '{0}')""".format(prototypeKey)

if 'hardware' in payload :
    # create a new (next) version
    query += ",'"
    query += keyTemplate['hardware'] + str(keyVersion['hardware'] + 1)
    query += "'"
else :
    # inherit reference version from the prototype key
    query += """,(select
                      HW
                  from
                      CMS_TRG_L1_CONF.EMTF_KEYS
                  where
                      ID = '{0}')""".format(prototypeKey)

if 'daq' in payload or 'processor' in payload :
    # create a new (next) version
    query += ",'"
    query += keyTemplate['infraKey'] + str(keyVersion['infraKey'] + 1)
    query += "'"
else :
    # inherit reference version from the prototype key
    query += """,(select
                      INFRA
                  from
                      CMS_TRG_L1_CONF.EMTF_KEYS
                  where
                      ID = '{0}')""".format(prototypeKey)

query += ");"

sqlplus = subprocess.Popen( sqlplusCmd,
                            shell  = False,
                            stdout = subprocess.PIPE,
                            stdin  = subprocess.PIPE
                          )
retval = sqlplus.communicate(query)[0]

if '1 row created.' not in retval:
    print 'Failed query:', query
    exit(0)
else:
    print ('Successfully inserted new '
           + keyTemplate['key']
           + str(keyVersion['key'] + 1)
           + ' key')
