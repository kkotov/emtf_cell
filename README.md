## Short description

The [EMTF](http://iopscience.iop.org/1748-0221/8/12/C12034) is one of the custom-built
electronic systems of the CMS Level-1 Trigger (L1T). By means of pattern recognition EMTF
combines multiple track's segments produced by a particle in sensitive regions of the
detector and evaluates track's properties. The most important property, the particle's
momentum (or transverse momentum), is evaluated from track's bending in the inhomogeneous
magnetic field.

The EMTF system is controlled and monitored via PCI express (PCIe) connection to a regular
PC running the EMTF cell instance. EMTF cell is a system-specific implementation of 
[SWATCH](http://cactus.web.cern.ch/cactus/tsdocs/index.html) cell. EMTF cell provides an
expert-level interface to the system and in the same time allows the central SWATCH
framework to orchestrate all systems of the L1T coherently. Current repository contains
the EMTF cell implementation. 
