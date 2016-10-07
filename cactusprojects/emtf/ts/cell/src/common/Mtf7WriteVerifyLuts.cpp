#include "xdata/Integer.h"
#include "xdata/String.h"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7WriteVerifyLuts.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include "boost/lexical_cast.hpp"
#include <time.h>

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

PcLuts::PcLuts(int endcap, int sector)
{
    generateThLutPairs(endcap, sector, thLutPairs); // read theta LUTs
    // TODO: if the operation is not successful throw and exception and the the command should fail

    // read correction LUTs
    generateThCorrPairs(endcap, sector, thCorrPairs);
    // TODO: if the operation is not successful throw and exception and the the command should fail
}

void PcLuts::generateThLutPairs(int endcap, int sector, map<string, vector<unsigned long long>> & thLutPairs)
{
    boost::format regNameTemplate("prim_conv_th_lut_%s_0%u");
    vector<string> regNameStations = {"me1a", "me1b", "me2", "me3", "me4"};

    boost::format fileNameTemplate(config::pcLutsPath() + "vl_th_lut_endcap_%d_sec_%d_%s_ch_%u.lut");
    vector<string> fileNameStations = {"sub_1_st_1", "sub_2_st_1", "st_2", "st_3", "st_4"};

    // TODO: replace this with an exception
    assert(regNameStations.size() == fileNameStations.size());

    for(unsigned i=0; i<regNameStations.size(); ++i)
    {
        for(unsigned chamber=1; chamber<=9; ++chamber)
        {
            string regName = (regNameTemplate % regNameStations[i] % chamber).str();
            string fileName = (fileNameTemplate % endcap % sector % fileNameStations[i] % chamber).str();

            loadData(regName, fileName, thLutPairs);
        }
    }

    loadData((regNameTemplate % "me1n" % 3).str(), (fileNameTemplate % endcap % sector % "sub_1_st_1" % 13).str(), thLutPairs);
    loadData((regNameTemplate % "me1n" % 6).str(), (fileNameTemplate % endcap % sector % "sub_1_st_1" % 14).str(), thLutPairs);
    loadData((regNameTemplate % "me1n" % 9).str(), (fileNameTemplate % endcap % sector % "sub_1_st_1" % 15).str(), thLutPairs);
    loadData((regNameTemplate % "me2n" % 3).str(), (fileNameTemplate % endcap % sector % "st_2"       % 10).str(), thLutPairs);
    loadData((regNameTemplate % "me2n" % 9).str(), (fileNameTemplate % endcap % sector % "st_2"       % 11).str(), thLutPairs);
    loadData((regNameTemplate % "me3n" % 3).str(), (fileNameTemplate % endcap % sector % "st_3"       % 10).str(), thLutPairs);
    loadData((regNameTemplate % "me3n" % 9).str(), (fileNameTemplate % endcap % sector % "st_3"       % 11).str(), thLutPairs);
    loadData((regNameTemplate % "me4n" % 3).str(), (fileNameTemplate % endcap % sector % "st_4"       % 10).str(), thLutPairs);
    loadData((regNameTemplate % "me4n" % 9).str(), (fileNameTemplate % endcap % sector % "st_4"       % 11).str(), thLutPairs);
}

void PcLuts::generateThCorrPairs(int endcap, int sector, std::map<std::string, std::vector<unsigned long long>> & thCorrPairs)
{
    boost::format fileNameTemplate(config::pcLutsPath() + "vl_th_corr_lut_endcap_%d_sec_%s_%s_%s.lut");

    loadData("prim_conv_th_cor_me1a_01", (fileNameTemplate % endcap % sector % "sub_1_st_1" % "ch_1").str(), thCorrPairs);
    loadData("prim_conv_th_cor_me1a_02", (fileNameTemplate % endcap % sector % "sub_1_st_1" % "ch_2").str(), thCorrPairs);
    loadData("prim_conv_th_cor_me1a_03", (fileNameTemplate % endcap % sector % "sub_1_st_1" % "ch_3").str(), thCorrPairs);
    loadData("prim_conv_th_cor_me1b_01", (fileNameTemplate % endcap % sector % "sub_2_st_1" % "ch_1").str(), thCorrPairs);
    loadData("prim_conv_th_cor_me1b_02", (fileNameTemplate % endcap % sector % "sub_2_st_1" % "ch_2").str(), thCorrPairs);
    loadData("prim_conv_th_cor_me1b_03", (fileNameTemplate % endcap % sector % "sub_2_st_1" % "ch_3").str(), thCorrPairs);
    loadData("prim_conv_th_cor_me1n_03", (fileNameTemplate % endcap % sector % "sub_1_st_1" % "ch_13").str(), thCorrPairs);
}

