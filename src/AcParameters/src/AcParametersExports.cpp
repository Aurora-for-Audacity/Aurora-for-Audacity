/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  exports.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class AcParametersExports
\brief Data exports manager

  This class manages all Acoustics Parameters exports to/from
  files and clipboard.

*//*******************************************************************/
#include <aurora.h>

#include "AcParametersEffect.h"
#include "AcParametersExports.h"

void Aurora::AcParametersExports::StoreParametersTable(wxTextFile& fo, 
                                                       const int nFileType)
{
    Aurora::Exports::Separator sep =
      (nFileType == Aurora::Exports::FileFormat::Csv ? Aurora::Exports::Separator::Comma
                                                     : Aurora::Exports::Separator::Tab);
    const wxString separator = FieldsSeparator(sep);

    const auto& fcbs = m_pAp->Results(0).Frequencies();

    wxString value, line, caption;

    //Title generation
    line << "Aurora " << wxAURORA_VERSION 
         << " - ISO3382 Acoustical Parameter File";
    fo.AddLine(line, wxTextFileType_Dos);
    line = "ISO 3382 ";
    line << (fcbs.size() > 10 ? " THIRD" : " ") << "OCTAVE BAND "
         << "ACOUSTICAL PARAMETERS\n";
    fo.AddLine(line, wxTextFileType_Dos);

    //Caption generation
    caption.Clear();
    caption << "Filename" << separator;
    caption << "Frq.band [Hz]";

    auto& tracks = m_pAp->Tracks();

    for(auto fcb : fcbs)
    {
        caption << separator << Aurora::GetBandShortLabel(fcb);
    }
    caption << separator << "A";
    caption << separator << "Lin";
    
    for(int nCh = 0; nCh < m_pAp->GetChannelsCount(); nCh++)
    {
        line.Clear();
        line << "Channel " << (nCh+1) << " Parameters\n";
        fo.AddLine(line,    wxTextFileType_Dos);  
        fo.AddLine(caption, wxTextFileType_Dos);  

        const auto& results = m_pAp->Results(nCh);
        
        for(auto& param : m_exportedParameters)
        {
            line = tracks[nCh].GetName();
            line << separator;
            line << wxString(param);

            if(param == "IACC")
            {
                switch(m_pAp->GetIACCSpan())
                {
                    case Aurora::AcousticalParameters::IACCSpan::Early: 
                        line << " (Early)"; 
                        break;
                        
                    case Aurora::AcousticalParameters::IACCSpan::Late:  
                        line << " (Late)";  
                        break;
                        
                    case Aurora::AcousticalParameters::IACCSpan::All:
                        line << " (All)";   
                        break;
                }
            }
            const auto& fcbs = results.Frequencies();

            for(auto fcb : fcbs)
            {
                const auto& tp = results.Get(param, fcb);     
             
                line << separator
                     << (tp.isValid ? wxString::Format("%.3f", tp.value) : "--");
            }            
            const auto& atp = results.Get(param, 
                                          Aurora::AcParametersSpectrum::Band::AweightedBin());
            line << separator
                 << (atp.isValid ? wxString::Format("%.3f", atp.value)  : "--");

            const auto& ltp = results.Get(param, 
                                          Aurora::AcParametersSpectrum::Band::LinearBin());
            line << separator
                 << (ltp.isValid ? wxString::Format("%7.3f", ltp.value) : "--");
            
            fo.AddLine(line, wxTextFileType_Dos); 
        }
        line.Printf("\nRTU = RT User (%4.1f dB, %4.1f dB)\n",
                    m_pAp->GetUserMinLevel(),
                    m_pAp->GetUserMaxLevel());

        fo.AddLine(line, wxTextFileType_Dos);
    }
}

