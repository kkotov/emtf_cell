To install and run:

- recompile driver 'make clean; make'

- change if necessary  parameters in utca_sp12.cfg file
DEBUG=1 # Enable debug output
SLOTS=1 # Number of installed physical cards in the system
OWNER=root # Owner of /dev/utca_sp12x device files
GROUP=users # Group owner of /dev/utca_sp12x device files
MODE=664 # Default permission of /dev/urca_sp12 device files
 !!! Check if group name exists and that user actually belongs to it
- copy utca_sp12.cfg to /etc/ folder
- try to load driver under root with utca_sp12.local script
- check /sbin/lsmod if driver is loaded 
- check /var/log/messages for any obvious initialization errors
- check ownership and permissions of /dev/utca_sp12x device files
- try to access device from your user program 

