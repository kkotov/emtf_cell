#include "xdata/Integer.h"
#include "xdata/String.h"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/WriteVerifyPtLut.hpp"
#include "emtf/ts/cell/Common.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <time.h>
//#include <iomanip>
#include "xdata/Serializable.h"
#include "xdata/UnsignedInteger32.h"
#include "xdata/Vector.h"

using namespace std;
using namespace swatch;
using namespace core;
using namespace log4cplus;


namespace emtf {




static void print(const char *message)
{
    log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(emtf::config::log4cplusGeneralLogger()) );
    LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT(message));
}

static void print(const char *prefix, uint64_t val, const char *suffix="")
{
    std::stringstream oss;
    oss << prefix << std::hex << val << std::dec << suffix;
    log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(emtf::config::log4cplusGeneralLogger()) );
    LOG4CPLUS_INFO( generalLogger, LOG4CPLUS_TEXT( oss.str() ) );
}


static bool verifyPtLut(emtf::EmtfProcessor &processor);
static void writePtLut (emtf::EmtfProcessor &processor);

emtf::VerifyWritePtLut::VerifyWritePtLut(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<EmtfProcessor>()){}

swatch::action::Command::State emtf::VerifyWritePtLut::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Verifying the Pt LUT on the board.");
    setProgress(0.);

    if( verifyPtLut(processor) )
    {
        print("Pt LUT verification failed, trying to write ...");
        writePtLut(processor);
        if( verifyPtLut(processor) ){ 
            print("Pt LUT verification failed second time");
            return Functionoid::kError;
        }
    }
    print("Pt LUT verification succeed");
    setProgress(1.);
    return Functionoid::kDone;
}


emtf::VerifyPtLut::VerifyPtLut(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<EmtfProcessor>()){}

swatch::action::Command::State emtf::VerifyPtLut::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Verifying the Pt LUT on the board.");
    setProgress(0.);

    bool error = verifyPtLut(processor);
    setProgress(1.);

    return (error ? Functionoid::kError : Functionoid::kDone );
}

emtf::WritePtLut::WritePtLut(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<EmtfProcessor>()){
    registerParameter("updateRAMdelays", xdata::Boolean(false));
}

static void initPtLut(emtf::EmtfProcessor &processor);
static void setWritePtLutDelays(emtf::EmtfProcessor&, unsigned int (&)[72]);
static void setReadPtLutDelays (emtf::EmtfProcessor&, unsigned int (&)[72]);

swatch::action::Command::State emtf::WritePtLut::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Writing the Pt LUT to the board.");
    setProgress(0.);

    const xdata::Boolean& updateRAMdelays = params.get<xdata::Boolean>("updateRAMdelays");

    if( updateRAMdelays.value_ ){

        unsigned int writeDelays[72], readDelays[72];

        unsigned procIndex = processor.deviceIndex();
        if( procIndex > 11 ){
            std::ostringstream msg;
            msg << "Invalid processor device index: " << std::hex << processor.deviceIndex();
            throw std::runtime_error( msg.str() );
        }

        std::stringstream wdelFileName, rdelFileName;
        wdelFileName << config::rwDelaysPath() << "/wdel" << boost::format("%|02|") % procIndex << ".txt";
        rdelFileName << config::rwDelaysPath() << "/rdel" << boost::format("%|02|") % procIndex << ".txt";

        ifstream wdelFile(wdelFileName.str());
        if( wdelFile ){
            for(size_t i=0; i<72 && !wdelFile.eof(); i++){
                string tmp;
                wdelFile >> tmp;
                char *endptr = NULL;
                writeDelays[i] = strtoul(tmp.c_str(),&endptr,10);
            }
            wdelFile.close();
        } else {
            std::stringstream oss;
            oss << "Cannot open " << wdelFileName.str();
            log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(config::log4cplusGeneralLogger()) );
            LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT(oss.str()));
            return Functionoid::kError;
        }

        ifstream rdelFile(rdelFileName.str());
        if( rdelFile ){
            for(size_t i=0; i<72 && !rdelFile.eof(); i++){
                string tmp;
                rdelFile >> tmp;
                char *endptr = NULL;
                readDelays[i] = strtoul(tmp.c_str(),&endptr,10);
            }
            rdelFile.close();
        } else {
            std::stringstream oss;
            oss << "Cannot open " << rdelFileName.str();
            log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(config::log4cplusGeneralLogger()) );
            LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT(oss.str()));
            return Functionoid::kError;
        }

        initPtLut(processor);
        setWritePtLutDelays(processor, writeDelays);
        setReadPtLutDelays (processor, readDelays);
    }

    writePtLut(processor);
    setProgress(1.);

    return Functionoid::kDone;
}


