#!/usr/bin/perl -w

use Getopt::Long;
$schemaversion="-ver-2-0";
%options = ();                          # hash of options (set getopt below)

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
    print "        required for sql output: --vendor {pci-vendorId} \n";
    print "        required for sql output: --device {pci-deviceId} \n";
    print "        optional for sql output: --cardDescription\n";
    print "        optional for sql output: --cardUsers\n";
    print "        optional for sql output: --serialnumber\n";
    print "        optional for sql output: --manufacturerId\n";
    print "        optional for sql output: --currentUser\n";
    print "        optional for sql output: --userComment\n";
    print "        optional for sql output: --location\n";
}


################################################################################################
sub printXmlPci {
	printf OUTPUT "\t<PCI_ADDRESS _type=\"Module\" ITEM_NAME=\"%s\">\n",$name;
	printf OUTPUT "\t\t<ADDRESS _type=\"number\">0x%s</ADDRESS>\n",$address;
	if ( $bar ne "-1" ) {
	    printf OUTPUT "\t\t<BAR _type=\"number\">%s</BAR>\n",$bar;
	} else {
	    printf OUTPUT "\t\t<BAR _type=\"number\">0</BAR>\n";
	}
	printf OUTPUT "\t\t<READ_OR_WRITE _type=\"string\">%s</READ_OR_WRITE>\n",$access;
	printf OUTPUT "\t\t<SPACE _type=\"string\">%s</SPACE>\n",$pcispace;
	printf OUTPUT "\t\t<MASK _type=\"number\">0x%s</MASK>\n",$mask;
	if($description){
	    printf OUTPUT "\t\t<DESCRIPTION _type=\"string\">%s</DESCRIPTION>\n",$description;
	}
	printf OUTPUT "\t</PCI_ADDRESS>\n";
}

################################################################################################
sub printSqlPci {
    print OUTPUT "INSERT INTO\n" .
        "PCI_ADDRESS (TYPE_ID, ITEM_NAME, ADDRESS, BAR, SPACE, MASK, READ_OR_WRITE, DESCRIPTION)\n";
    printf OUTPUT "VALUES ('%s', '%s', %d, %d, '%s', %d, '%s', '%s');\n",
    $typeId, $name, hex "0x$address",  $bar, $pcispace, hex  "0x$mask", $access, $description;
}

################################################################################################
sub printHTMLPci {
    print OUTPUT "<TR>\n";
    printf OUTPUT "   <TD>%s</TD>\n", $name;
    printf OUTPUT "   <TD align=\"center\" bgcolor=\"#d0d0d0\">%s</TD>\n", "0x$address";
    if ( $bar ne "-1" ) {
	printf OUTPUT "   <TD align=\"center\">%s</TD>\n", $bar;
    } else {
	printf OUTPUT "   <TD align=\"center\"></TD>\n";
    }
    printf OUTPUT "   <TD align=\"center\" bgcolor=\"#c0c0c0\">%s</TD>\n", $pcispace;
    printf OUTPUT "   <TD align=\"center\">%s</TD>\n", "0x$mask";
    printf OUTPUT "   <TD align=\"center\" bgcolor=\"#c0c0c0\">%s</TD>\n", $access;
    printf OUTPUT "   <TD align=\"center\">%s</TD>\n", $description;
    print OUTPUT "</TR>\n";
}

################################################################################################
sub checkFor {
    my ( $line ) = @_;
    if (/^\s*(\w+)\s+configuration\s+([\da-fA-F]{1,8})\s+([\da-fA-F]{1,8})\s+([01])\s+([01])\s*(.*)$/){
# configuration
	$name = $1;
	$address = $2;
	$mask = $3;
	$access = ($4?'r':'') . ($5?'w':'');
	$description = $6;
	$pcispace = "configuration";
	$bar = -1;
    } elsif (/^\s*(\w+)\s+memory\s+(\d+)\s+([\da-fA-F]{1,8})\s+([\da-fA-F]{1,8})\s+([01])\s+([01])\s+[01]?\s*(.*)$/){
# memory
	$name = $1;
	$bar = $2;
	$address = $3;
	$mask = $4;
	$access = ($5?'r':'') . ($6?'w':'');
	$description = $7;
	$pcispace = "memory";
    } elsif (/^\s*(\w+)\s+io\s*(.*)$/){
# io not recognised in ascii reader either
	print STDERR "IO access mode not implemented!\n";
	return 0;
    } else {
	return 0;
    }

    $description =~ s/</[/g;
    $description =~ s/>/]/g;

    if ( $xmlOut ) {
	printXmlPci();
    } elsif ( $htmlOut ) {
	printHTMLPci();
    }
    return 1;
}

