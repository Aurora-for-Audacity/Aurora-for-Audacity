/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  XFunctionsExports.cpp

  Angelo Farina - Simone Campanini

**********************************************************************/
#include <aurora.h>

#include <wx/wfstream.h>

#include "XFunctionsEffect.h"
#include "XFunctionsData.h"
#include "XFunctionsPlot.h"
#include "XFunctionsExports.h"

int Aurora::XFunctionsExports::GetShownPlotType(const int nPlot)
{
   const size_t nds = m_dataPlots[nPlot]->GetDataSeriesCount();

   for (size_t n = 0; n < nds; ++n)
   {
       auto ds = m_dataPlots[nPlot]->GetDataSerie(n);
       auto sds = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds);

       if (sds && sds->IsShown())
       {
           return (int)n;
       }
   }
   return 0;
}

wxString Aurora::XFunctionsExports::GetDataLabel(const int nType)
{
   switch(nType)
   {
      case Aurora::XFunctionsPlotType::AutoCorrelationLeft:  return "Gll";
      case Aurora::XFunctionsPlotType::AutoCorrelationRight: return "Grr";
      case Aurora::XFunctionsPlotType::Magnitude:  return "Magnitude";
      case Aurora::XFunctionsPlotType::Phase:      return "Phase";
      case Aurora::XFunctionsPlotType::Real:       return "Real";
      case Aurora::XFunctionsPlotType::Imaginary:  return "Imaginary";
      case Aurora::XFunctionsPlotType::Alpha:      return "Alpha";
      case Aurora::XFunctionsPlotType::Coherence:  return "Coherence";
   }
   return wxT("");   
}
void Aurora::XFunctionsExports::SetDataSources(Aurora::XFunctionsPlot* upperPlot,
                                               Aurora::XFunctionsPlot* lowerPlot)
{
    m_dataPlots.clear();
    m_dataPlots.push_back(upperPlot);
    m_dataPlots.push_back(lowerPlot);
}

double Aurora::XFunctionsExports::GetDataValue(Aurora::XFunctionsSpectralDataSerie& ds, 
                                               const int unIdx, 
                                               const double frq, 
                                               const double rate)
{
    // TODO  Change values according to plot.cpp
    Aurora::Correlator& xf = ds.GetCorrelator();

    switch(ds.GetHandlerId())
    {
        case Aurora::XFunctionsPlotType::AutoCorrelationLeft: return xf.Gll().Real(unIdx);
        case Aurora::XFunctionsPlotType::AutoCorrelationRight: return xf.Grr().Real(unIdx);
        case Aurora::XFunctionsPlotType::Magnitude:  return Magnitude(ds.Y()[unIdx]);
        case Aurora::XFunctionsPlotType::Phase:      return DegPhase(ds.Y()[unIdx]);
        case Aurora::XFunctionsPlotType::Real:       return ds.Y().Real(unIdx);
        case Aurora::XFunctionsPlotType::Imaginary:  return ds.Y().Imag(unIdx);
        case Aurora::XFunctionsPlotType::Coherence:  return xf.C().Real(unIdx);
        case Aurora::XFunctionsPlotType::Alpha:
        {
             return (frq > 24.0 && frq < (rate / 5.12)) // k < unSpectrumLength/2.56
                     ? 2.0 * std::fabs(xf.Glr().Real(unIdx))
                      / ((xf.Gll().Real(unIdx) + xf.Grr().Real(unIdx)) / 2.0
                          + std::fabs(xf.Glr().Real(unIdx)))
                     : 0.0;
        }
    }
    return 0.0;
}

