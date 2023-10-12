/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  exports.h

  Angelo Farina - Simone Campanini

************************************************************************/
#ifndef __AURORA_ACPARAM_EXPORTS_H__
#define __AURORA_ACPARAM_EXPORTS_H__

namespace Aurora
{
    class AcParametersExports : public Aurora::Exports 
    {
      public:
/*        enum FileType
        { 
            FT_PARAMETER_TXT,
            FT_PARAMETER_CSV,
            FT_RAMSETE_ECHOGRAM_PRN 
        };*/

      private:
        AcousticalParameters *m_pAp = nullptr;
        wxString m_appendToFileFn;

        std::vector<std::string> m_exportedParameters;

        void StoreParametersTable(wxTextFile& fo, const int nFileType = FileFormat::Txt);
        void StoreEchogram(wxTextFile& fo);
        
      protected:
        bool BuildParametersTable(wxString &dataBuffer,
                                    const int nDest = DataDestination::File,
                                    const int nFileType = FileFormat::Txt);

      public:
        bool SaveToFile();
        void AppendResultsToFile();

        wxString& GetAppendToFileName() { return m_appendToFileFn; }
        
        void SetAppendToFileName(const wxString& name) { m_appendToFileFn = name; }
        void SetExportedParameters(const std::vector<std::string>& parametersList);
        
        AcParametersExports(AcousticalParameters* pAp);
    };
}       
       

#endif // __AURORA_ACPARAM_EXPORTS_H__