emtf::VerifyPtLutVersion::VerifyPtLutVersion(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("pt_lut_version", xdata::UnsignedInteger(1));
}

swatch::action::Command::State emtf::VerifyPtLutVersion::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Check the Pt LUT version.");
    setProgress(0.);

    Command::State commandStatus = Functionoid::kDone;

    const uint32_t ptLutVersionDB = (params.get<xdata::UnsignedInteger>("pt_lut_version").value_);

    uint32_t ptLutVersionFile = 0;


    ifstream file( config::ptLutPath(), ios::in | std::ios::binary);
    if(file.is_open())
    {
        file.read((char*)&ptLutVersionFile, sizeof(ptLutVersionFile));
        file.close();

        std::stringstream oss;
        oss << "Pt LUT version in file: " << ptLutVersionFile;
        log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(config::log4cplusGeneralLogger()) );
        LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT(oss.str()));
    }

    if((ptLutVersionFile&0x1FF) != (ptLutVersionDB&0x1FF))
    {
        commandStatus = Functionoid::kError;
    }
    setProgress(1.);

    return commandStatus;
}

//////////////////////////////////////////////////////////////

#define CHIP_SIZE 0x2000000L // size of one RLDRAM chip in 18-bit words (32 MW)
#define CHIP_COUNT 16L // how many RLDRAM chips on board
#define RL_MEM_SIZE (CHIP_SIZE*CHIP_COUNT) //size of RLDRAM in 18-bit words (512 MW = 1GB)

uint32_t* emtf::PtLut::data_buf = 0;
uint32_t* emtf::PtLut::addr_buf = 0;
boost::mutex emtf::PtLut::mtx;

uint32_t emtf::PtLut::size(void) throw ()
{
    uint32_t retval = 0;
    boost::unique_lock<boost::mutex> lock(emtf::PtLut::mtx);

    if( data_buf != 0 && addr_buf != 0 )
        retval = RL_MEM_SIZE;

    return retval;
}

const uint32_t* emtf::PtLut::getData(void) throw (std::runtime_error)
{
    boost::unique_lock<boost::mutex> lock(emtf::PtLut::mtx);

    if( data_buf == 0 )
        try
        {
            readLUT();
        }
        catch ( std::exception &e )
        {
            if( data_buf )
            {
                delete [] data_buf;
                data_buf = 0;
            }
            throw ( std::runtime_error( e.what() ) );
        }
        catch ( ... )
        {
            if( data_buf )
            {
                delete [] data_buf;
                data_buf = 0;
            }
            throw( std::runtime_error( "Unidentified problem while reading Pt LUT" ) );
        }

    return data_buf;
}

const uint32_t* emtf::PtLut::getAddress(void) throw (std::runtime_error)
{
    boost::unique_lock<boost::mutex> lock(emtf::PtLut::mtx);

    if( addr_buf == 0 ){
        try
        {
            addr_buf = new uint32_t [ RL_MEM_SIZE/2 ];
        }
        catch ( std::bad_alloc &e )
        {
            addr_buf = 0;
            throw( std::runtime_error( e.what() ) );
        }
        catch ( ... )
        {
            if( addr_buf )
            {
                delete [] addr_buf;
                addr_buf = 0;
            }
            throw( std::runtime_error( "Unidentified problem while generating Pt LUT addresses" ) );
        }

        bzero(addr_buf, RL_MEM_SIZE * sizeof(uint32_t)/2 );

        // just fill addresses for file contents
        for (uint32_t i = 0; i < RL_MEM_SIZE/2; i++)
            addr_buf[i] = (i*2); // address progresses by 2 because two words at a time are written
    }

    return addr_buf;
}