bool Aurora::XFunctionsExports::BuildParametersTable(wxString &dataBuffer, 
                                                     const int nDest, 
                                                     const int nFileType)
{
    auto ds = m_dataPlots[0]->GetDataSerie(0);
    auto sds = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds);
    const size_t spectrumLength = sds->Y().Length();

    const bool copyToClipboard = (nDest == Aurora::Exports::DataDestination::Clipboard);
    
    Aurora::Exports::Separator sep =
      (nFileType == Aurora::Exports::FileFormat::Csv ? Aurora::Exports::Separator::Comma
                                                     : Aurora::Exports::Separator::Tab);
    
   wxString separator = FieldsSeparator(sep);
   wxString lineBreak(FILES_LINE_BREAK);
   
   if(copyToClipboard)
   {
       if(spectrumLength > MAX_CLIPBOARD_ROWS)
       {
           return false;
       }
       
       lineBreak = "\n";
       separator = "\t";
       dataBuffer.Clear();
   }
   
   wxString line; //, caption, value;
   
   //Title generation
    line  = "Aurora - Cross Functions ";
    line << wxAURORA_VERSION;
    line << " - Spectral Values";
    line << lineBreak;

    if(copyToClipboard)
    {
        dataBuffer.Append(line);
    }
    else
    {
        
        m_outputStream->WriteString(line);
    }
    line = "Transform type: ";
    line << separator << m_xfnLabel << separator;
    line << lineBreak;
    
    if(copyToClipboard)
    {
        dataBuffer.Append(line);
    }
    else
    {
        m_outputStream->WriteString(line);
    }
    int nUpperPlotType = GetShownPlotType(0);
    int nLowerPlotType = GetShownPlotType(1);
   
    // Caption generation
    line  = "Freq. [Hz]";
    line << separator;

    if(m_bExportAll)
    {
        for(size_t n = 0; n < m_dataPlots[0]->GetDataSeriesCount(); ++n)
        {
            auto ds = m_dataPlots[0]->GetDataSerie(n);
            auto sds = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds);
            line << separator << GetDataLabel(sds->GetHandlerId());
        }
        line << lineBreak;
    }
    else 
    {
        line << GetDataLabel(nUpperPlotType) << separator; // upper plot
        line << GetDataLabel(nLowerPlotType) << lineBreak; // lower plot
    }
        
    if(copyToClipboard)
    {
        dataBuffer.Append(line);
    }
    else
    {
        m_outputStream->WriteString(line);
    }

    double frq;
    double frqSpan = m_fHi - m_fLo;    
    double rate = sds->GetRate();

    const size_t nBegin = size_t( m_fLo * spectrumLength/(rate/2.0) );
    const size_t nEnd   = size_t( m_fHi * spectrumLength/(rate/2.0) );
    
    InitProgressMeter((int)nEnd);

    if(m_bExportAll)
    {
        for(size_t k = nBegin; k < nEnd; k++)
        {
            frq = double(k * frqSpan / spectrumLength);
            line.Clear();
            line << frq; // frequency slot

            for(size_t n = 0; n < m_dataPlots[0]->GetDataSeriesCount(); ++n)
            {
                auto ds = m_dataPlots[0]->GetDataSerie(n);
                auto sds = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds);
                line << separator << GetDataValue(*sds, (int)k, frq, rate);
            }
            line <<  lineBreak;

            if(copyToClipboard)
            {
                dataBuffer.Append(line);
            }
            else
            {
                m_outputStream->WriteString(line);
            }

            if(!(k % 1000)) 
            {
                UpdateProgressMeter((int)k);
            }
        }
    }
    else
    {
        for(size_t k = nBegin; k < nEnd; k++)
        {
            frq = double(k * frqSpan / spectrumLength);
            line.Clear();
            line << frq << separator; // frequency slot

            auto ds0 = m_dataPlots[0]->GetDataSerie(nUpperPlotType);
            auto sds0 = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds0);

            line << GetDataValue(*sds0, (int)k, frq, rate) << separator;

            auto ds1 = m_dataPlots[1]->GetDataSerie(nLowerPlotType);
            auto sds1 = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds1);

            line << GetDataValue(*sds1, (int)k, frq, rate) << lineBreak;

            if(copyToClipboard)
            {
                dataBuffer.Append(line);
            }
            else
            {
                m_outputStream->WriteString(line);
            }

            if(!(k % 1000)) 
            {
                UpdateProgressMeter((int)k);
            }
        }
    }
   
   DestroyProgressMeter();
   
   return true;
}

