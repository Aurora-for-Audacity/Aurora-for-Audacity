/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  StiExports.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::STIShowExports
\brief Data exports manager

  This class manages all Acoustics Parameters exports to/from
  files and clipboard.

*//*******************************************************************/
#include <aurora.h>

#include <ModuleManager.h>
#include <PluginManager.h>

#include "sti.h"
#include "StiTrack.h"
#include "StiEffect.h"
#include "StiExports.h"

// ----------------------------------------------------------------------------
// Aurora::STISpectrumExport implementation
// ----------------------------------------------------------------------------
bool Aurora::STISpectrumExports::BuildParametersTable(wxString& dataBuffer, 
                                                      const int nDest, 
                                                      const int nFileType) 
{
    Aurora::Exports::Separator sep =
      (nFileType == Aurora::Exports::FileFormat::Csv ? Aurora::Exports::Separator::Comma
                                                     : Aurora::Exports::Separator::Tab);
    wxString separator = FieldsSeparator(sep);

    wxString lineBreak(FILES_LINE_BREAK);
    
    if(nDest == Aurora::Exports::DataDestination::Clipboard)
    {    
        lineBreak = "\n";
        separator = "\t";
    }

    dataBuffer = "Background Noise Left (dB)";
    dataBuffer += separator;
    dataBuffer += "Background Noise Right (dB)";
    dataBuffer += separator;

    dataBuffer += "Signal Level Left (dB)";
    dataBuffer += separator;
    dataBuffer += "Signal Level Right (dB)";
    dataBuffer += separator;

    dataBuffer += "Signal + Noise Level Left (dB)";
    dataBuffer += separator;
    dataBuffer += "Signal + Noise Level Right (dB)";
    dataBuffer += separator;

    dataBuffer += lineBreak;

    
    wxString value;
    auto& left  = m_pSti->GetResults(CH_LEFT);
    auto& right = m_pSti->GetResults(CH_RIGHT);
    
    left.noiseSpectrum.ForEach(false, [&](const size_t i, 
                                          const float fcb, 
                                          double& leftNoiseValue)
    {
        
        value.Printf("%f", leftNoiseValue);
        dataBuffer += value + separator;
        value.Printf("%f", right.noiseSpectrum[fcb]);
        dataBuffer += value + separator;

        value.Printf("%f", left.signalSpectrum[fcb]);
        dataBuffer += value + separator;
        value.Printf("%f", right.signalSpectrum[fcb]);
        dataBuffer += value + separator;
        
        value.Printf("%f", left.sigNoiseSpectrum[fcb]);
        dataBuffer += value + separator;
        value.Printf("%f", right.sigNoiseSpectrum[fcb]);
        dataBuffer += value + separator;

        dataBuffer += lineBreak;
    });
    return true;
}

