/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  exports.cpp

  Angelo Farina - Simone Campanini

**********************************************************************/
#include <aurora.h>

#include <widgets/ProgressDialog.h>

#include "ThAnalyzerEffect.h"
#include "ThAnalyzerExports.h"


void Aurora::ThAnalyzerExports::StoreParametersTable(wxTextFile *fo, 
                                                     int nFileType)
{
    wxString buffer;
    BuildParametersTable(buffer, nFileType);
    fo->AddLine(buffer, wxTextFileType_Dos);
}

void Aurora::ThAnalyzerExports::StoreTimeHistory(wxTextFile *fo, 
                                                 int nFileType)
{
    wxString value, line;

    double  dbTau        = 0.001; // 1 ms
    double dbCurrentTime = 0.0;

    int nCh;
    const int nChnls = (int)m_pThan->GetSignalTracks().size();

    std::vector<double> fullScales(nChnls, 0.0);

    for(nCh = 0; nCh < nChnls; nCh++)
    {
        fullScales[nCh] = m_pThan->GetSignalTrack(nCh).GetFullScale();
    }

    // inizializzo a zero il progress meter
    // ProgressDialog progress("Time History Analyzer",
    //                         "Saving Slow Time History...");

    auto& results = m_pThan->GetResults();
    
    Aurora::SampleCount i;
    Aurora::SampleCount length = results[TimeHistoryAnalyzer::TC_SLOW].GetBufferLength();

    Aurora::Exports::Separator sep =
      (nFileType == Aurora::Exports::FileFormat::Csv ? Aurora::Exports::Separator::Comma
                                                     : Aurora::Exports::Separator::Tab);
    const wxString separator = FieldsSeparator(sep);
    
    if(m_nCurrentPlotType == TimeHistoryAnalyzer::TC_SLOW)
    {
        // Save a sample every second, discarding others
        line  = "Time (s)";
        line << separator;
    
        for(nCh = 0; nCh < nChnls; nCh++)
        {
            //Header generation
            line << "Slow " << m_pThan->GetSignalTrack(nCh).GetName();
            line << " [" << (nCh+1) << "]";
            line << separator;
        }
        fo->AddLine(line, wxTextFileType_Dos);

        double sample = 0.0;
        
        for(i = 999; i < length; i += 1000)
        {
            line.Printf("%.6f", dbCurrentTime);
            
            for(nCh = 0; nCh < nChnls; nCh++)
            {
                sample =   results[nCh].GetBuffer(TimeHistoryAnalyzer::TC_SLOW)[i]
                         + fullScales[nCh];
                value.Printf("%.3f", sample);
                //float(m_pResults[nCh].m_aadbBuffers[TimeHistoryAnalyzer::TC_SLOW][i] + fullScales[nCh]));
                line << separator;
                line << value;
            }
            fo->AddLine(line, wxTextFileType_Dos);

            //      if((step % 100) == 0)   // to speedup not update at every step      
            //         if(progress.Update(step, nStep) <= 0) break;

            dbCurrentTime += 1.0; // 1 s
        }
    } 
    else 
    {
        // Save a sample every millisecond

        //Header generation
        line  = "Time (ms)" + separator;
        
        for(nCh = 0; nCh < nChnls; nCh++)
        {
            line << "RMS (1 ms) " << m_pThan->GetSignalTrack(nCh).GetName();
            line << " [" << (nCh+1) << "]";
            line << separator;
        }
        for(nCh = 0; nCh < nChnls; nCh++)
        {
            line << "ITU (30 ms) " << m_pThan->GetSignalTrack(nCh).GetName();
            line << " [" << (nCh+1) << "]";
            line << separator;
        }
        for(nCh = 0; nCh < nChnls; nCh++)
        {
            line << "Impulse " << m_pThan->GetSignalTrack(nCh).GetName();
            line << " [" << (nCh+1) << "]";
            line << separator;
        }
        for(nCh = 0; nCh < nChnls; nCh++)
        {
            line << "Fast " << m_pThan->GetSignalTrack(nCh).GetName();
            line << " [" << (nCh+1) << "]";
            line << separator;
        }
        for(nCh = 0; nCh < nChnls; nCh++)
        {
            line << "Slow " << m_pThan->GetSignalTrack(nCh).GetName();
            line << " [" << (nCh+1) << "]";
            line << separator;
        }
        fo->AddLine(line, wxTextFileType_Dos);
        double sample;

        for(i = 0; i < length; i++)
        {
            line.Printf("%.6f", dbCurrentTime);

            for(nCh = 0; nCh < nChnls; nCh++)
            {
                sample =   results[nCh].GetBuffer(TimeHistoryAnalyzer::TC_RMS)[i] 
                         + fullScales[nCh];
                value.Printf("%.3f", sample);
                line << separator;
                line << value;
            }
            
            for(nCh = 0; nCh < nChnls; nCh++)
            {
                sample =   results[nCh].GetBuffer(TimeHistoryAnalyzer::TC_ITU)[i] 
                         + fullScales[nCh];
                value.Printf("%.3f", sample);
                line << separator;
                line << value;
            }
            
            for(nCh = 0; nCh < nChnls; nCh++)
            {
                sample =   results[nCh].GetBuffer(TimeHistoryAnalyzer::TC_IMP)[i] 
                         + fullScales[nCh];
                value.Printf("%.3f", sample);
                line << separator;
                line << value;
            }
            
            for(nCh = 0; nCh < nChnls; nCh++)
            {
                sample =   results[nCh].GetBuffer(TimeHistoryAnalyzer::TC_FAST)[i] 
                         + fullScales[nCh];
                value.Printf("%.3f", sample);
                line << separator;
                line << value;
            }
            
            for(nCh = 0; nCh < nChnls; nCh++)
            {
                sample =   results[nCh].GetBuffer(TimeHistoryAnalyzer::TC_SLOW)[i] 
                         + fullScales[nCh];
                value.Printf("%.3f", sample);
                line << separator;
                line << value;
            }
            fo->AddLine(line, wxTextFileType_Dos);

             //ProgressMeter(ci,i,Nstep);
        //      if((step % 100) == 0)   // to speedup not update at every step      
        //         if(progress.Update(step, nStep) <= 0) break;
            dbCurrentTime += dbTau; // 1 s
        }
       // distruggo il progress meter, ho finito!
       //ProgressDestroy(ci);
    }

    printf("done.\n");
}