bool PcLuts::loadData(string regName, string fileName, map<string, vector<unsigned long long>> & thLutPairs)
{
    bool result = true;
    vector<unsigned long long> values;

    ifstream file(fileName, ios::in);
    if(file.is_open())
    {
        string line;

        // read all the lines from the currently processing file
        while(getline(file, line)) // TODO: check for problems while you're reading the date
        {
            const int value = stoi(line, NULL, 16); // the function never fails so try to use boost instead

            values.push_back(value);
        }

        file.close();

        thLutPairs.insert(pair<string, vector<unsigned long long>> (regName, values));
    }
    else
    {
        result = false;
    }

    return result;
}


WritePcLuts::WritePcLuts(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<Mtf7Processor>()),
    pcLuts(NULL)
{
    pcLuts = new PcLuts(processor.endcap(), processor.sector());
}

swatch::core::Command::State WritePcLuts::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Write the PC LUTs to the board.");

    Command::State commandStatus = ActionSnapshot::kDone;

    auto thLutPairs = pcLuts->getThLutPairs();
    // write(processor, thLutPairs);

    auto thCorrPairs = pcLuts->getCorrPairs();
    // write(processor, thLutPairs);

    setProgress(1.);

    return commandStatus;
}

void WritePcLuts::write(Mtf7Processor &processor, map<string, vector<unsigned long long>> &pairs)
{
    for(auto it=pairs.begin(); it!=pairs.end(); ++it)
    {
        auto fileData = (*it).second;

        processor.writeBlock((*it).first, sizeof(unsigned long long)*fileData.size(), reinterpret_cast<char *>(fileData.data()));
    }
}


VerifyPcLuts::VerifyPcLuts(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<Mtf7Processor>()),
    pcLuts(NULL)
{
    pcLuts = new PcLuts(processor.endcap(), processor.sector());
}

swatch::core::Command::State VerifyPcLuts::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Verify the PC LUTs.");

    Command::State commandStatus = ActionSnapshot::kDone;

    auto thLutPairs = pcLuts->getThLutPairs();
    verify(processor, thLutPairs, commandStatus);

    auto thCorrPairs = pcLuts->getCorrPairs();
    verify(processor, thLutPairs, commandStatus);

    setProgress(1.);

    return commandStatus;
}

void VerifyPcLuts::verify(Mtf7Processor &processor, map<string, vector<unsigned long long>> &pairs, Command::State &status)
{
    for(auto it=pairs.begin(); it!=pairs.end(); ++it)
    {
        vector<unsigned long long> boardData((*it).second.size());
        processor.readBlock((*it).first, sizeof(unsigned long long)*boardData.size(), reinterpret_cast<char *>(boardData.data()));

        auto fileData = (*it).second;

        for(unsigned i=0; i<boardData.size(); ++i)
        {
            if(fileData[i] != boardData[i])
            {
                status = ActionSnapshot::kError;
            }
        }
    }
}


VerifyPcLutsVersion::VerifyPcLutsVersion(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("pc_lut_version", xdata::String("YYYY-MM-DD"));
}

VerifyPcLutsVersion::~VerifyPcLutsVersion()
{ }

swatch::core::Command::State VerifyPcLutsVersion::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Check the PC LUTs version.");

    Command::State commandStatus = ActionSnapshot::kDone;

    const string pcLutVersionDB = (params.get<xdata::String>("pc_lut_version").value_);

    string pcLutVersionFile("invalid PC Lut version");


    ifstream file((config::pcLutsPath() + "version"), ios::in);
    if(file.is_open())
    {
        getline(file, pcLutVersionFile); // TODO: check if the reading is successful

        file.close();
    }

    if(pcLutVersionFile != pcLutVersionDB)
    {
        commandStatus = ActionSnapshot::kError;
    }

    return commandStatus;
}

} // namespace


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

uint32_t* emtf::PtLut::getData(void) throw (std::runtime_error)
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

uint32_t* emtf::PtLut::getAddress(void) throw (std::runtime_error)
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

#include<stdexcept>

emtf::VerifyWritePtLut::VerifyWritePtLut(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<Mtf7Processor>()){}

using namespace log4cplus;