bool Aurora::STISpectrumExports::LoadFromFile(wxTextFile& splDataFile)
{
    // TODO: missin' error check on reading
    
    double dbValue;
    wxString dataBuffer;
    dataBuffer = splDataFile.GetFirstLine();
    const auto& fcbs = m_pSti->GetResults(CH_LEFT).noiseSpectrum.RangedFrequencies();
    
    while(!splDataFile.Eof()) 
    {        
        size_t i = 0;

        if(dataBuffer.Contains("BACKGROUND NOISE LEFT"))
        {
            dataBuffer = splDataFile.GetNextLine();    
            
            for( ; i < fcbs.size() && ! splDataFile.Eof(); ++i)
            {
                dataBuffer.ToDouble(&dbValue);
                m_pSti->SetNoiseLevel(dbValue, fcbs[i], CH_LEFT, true);
                
                dataBuffer = splDataFile.GetNextLine();
            }
        }
        else if(dataBuffer.Contains("SIGNAL LEVEL LEFT"))
        {
            dataBuffer = splDataFile.GetNextLine();
            
            for( ; i < fcbs.size() && ! splDataFile.Eof(); ++i)
            {
                dataBuffer.ToDouble(&dbValue);
                m_pSti->SetSignalLevel(dbValue, fcbs[i], CH_LEFT, true);
                
                dataBuffer = splDataFile.GetNextLine();
            }
        }
        else if(dataBuffer.Contains("BACKGROUND NOISE RIGHT"))
        {
            dataBuffer = splDataFile.GetNextLine();
            
            for( ; i < fcbs.size() && ! splDataFile.Eof(); ++i)
            {
                dataBuffer.ToDouble(&dbValue);
                m_pSti->SetNoiseLevel(dbValue, fcbs[i], CH_RIGHT, true);
                
                dataBuffer = splDataFile.GetNextLine();
            }
        }
        else if(dataBuffer.Contains("SIGNAL LEVEL RIGHT"))
        {
            dataBuffer = splDataFile.GetNextLine();
            
            for( ; i < fcbs.size() && ! splDataFile.Eof(); ++i)
            {
                dataBuffer.ToDouble(&dbValue);
                m_pSti->SetSignalLevel(dbValue, fcbs[i], CH_RIGHT, true);
                
                dataBuffer = splDataFile.GetNextLine();
            }
        }
        else
        {
            dataBuffer = splDataFile.GetNextLine();
        }
    }
    return true;
}

bool Aurora::STISpectrumExports::SaveToFile(wxTextFile& splDataFile)
{
    const wxString noiseCaption[]  = { "---BACKGROUND NOISE LEFT (dB)--- ", 
                                       "---BACKGROUND NOISE RIGHT (dB)---" };
    const wxString signalCaption[] = { "---SIGNAL LEVEL LEFT (dB)---     ",
                                       "---SIGNAL LEVEL RIGHT (dB)---    " };
    wxString line;

    for(int nCh = 0; nCh < 2; nCh++)
    {
        auto& results  = m_pSti->GetResults(nCh);
        
        line = noiseCaption[nCh];
        splDataFile.AddLine(line, wxTextFileType_Dos);
        
        results.noiseSpectrum.ForEach(false, [&](const size_t i, 
                                                 const float fcb, 
                                                 double& noiseValue)
        {
            line.Printf("%.1f", noiseValue);
            splDataFile.AddLine(line, wxTextFileType_Dos);
        });
        line = signalCaption[nCh];
        splDataFile.AddLine(line, wxTextFileType_Dos);
  
        results.noiseSpectrum.ForEach(false, 
                                      [&](const size_t i,
                                          const float fcb, 
                                          double& signalValue)
        {
            line.Printf("%.1f", signalValue);
            splDataFile.AddLine(line, wxTextFileType_Dos);
        });
    }
    return splDataFile.Write(wxTextFileType_Dos);
}

bool Aurora::STISpectrumExports::OpenSavePrompt(const int nType)
{
    bool bIsOpen = (nType == SAVE_PROMPT) ? false : true;
    bool bRetVal = true;

    wxFileDialog dlg(nullptr,
                     bIsOpen ? "Open SPL file"
                             : "Save SPL values to file", 
                     "", 
                     m_fileName,
                     "Aurora SPL files (*.txt)|*.txt|All files|*.*",
                     (bIsOpen ? wxFD_OPEN | wxFD_FILE_MUST_EXIST 
                              : wxFD_SAVE | wxFD_OVERWRITE_PROMPT) );
   
    if(dlg.ShowModal() != wxID_OK)
    {
        return false;
    }

    m_fileName = dlg.GetPath();

    wxTextFile splDataFile(m_fileName);

    if(bIsOpen)
    {
        bRetVal = splDataFile.Open();
    }
    else
    {
        if(splDataFile.Exists())
        {
            if((bRetVal = splDataFile.Open()))
            {
                splDataFile.Clear();
            }
        }
        else
        {
            bRetVal = splDataFile.Create();
        }
    }

    if(!bRetVal)
    {
        Aurora::STIEffect::MessageBox(bIsOpen ? "Error on file access.\nCheck "
                                                 "permissions."
                                              : "Error on file access.\nCheck "
                                                "permissions/file existence.",
                                      Aurora::MessageType::Error);

        return false;
    }

    if(bIsOpen)
    {
        bRetVal = LoadFromFile(splDataFile);
    }
    else
    {
        bRetVal = SaveToFile(splDataFile);
    }

    if(!bRetVal)
    {
        Aurora::STIEffect::MessageBox(bIsOpen ? "Error opening file."
                                              : "Error writing file",
                                      Aurora::MessageType::Error);
    }
    else if(!bIsOpen)
    {
        Aurora::STIEffect::MessageBox("SPL data successifully saved.",
                                      Aurora::MessageType::Info);
    }
    return bRetVal;
}

