
/**
*
*
*  \mainpage The generic API of the Hardware Access Library
*            
*   \section Introduction
*            The Hardware Access Library  is an API to access hardware
*            modules  from  application   programs.  The  idea  is  to 
*            make the access to the  hardware easy by letting the user
*            access logical items in  their modules. The logical items
*            are based on an Address Table of the module.  They can be 
*            registers, single bits or  bitfields in a register. Every
*            item  has an  easy to  remember  name which  is used  for 
*            referencing it. 
*            
*            A small  scripting facility is provided.  This allows the 
*            user  to  write  sequences  of  commands  to  access  the 
*            hardware.  This   is  especially  useful   for  debugging 
*            hardware  or during first  contacts with  a new  piece of 
*            equipment  where a  lot of  "trial and  error" has  to be 
*            performed. Sequences  can be changed  without recompiling 
*            anything  which  makes them  efficient  to  use in  these 
*            cases. Minimal support  for control statements like loops
*            or conditional jumps is provided. 
*            
*            The   Library   is   platform-   and   operating   system
*            independent. It has  to be used with a  driver which does
*            the actual access to  the hardware and which contains the
*            platform and  operating system specific  software. In the
*            library a  homogeneous interface  to the driver  has been
*            defined     (the    VMEBusAdapterInterface     and    the
*            PCIBusAdapterInterface).    An  implementation   of  this
*            interface for  each driver to be used  is necessary.  The
*            standard  "package"   provides  some  implementations  of
*            BusAdapters: One can be used  for PCI accesses in a Linux
*            PC.  The  driver is contained  in the I2Ocore  library of
*            Eric Cano.   Another BusAdapter is provided  to control a
*            VME crate from a Linux PC with the SBS PCI-VME BusAdapter
*            hardware (model  620). A third interface  is provided for
*            the   National  Instruments  PC-to-VME   interface  (also
*            Linux).  Finally there is a BusAdapter for the Mv2304 CPU
*            board  from  Motorola  running  VxWorks. For  this  board
*            busAdapters for the VME bus  and for the two PMC slots on
*            the board are provided.
*
*            In  order  to transfer  data  at  high  speed there  are 
*            commands  to   read  and   write  blocks  of   data.  The 
*            performance  of these commands  depend on  the underlying 
*            driver software for the hardware/bus-system involved. 
*            
*            For  general   information  on  Installation   and  usage
*            (including  some  examples)   please  consult  the  users
*            manual. It  is provided with  the distribution or  can be
*            found on the WWW).
*            
*   \section documentationGuide Guide to the API Documentation
*            Most of the documented classes  in the HAL are not needed
*            to be  known by  the user. The  user is only  expected to 
*            know  some  basic concepts  which  are  described in  the 
*            Users  manual,  and to  know  the  API  of the  interface 
*            classes.    The    most     important    one    is    the 
*            HardwareDeviceInterface. Here  the methods to  be used to 
*            interact with  the Hardware are  described. This inteface 
*            is  implemented  by  the  VMEDevice  and  the  PCIDevice. 
*            Of  these classes  only  the constructor  is needed.  All 
*            methods    which     are    not    declared     by    the 
*            HardwareDeviceInterface  are for  internal use  and might 
*            change  in  future versions  of  the  HAL.  It should  be 
*            avoided to use them.
*            
*            The constructors  of the  VMEDevice or PCIDevice  need an
*            Addresstable   and   a   BusAdapter   as   a   parameter.
*            Addresstables  implement the  AddressTableInterface which
*            declares the user API. Probably only some of the declared
*            functions will  be used  in user programs  whereas others
*            are only used internally  by the HAL. The constructors of
*            the   VMEAddressTable    and   PCIAddressTable   need   a
*            AddressTableReader     as    data     source.     Various
*            implementations of these readers provided by the HAL.
*            
*            The  API of  the BusAdapter  should not  be used  by user
*            programs  (except for  the constructor  of  course).  The
*            BusAdapter  methods  are used  by  the  HAL  in order  to
*            transfer  data  to  and  from the  device  driver.   Only
*            implementers of BusAdapters must study this API.
*            
*            If sequences are used the API of the CommandSequencer the
*            CommandSequence   and    the   implementations   of   the 
*            CommandSequenceReader should be studied. 
*            
*            The  HardwareAccessException   defines  an  interface  to 
*            retrieve information on the cause of the exception. 
*
*   \section VME64x VME64x
*            The HAL contains objects  which support the plug and play
*            mechanism   of   VME64x.   The  manual   documents   this 
*            functionality. The  most important class for  the user is 
*            the  VME64xCrate. This class configures  a whole crate of 
*            VME64x  modules.  If  VME64x  modules  and  standard  VME 
*            modules are  mixed in a  crate the StaticVMEConfiguration 
*            is    needed.     To    use    the     VME64xCrate    the 
*            AddressTableContainerInterface           and          the 
*            ModuleMapperInterface (and  one of there implementations)
*            are needed. 
*            
*            For debugging VME64x modules  there are some more classes
*            which         could        be         usefull.        The 
*            VMEConfigurationSpaceHandler,     the     VMESlot,    the 
*            VMEConfigurationSpaceDevice and the VME64xFunction are 
*            the  most important  of these. 
*            
*    @author Christoph Schwick (Christoph.Schwick@cern.ch)
* $Revision: 1.8 $
*     $Date: 2004/08/02 13:56:47 $
*
*
**/