void emtf::PtLut::readLUT(void) throw ( std::bad_alloc, std::ios_base::failure, std::runtime_error )
{
    // reserve buffers
    // each 32-bit word contains one 18-bit word for RLDRAM
    data_buf = new uint32_t [ RL_MEM_SIZE ];

    bzero(data_buf, RL_MEM_SIZE * sizeof(uint32_t) );

    ifstream file( config::ptLutPath(), ios::in | std::ios::binary);
    if( file.is_open() )
    {
        file.read( (char*)data_buf,  RL_MEM_SIZE * sizeof(uint32_t) );
        if( file.bad() || file.gcount() != RL_MEM_SIZE * sizeof(uint32_t) )
        {
            std::ostringstream msg;
            msg << "reading failure, read bytes: 0x" << std::hex << file.gcount() << " bytes" << std::flush;
            file.close();
            throw ( std::runtime_error( msg.str() ) );
        }
        file.close();
    }
}

//////////////////////////////////////////////////////////////

// each 32-bit PC word contains one 18-bit RLDRAM word
#define RL_DATA_SIZE_B (RL_MEM_SIZE*4L) // size of memory data buffer in bytes
#define FW_DATA_SIZE_B 0x2000 // size of data buffer in firmware, bytes
#define FW_ADDR_SIZE_B 0x1000 // size of addr buffer in firmware, bytes

#define XFER_SIZE_B 0x400 // 0x800 // one transfer via core link, bytes
#define XFERS_FW_DATA (FW_DATA_SIZE_B/XFER_SIZE_B) // count of transfers per fw data buffer
#define XFERS_FW_ADDR (FW_ADDR_SIZE_B/XFER_SIZE_B) // count of transfers per fw addr buffer
#define RL_BUFS_ALL (RL_DATA_SIZE_B/FW_DATA_SIZE_B) // count of data&address buffers to fill = 0x40000
#define RL_BUFS (RL_BUFS_ALL)

#define ODT_ON  0x40008
#define ODT_OFF 0x40000

static void write_mrs(emtf::EmtfProcessor &processor, uint32_t cs, uint32_t code);

static void writePtLut(emtf::EmtfProcessor &processor)
{

    const uint32_t *data_buf = emtf::PtLut::getData();
    const uint32_t *addr_buf = emtf::PtLut::getAddress();

    // Setting write and read delay registers

///    initPtLut(processor);
///    setWritePtLutDelays(processor);
///    setReadPtLutDelays(processor);

    // Writing blocks to the board

    write_mrs(processor, 0x01010101, ODT_ON); // turn ODT on, only on one chip at the end of each quad

    ///clock_t start = clock();

    for (int i = 0; i < RL_BUFS; i++) {

        // fill address buffer in FW
        for (int j = 0; j < XFERS_FW_ADDR; j++)
            processor.writeBlock64("ptlut_addr", XFER_SIZE_B, (char*)(addr_buf + i*FW_ADDR_SIZE_B/4 + j*XFER_SIZE_B/4), j*XFER_SIZE_B);
        // fill data buffer in FW
        for (int j = 0; j < XFERS_FW_DATA; j++)
            processor.writeBlock64("ptlut_mem", XFER_SIZE_B, (char*)(data_buf + i*FW_DATA_SIZE_B/4 + j*XFER_SIZE_B/4), j*XFER_SIZE_B );

        // send command
        processor.write64("ptlut_write_cmd", 0x1);
        // remove
        processor.write64("ptlut_write_cmd", 0x0);

        // wait until not busy
        for(uint64_t val=1; val==1; processor.read64("ptlut_busy", val) ); // maybe sleep a bit?
    }


    ///clock_t end = clock() - start;
    ///std::cout<< "Write time: "<< (double)end / ((double)CLOCKS_PER_SEC) << " s" << std::endl;

    write_mrs(processor, 0xffffffff, ODT_OFF); // turn ODT off
}