Aurora::STISpectrumExports::STISpectrumExports(Aurora::STIEffect* pAs, 
                                               const bool bIsStereo)
  : m_pSti(pAs),
    m_bIsStereo(bIsStereo)
{
    m_fileName = "SPL.txt";
    assert(m_pSti != nullptr);
}


// ----------------------------------------------------------------------------
// Aurora::STIShowExport implementation
// ----------------------------------------------------------------------------
bool Aurora::STIShowExports::BuildParametersTable(wxString &dataBuffer, 
                                                  const int nDest, 
                                                  const int nFileType)
{
    const wxString chnlName[] = { "Left Channel STI",
                                  "Right Channel STI" };
                                        
    const wxString bands[] = { " 125", " 250", " 500", "1000",
                               "2000", "4000", "8000" };
                                    
    const wxString pars[]  = { "0.63 ", "0.8  ", "1    ", "1.25 ",
                               "1.6  ", "2    ", "2.5  ", "3.15 ",
                               "4    ", "5    ", "6.3  ", "8    ",
                               "10   ", "12.5 " };

    Aurora::Exports::Separator sep =
      (nFileType == Aurora::Exports::FileFormat::Csv ? Aurora::Exports::Separator::Comma
                                                     : Aurora::Exports::Separator::Tab);
    wxString separator = FieldsSeparator(sep);

    wxString lineBreak(FILES_LINE_BREAK);
    
    if(nDest == Aurora::Exports::DataDestination::Clipboard)
    {
        lineBreak = "\n";
        separator = "\t";
    }

    //Title generation
    dataBuffer << "Aurora " << wxAURORA_VERSION 
               << " - Speech Transmission Index calculation results"
               << lineBreak
               << lineBreak 
               << "MTF and STI"
               << lineBreak
               << lineBreak; 

    size_t nBand;
    wxString value;
    
    for(int nCh = 0; nCh < (m_bIsStereo ? 2 : 1); nCh++)
    {
        // nome canale
        if(m_bIsStereo) 
        {
            dataBuffer << chnlName[nCh] << lineBreak;
        }
        else             
        {
            dataBuffer << "Parameters" << lineBreak;
        }
        
        auto& results  = m_pSti->GetResults(nCh);
        
        // parametri canale
        // scrivo la prima riga, quella con le bande
        dataBuffer << "F [Hz]";
        
        for(nBand = 0; nBand < 7; nBand++) 
        {
            dataBuffer << separator << bands[nBand];
        }
        dataBuffer << lineBreak;
        
        // scrivo i parametri
        // parametri: nome + parametro banda per banda
        auto& modFrequencies = results.tMTFf.GetFrequencies();
        size_t n = 0;
        
        for(auto mf : modFrequencies)
        {
            dataBuffer << pars[n++];
            
            auto& mtfs = results.tMTFf.GetSpectrum(mf);

            mtfs.ForEach(false, 
                         [&](const size_t i, const float fcb, double& mtfValue)            
            {
                value.Printf("%.3f", mtfValue);
                dataBuffer << separator << value;
            });
            dataBuffer << lineBreak;
        }
        
        // Scrivo i valori di STI di Banda
        dataBuffer << lineBreak << "OCTAVE BAND STI values" << lineBreak;
        
        // scrivo la prima riga, quella con le bande
        dataBuffer << "F [Hz]";
        
        for(nBand = 0; nBand < 7; nBand++)
        {
            dataBuffer << separator << bands[nBand];
        }
        dataBuffer << lineBreak;
        
        dataBuffer << "MTI";
        
        results.aMTI.ForEach(false, 
                             [&](const size_t i, const float fcb, double& mtiValue)
        {
            dataBuffer << separator;
            //tmp=MTI[i2*2+Chan];           TODO: Check MTI versus STIValue
            value.Printf("%.3f", mtiValue);
            dataBuffer << value;
        });
        dataBuffer << lineBreak;

        // Scrivo i valori di STI Complessivo
        dataBuffer << lineBreak << "TOTAL STI values" << separator;

        dataBuffer << (m_pSti->IsSNCorrection() ? "Noise Corrected"
                                               : "No Noise Corr.");
        dataBuffer << separator;

        dataBuffer << (m_pSti->IsMaskingCorrection() ? "Mask Corrected"
                                                    : "No Mask Corr.");
        dataBuffer << lineBreak;

        value.Printf("%.3f", results.STImale);
        dataBuffer << "STI Male =" << separator << value << lineBreak;

        value.Printf("%.3f", results.STIfemale);
        dataBuffer << "STI Female =" << separator << value << lineBreak;

        value.Printf("%.3f", results.RaSTI);
        dataBuffer << "RaSTI =" << separator << value << lineBreak;

        value.Printf("%.3f", results.STItel);
        dataBuffer << "STItel =" << separator << value << lineBreak;

        value.Printf("%.3f", results.STIpa);
        dataBuffer << "STIpa =" << separator << value << lineBreak;

        dataBuffer << lineBreak;
    }
    return true;
}


