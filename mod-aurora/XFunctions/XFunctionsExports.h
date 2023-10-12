/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  exports.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_XFUNCTIONS_EXPORTS_H__
#define __AURORA_XFUNCTIONS_EXPORTS_H__

#include <wx/txtstrm.h>

namespace Aurora
{
    class XFunctionsExports : public Aurora::Exports
    {
      private:
        wxString m_xfnLabel;

        bool   m_bExportAll = false;

        double m_fLo =     0.0;  // Lower  frequency bound
        double m_fHi = 24000.0;  // Higher frequency bound
        
        std::vector<XFunctionsPlot*> m_dataPlots;

        std::unique_ptr<wxTextOutputStream> m_outputStream;        
        std::unique_ptr<wxProgressDialog>   m_progressDlg;

        int      GetShownPlotType(const int nPlot);
        wxString GetDataLabel(const int type);
        double   GetDataValue(XFunctionsSpectralDataSerie& ds, 
                              const int unIdx, 
                              const double dbFrq, 
                              const double dbRate);
        
        protected:
        bool BuildParametersTable(wxString &data_buffer,
                                const int nDest     = DataDestination::File,
                                const int file_type = FileFormat::Txt);
        
        public:
        bool SaveToFile();
        bool AppendTimeDelay(const double dbTriggerTime, const double dbMaxDelay);
        
        bool IsExportAllSet() const { return m_bExportAll; }
        
        void SetExportAll(const bool bValue) { m_bExportAll = bValue; }

        void SetDataSources(XFunctionsPlot* upperPlot,
                            XFunctionsPlot* lowerPlot);

        void SetFrequencyBounds(const double fLo, const double fHi)  
        { 
            m_fLo = fLo;
            m_fHi = fHi;             
        }

        void SetFunctionLabel(wxString& label)  { m_xfnLabel = label;
                                                  m_fileName = "XFN_" + m_xfnLabel;}
        
        void InitProgressMeter  (const int nTotal);
        bool UpdateProgressMeter(const int step);
        void DestroyProgressMeter();
        
        XFunctionsExports() { }
        XFunctionsExports(XFunctionsPlot* upperPlot,
                          XFunctionsPlot* lowerPlot);

        XFunctionsExports(const wxString& xfnLabel = wxEmptyString);
    };
} // namespace 

#endif // __AURORA_FUNCTIONS_EXPORTS_H__

