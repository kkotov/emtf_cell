#!/usr/bin/perl -w

use Getopt::Long;
$schemaversion="-ver-2-0";
$vme64x = 0;
%options = ();                          # hash of options (set getopt below)

################################################################################################
sub showusage {

    print "\n";
    print "Usage :\n";
    print "\n";
    print "ASCIIVMEConverter.pl --options asciifile \n\n";
    print "   options:\n\n";
    print "        required: [ --localSchema | --remoteSchema ]\n";
    print "        required: [ --xml | --sql ] \n";
    print "        required: --typeId {typeId} \n";
    print "        required: --outfile {filename}\n\n";
    print "        required for sql and vme: --baseaddress (hex format e.g. '008c0000' ) \n";
    print "                                  is set automatically to  ffffffff for vme64x \n\n";
    print "        optional for sql output: --cardDescription\n";
    print "        optional for sql output: --cardUsers\n";
    print "        optional for sql output: --serialnumber\n";
    print "        optional for sql output: --manufacturerId\n";
    print "        optional for sql output: --currentUser\n";
    print "        optional for sql output: --userComment\n";
    print "        optional for sql output: --location\n";
}

################################################################################################
sub printXmlVme {
    printf OUTPUT "\t<VME_ADDRESS _type=\"Module\" ITEM_NAME=\"%s\">\n",$name;
    printf OUTPUT "\t\t<ADDRESS _type=\"number\">0x%s</ADDRESS>\n",$address;
    printf OUTPUT "\t\t<ADDRESS_MODIFIER _type=\"number\">0x%s</ADDRESS_MODIFIER>\n",$am;
    printf OUTPUT "\t\t<READ_OR_WRITE _type=\"string\">%s</READ_OR_WRITE>\n",$access;
    printf OUTPUT "\t\t<WIDTH _type=\"number\">%d</WIDTH>\n",$width;
    printf OUTPUT "\t\t<MASK _type=\"number\">0x%s</MASK>\n",$mask;
    if($description){
        printf OUTPUT "\t\t<DESCRIPTION _type=\"string\">%s</DESCRIPTION>\n",$description;
    }
    printf OUTPUT "\t</VME_ADDRESS>\n";
}

################################################################################################
sub printXmlVme64x {
    printf OUTPUT "\t<VME64X_ADDRESS _type=\"Module\" ITEM_NAME=\"%s\">\n",$name;
    printf OUTPUT "\t\t<ADDRESS _type=\"number\">0x%s</ADDRESS>\n",$address;
    printf OUTPUT "\t\t<SPACE _type=\"string\">%s</SPACE>\n",$vmeSpace;
    printf OUTPUT "\t\t<MAP _type=\"number\">%s</MAP>\n",$map;
    printf OUTPUT "\t\t<READ_OR_WRITE _type=\"string\">%s</READ_OR_WRITE>\n",$access;
    printf OUTPUT "\t\t<MASK _type=\"number\">0x%s</MASK>\n",$mask;
    if($description){
        printf OUTPUT "\t\t<DESCRIPTION _type=\"string\">%s</DESCRIPTION>\n",$description;
    }
    printf OUTPUT "\t</VME64X_ADDRESS>\n";
}

################################################################################################
sub printSqlVme {

    if ( $module_baseaddress eq "ffffffff" ) {
        print "You must give a baseaddress if you want sql output for a vme module!\n\n";
        showusage();
        exit -1;
    }
    print OUTPUT "INSERT INTO\n" .
        "VME_ADDRESS (TYPE_ID, ITEM_NAME, ADDRESS, MASK, ADDRESS_MODIFIER, WIDTH, READ_OR_WRITE, DESCRIPTION)\n";
    printf OUTPUT "VALUES ('%s', '%s', %d, %d, %d, %d, '%s', '%s');\n",
    $typeId, $name, hex "0x$address",   hex  "0x$mask",  hex "0x$am",   $width, $access,   $description;
}

################################################################################################
sub printSqlVme64x {
    print OUTPUT "INSERT INTO\n" .
        "VME64X_ADDRESS (TYPE_ID, ITEM_NAME, ADDRESS, SPACE, MAP, MASK, READ_OR_WRITE, DESCRIPTION)\n";
    printf OUTPUT "VALUES ('%s', '%s', %d, '%s', %d, %d, '%s', '%s');\n",
    $typeId, $name, hex "0x$address", $vmeSpace, $map, hex "0x$mask", $access, $description;
}