static bool verifyPtLut(emtf::EmtfProcessor &processor)
{
    // reserve buffers for tests
    // each 32-bit word contains one 18-bit word for RLDRAM
    boost::shared_ptr<uint64_t[]> data_buf( new uint64_t [ FW_DATA_SIZE_B/sizeof(uint64_t) ] );
    boost::shared_ptr<uint32_t[]> addr_buf( new uint32_t [ FW_ADDR_SIZE_B/sizeof(uint32_t) ] );

    const uint64_t *ref_buf = (uint64_t *)emtf::PtLut::getData();

    bzero(data_buf.get(), FW_DATA_SIZE_B);
    bzero(addr_buf.get(), FW_ADDR_SIZE_B);

    bool error = false;
    for(unsigned int block=0; block<1000; block++)
    {
        // Generating random addresses for selective read'n'compares
        for(unsigned int j=0, prev_rand=0; j<FW_ADDR_SIZE_B/sizeof(uint32_t); j++)
        {
            uint32_t new_rand = 0;
            // make sure the new address does not hit same bank in same chip as old one
            // for 8-ns chips, just one old clock can be analyzed
            // for 10-ns chips, two clocks have to be analyzed
            while ( 1 ) {
                new_rand = ( random() & 0x1ffffffe ); // j%96
                uint32_t prev_bank =  prev_rand & 0x1f;
                uint32_t prev_chip = (prev_rand >> 25) & 0xf;
                uint32_t new_bank =  new_rand & 0x1f;
                uint32_t new_chip = (new_rand >> 25) & 0xf;
                if( prev_bank != new_bank || prev_chip != new_chip ) break;
            }
            prev_rand = new_rand;

            addr_buf.get()[j] = new_rand;
        }

        // fill address buffer in FW
        for(int j = 0; j < XFERS_FW_ADDR; j++)
            processor.writeBlock64("ptlut_addr", XFER_SIZE_B, (char*)(addr_buf.get() + j*XFER_SIZE_B/4), j*XFER_SIZE_B);

        // send command
        processor.write64("ptlut_read_cmd", 0x1);
        // remove
        processor.write64("ptlut_read_cmd", 0x0);

        // wait until not busy
        for(uint64_t val=1; val==1; processor.read64("ptlut_busy", val) ); // maybe sleep a bit?

        memset(data_buf.get(), 0x55, sizeof(data_buf));

        // fill data buffer in FW
        for(int j = 0; j < XFERS_FW_DATA; j++)
            processor.readBlock64("ptlut_mem", XFER_SIZE_B, (char*)(data_buf.get() + j*XFER_SIZE_B/8), j*XFER_SIZE_B );

        ///if( (block%100) == 0 ) print("Progress: ",block,"/0x400");

        // now compare
        for(int j = 0, err_count = 0; j < FW_DATA_SIZE_B/8; j++)
        {
            // get written data from global buffer, from random address
            uint64_t wd = ref_buf[addr_buf.get()[j]/2];
            // get data that were read from that random address
            uint64_t rd = data_buf.get()[j];
            uint64_t xord = wd ^ rd;
            if (xord != 0)
            {
                if (err_count < 0) // suppress it for now
                {
                    std::ostringstream msg;
                    msg << "Mismatch: j:" << j << " addr: "<< std::hex << addr_buf[j]
                        << " w: " << wd << " r: " << rd << " e: " << xord << std::dec << std::flush;
                    print( msg.str().c_str() );
                }
                err_count++;
                error = true;
            }
        }

    }

    return error;
}

// contents of MRTs, see RLDRAM3_registers.xlsx
#define MR0 0x10 // for 093E memory
#define MR1 0x400e0
#define MR2 0x80000 // normal operation