########################################################################################################
# default values:
$card_description = "";
$card_users = "";
$outfile = "";
$typeId = "";
$vendor = "";
$device = "";
$module_serial = "";
$module_cardId = "";
$module_currentUser = "";
$module_userComment = "";
$module_location = "";

# read options
Getopt::Long::config qw( no_ignore_case permute bundling auto_abbrev );

my $optOk = GetOptions( \%options,
                      "localSchema",
                      "remoteSchema",
                      "xml",
                      "html",
                      "sql",
		      "outfile=s",\$outfile,
		      "typeId=s",\$typeId,
                      "vendor=s",\$vendor,
                      "device=s",\$device, 
                      "cardDescription=s",   \$card_description,
                      "cardUsers=s",         \$card_users,
                      "serialnumber=s",      \$module_serial,
                      "manufacturerId=s",    \$module_cardId,
                      "currentUser=s",       \$module_currentUser,
                      "userComment=s",       \$module_userComment,
                      "location=s",          \$module_location);

$infile = $ARGV[0];

if ( $optOk == 0 ) {
    showusage();
    exit 1;
}

if ( ( $outfile eq "" ) || ( $typeId eq "" ) || ( $infile eq "" ) ||
     ( ( ! exists $options{xml} ) && ( ! exists $options{sql} ) && ( ! exists $options{html} ) ) ||
     ( exists $options{xml} && (( ! exists $options{localSchema} ) && ( ! exists $options{remoteSchema} )) ) ||
     ( exists $options{sql} && ( ($vendor eq "") || ($device eq "") )) ) {
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
    $htmlOut = 0;
    $commentStart = "<!-- ";
    $commentEnd = " --> ";
} elsif ( exists $options{html} ) {
    $xmlOut = 0;
    $htmlOut = 1;
    $commentStart = "<!-- ";
    $commentEnd = " --> ";
} else {
    $xmlOut = 0;
    $htmlOut = 0;
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
} elsif ( $htmlOut ) {
print OUTPUT << "EOF"
<TABLE CELLPADDING=0>
    <TR>
        <TD align=\"center\"><b>Item</b></TD>
	<TD align=\"center\" bgcolor=\"#d0d0d0\"><b>Address</b></TD>
	<TD align=\"center\"><b>BAR</b></TD>
	<TD align=\"center\" bgcolor=\"#d0d0d0\"><b>Space</b></TD>
	<TD align=\"center\"><b>Mask</b></TD>
	<TD align=\"center\" bgcolor=\"#d0d0d0\"><b>Read/Write</b></TD>
	<TD align=\"center\"><b>Description</b></TD>
    </TR>
EOF
    #print OUTPUT "PCI_ADDRESS (TYPE_ID, ITEM_NAME, ADDRESS, BAR, SPACE, MASK, READ_OR_WRITE, DESCRIPTION)\n";
} else {
print OUTPUT <<"EOF";
/* File generated by $0 $ARGV[0] */
INSERT INTO 
CARD_TYPE ( TYPE_ID, DESCRIPTION, USERS )
VALUES 	  ( '$typeId', '$card_description', '$card_users' );

INSERT INTO 
PCI_CARD  ( SERIAL_NUMBER, TYPE_ID, CARD_ID, VENDOR, DEVICE, CURRENT_USER, USER_COMMENT, LOCATION )
EOF
printf OUTPUT "VALUES 	  ( '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');\n",
$module_serial, $typeId, $module_cardId, $vendor, $device, $module_currentUser, $module_userComment, $module_location;

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
} elsif ( $htmlOut ) {
    print OUTPUT "</TABLE>\n";
} else {
    print OUTPUT "commit;\n";
}
close OUTPUT;
