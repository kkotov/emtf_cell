#!/bin/sh
# Detects which OS and if it is Linux then it will detect which Linux Distribution.
#
# Novell SUSE	/etc/SuSE-release
# Red Hat	/etc/redhat-release, /etc/redhat_version
# Fedora	/etc/fedora-release
# Slackware	/etc/slackware-release, /etc/slackware-version
# Debian	/etc/debian_release, /etc/debian_version,
# Mandrake	/etc/mandrake-release
# Yellow dog	/etc/yellowdog-release
# Sun JDS	/etc/sun-release
# Solaris/Sparc	/etc/release
# Gentoo	/etc/gentoo-release
# UnitedLinux	/etc/UnitedLinux-release
# ubuntu	/etc/lsb-release
# CERN Linux    /etc/redhat-release
#

OS=`uname -s`
REV=`uname -r`
MACH=`uname -m`

GetVersionFromFile()
{
	VERSION=`cat $1 | tr "\n" ' ' | sed s/.*VERSION.*=\ // `
}

if [ "${OS}" = "SunOS" ] ; then
	OS=Solaris
	ARCH=`uname -p`	
	OSSTR="${OS} ${REV}(${ARCH} `uname -v`)"
elif [ "${OS}" = "AIX" ] ; then
	OSSTR="${OS} `oslevel` (`oslevel -r`)"
elif [ "${OS}" = "Linux" ] ; then
	KERNEL=`uname -r`
	if [ -f /etc/redhat-release ] ; then
		#TYPE=`head -n1 -c21 /etc/redhat-release`
		if [ `grep -c "Scientific Linux CERN SLC release" /etc/redhat-release` = 1 ] ; then
			PSUEDONAME=`cat /etc/redhat-release | sed s/.*\(// | sed s/\)//`
			REV=`cat /etc/redhat-release | sed s/.*release\ // | sed s/\ .*//`
			DIST=`echo $REV | awk -F . '{ print ("slc"$1) }'`
		elif [ `grep -c "Scientific Linux CERN Release" /etc/redhat-release` = 1 ] ; then
			PSUEDONAME=`cat /etc/redhat-release | sed s/.*\(// | sed s/\)//`
			REV=`cat /etc/redhat-release | sed s/.*Release\ // | sed s/\ .*//`
			DIST=`echo $REV | awk -F . '{ print ("slc"$1) }'`
		elif [ `grep -c "Scientific Linux CERN release" /etc/redhat-release` = 1 ] ; then
			PSUEDONAME=`cat /etc/redhat-release | sed s/.*\(// | sed s/\)//`
			REV=`cat /etc/redhat-release | sed s/.*release\ // | sed s/\ .*//`
			DIST=`echo $REV | awk -F . '{ print ("slc"$1) }'`
		else
			DIST='RedHat'
			PSUEDONAME=`cat /etc/redhat-release | sed s/.*\(// | sed s/\)//`
			REV=`cat /etc/redhat-release | sed s/.*release\ // | sed s/\ .*//`
		fi
	elif [ -f /etc/SuSE-release ] ; then
		DIST=`cat /etc/SuSE-release | tr "\n" ' '| sed s/VERSION.*//`
		REV=`cat /etc/SuSE-release | tr "\n" ' ' | sed s/.*=\ //`
	elif [ -f /etc/mandrake-release ] ; then
		DIST='Mandrake'
		PSUEDONAME=`cat /etc/mandrake-release | sed s/.*\(// | sed s/\)//`
		REV=`cat /etc/mandrake-release | sed s/.*release\ // | sed s/\ .*//`
	elif [ -f /etc/debian_version ] ; then
		DIST="Debian `cat /etc/debian_version`"
		REV=""

	fi
	if [ -f /etc/UnitedLinux-release ] ; then
		DIST="${DIST}[`cat /etc/UnitedLinux-release | tr "\n" ' ' | sed s/VERSION.*//`]"
	fi
	
	OSSTR="${OS} ${DIST} ${REV}(${PSUEDONAME} ${KERNEL} ${MACH})"

fi


echo ${DIST}
