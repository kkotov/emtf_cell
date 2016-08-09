#include "xdata/Integer.h"
#include "xdata/String.h"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7WriteVerifyLuts.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include "boost/lexical_cast.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

const string PcLuts::prefix("/home/emtfts/emtf_cell_config/vl_lut");

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

    boost::format fileNameTemplate(PcLuts::prefix + "vl_th_lut_endcap_%d_sec_%d_%s_ch_%u.lut");
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
    boost::format fileNameTemplate(PcLuts::prefix + "vl_th_corr_lut_endcap_%d_sec_%s_%s_%s.lut");

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
    pcLuts(NULL)
{ }

swatch::core::Command::State WritePcLuts::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Write the PC LUTs to the board.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    Command::State commandStatus = ActionSnapshot::kDone;

    const int endcap = processor.endcap();
    const int sector = processor.sector();

    pcLuts = new PcLuts(endcap, sector);

    auto thLutPairs = pcLuts->getThLutPairs();
    // verify(processor, thLutPairs, commandStatus);

    auto thCorrPairs = pcLuts->getCorrPairs();
    // verify(processor, thLutPairs, commandStatus);

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
    Command(aId, aActionable, xdata::Integer(0))
{ }

swatch::core::Command::State VerifyPcLuts::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Verify the PC LUTs.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    Command::State commandStatus = ActionSnapshot::kDone;

    const int endcap = processor.endcap();
    const int sector = processor.sector();

    pcLuts = new PcLuts(endcap, sector);

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


    ifstream file((PcLuts::prefix + "version"), ios::in);
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

