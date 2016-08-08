#ifndef __HardwareDeviceInterface
#define __HardwareDeviceInterface

#include <string>
#include <iostream>

#include "hal/AddressTableInterface.hh"
#include "hal/NoSuchItemException.hh"
#include "hal/IllegalOperationException.hh"
#include "hal/IllegalValueException.hh"
#include "hal/VerifyException.hh"
#include "hal/UnsupportedException.hh"
#include "hal/BusAdapterException.hh"
#include "hal/MaskBoundaryException.hh"
#include "hal/TimeoutException.hh"
#include "hal/AddressOutOfLimitsException.hh"

namespace HAL {

/**
*
*
*     @short A  type  in order  to  determine  the  operation mode  of 
*            hardware write accesses.
*            
*            In the hardware access  lilbrary there is the possibility
*            to read back  any written value and check  the read value 
*            against the  one written.  This feature is  useful during 
*            debugging of hardware.
*            
*            The  type  defined in  this  enum  is  used in  order  to 
*            specify  for  each  write  access  whether  or  not  this 
*            feature should be  in use. The user has  to pay attention 
*            to  use  this  feature  only  for items  which  are  also 
*            readable. 
*
*       @see HardwareDeviceInterface
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/
enum HalVerifyOption { HAL_NO_VERIFY, HAL_DO_VERIFY };

/**
*
*
*     @short Needed if a block of data is transfered to or from Fifos.
*            
*            If  a block of  data should  be transfered  from or  to a 
*            Fifo, the address of  the Fifo should stay constant. This
*            type is  used to specify this behaviour  for the transfer 
*            of blocks  of data (writeBlock and  readBlock). Note that 
*            this variable is NOT  relevant for VME if the BusAdapter
*            lets  the driver use  Block Transfer,  because it  is the 
*            hardware of the  slave which decides what to  do with the 
*            addresses  if a block  transfer is  issued. On  the other 
*            hand  if  the  BusAdapter  implements the  blockRead  and 
*            blockWrite  with   single  accesses  the   software  will 
*            respect the settings of the relevant flags.
*            
*            Confused?  read the  documentation of  the  readBlock and 
*            writeBlock   functions.    Still   confused?   You   must 
*            unfortunately know more details about VME specification.
*
*       @see HardwareDeviceInterface
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/

enum HalAddressIncrement { HAL_DO_INCREMENT, HAL_NO_INCREMENT };

/**
*
*
*     @short Controles the operation of the pollItem method.
*            
*            While polling  an item with  the pollItem method  you can 
*            decide if you  want to poll until the item  is equal to a 
*            referenceValue which  you give to the routine,  or if you 
*            want  to  poll  until  an  item  is  different  from  the 
*            referenceValue. This  enum is used  to choose one  of the 
*            two methods in the pollItem method.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/
enum HalPollMethod { HAL_POLL_UNTIL_EQUAL, HAL_POLL_UNTIL_DIFFERENT };
/**
*
*
*     @short The interface to ALL hardware devices.
*            
*            This Interface defines all operations the user can invoke
*            in order  to interact with an  arbitrary hardware device.
*            Hardware  devices   may  have  more   functionality  than
*            described in this interface depending on which technology
*            they are built (e.g. for VME devices an interface for the
*            block  transfer  could  be foreseen).   These  additional
*            functionalities  if  they are  present  are described  in
*            separate interfaces.
*            
*            Most of the  functions act on so called  items defined in
*            an  AddressTable. An  item is  a register  or  a bitfield
*            which  forms an  entity  to  be written  or  read by  the
*            software.  (See  documentation  of the  AddressTable  for
*            details.)
*            
*            In  addition most  of the  functions contain  an optional 
*            parameter  named "offset".  This offset  is added  to the 
*            address  of the  item before  the requested  operation is 
*            carried  out.  If the  offset  is  not  specified in  the 
*            method invokation  it defaults  to zero. Offset  might be 
*            useful in order to  access memory regions.
*            
*            All items which write  data into the hardware device have
*            an  optional parameter "verifyFLag"  which by  default is
*            set to HAL_NO_VERIFY. If set to HAL_DO_VERIFY the written
*            data is read out  immediately after the write process and
*            checked against the data  written. An exception is thrown
*            in case the  read resulted in a different  value than the
*            one written. It is  the user's responsibility to use this
*            option only on items which are readable. For this flag on
*            purpose a  new type  has been defined  in order  to catch
*            programming errors at compile time: If the verifyFlag was
*            boolean a  user could errorneously write  a command like:
*            "myCard.write("myItem", data,  offset);" forgetting about
*            the verifyFlag.  The compiler will not  complain since it
*            will convert the  offset of type unsinged long  to a bool
*            value  and happily  take the  default value  '0'  for the
*            offset.  With  the type 'HalVerifyOption'  introduced for
*            the  verifyFlag this  kind of  error will  be  catched at
*            compile-time.
*            
*            
*       @see AddressTableInterface, AddressTable
*    @author Christoph Schwick 
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/

class HardwareDeviceInterface {
public:

  virtual ~HardwareDeviceInterface() {};

  /**
   * Write a data word without applying the mask for the item.
   * @param verifyFlag allows automatic verification of the written data
   *        if set to "HAL_DO_VERIFY". If the item is readable it is read
   *        back and compared to the originally written value. The 
   *        VerifyException is thrown if no coincidenc is found.
   */
  virtual void unmaskedWrite( std::string item, 
                              uint32_t data,
                              HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                              uint32_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException, 
           VerifyException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  virtual void unmaskedWrite64( std::string item, 
                              uint64_t data,
                              HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                              uint64_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException, 
           VerifyException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  /**
   * Write a new value for an item into the hardware device.
   * This is  the function which is  usually used in  order to write
   * data  to  the hardware.  It  is  important  to understand  some
   * details of what is happening when invoking the method:
   *  - The data is shifted into the bit positions indicated by 
   *    the items mask.
   *  - If the item is readable : read the value currently contained 
   *    at the address defined by the offset and the items adddress. 
   *    Then substitute those bits of the read word which correspond
   *    to the items mask with the new data. 
   *  - If the item is NOT readable : set the bits which correspond 
   *    to the items mask with to the new data. All other bits are 
   *    set to 0.
   *
   * The aim of  the procedure is that new values  can be written to
   * bitfields without  destroying the  value contained in  bits not
   * belonging to the  item. This of course is  only possible if the
   * item is also readable.
   * @param verifyFlag allows automatic verification of the written data
   *        if set to "HAL_DO_VERIFY". If the item is readable it is read
   *        back and compared to the originally written value. The 
   *        VerifyException is thrown if no coincidenc is found.
   *
   */
  virtual void write( std::string item, 
                      uint32_t data, 
                      HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                      uint32_t offset = 0 ) const
    throw ( NoSuchItemException, 
	    IllegalOperationException, 
	    VerifyException,
	    BusAdapterException,
	    MaskBoundaryException,
	    AddressOutOfLimitsException ) = 0;

  virtual void write64( std::string item, 
                      uint64_t data, 
                      HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                      uint64_t offset = 0 ) const
    throw ( NoSuchItemException, 
	    IllegalOperationException, 
	    VerifyException,
	    BusAdapterException,
	    MaskBoundaryException,
	    AddressOutOfLimitsException ) = 0;

  /**
   * Perform a write access in order to trigger side effects.
   * Often in  hardware modules  certain functionality or  action is
   * triggered by  a write-access to  a specific address,  where the
   * data value  transfered does not  matter but only the  so called
   * side-effects of the write operation are exploited. (For example
   * to invoke some reset function.) This function should be used in
   * these  cases. Using it  consequently in  these cases  makes the
   * code  readable. The function  is as  efficient as  the unmasked
   * write function.
   * The function actually writes a 0 into the hardware.
   */
  virtual void writePulse( std::string item, 
                           uint32_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  virtual void writePulse64( std::string item, 
                           uint64_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  /**
   * Read a data word without applying the items mask.
   */
  virtual void unmaskedRead( std::string item,
                             uint32_t* result,
                             uint32_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  virtual void unmaskedRead64( std::string item,
                             uint64_t* result,
                             uint64_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  /**
   * Read an item from the hardware device.
   * This is the function to be  normally used in order to read data
   * from  the hardware. The  items bit  mask in  used to  shift the
   * result that  the items least  significant bit becomes bit  0 of
   * the result.
   */
  virtual void read( std::string item, 
                     uint32_t* result,
                     uint32_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
           MaskBoundaryException,
	   AddressOutOfLimitsException) = 0;

  virtual void read64( std::string item, 
                     uint64_t* result,
                     uint64_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
           MaskBoundaryException,
	   AddressOutOfLimitsException) = 0;

  /**
   * Read an item from the hardware and check the result.
   * For debugging and automated tests this function reads a value
   * from the hardware and checks it against an expected value. 
   * In case the function finds a descrepancy between the expected 
   * and the read value a message is printed to the standard output
   * containing the item name, the read value and the expected 
   * value. 
   * @param expectation is the value which is expected to be read from
   *        item.
   * @param faultMessage might contain additional information to be
   *        printed in case of a non successful test.
   * @return false is returned in case of an unsuccessfull test; true
   *        otherwise.
   */
  virtual bool check( std::string item,
                      uint32_t expectation,
                      std::string faultMessage = "",
                      uint32_t offset = 0,
		      std::ostream& os = std::cout ) const
    throw( NoSuchItemException,
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  virtual bool check64( std::string item,
                      uint64_t expectation,
                      std::string faultMessage = "",
                      uint64_t offset = 0,
		      std::ostream& os = std::cout ) const
    throw( NoSuchItemException,
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  /**
   * Perform a read access in order to trigger side effects.
   * This  function is  performing  a read  access  to trigger  some
   * hardware specific  activity (side effect). No  data is actually
   * returned to the user.
   */
  virtual void readPulse( std::string item, 
                          uint32_t offset = 0 ) const
  throw (NoSuchItemException, 
         IllegalOperationException,
         BusAdapterException,
	 AddressOutOfLimitsException) = 0;

  virtual void readPulse64( std::string item, 
                          uint64_t offset = 0 ) const
  throw (NoSuchItemException, 
         IllegalOperationException,
         BusAdapterException,
	 AddressOutOfLimitsException) = 0;

  /**
   * Set a single bit.
   * This function  can be used if  the item describes  a flag (i.e.
   * exactly one bit is set to  '1' in the item's bit mask.). If the
   * flag  needs to  be  set according  to  a value  contained in  a
   * variable the write command should be used.
   * @param verifyFlag allows automatic verification of the written data
   *        if set to "HAL_DO_VERIFY". If the item is readable it is read
   *        back and compared to the originally written value. The 
   *        VerifyException is thrown if no coincidenc is found.
   */
  virtual void setBit( std::string item,
                       HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                       uint32_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException, 
           VerifyException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  virtual void setBit64( std::string item,
                       HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                       uint64_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException, 
           VerifyException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  /**
   * Reset a single bit.
   * This function  can be used if  the item describes  a flag (i.e.
   * exactly one bit is set to  '1' in the item's bit mask.). If the
   * flag  needs to be  reset according  to a  value contained  in a
   * variable the write command should be used.
   * @param verifyFlag allows automatic verification of the written data
   *        if set to "HAL_DO_VERIFY". If the item is readable it is read
   *        back and compared to the originally written value. The 
   *        VerifyException is thrown if no coincidenc is found.
   */
  virtual void resetBit( std::string item,
                         HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                         uint32_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException, 
           VerifyException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  virtual void resetBit64( std::string item,
                         HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                         uint64_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException, 
           VerifyException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  /**
   * Test the value of a single bit.
   * This function  can be used if  the item describes  a flag (i.e.
   * exactly one bit is set to '1' in the item's bit mask.)
   */
  virtual bool isSet( std::string item, 
                      uint32_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  virtual bool isSet64( std::string item, 
                      uint64_t offset = 0 ) const
    throw (NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException,
	   AddressOutOfLimitsException) = 0;

  /**
   * A function to read a block of data into the host memory.
   * The buffer to be filled consists of an allocated space of 
   * bytes. The user has to make sure that this buffer is large 
   * enough to receive all data to be read. For this function
   * you must read the documentation in the technology dependent 
   * classes VMEDevice or PCIDevice.
   *
   * @see VMEDevice, PCIDevice
   * 
   * @param startItem is a item in the AddressTable which defines the 
   *        startaddress of the block in the hardware device. Note that
   *        the addressModifier of this item determines how the transfer
   *        will be performed. Blocktransfers are only initiated if the 
   *        correct addressmodifier is associated with this item (and 
   *        of course, if the used BusAdapter is supporting BlockTransfers).   
   * @param length is the number of bytes the user wants to transfer.
   *        Note that the length must be a multiple of the dataWidth
   *        specified for the item (in the AddressTable), otherwise 
   *        an exception is thrown.
   * @param buffer points to the destination in the host computer where to 
   *        transfer the data to. It is the responsibility of the user
   *        to provide a data buxffer with sufficient space! This 
   *        library cannot prevent core-dumps and segmentation faults
   *        here. (It starts to become less user-friendly here, but 
   *        if you want performance, that's life !!!!)
   *        On purpose this buffer has been specified to be of the 
   *        type (char) since this makes sure that it is just an
   *        array of bytes. 
   * @param verifyFlag allows automatic verification of the written data
   *        if set to "HAL_DO_VERIFY". If the startItem is readable the 
   *        data block is read back with the readBlock function and 
   *        the read block is compared to the orgiginal block written.
   *        The VerifyException is thrown if no coincidence is found.
   * @param addressBehaviour determines if the address in the 
   *        hardware device is incremented for every dataword to be 
   *        transfered or if it stays constant (in order to read out
   *        some Fifo-like memory). NOTE THAT YOU CANNOT ALWAYS 
   *        GUARANTEE THAT YOUR SYSTEMS BEHAVES ACCORDING TO THE 
   *        SETTING OF THIS FLAG. The reason for this is explained
   *        in the documentation of this function in the VMEDevice.
   *        (Instead for PCIDevices the funtion behaves as you expect.)
   * @param offset is optional and is the an offset in number of bytes
   *        which in present is added to the address correspondig to 'item'.
   *        The resulting address must be aligned with respect to the
   *        dataWidth of 'item'.
   * @throws IllegalValueException is thrown if the length is not a 
   *         multiple of the dataWidth defined for the item.
   * @throws UnsupportedException if the desired addressBehaviour is not
   *         implemented by the attached BusAdapter.
   */
  virtual void writeBlock( std::string startItem, 
                           uint32_t length,
                           char *buffer,
                           HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                           HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                           uint32_t offset = 0 ) const
    throw ( NoSuchItemException,
            IllegalValueException, 
            UnsupportedException, 
            VerifyException,
            IllegalOperationException,
            BusAdapterException,
	    AddressOutOfLimitsException ) = 0;

  virtual void writeBlock64( std::string startItem, 
                           uint64_t length,
                           char *buffer,
                           HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                           HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                           uint64_t offset = 0 ) const
    throw ( NoSuchItemException,
            IllegalValueException, 
            UnsupportedException, 
            VerifyException,
            IllegalOperationException,
            BusAdapterException,
	    AddressOutOfLimitsException ) = 0;

  /**
   * A function to a block write of data into the hardware device.
   * The data to be written is contained in a buffer of the host.
   * The user has to make sure that this buffer is at least as large 
   * as indicated by the length argument. For this function
   * you must read the documentation in the technology dependent 
   * classes VMEDevice or PCIDevice.
   *
   * @see VMEDevice, PCIDevice
   * 
   * @param startItem is a item in the AddressTable which defines the 
   *        startaddress of the block in the hardware device. Note that
   *        the addressModifier of this item determines how the transfer
   *        will be performed. Blocktransfers are only initiated if the 
   *        correct addressmodifier is associated with this item (and 
   *        of course, if the used BusAdapter is supporting BlockTransfers).
   * @param length is the number of bytes the user wants to transfer.
   *        Note that the length must be a multiple of the dataWidth
   *        specified for the item (in the AddressTable), otherwise 
   *        an exception is thrown.
   * @param buffer points to the data in the host computer from where to 
   *        transfer the data. It is the responsibility of the user
   *        to make sure that this buffer is at least as large as 
   *        indicated by the length paramenter! This 
   *        library cannot prevent core-dumps and segmentation faults
   *        here! (It starts to become less user-friendly here, but 
   *        if you want performance, that's life !!!!)
   *        On purpose this buffer has been specified to be of the 
   *        type (char) since this makes sure that it is just an
   *        array of bytes. 
   * @param addressBehaviour determines if the address at in the 
   *        hardware device is incremented for every dataword to be 
   *        transfered or if it stays constant (in order to read out
   *        some Fifo-like memory). NOTE THAT YOU CANNOT ALWAYS 
   *        GUARANTEE THAT YOUR SYSTEMS BEHAVES ACCORDING TO THE 
   *        SETTING OF THIS FLAG. The reason for this is explained
   *        in the documentation of this function in the VMEDevice.
   *        (Instead for PCIDevices the funtion behaves as you expect.)
   * @param offset is optional and is the an offset in number of bytes
   *        which in present is added to the address correspondig to 'item'.
   *        The resulting address must be aligned with respect to the
   *        dataWidth of 'item'.
   * @throws IllegalValueException is thrown if the length is not a 
   *         multiple of the dataWidth defined for the item.
   * @throws UnsupportedException if the desired addressBehaviour is not
   *         implemented by the attached BusAdapter.
   */
  virtual void readBlock( std::string startItem, 
                          uint32_t length,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                          uint32_t offset = 0 ) const
    throw (NoSuchItemException,
           IllegalValueException, 
           UnsupportedException, 
           BusAdapterException,
           IllegalOperationException,
	   AddressOutOfLimitsException) = 0;

  virtual void readBlock64( std::string startItem, 
                          uint64_t length,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                          uint64_t offset = 0 ) const
    throw (NoSuchItemException,
           IllegalValueException, 
           UnsupportedException, 
           BusAdapterException,
           IllegalOperationException,
	   AddressOutOfLimitsException) = 0;

  /**
   * This function polls an item and does not return until the polling condition
   * is matched. 
   *
   * The function allows to poll until a reference value is read back from the 
   * hardware or until the value read from the hardware is different from the 
   * reference value. The user must give a timeout value in ms. If the call to this
   * function takes longer than the timeout a TimeoutException is thrown.
   * @param referenceValue is the value against the polled item is checked. The 
   *        value of the pollMethod parameter defines its meaning (see below).
   * @param timeout determines the time in milliseconds after which a TimeoutException
   *        will be thrown if the poll condition has not been encountered before.
   *        This parameter is necessary in order to avoid that faulty hardware or buggy
   *        programs make this function to block forever. The timeout value must be 
   *        given in milliseconds. It is only garanteed that the exception is not 
   *        thrown before the timout-period has passed. It might though pass a 
   *        significant longer time before the exception is thrown (this depends on 
   *        the implementation of the routine).
   * @param *result points to a variable which will contain the value of the last 
   *        read access performed during polling. If the call to pollItem has been
   *        terminated by an exception there is no meaningful value in this variable.
   * @param pollMethod can take the value HAL_POLL_UNTIL_EQUAL or HAL_POLL_UNTIL_DIFFERENT.
   *        If the first value (which is the default) is given, the call blocks 
   *        until the value read back from the item is equal to the referenceValue.
   *        If the second value is given, the call blocks until the value read back
   *        from the item is not equal to the referenceValue.
   */
  virtual void pollItem( std::string item,
                         uint32_t referenceValue,
                         uint32_t timeout,
                         uint32_t *result,
                         HalPollMethod pollMethod = HAL_POLL_UNTIL_EQUAL,
                         uint32_t offset = 0 ) const
    throw ( NoSuchItemException,
            IllegalOperationException,
            BusAdapterException,
            MaskBoundaryException,
            TimeoutException,
	    AddressOutOfLimitsException ) = 0;

  virtual void pollItem64( std::string item,
                         uint64_t referenceValue,
                         uint64_t timeout,
                         uint64_t *result,
                         HalPollMethod pollMethod = HAL_POLL_UNTIL_EQUAL,
                         uint64_t offset = 0 ) const
    throw ( NoSuchItemException,
            IllegalOperationException,
            BusAdapterException,
            MaskBoundaryException,
            TimeoutException,
	    AddressOutOfLimitsException ) = 0;

  /**
   * Retrieve a handle to the AddressTableInterface.
   * In some  cases it might be  desirable to get  the AddressMap of
   * the hardware device (for  example to get addresses subsequently
   * used in unmasked read/write commands for time critical parts of
   * the program). Therefore a  method is provided which returns the
   * Memory Map.
   */
  virtual const AddressTableInterface& getAddressTableInterface() const = 0;

  /**
   * Print out the AddressTable of the hardware device
   * This  prints  out  the   AddressTable  which  is  used  by  the
   * HardwareDevice to resolve the addresses of the various items.
   */
  virtual void printAddressTable() const = 0;

}; 
} /* namespace HAL */

#endif /*__HardwareDeviceInterface */

