/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  exports.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_ITUP56_EXPORTS_H__
#define __AURORA_ITUP56_EXPORTS_H__

namespace Aurora
{
    class ThAnalyzerExports : public Aurora::Exports
    {
        enum FileType 
        { 
            kParametersTxt,
            kParametersCsv,
            kTimeHistoryTxt,
            kTimeHistoryCsv
        };

        TimeHistoryAnalyzer* m_pThan = nullptr;

        int m_nCurrentPlotType = 0;

        void StoreParametersTable(wxTextFile *fo, int file_type = FileFormat::Txt);
        void StoreTimeHistory    (wxTextFile *fo, int file_type = FileFormat::Txt);
        
      protected:
        bool BuildParametersTable(wxString &data_buffer,
                                  const int nDest     = DataDestination::File,
                                  const int file_type = FileFormat::Txt);
      public:
        bool SaveToFile(); 
        void SetCurrentPlotType(const int nType) { m_nCurrentPlotType = nType; }

        ThAnalyzerExports(TimeHistoryAnalyzer* than);
    };
} // namespace 

#endif // __AURORA_ITUP56_EXPORTS_H__
