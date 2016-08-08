%define pfx /opt/freescale/rootfs/%{_target_cpu}
%define __os_install_post %{nil}

Summary         : Hardware Access Library
Name            : hal-cms
Version         : 1.0
Release         : 1
License         : Public Domain, not copyrighted
Vendor          : Christoph Schwick
Packager        : Christoph Schwick
Group           : CMS/DAQ
Source          : %{name}-%{version}.tar.bz2
BuildRoot       : %{_tmppath}/%{name}
Prefix          : %{pfx}

%Description
%{summary}

%Prep
%setup

%Build
make -f Makefile-embedded prefix=%{_prefix}

%Install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/lib
install -m 755 xcept/lib/*.so   $RPM_BUILD_ROOT/%{pfx}/usr/lib
install -m 755 utilities/lib/*.so   $RPM_BUILD_ROOT/%{pfx}/usr/lib
install -m 755 generic/lib/*.so   $RPM_BUILD_ROOT/%{pfx}/usr/lib
install -m 755 busAdapter/dummy/lib/*.so   $RPM_BUILD_ROOT/%{pfx}/usr/lib
install -m 755 busAdapter/pci/lib/*.so   $RPM_BUILD_ROOT/%{pfx}/usr/lib

%Clean
rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%{pfx}/*