swatch::core::Command::State emtf::VerifyWritePtLut::code(const swatch::core::XParameterSet& params)
{
    if( verify() )
    {
        write();
        if( verify() ) 
            return ActionSnapshot::kError;
    }
    return ActionSnapshot::kDone;
}

void emtf::VerifyWritePtLut::write(void)
{
    setStatusMsg("Write the Pt LUT to the board.");

    setProgress(0.);

    uint32_t *data_buf = emtf::PtLut::getData();
    uint32_t *addr_buf = emtf::PtLut::getAddress();

    log("Setting write and read delay registers");

    init();
    setWriteDelays();
    setReadDelays();

    log("Writing blocks to the board");

    write_mrs(0x01010101, ODT_ON); // turn ODT on, only on one chip at the end of each quad

    ///clock_t start = clock();

    for (int i = 0; i < RL_BUFS; i++) {

        setProgress(i/float(RL_BUFS));

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

    write_mrs(0xffffffff, ODT_OFF); // turn ODT off
}


emtf::VerifyPtLutVersion::VerifyPtLutVersion(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("pt_lut_version", xdata::UnsignedInteger(1));
}

swatch::core::Command::State emtf::VerifyPtLutVersion::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Check the Pt LUT version.");

    Command::State commandStatus = ActionSnapshot::kDone;

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

    if(ptLutVersionFile != ptLutVersionDB)
    {
        commandStatus = ActionSnapshot::kError;
    }

    return commandStatus;
}

bool emtf::VerifyWritePtLut::verify(void)
{
    setStatusMsg("Check the Pt LUT on the board.");

    log("Allocating space for pT LUT in RAM");
    setProgress(0.);

    // reserve buffers for tests
    // each 32-bit word contains one 18-bit word for RLDRAM
    boost::shared_ptr<uint64_t[]> data_buf( new uint64_t [ FW_DATA_SIZE_B/sizeof(uint64_t) ] );
    boost::shared_ptr<uint32_t[]> addr_buf( new uint32_t [ FW_ADDR_SIZE_B/sizeof(uint32_t) ] );

    uint64_t *ref_buf = (uint64_t *)emtf::PtLut::getData();

    bzero(data_buf.get(), FW_DATA_SIZE_B);
    bzero(addr_buf.get(), FW_ADDR_SIZE_B);

//    log("Setting read delay registers");
//    setReadDelays();

    log("Reading blocks from the board");

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

        ///if( (block%100) == 0 ) log("Progress: ",block,"/0x400");

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
                    log( msg.str().c_str() );
                }
                err_count++;
                error = true;
            }
        }

    }

    return error;
}


void emtf::VerifyWritePtLut::log(const char *prefix, uint64_t val, const char *suffix)
{
    std::stringstream oss;
    oss << prefix << std::hex << val << std::dec << suffix;
    log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(config::log4cplusGeneralLogger()) );
    LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT(oss.str()));
}

void emtf::VerifyWritePtLut::log(const char *message)
{
    log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(config::log4cplusGeneralLogger()) );
    LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT(message));
}

// contents of MRTs, see RLDRAM3_registers.xlsx
#define MR0 0x10 // for 093E memory
#define MR1 0x400e0
#define MR2 0x80000 // normal operation

void emtf::VerifyWritePtLut::init(void)
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
        write_mrs(0xffffffff, mrs[i]);
        usleep (10000);
    }

    write_mrs(0xffffffff, ODT_OFF);

    uint64_t value = 0;
    processor.read64("ptlut_delay_ctl_locked", value);
    log("delay_ctrl lock status: 0x", ((value >> 22) & 0xf) );

    // reset IDELAY_CONTROL
    processor.write64("ptlut_dbdel_rst",0x1); // IO and IDELAY_CONTROL reset
    usleep(10000);

    // remove reset bit
    processor.write64("ptlut_dbdel_rst", 0x0);

    processor.read64("ptlut_delay_ctl_locked", value);
    log("delay_ctrl lock status: 0x", ((value >> 22) & 0xf) );

    // enable refresh, program RX clock domain crossing polarity
    processor.write64("ptlut_refresh_en",0x1);
    // program wait times after refresh and read cycles
    processor.write64("ptlut_rd_bank_timeout",0x1);
    processor.write64("ptlut_refresh_bank_timout",0x1);
    processor.write64("ptlut_core_rq_mask",0x7); // ptlut requests enable mask
}