bool Aurora::ThAnalyzerExports::BuildParametersTable(wxString &buffer, 
                                                     const int nDest, 
                                                     const int nFileType)
{
    Aurora::Exports::Separator sep =
      (nFileType == Aurora::Exports::FileFormat::Csv ? Aurora::Exports::Separator::Comma
                                                     : Aurora::Exports::Separator::Tab);
    const wxString separator = FieldsSeparator(sep);

    int nParIdx = 0;
    wxString line, value;

    buffer.Clear();

    //Title generation
    line  = "Aurora - Time History Analyzer (v.";
    //   line << wxAURORA_VERSION;
    line << Aurora::ModuleVersionNumber(Aurora::ReleaseType::ART_BETA, 0) 
         << "";
    line << " - Results of ITU-T P56 Calculations";
    line << LINE_BREAK;

    buffer.Append(line);

    // Caption generation
    line  = "Channel";
    line << separator;
    line << "Leq (dB)" << separator;
    line << "SEL (dB)" << separator;
    line << "Total Duration (s)" << separator;
    line << "Active SpeenCh Level (dB)" << separator;
    line << "Threshold Level (dB)" << separator;
    line << "Activity Ratio (%)" << separator;
    line << "Max Peak SPL (dB)"  << separator;
    line << "Max Impulse SPL (dB)" << separator;
    line << "Max Fast SPL (dB)"  << separator;
    line << "Max Slow SPL (dB)"  << separator;
    line << "Event Duration (s)" << separator;
    line << "Impulsive Event"    << LINE_BREAK;
        
    buffer.Append(line);

    const int nChnlsCount = (int)m_pThan->GetSignalTracks().size();
    auto& results = m_pThan->GetResults();

    for(int nCh = 0; nCh < nChnlsCount; nCh++)
    {
        // Parameters
        //line = mThan->GetTrackName(nCh); // TODO Add track names
        line.Printf("Ch %d", nCh);
        line <<" [" << (nCh+1) << "]";
        line << separator;
        
        for(nParIdx  = TimeHistoryAnalyzer::THA_AVG; 
            nParIdx <= TimeHistoryAnalyzer::THA_DUR; nParIdx++)
        {
        
            value.Printf("%.2f", results[nCh].GetParameter(nParIdx));
            line << value << separator;
        }
        value =  (results[nCh].IsImpulsive()) ? "yes" : "no";
        line << value << LINE_BREAK;
        
        buffer.Append(line);
    }

    // Parameters
    line = "Average";
    line << separator;

    for(nParIdx  = TimeHistoryAnalyzer::THA_AVG; 
        nParIdx <= TimeHistoryAnalyzer::THA_DUR; nParIdx++)
    {
        value.Printf("%.2f", results[nChnlsCount].GetParameter(nParIdx));
        line << value << separator;
    }
    value = (results[nChnlsCount].IsImpulsive()) ?  "yes" : "no";
    line << value << LINE_BREAK;

    buffer.Append(line);

    return true;
}
 