void Aurora::AcParametersExports::StoreEchogram(wxTextFile& fo)
{
    wxString str, line, caption;

    Aurora::SampleCount i, j;

    auto& tracks = m_pAp->Tracks();

    //Header generation
    str.Clear();
    str << "Aurora " << wxAURORA_VERSION 
        << " - Energetic Impulse Response File";
    fo.AddLine(str, wxTextFileType_Dos); 
    str.Printf("N. Impulse Responses = %d", m_pAp->GetChannelsCount());
    fo.AddLine(str, wxTextFileType_Dos);
    str.Printf("Time resolution = 1 ms");
    fo.AddLine(str, wxTextFileType_Dos);

    std::vector<float> fcbs = m_pAp->Results(0).Frequencies();
    fcbs.push_back(Aurora::AcParametersSpectrum::Band::AweightedBin());
    fcbs.push_back(Aurora::AcParametersSpectrum::Band::LinearBin());
    
    std::vector< TSamplesVector > buffers(fcbs.size());
    
    int nCh = 0; 

    for (auto& track : tracks)
    {
        int nBd = 0;

        // Create 1 ms RMS Impulse Response         
        Aurora::SampleCount decimationStep = Aurora::SampleCount(track.GetRate() / 1000.0);
        Aurora::SampleCount rmsDataLength  = (track.GetLength() / decimationStep) + 1;

        const double scaleFactor = std::pow(10.0, track.GetFullScale() / 10.0)
                                   / track.GetRate();
        for (auto fcb : fcbs)
        {
            buffers[nBd].Resize(rmsDataLength);
            track.ApplyFilter(fcb);

            Aurora::Sample* data = track.Filtered().Samples();

            for(i = track.GetFirstArrivalTime(); 
                i < track.GetLength(); 
                i++)
            {
                j = Aurora::SampleCount( i / decimationStep );
                buffers[nBd][j] += data[i] * data[i];
            }

            // Rescale buffer values to obtain correct dB levels
            buffers[nBd] *= scaleFactor;
            nBd++;
        }

        str.Printf("Impulse Response n. = %d", nCh + 1);
        fo.AddLine(str, wxTextFileType_Dos); 
        str.Printf("N. of points=  %d", int(rmsDataLength));
        fo.AddLine(str, wxTextFileType_Dos); 
      
        for(i = 0; i < rmsDataLength; i++)
        {
            line.Clear();

            for(auto& buffer : buffers)
            {
                str.Printf("%e ", buffer[i]);
                line.Append(str);
            }
            fo.AddLine(line, wxTextFileType_Dos); 
        }
        nCh++;
    }
}

bool Aurora::AcParametersExports::BuildParametersTable(wxString &dataBuffer, 
                                                       const int nDest, 
                                                       const int nFileType)
{
    assert(m_exportedParameters.size() > 0);
    
   wxString str, line, caption;

   const auto& fcbs = m_pAp->Results(0).Frequencies();
   
    Aurora::Exports::Separator sep =
      (nFileType == Aurora::Exports::FileFormat::Csv ? Aurora::Exports::Separator::Comma
                                                     : Aurora::Exports::Separator::Tab);
    wxString separator = FieldsSeparator(sep);

   dataBuffer.Clear();
   
   //Title generation
   dataBuffer << "Aurora " << wxAURORA_VERSION 
              << " - ISO3382 Acoustical Parameter Table\n"
              << "ISO 3382" << (fcbs.size() > 10 ? " THIRD" : " ") 
              << " OCTAVE BAND ACOUSTICAL PARAMETERS\n\n";

   //Caption generation
   caption << "Filename"      << separator;
   caption << "Frq.band [Hz]" << separator;
   
    for(auto fcb : fcbs)
    {
        caption << Aurora::GetBandShortLabel(fcb) << separator;
    }

    for(int nCh = 0; nCh < m_pAp->GetChannelsCount(); nCh++)
    {
        line.Clear();   
        line << "Channel " << (nCh+1) << " Parameters\n\n";
        dataBuffer << line;
        dataBuffer << (caption + LINE_BREAK);

        auto& tracks = m_pAp->Tracks();

        for(auto& param : m_exportedParameters)
        {
            line = tracks[nCh].GetName();
            line << separator << wxString(param);

            const auto& results = m_pAp->Results(nCh);
            const auto& fcbs = results.Frequencies();
            
            for(auto fcb : fcbs)
            {
                const auto& tp = results.Get(param, fcb);     
             
                line << separator
                     << (tp.isValid ? wxString::Format("%.3f", tp.value) : "--");
            }
            
            const auto& atp = results.Get(param, 
                                          Aurora::AcParametersSpectrum::Band::AweightedBin());
            line << separator
                 << (atp.isValid ? wxString::Format("%.3f", atp.value)  : "--");

            const auto& ltp = results.Get(param, 
                                          Aurora::AcParametersSpectrum::Band::LinearBin());
            line << separator
                 << (ltp.isValid ? wxString::Format("%7.3f", ltp.value) : "--");
                 
            dataBuffer << (line + LINE_BREAK);

      }
      line.Printf("\nRTU = RT User (%.1f dB, %.1f dB)\n\n",
                  m_pAp->GetUserMinLevel(),
                  m_pAp->GetUserMaxLevel());
                  
      dataBuffer << line;
   }
   return true;
}