void emtf::VerifyWritePtLut::write_mrs(uint32_t cs, uint32_t code)
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

void emtf::VerifyWritePtLut::setWriteDelays(void)
{
    const unsigned short wdel00[72] =
        { 10, 9, 8,10, 8, 9, 8, 8,10, 9, 9, 9,10, 9, 9, 9, 9,10,
          11,11,11,11,10,11,11,10,11,11,10,10,10, 9,10,10,10,10,
          8, 7, 8, 8, 9,10, 8, 6, 8, 9, 7, 9, 8,10,10, 9, 7, 9,
          8, 9, 9, 9, 9, 9, 9, 9, 8, 9,10,10, 9,10, 9,10, 9, 9 };

//    const unsigned short wdel01[72] = // one of the boards in b.904
//        {10,10, 8,11, 8, 9, 8, 8,10, 9, 9, 8,10, 8, 9, 8, 8, 9,
//         10, 9, 9, 9, 8,10,10, 9, 9,11,10,10,10,10,10,10,10,10,
//          9, 7, 8, 7, 8,10, 8, 6, 8, 9, 6, 9, 8, 9,10, 9, 7, 9,
//          8, 9, 8, 9, 9, 8, 9, 9, 8, 8, 9, 9, 8, 9, 8, 9, 9, 8};

    const unsigned short wdel01[72] =
        {  9, 9, 8,10, 8, 9, 8, 8, 9, 9, 9, 9,10, 9, 9, 8, 9,10,
          10,10, 9,10, 9,11,10,10,10,10,10, 9, 9, 9,10,10, 9,10,
           8, 7, 8, 7, 9, 9, 8, 6, 8, 9, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 9, 9, 9,10,10, 9,10,10, 9, 9, 9 };

    const unsigned short wdel02[72] =
        {  9, 9, 8,10, 8, 9, 8, 8, 9, 9, 9, 8,10, 9, 9, 8, 8, 9,
          10,10, 9,10, 8,10,10, 9, 9,10,10,10, 9, 9, 9,10, 9,10,
           8, 7, 8, 7, 8, 9, 8, 6, 8, 9, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 8, 9, 8, 9, 9, 9, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9 };

    const unsigned short wdel03[72] =
        {  9, 9, 8,10, 8, 9, 8, 8,10, 9, 9, 9,10, 9, 9, 9, 9,10,
          10, 9, 9,10, 9,10,10, 9,10,10,10,10,10, 9,10,10, 9,10,
           8, 7, 8, 8, 9,10, 8, 6, 8,10, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 9, 9, 9,10,10, 9,10, 9,10, 9, 9 };

    const unsigned short wdel04[72] =
        { 10,10, 9,11, 8, 9, 9, 9,10, 9,10, 9,10, 9, 9, 9, 9,10,
          12,11,11,11,10,12,11,11,11,11,10,10,10, 9,10,10, 9,10,
           9, 7, 8, 7, 9,10, 8, 6, 8, 9, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9,10, 8, 9, 9, 9, 9,10, 9,10,10, 9 };

    const unsigned short wdel05[72] =
        { 10, 9, 8,11, 8, 9, 8, 8,10,10, 9, 9,10, 9, 9, 9, 9,10,
          11,11,10,11,10,11,11,10,10,11,10,10,10,10,10,10, 9,10,
           9, 7, 8, 8, 9,10, 8, 6, 8,10, 7,10, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 8, 8, 9, 9, 9,10,10, 9,10, 9, 9 };

    const unsigned short wdel06[72] =
        {  9, 9, 8,10, 8, 9, 9, 8,10,10,10, 9,10, 9, 9, 9, 9,10,
          10,10, 9,10, 8,10,10, 9,10,10,10,10,10, 9,10,10, 9,10,
           9, 7, 8, 7, 9,10, 8, 7, 8,10, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 9, 9, 9,10,10,10,10, 9,10,10, 9 };

    const unsigned short wdel07[72] =
        {  9,10, 9,10, 8, 8, 8, 8,10, 9,10, 9,10, 9, 8, 8, 8, 9,
          10, 9, 9,10, 9,10,10, 9, 9,10,10,10,10, 9,10,10, 9,10,
           9, 7, 9, 8, 9,10, 8, 7, 9, 9, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 9, 8, 9,10,10, 9,10,10,10, 9, 9 };

    const unsigned short wdel08[72] =
        { 10, 9, 8,11, 8, 9, 8, 8,10,10, 9, 9,10, 9, 9, 9, 9,10,
          11,11,10,11, 9,11,11,10,10,10,10,10,10, 9,10,10, 9,10,
           9, 7, 8, 8, 9,10, 8, 7, 8,10, 7, 9, 8,10,10, 9, 7, 9,
           9, 9, 9, 9, 9, 9, 9, 9, 8, 9, 9, 9, 9,10, 9,10, 9, 9 };

    const unsigned short wdel09[72] =
        { 10, 9, 8,10, 8, 9, 9, 8,10, 9, 9, 8, 9, 9, 9, 9, 9,10,
          10,10,10,10, 9,11,11,10,10,10, 9, 9, 9, 9, 9,10, 9,10,
           9, 7, 8, 8, 9,10, 8, 6, 8, 9, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 9, 8, 9,10, 9,10, 9,10,10,10, 9 };

    const unsigned short wdel10[72] =
        { 10, 9, 8,10, 9, 9, 9, 8,10,10, 9, 9,10, 9, 9, 9, 9,10,
          11,11,10,11,10,11,11,10,11,10, 9,10,10, 9,10,10, 9,10,
           8, 7, 8, 7, 9,10, 8, 7, 8,10, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 9, 8, 9, 9, 9, 9,10,10,10, 9, 9 };

    const unsigned short wdel11[72] =
        { 10, 9, 8,10, 8, 9, 8, 9,10,10,10, 9,10,10, 9, 9, 9,10,
          11,11,11,11, 9,11,11, 9,10,11,10,10,10,10,10,11, 9,10,
           9, 7, 8, 8, 9,10, 8, 7, 8, 9, 7, 9, 8,10,10, 9, 7, 9,
           8, 9, 9, 9, 9, 9, 9, 9, 8, 9,10, 9, 9,10, 9,10, 9, 9 };

    const unsigned short *wdel = 0;
    switch( processor.deviceIndex() )
    {
        case  0: wdel = wdel00; break;
        case  1: wdel = wdel01; break;
        case  2: wdel = wdel02; break;
        case  3: wdel = wdel03; break;
        case  4: wdel = wdel04; break;
        case  5: wdel = wdel05; break;
        case  6: wdel = wdel06; break;
        case  7: wdel = wdel07; break;
        case  8: wdel = wdel08; break;
        case  9: wdel = wdel09; break;
        case 10: wdel = wdel10; break;
        case 11: wdel = wdel11; break;
        default:
        {
            std::ostringstream msg;
            msg << "Invalid processor device index: " << std::hex << processor.deviceIndex();
            throw std::runtime_error( msg.str() );
        }
    }

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

void emtf::VerifyWritePtLut::setReadDelays(void)
{
    const unsigned short rdel00[72] =
      { 12,13,14,13,14,12,12,13,13,13,14,14,14,14,13,12,14,14,
        11,11,10,11,11,11,11,11,11,12,11,11,12,11,11,12,12,12,
        11,10,11,10,11,11,11,11,10,12,11,11,11,12,11,11,12,11,
        11,11,11,12,11,11,12,11,11,13,12,12,12,12,11,12,11,11 };

//    const unsigned short rdel01[72] = // one of the boards in b.904
//      { 13,14,14,14,14,13,13,14,14,14,14,14,14,15,14,13,14,15,
//        12,12,12,13,12,12,11,12,11,12,12,13,13,12,12,12,13,12,
//        11,11,11,11,11,11,12,12,11,11,11,11,11,12,11,12,12,12,
//        12,11,11,11,12,10,11,11,11,12,11,11,11,12,10,11,11,11 };

    const unsigned short rdel01[72] =
      { 13,14,14,14,14,13,13,14,13,14,16,15,15,16,15,13,15,14,
        12,12,11,12,11,11,11,11,13,12,12,13,14,12,12,12,13,13,
        11,11,11,10,11,10,11,12,11,12,12,11,11,12,11,13,13,11,
        13,12,12,13,13,12,13,11,11,14,13,13,13,14,12,13,11,11 };

    const unsigned short rdel02[72] =
      { 14,15,15,15,16,15,14,15,15,16,16,16,17,17,15,15,17,16,
        12,13,12,14,12,12,12,14,12,13,13,14,15,13,13,14,15,14,
        11,11,11,11,11,11,12,13,12,12,13,11,11,12,12,13,13,12,
        13,12,12,13,13,12,13,11,11,15,14,13,14,14,13,14,12,12 };

    const unsigned short rdel03[72] =
      { 14,14,14,15,15,13,13,15,14,14,15,14,16,15,14,14,15,15,
        11,11,11,12,11,12,11,12,12,12,12,12,13,12,12,12,13,13,
        11,11,11,11,12,11,11,11,11,11,12,11,11,12,11,12,11,11,
        12,12,12,12,12,11,11,11,12,13,12,12,12,12,12,12,11,12 };

    const unsigned short rdel04[72] =
      { 13,14,14,14,14,13,13,14,13,15,16,15,15,15,14,14,15,15,
        11,11,11,12,11,11,11,11,11,12,12,12,13,12,12,12,13,12,
        11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,11,
        12,12,11,11,12,11,11,10,11,13,12,12,11,13,11,12,11,11 };

    const unsigned short rdel05[72] =
      { 13,14,13,13,14,12,12,13,13,15,16,14,15,15,13,13,14,14,
        11,11,11,12,11,11,10,12,11,11,12,12,12,12,12,12,13,13,
        11,11,11,11,11,11,11,11,11,12,11,11,11,12,11,12,12,12,
        12,11,11,11,12,11,11,11,11,13,12,11,12,12,12,12,11,11 };

    const unsigned short rdel06[72] =
      { 13,14,13,14,14,13,12,13,13,14,15,14,15,16,14,14,15,14,
        11,11,11,12,11,11,10,11,11,11,11,12,13,12,11,12,12,13,
        11,10,10,10,11,10,10,11,10,12,11,11,11,11,11,11,12,11,
        11,11,11,11,11,11,11,10,11,13,11,11,11,12,11,12,11,11 };

    const unsigned short rdel07[72] =
      { 11,13,13,12,13,11,12,12,11,12,14,14,13,14,12,13,13,13,
        11,11,11,12,11,11,11,11,11,11,11,11,12,11,11,11,11,11,
        11,10,10,10,10,10,11,10,10,11,11,10,11,11,11,12,11,11,
        12,12,11,12,11,11,12,10,10,13,12,12,12,12,11,12,10,10 };

    const unsigned short rdel08[72] =
      { 13,13,13,13,14,12,12,13,13,14,14,14,14,16,13,13,14,14,
        11,11,11,11,11,11,11,11,11,12,12,11,13,12,12,12,12,12,
        11,10,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,11,
        12,12,11,12,11,11,12,11,11,13,12,12,12,12,11,13,11,11 };

    const unsigned short rdel09[72] =
      { 11,13,13,13,13,11,12,13,12,14,15,14,14,15,13,13,14,15,
        11,12,10,11,11,12,11,12,12,12,11,11,12,12,12,11,12,12,
        11,11,11,11,11,10,10,11,11,11,12,11,11,12,11,11,12,11,
        11,11,11,11,11,11,11,11,11,13,11,12,12,12,11,12,11,11 };

    const unsigned short rdel10[72] =
      { 13,14,14,14,13,13,13,13,13,14,15,15,14,15,13,13,15,15,
        11,11,11,12,11,11,10,11,11,11,11,12,13,12,12,11,12,12,
        11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
        13,12,12,12,12,11,12,11,10,14,12,12,13,13,11,12,11,11 };

    const unsigned short rdel11[72] =
      { 13,14,14,14,14,13,13,13,13,14,15,14,15,15,14,14,15,14,
        11,12,12,12,12,12,12,12,12,12,12,14,14,12,12,13,14,14,
        11,11,11,11,12,11,11,12,11,12,12,12,12,12,12,12,12,12,
        11,11,11,11,11,10,11,11,11,13,12,12,12,12,11,12,11,12 };

    const unsigned short *rdel = 0;
    switch( processor.deviceIndex() )
    {
        case  0: rdel = rdel00; break;
        case  1: rdel = rdel01; break;
        case  2: rdel = rdel02; break;
        case  3: rdel = rdel03; break;
        case  4: rdel = rdel04; break;
        case  5: rdel = rdel05; break;
        case  6: rdel = rdel06; break;
        case  7: rdel = rdel07; break;
        case  8: rdel = rdel08; break;
        case  9: rdel = rdel09; break;
        case 10: rdel = rdel10; break;
        case 11: rdel = rdel11; break;
        default:
        {
            std::ostringstream msg;
            msg << "Invalid processor device index: " << std::hex << processor.deviceIndex();
            throw std::runtime_error( msg.str() );
        }
    }

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