bool Aurora::ThAnalyzerExports::SaveToFile() 
{
    wxFileDialog saveFileDialog(nullptr,
                                "Save results to file",
                                "",
                                m_fileName,
                                "Parameter File (*.txt)|*.txt|"
                                "Parameter File (*.csv)|*.csv|"
                                "Time History (*.txt)|*.txt|"
                                "Time History (*.csv)|*.csv",
                             wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(saveFileDialog.ShowModal() != wxID_OK)
    {
        return false;
    }

    m_fileName = saveFileDialog.GetPath();
    const int type = saveFileDialog.GetFilterIndex();

    auto fmt =
        (type == kParametersCsv || type == kTimeHistoryCsv) ? Aurora::Exports::FileFormat::Csv
                                                            : Aurora::Exports::FileFormat::Txt;
    
    m_fileName << Extension(fmt);
    saveFileDialog.SetPath(m_fileName);

    wxTextFile resultsFile;

    if( resultsFile.Create(m_fileName) || resultsFile.Open(m_fileName) )
    {
        //file writing
        resultsFile.Clear();

        switch(type)
        {
            case kParametersTxt:
               StoreParametersTable(&resultsFile, fmt);
               break;
            case kParametersCsv:
               StoreParametersTable(&resultsFile, fmt);
               break;
            case kTimeHistoryTxt:
               StoreTimeHistory(&resultsFile, fmt);
               break;
            case kTimeHistoryCsv:
               StoreTimeHistory(&resultsFile, fmt);
               break;
        }

        //flush data to file.
        if( ! resultsFile.Write(wxTextFileType_Dos))
        {
            Aurora::TimeHistoryAnalyzerEffect::MessageBox("Write error.",
                                                          Aurora::MessageType::Error);
            return false;
        }
        else
        {
            Aurora::TimeHistoryAnalyzerEffect::MessageBox("Datas successifully saved.",
                                                          Aurora::MessageType::Info);
        }
    }
    else
    {
        Aurora::TimeHistoryAnalyzerEffect::MessageBox("Error on file creation.",
                                                      Aurora::MessageType::Error);
        return false;
    }
    return true;
}

// ---- ctors
Aurora::ThAnalyzerExports::ThAnalyzerExports(Aurora::TimeHistoryAnalyzer* than)
 : Aurora::Exports(),
   m_pThan(than)
{
   m_fileName = "ITUP56";
}
