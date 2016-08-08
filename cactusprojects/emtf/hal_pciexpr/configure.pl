#!/usr/bin/perl

use strict;

`cp Makefile.in.template Makefile.in`;

my $SBSDIR        = "/usr/local/SBS/1003";
my $CAENDIR       = "/usr/local";
my $GENERICPCIDIR = "/usr";    # should be moved to /usr/local
my $XERCESDIR     = "/usr";

print "\n******************************************************************************\n\n";
probeXerces();
probeSBS();
probeCAEN();
probeGenericPci();
print "\n******************************************************************************\n\n";

editMakefile( "CONFIG_HAS_RUN", "YES");
exit;


sub probeXerces {
    if ( ! -d "${XERCESDIR}/include/xercesc" ) {
	print "No xml support: No xerces header files in ${XERCESDIR}/include/xercesc\n";
	print "                You can edit the Makefile.in by hand if you need xml support.\n";
	return -1;
    }
    editMakefile( "XERCESDIR", $XERCESDIR );
}

sub probeSBS {
    if ( (! -d "$SBSDIR") or
	 (! -f "/usr/lib/libbtp.a" ) ) {
	print "No SBS VMEAdapter. Include files expected in $SBSDIR/include\n";
	print "                   Library expected in /usr/lib/libbtp.a\n";
	return -1;
    }
    editMakefile( "SBSDIR", $SBSDIR );       
}

sub probeCAEN {
    if ( -d "/usr/local/CAEN/CAEN-VME" ) {
	# old installation of CAEN VME stuff
        print "An old (not rpm based) installation of the CAEN VME software exists on ";
        print "this machine. Please remove this installation before compiling the hal.";
        print "The directory /usr/local/CAEN should be removed. ANY CAENVME library";
        print "/usr/lib/libCAENVME* should be removed.";
        return -1;
    } elsif ( -e "/usr/lib/libCAENVME*" ) {
        print "A CAENVME library /usr/lib/libCAENVME* from an old installation of the CAENVME";
        print "software has been found in /usr/lib. Remove all files /usr/lib/libCAENVME* before";
        print "proceeding.";
        return -1;
    } if ( ( ! ( -e "${CAENDIR}/include/CAENVMElib.h") ) 
           or  ( ! ( -l "${CAENDIR}/lib/libCAENVME.so") ) ) {
	print "No CAEN VME adapter. (Library is expected in /usr/local/lib/libCAENVME*\n";
	print "                      include files are expected in /usr/local/include/CAENVMElib.h)\n";
	return -1;
    }

    # check the version
    my $txt = `ls -l $CAENDIR/lib/libCAENVME.so`; chomp $txt;
    $txt =~ m/ -> .*libCAENVME\.so\.(\d+)\.(\d+)$/;
    my $majorVersion = $1;
    my $minorVersion = $2;
    if ( $majorVersion < 2 || 
	 ($majorVersion == 2 && $minorVersion < 5) ) {
	print "CAEN software version too low: $majorVersion.$minorVersion. Should be at lease 2.5. Upgrade the CAENVME software.\n";
	return -1;
    }
    
    editMakefile( "CAENDIR", $CAENDIR );
    print "CAEN Version is $majorVersion.$minorVersion\n";

}

sub probeGenericPci {
    if ( ( ! -f "/usr/include/generic-pci-access.hh" ) or
	 ( ! -f "/usr/lib/libgeneric-pci.so") ) {
	print "No Generic Linux PCI Adapter. No generic pci driver installed in $GENERICPCIDIR.\n";
	return -1;
    }
    editMakefile( "GENERICPCIDIR", $GENERICPCIDIR );
}

sub editMakefile {
    my ($var, $value) = @_;
    open IN, "Makefile.in" || die "Could not open Makefile.in!\n";
    open OUT, ">Makefile.in.tmp" || die "could not open Makefile.in.tmp\n";
    while ( my $line = <IN> ) {
	if ( $line =~ s/^$var=.*/$var=$value/ ) {
	    print "$line";
	}
	print OUT $line;
    }
    close IN;
    close OUT;
    `cp Makefile.in.tmp Makefile.in`;
}

