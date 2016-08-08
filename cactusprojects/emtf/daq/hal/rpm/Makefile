INSTALL_PATH = /usr/local/xdaq
HAL_PATH     = $(shell pwd)/..
PWD          = $(shell pwd)

#check the syntax of the tag: if not compliant the variables will be empty
VER_MAJOR    := $(shell perl -p -e '$$_=s/.*(\d\d)_(\d\d)_(\d\d)/\1/?$$_:""' CVS/Tag)
VER_MINOR    := $(shell perl -p -e '$$_=s/.*(\d\d)_(\d\d)_(\d\d)/\2/?$$_:""' CVS/Tag)
VER_PATCH    := $(shell perl -p -e '$$_=s/.*(\d\d)_(\d\d)_(\d\d)/\3/?$$_:""' CVS/Tag)

VER_EXISTS = 
ifdef VER_MAJOR
ifdef VER_MINOR
ifdef VER_PATCH
VER_EXISTS = 1
endif
endif
endif


.PHONY: versioncheck
versioncheck:
ifeq ($(VER_EXISTS),)
	@echo "*******************************************************************"
	@echo "***                                                             ***"
	@echo "*** Could not find a valid version in the file CVS/Tag.         ***"
	@echo "***                                                             ***"
	@echo "*** Either your tag is not compliant (should end with jj_nn_pp, ***"
	@echo "*** where jj is the major, nn the minor and pp the patch number ***"
	@echo "*** or you have not checked out a tagged version!               ***"
	@echo "***                                                             ***"
	@echo "*******************************************************************"
else 
	@echo
	@echo Major number: $(VER_MAJOR)
	@echo Minor number: $(VER_MINOR)
	@echo Patch number: $(VER_PATCH)
	@echo
endif



.PHONY: rpm
rpm: versioncheck CVS/Tag spec_update makerpm





.PHONY: testrpm
testrpm: spec_update_for_test makerpm
	@echo "***********************************************************"
	@echo "*** RPM built for test purposes (dummy version numbers) ***"
	@echo "***********************************************************"




.PHONY: makerpm
makerpm:
	mkdir -p RPMBUILD/{RPMS/{i386,i586,i686,x86_64},SPECS,BUILD,SOURCES,SRPMS}
	rpmbuild -bb --define "_topdir $(PWD)/RPMBUILD"  hal.spec
	rpmbuild -bb --define "_topdir $(PWD)/RPMBUILD"  hal-caen.spec
	rpmbuild -bb --define "_topdir $(PWD)/RPMBUILD"  hal-sbs.spec
	rpmbuild -bb --define "_topdir $(PWD)/RPMBUILD"  hal-pci.spec
	find ./RPMBUILD -name "*.rpm" | xargs -i mv {} .
	rm -rf ./RPMBUILD rpm{rc,macros}.local





.PHONY: spec_update
spec_update: 
	cp hal.spec.template hal.spec
	perl -p -i -e 's/__version__/$(VER_MAJOR).$(VER_MINOR)/' hal.spec 
	perl -p -i -e 's/__release__/$(VER_PATCH)/' hal.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal.spec

	cp hal-caen.spec.template hal-caen.spec
	perl -p -i -e 's/__version__/$(VER_MAJOR).$(VER_MINOR)/' hal-caen.spec 
	perl -p -i -e 's/__release__/$(VER_PATCH)/' hal-caen.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal-caen.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal-caen.spec

	cp hal-sbs.spec.template hal-sbs.spec
	perl -p -i -e 's/__version__/$(VER_MAJOR).$(VER_MINOR)/' hal-sbs.spec 
	perl -p -i -e 's/__release__/$(VER_PATCH)/' hal-sbs.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal-sbs.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal-sbs.spec

	cp hal-pci.spec.template hal-pci.spec
	perl -p -i -e 's/__version__/$(VER_MAJOR).$(VER_MINOR)/' hal-pci.spec 
	perl -p -i -e 's/__release__/$(VER_PATCH)/' hal-pci.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal-pci.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal-pci.spec





.PHONY: spec_update_for_test
spec_update_for_test: 
	cp hal.spec.template hal.spec
	perl -p -i -e 's/__version__/777.888/' hal.spec 
	perl -p -i -e 's/__release__/999/' hal.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal.spec

	cp hal-caen.spec.template hal-caen.spec
	perl -p -i -e 's/__version__/777.888/' hal-caen.spec 
	perl -p -i -e 's/__release__/999/' hal-caen.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal-caen.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal-caen.spec

	cp hal-sbs.spec.template hal-sbs.spec
	perl -p -i -e 's/__version__/777.888/' hal-sbs.spec 
	perl -p -i -e 's/__release__/999/' hal-sbs.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal-sbs.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal-sbs.spec

	cp hal-pci.spec.template hal-pci.spec
	perl -p -i -e 's/__version__/777.888/' hal-pci.spec 
	perl -p -i -e 's/__release__/999/' hal-pci.spec
	perl -p -i -e 's#__prefix__#$(INSTALL_PATH)#' hal-pci.spec
	perl -p -i -e 's#__haldir__#$(HAL_PATH)#' hal-pci.spec





.PHONY: clean
clean :
	-rm *.rpm
	-rm hal.spec
	-rm hal-sbs.spec
	-rm hal-pci.spec
	-rm hal-caen.spec