bool Aurora::XFunctionsExports::AppendTimeDelay(const double triggerTime,
                                                const double maxDelay)
{
    wxString fn("timedelay.txt");
    wxFFileOutputStream timeDelayFile(fn, "a+b"); //append

    if( !timeDelayFile.IsOk() )
    {
        Aurora::XFunctionsEffect::MessageBox("Error on file creation.",
                                             Aurora::MessageType::Error);
        return false;
    }

    //file writing
    m_outputStream.reset(new wxTextOutputStream(timeDelayFile));

    // Write file
    if (triggerTime <= maxDelay)
    {
        m_outputStream->WriteString( wxNow() + " ");
        m_outputStream->WriteDouble(triggerTime * 1000.0);
        m_outputStream->WriteString(" ms, ");
    }
    else
    {
        m_outputStream->WriteString( wxNow() + " unavail.");
    }
    m_outputStream->WriteDouble(maxDelay * 1000.0);
    m_outputStream->WriteString(" ms.\n");

    timeDelayFile.Close();
    m_outputStream.reset(nullptr);

    return true;
}

bool Aurora::XFunctionsExports::SaveToFile() 
{   
   wxFileDialog dlg(nullptr,
                    "Save spectrum to file",
                    "",
                    m_fileName,
                    "Spectrum File tab-separated (*.txt)|*.txt|"
                    "Spectrum File comma-separated (*.csv)|*.csv",
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

   if(dlg.ShowModal() != wxID_OK)
   {
        return false;
   }

    m_fileName = dlg.GetPath();
    auto fmt = (Aurora::Exports::FileFormat)dlg.GetFilterIndex();
    wxString ext = Extension(fmt);
    
    if(m_fileName.Find(ext) == wxNOT_FOUND)
    {
        m_fileName << ext;
    }

    dlg.SetPath(m_fileName);

    wxFFileOutputStream resultsFile(m_fileName);
    
    if( !resultsFile.IsOk() )
    {
       Aurora::XFunctionsEffect::MessageBox("Error on file creation.",
                                            Aurora::MessageType::Error);
       return false;
    }

    //init text output stream
    wxString emptyString;
    m_outputStream.reset(new wxTextOutputStream(resultsFile));

    switch(fmt)
    {
        case Aurora::Exports::FileFormat::Txt:
            BuildParametersTable(emptyString,
                                 Aurora::Exports::DataDestination::File,
                                 Aurora::Exports::FileFormat::Txt);
            break;
            
        case Aurora::Exports::FileFormat::Csv:
            BuildParametersTable(emptyString,
                                 Aurora::Exports::DataDestination::File,
                                 Aurora::Exports::FileFormat::Txt);
            break;
            
        default:
            wxLogDebug("Unsupported format");
            assert(1);
    }
    
    Aurora::XFunctionsEffect::MessageBox("Datas successifully saved.",
                                         Aurora::MessageType::Info);

    m_outputStream.reset(nullptr);
    return true;
}

void Aurora::XFunctionsExports::InitProgressMeter(const int nTotal) 
{
   m_progressDlg.reset(new wxProgressDialog("Aurora Cross Functions",
                                            "Exporting data...",
                                             nTotal,
                                             nullptr,
                                             wxPD_AUTO_HIDE 
                                              | wxPD_APP_MODAL 
                                              | wxPD_CAN_ABORT
                                              | wxPD_ESTIMATED_TIME 
                                              | wxPD_REMAINING_TIME));
    
}

bool Aurora::XFunctionsExports::UpdateProgressMeter(const int step)
{  
   return m_progressDlg->Update(step);
}

void Aurora::XFunctionsExports::DestroyProgressMeter()
{
   m_progressDlg.reset(nullptr);
}

// ---- ctors
Aurora::XFunctionsExports::XFunctionsExports(XFunctionsPlot* upperPlot,
                                             XFunctionsPlot* lowerPlot)
{
    m_dataPlots.push_back(upperPlot);
    m_dataPlots.push_back(lowerPlot);
}

Aurora::XFunctionsExports::XFunctionsExports(const wxString& xfnLabel)
 : Aurora::Exports(),
   m_xfnLabel(xfnLabel)
{
   m_fileName = "XFN_" + m_xfnLabel;
}