################################################################################################
sub checkFor {
    my ( $line ) = @_;
# check for a vme64x memory item (table printed by HAL)
#      key "memory" map am width address mask read write description
    if ( $line =~ /^\s*([\w_-]+)\s+memory\s+([0-7])\s+([\da-fA-F]{2})\s+(\d)\s+([\da-fA-F]{1,8})\s+([\da-fA-F]{1,8})\s+([01])\s+([01])\s+(.*)$/){
        $vme64x = 1;
        $name = $1;
        $map = $2;
        $am = $3;
        $width = $4;
        $address = $5;
        $mask = $6;
        $access = ( ($7?'r':'') . ($8?'w':''));
        $description = $9;
        $vmeSpace = "memory";
# check for a vme64x configuration item (table printed by HAL)
#     key "configuration" am width address mask read write description 
    } elsif ( $line =~ /^\s*([\w_-]+)\s+configuration\s+([\da-fA-F]{2})\s+(\d)\s+([\da-fA-F]{1,8})\s+([\da-fA-F]{1,8})\s+([01])\s+([01])\s+(.*)$/) {
        $vme64x = 1;
        $name = $1;
        $am = $2;
        $width = $3;
        $address = $4;
        $mask = $5;
        $access = ( ($6?'r':'') . ($7?'w':''));
        $description = $8;
        $vmeSpace = "configuration";
        $map = 0;
# check for a vme64x configuration item (table according to ASCII syntax)
#     key "configuration|memory" map address mask read write description 
    } elsif ( $line =~ /^\s*([\w_-]+)\s+([configurationmemory]+)\s+([0-7])\s+([\da-fA-F]{1,8})\s+([\da-fA-F]{1,8})\s+([01])\s+([01])\s+(.*)$/) {
        $vme64x = 1;
        $name = $1;
        $vmeSpace = $2;
        $map = $3;
        $address = $4;
        $mask = $5;
        $access = ( ($6?'r':'') . ($7?'w':''));
        $description = $8;
# check for a vme item: (according to ASCII syntax or printed version)
#     key AM width address mask read write description
    } elsif ( $line =~ /^\s*([\w_-]+)\s+([\da-fA-F]{2})\s+(\d)\s+([\da-fA-F]{1,8})\s+([\da-fA-F]{1,8})\s+([01])\s+([01])\s+(.*)$/) {
        $vmeSpace = "memory";
        $name = $1;
        $am = $2;
        $width = $3;
        $address = $4;
        $mask = $5;
        $access = ( ($6?'r':'') . ($7?'w':''));
        $description = $8;
        $map = 0;
    } else {
        return 0;
    }
    $description =~ s/</[/g;
    $description =~ s/>/]/g;

    if ( $vme64x == 1 ) {
        if ( $xmlOut ) {
            printXmlVme64x();
        } else {
            printSqlVme64x();
        }
    } else {
        if ( $xmlOut ) {
            printXmlVme();
        } else {
            printSqlVme();
        }
    } 
    return 1;
}

########################################################################################################
########################################################################################################

# default values:
$card_description = "";
$card_users = "";
$outfile = "";
$typeId = "";
$module_serial = "";
$module_cardId = "";
$module_baseaddress = "ffffffff";
$module_currentUser = "";
$module_userComment = "";
$module_location = "";

# read options
Getopt::Long::config qw( no_ignore_case permute bundling auto_abbrev );

my $optOk = GetOptions( \%options,
                      "localSchema",
                      "remoteSchema",
                      "xml",
                      "sql",
		      "outfile=s",\$outfile,
		      "typeId=s",\$typeId,
                      "cardDescription=s",   \$card_description,
                      "cardUsers=s",         \$card_users,
                      "serialnumber=s",      \$module_serial,
                      "manufacturerId=s",    \$module_cardId,
                      "baseaddress=i",       \$module_baseaddress,
                      "currentUser=s",       \$module_currentUser,
                      "userComment=s",       \$module_userComment,
                      "location=s",          \$module_location);


$infile = $ARGV[0];

if ( ! $optOk ) {
    showusage();
    exit 1;
}


if ( ( $outfile eq "" ) || ( $typeId eq "" ) || ( $infile eq "" ) ||
     ( ( ! exists $options{xml} ) && ( ! exists $options{sql} ) ) ||
     ( exists $options{xml} && (( ! exists $options{localSchema} ) && ( ! exists $options{remoteSchema} )))) {
    showusage(); 
    exit -1;
}


if ( exists $options{remoteSchema} ) {
    $schema = "http://xdaq.web.cern.ch/xdaq/xsd/HardwareAddressTable$schemaversion.xsd";
} else {
    $schema = "HardwareAddressTable$schemaversion.xsd";
}

# for easy use later:
if ( exists $options{xml} ) {
    $xmlOut = 1;
    $commentStart = "<!-- ";
    $commentEnd = " --> ";
} else {
    $xmlOut = 0;
    $commentStart = "/* ";
    $commentEnd = " */";
}

open ( OUTPUT, ">$outfile" ) || die "Could not open $outfile";


# print the preambel of the xml file
###########################################
if ( $xmlOut ) {
print OUTPUT  <<"EOF";
<?xml version="1.0" encoding="UTF-8"?>
<!--XML file generated by from $infile-->
<CARD_TYPE xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="$schema" _type="Module" TYPE_ID="$typeId">
EOF
} else {
print OUTPUT <<"EOF";
/* File generated by $0 $ARGV[0] */
INSERT INTO 
CARD_TYPE ( TYPE_ID, DESCRIPTION, USERS )
VALUES 	  ( '$typeId', '$card_description', '$card_users' );

INSERT INTO 
VME_CARD  ( SERIAL_NUMBER, TYPE_ID, CARD_ID, BASE_ADDRESS, CURRENT_USER, USER_COMMENT, LOCATION )
EOF
printf OUTPUT "VALUES 	  ( '%s', '%s', '%s', %d, '%s', '%s', '%s');\n",
$module_serial, $typeId, $module_cardId, hex "0x$module_baseaddress", $module_currentUser, $module_userComment, $module_location;

}

# Here the parsing of the text file starts:
###########################################

while (<>) {

    if (/^\s*(\*.*)?$/) {
# comment 
	print OUTPUT  "$commentStart $1 $commentEnd\n" if (defined $1);
    } elsif ( checkFor( $_ ) ) {
    } else {
# line not recognised
	print STDERR "Skipped line: ", $_;
    }
}

if ( $xmlOut ) {
    print OUTPUT  "</CARD_TYPE>\n";
} else {
    print OUTPUT "commit;\n";
}
close OUTPUT;