bool Aurora::AcParametersExports::SaveToFile()
{
    bool bRetVal = true;
    auto& tracks = m_pAp->Tracks();

    wxString name("AP_");
    name << tracks[CH_LEFT].GetName();
    wxFileName fn(name);
    fn.ClearExt();

    wxFileDialog dlg(nullptr,
                     "Save results to file",
                     "",
                     fn.GetFullName(),
                     "Parameter File (*.txt)|*.txt|"
                     "Parameter File (*.csv)|*.csv|"
                     "Ramsete Echogram (*.prn)|*.prn",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   
    if(dlg.ShowModal() != wxID_OK)
    {
        return false;
    }

    fn = dlg.GetPath();
    auto fmt = (Aurora::Exports::FileFormat)dlg.GetFilterIndex();
    wxString ext = Extension(fmt);
    
    fn.SetExt(ext);
    

    //dlg.SetPath(m_fileName);

    wxTextFile resultsFile;

    if(fn.FileExists())
    {
        if(!resultsFile.Open(fn.GetFullPath()))
        {
            Aurora::AcParametersEffect::MessageBox("Cannot open file for writing",
                                                   Aurora::MessageType::Error);
            return false;
        }
        resultsFile.Clear();
    }
    else
    {
        if(!resultsFile.Create(fn.GetFullPath()))
        {
            Aurora::AcParametersEffect::MessageBox("Cannot create file. Check "
                                                   "permissions.",
                                                   Aurora::MessageType::Error);
            return false;
        }
    }

    //file writing
    switch(fmt)
    {
        case Aurora::Exports::FileFormat::Txt:
                StoreParametersTable(resultsFile, fmt);
                break;
        case Aurora::Exports::FileFormat::Csv:
                StoreParametersTable(resultsFile, fmt);
                break;
        case Aurora::Exports::FileFormat::Prn:
                StoreEchogram(resultsFile);
                break;
    }

        //flush data to file.
    if( ! resultsFile.Write(wxTextFileType_Dos) )
    {
        Aurora::AcParametersEffect::MessageBox("Write error.",
                                               Aurora::MessageType::Error);
        bRetVal =  false;
    }
    else
    {
        Aurora::AcParametersEffect::MessageBox("Datas successifully saved.",
                                               Aurora::MessageType::Info);
    }
    return bRetVal;
}

void Aurora::AcParametersExports::AppendResultsToFile()
{
    assert(m_exportedParameters.size() > 0);
   
    wxString line, str;
    wxString caption = "Filename";

    for(auto& param : m_exportedParameters)
    {
        const auto& fcbs = m_pAp->Results(0).Frequencies();
        
        for(auto fcb : fcbs)
        {
            caption << "\t" << wxString(param) << "_" 
                    << Aurora::GetBandShortLabel(fcb);
        }
        caption << "\t" << wxString(param) << "_A"; 
        caption << "\t" << wxString(param) << "_Lin";        
    }

    wxTextFile tf;

    if(! tf.Open(m_appendToFileFn) )
    {
        if(! tf.Create(m_appendToFileFn))
        {
            ::wxMessageBox("Error on file creation.",
                           "Error",
                           wxOK | wxICON_ERROR);
            return;
        }
    }

    line = "Aurora ";
    line << wxAURORA_VERSION << " - ISO3382 Acoustical Parameter File\n";
    tf.AddLine(line, wxTextFileType_Dos);
    tf.AddLine(caption, wxTextFileType_Dos);

    auto& tracks = m_pAp->Tracks();

    for(int nCh = 0; nCh < int(tracks.size()); nCh++)
    {
        line = tracks[nCh].GetName();
        line << " Ch " << (nCh + 1);
        
        for(auto& param : m_exportedParameters)
        {
            const auto& results = m_pAp->Results(nCh);
            
            for(auto fcb : results.Frequencies())
            {
                const auto& tp = results.Get(param, fcb);                
                line << (tp.isValid ? wxString::Format("\t%7.3f", tp.value)
                                    : "\t--");
            }
            const auto& atp = results.Get(param, 
                                          Aurora::AcParametersSpectrum::Band::AweightedBin());
            line << (atp.isValid ? wxString::Format("\t%7.3f", atp.value)
                                 : "\t--");

            const auto& ltp = results.Get(param, 
                                          Aurora::AcParametersSpectrum::Band::LinearBin());
            line << (ltp.isValid ? wxString::Format("\t%7.3f", ltp.value)
                                 : "\t--");

        }
        tf.AddLine(line, wxTextFileType_Dos);

        if(! tf.Write(wxTextFileType_Dos))
        {
            Aurora::AcParametersEffect::MessageBox("Write error.",
                                                   Aurora::MessageType::Error);
            return;
        }
    }
    Aurora::AcParametersEffect::MessageBox("Data appended to file.",
                                           Aurora::MessageType::Info);
}

void Aurora::AcParametersExports::SetExportedParameters(const std::vector<std::string>& parametersList)
{
    m_exportedParameters.clear();
    m_exportedParameters = parametersList;
}

Aurora::AcParametersExports::AcParametersExports(AcousticalParameters *pAp)
  : Aurora::Exports(),
    m_pAp(pAp),
    m_appendToFileFn("AcousticalParameters.txt")
{
    assert(m_pAp);
    m_fileName = "AP_ir";
    SetExportedParameters(m_pAp->Results(0).Parameters());
}
