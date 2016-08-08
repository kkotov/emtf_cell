# $Id: Makefile,v 1.8 2007/06/11 17:30:50 cschwick Exp $


include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include $(XDAQ_ROOT)/config/mfDefs.extern_coretools
include $(XDAQ_ROOT)/config/mfDefs.coretools
ifdef STANDALONE
include $(XDAQ_ROOT)/Makefile.in
endif

Project=daq
Package=hal/generic
NEEDXML=1

ifdef STANDALONE
Project = .
ifndef XERCESDIR
NEEDXML=
endif
endif

Sources = \
          AddressTable.cc \
          PCIAddressTable.cc \
          VMEAddressTable.cc \
          AddressTableItem.cc \
          AddressTableReader.cc \
          VMEAddressTableASCIIReader.cc \
          VMEAddressTableDynamicReader.cc \
          PCIAddressTableASCIIReader.cc \
          PCIAddressTableDynamicReader.cc \
          GeneralHardwareAddress.cc \
          PCIHardwareAddress.cc \
          VMEHardwareAddress.cc \
          VME64xHardwareAddress.cc \
          HardwareDevice.cc \
          VMEDevice.cc \
          VME64xDevice.cc \
          PCIDevice.cc \
          AbstractCommandFactory.cc \
          CommandSequence.cc \
          CommandCreator.cc \
          CommandSequenceReader.cc \
          WriteFactory.cc \
          WriteCommand.cc \
          UnmaskedWriteFactory.cc \
          UnmaskedWriteCommand.cc \
          ReadCommand.cc \
          ReadFactory.cc \
          UnmaskedReadCommand.cc \
          UnmaskedReadFactory.cc \
          LabelCommand.cc \
          LabelFactory.cc \
          SetBitFactory.cc \
          SetBitCommand.cc \
          ResetBitFactory.cc \
          ResetBitCommand.cc \
          DefineFactory.cc \
          DefineCommand.cc \
          AddFactory.cc \
          AddCommand.cc \
          GotoCommand.cc \
          GotoFactory.cc \
          PrintCommand.cc \
          PrintFactory.cc \
          CheckCommand.cc \
          CheckFactory.cc \
          PollItemCommand.cc \
          PollItemFactory.cc \
          CommandSequenceASCIIReader.cc \
          CommandSequencer.cc \
          StopWatch.cc \
          StaticVMEConfiguration.cc \
          StaticVMEConfigurationReader.cc \
          StaticVMEConfigurationASCIIFileReader.cc \
          StaticVMEItem.cc \
          VME64xCrate.cc \
          VME64xFunction.cc \
          VME64xMappedWindow.cc \
          VMEConfigurationSpaceAddressReader.cc \
          VMEConfigurationSpaceDevice.cc \
          VMEConfigurationSpaceHandler.cc \
          VMESlot.cc 
ifdef NEEDXML 
Sources += VMEAddressTableXMLFileReader.cc \
          VMEAddressTableDOMReader.cc \
          DOMToVMEMapConverter.cc \
          PCIAddressTableXMLFileReader.cc \
          DOMToPCIMapConverter.cc \
          XMLParserErrorHandler.cc \
          ASCIIFileModuleMapper.cc \
          ASCIIFileAddressTableContainer.cc
endif

DynamicLibrary= GenericHAL
StaticLibrary=


ifdef STANDALONE
IncludeDirs += $(BUILD_HOME)/$(Project)/hal/xcept/include
ifdef XERCESDIR
IncludeDirs += $(XERCESDIR)/include
endif 
else
Sources += version.cc
IncludeDirs = \
	$(XCEPT_INCLUDE_PREFIX) \
	$(XERCES_INCLUDE_PREFIX) \
	$(CONFIG_INCLUDE_PREFIX)
endif

LibraryDirs = 


DependentLibraryDirs = 

DependentLibraries = 

UserCFlags =
UserCCFlags = 
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

# These libraries can be platform specific and
# potentially need conditional processing

ExternalObjects =

# Compile the source files and create a shared library
#
Executables=

# Local Makefile.rules is a copy of TriDAS/config/Makefile.rules plus a
# modification to use the Libraries tag when building a shared library
include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules
