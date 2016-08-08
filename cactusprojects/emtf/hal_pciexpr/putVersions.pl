#!/usr/bin/perl -w
use strict;

my @packs = ( "generic", "utilities", "busAdapter/dummy",
	      "busAdapter/caen", "busAdapter/pci" );

my $tag = "4_1_1";

foreach my $pack ( @packs ) {

    $tag = updateString( "Enter version for \"$pack\" [$tag] : ", "$tag" );    
    if ( $tag !~ m/(\d+)_(\d+)_(\d+)$/ ) {
	print "Format error of tag. Must be : ma_mi_pa\n";
	exit;
    }
    my $major = $1;
    my $minor = $2;
    my $patch = $3;
#    my $release = $4;

    print "Setting version to ${major}_${minor}_${patch} in $pack/include/hal/$pack/version.h\n";
    
    `perl -p -i -e 's#_VERSION_MAJOR \\d+\\s*\$#_VERSION_MAJOR $major\\n#' $pack/include/hal/$pack/version.h`;
    `perl -p -i -e 's#_VERSION_MINOR \\d+\\s*\$#_VERSION_MINOR $minor\\n#' $pack/include/hal/$pack/version.h`;
    `perl -p -i -e 's#_VERSION_PATCH \\d+\\s*\$#_VERSION_PATCH $patch\\n#' $pack/include/hal/$pack/version.h`;

}

sub updateString {
    my ($promptString, $default ) = @_;
    if ( $promptString eq "" ) {
        print ("Enter string [$default] : ");
    } else {
        print $promptString;
    }
    my $input = "";
    $input = <STDIN>;
    chop $input;
    my $result = "";
    if ( $input eq "" ) {
        $result = $default;
    } else {
        $result = $input;
    }
    return $result;
}