bool Aurora::STIShowExports::SavePrompt()
{
    bool bRetVal = true;

    wxFileName fn(m_fileName);
    
    wxFileDialog dlg(nullptr, 
                     "Save results to file", 
                     "", 
                     fn.GetFullName(),
                      "Tab separated file (*.txt)|*.txt|Comma separated File (*.csv)|*.csv",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

    if(dlg.ShowModal() != wxID_OK)
    {
        return false;
    }

    fn = dlg.GetPath();
    const int nType = dlg.GetFilterIndex();
    auto fmt = (Aurora::Exports::FileFormat)dlg.GetFilterIndex();
    wxString ext = Extension(fmt);
    
    fn.SetExt(ext);

    dlg.SetPath(fn.GetFullPath());

    wxTextFile resultsFile(fn.GetFullPath());

    bRetVal = resultsFile.Exists() ? resultsFile.Open()
                                   : resultsFile.Create();

    if(bRetVal)
    {
        wxString dataBuffer;
        BuildParametersTable(dataBuffer, Aurora::Exports::DataDestination::File, nType);
        resultsFile.AddLine(dataBuffer,wxTextFileType_Dos);

        //flush data to file.
        if((bRetVal = resultsFile.Write(wxTextFileType_Dos)))
        {
            Aurora::STIEffect::MessageBox("Datas successifully saved.",
                                          Aurora::MessageType::Info);
        }
        else
        {
            Aurora::STIEffect::MessageBox("Write error.",
                                          Aurora::MessageType::Error);
        }
    }
    else
    {
        Aurora::STIEffect::MessageBox("Error on file creation.",
                                      Aurora::MessageType::Error);
    }
    return bRetVal;
}


Aurora::STIShowExports::STIShowExports(Aurora::STIEffect* pAs, 
                                       const bool bIsStereo)
  : m_pSti(pAs),
    m_bIsStereo(bIsStereo)
{
    m_fileName = "STI";
    assert(m_pSti);
}