static void initPtLut(emtf::EmtfProcessor &processor)
{
    uint64_t wr_lat  = 4;
    uint64_t rd_lat  = 15;// for version with RX FIFO
    uint64_t wr_term = wr_lat + 0x403; //0x408;//0x407;
    uint64_t rd_term = rd_lat + 0x3fe;//0x40a;
    uint64_t wr_shift = 1;
    uint64_t rd_shift = 0;
    uint64_t qr_sel = rd_lat - 5;
    uint64_t qw_sel = wr_lat - 2;
    uint64_t we_sel = rd_lat - 4; //8;

    processor.write64("ptlut_wr_term_count",wr_term);
    processor.write64("ptlut_rd_term_count",rd_term);
    processor.write64("ptlut_wr_latency",wr_lat);
    processor.write64("ptlut_rd_latency",rd_lat);
    processor.write64("ptlut_wr_phase_shift",wr_shift);
    processor.write64("ptlut_rd_phase_shift",rd_shift);
    processor.write64("ptlut_dpm_wr_del",we_sel);
    processor.write64("ptlut_wr_quad_del",qw_sel);
    processor.write64("ptlut_rd_quad_del",qr_sel);

    processor.write64("ptlut_chips_rst",0x1);
    usleep(10000);

    processor.write64("ptlut_chips_rst",0x0);
    usleep(10000);

    uint64_t mrs[3] = {MR0, MR1, MR2};
    for (int i = 0; i < 3; i++)
    {
        write_mrs(processor, 0xffffffff, mrs[i]);
        usleep (10000);
    }

    write_mrs(processor, 0xffffffff, ODT_OFF);

    uint64_t value = 0;
    processor.read64("ptlut_delay_ctl_locked", value);
    print("delay_ctrl lock status: 0x", ((value >> 22) & 0xf) );

    // reset IDELAY_CONTROL
    processor.write64("ptlut_dbdel_rst",0x1); // IO and IDELAY_CONTROL reset
    usleep(10000);

    // remove reset bit
    processor.write64("ptlut_dbdel_rst", 0x0);

    processor.read64("ptlut_delay_ctl_locked", value);
    print("delay_ctrl lock status: 0x", ((value >> 22) & 0xf) );

    // enable refresh, program RX clock domain crossing polarity
    processor.write64("ptlut_refresh_en",0x1);
    // program wait times after refresh and read cycles
    processor.write64("ptlut_rd_bank_timeout",0x1);
    processor.write64("ptlut_refresh_bank_timout",0x1);
    processor.write64("ptlut_core_rq_mask",0x7); // ptlut requests enable mask
}

static void write_mrs(emtf::EmtfProcessor &processor, uint32_t cs, uint32_t code)
{
    // chip select mask into data buffer
    // bits 17:0 to bits 17:0
    // bits 31:18 to bits 45:32
    uint64_t value = cs;
    value = ((value << 14) & 0x3fff00000000ULL) | (cs & 0x3ffffULL);

    processor.writeBlock64("ptlut_mem", 8, (char*)&value, 0);

    // write command code into address buffer A=0
    uint64_t _code = code;
    processor.writeBlock64("ptlut_addr", 8, (char*)&_code, 0);

    // send command
    processor.write64("ptlut_mrs_cmd", 0x1);
    processor.write64("ptlut_mrs_cmd", 0x0);
}

static void setWritePtLutDelays(emtf::EmtfProcessor &processor, unsigned int (&wdel)[72])
{
    for(unsigned int i=0; i<72; i++)
    {
        std::stringstream reg;
        reg << "ptlut_db_out_del_" << i;
        processor.write64( reg.str(), wdel[i] );
        processor.write64("ptlut_dbdel_ld", 0x1);
        processor.write64("ptlut_dbdel_ld", 0x0);
    }

    processor.write64("ptlut_inp_clk_del", 0x5);
    processor.write64("ptlut_dbdel_ld", 0x1);
    processor.write64("ptlut_dbdel_ld", 0x0);
}

static void setReadPtLutDelays(emtf::EmtfProcessor &processor, unsigned int (&rdel)[72])
{
    for(unsigned int i=0; i<72; i++)
    {
        std::stringstream reg;
        reg << "ptlut_db_inp_del_" << i;
        processor.write64( reg.str(), rdel[i] );
        processor.write64("ptlut_dbdel_ld", 0x1);
        processor.write64("ptlut_dbdel_ld", 0x0);
    }

    processor.write64("ptlut_inp_clk_del", 0x5);
    processor.write64("ptlut_dbdel_ld", 0x1);
    processor.write64("ptlut_dbdel_ld", 0x0);
}

} // namespace

