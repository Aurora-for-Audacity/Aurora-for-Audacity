/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  StiExports.h

  Angelo Farina - Simone Campanini

************************************************************************/
#ifndef __AURORA_STI_EXPORTS_H__
#define __AURORA_STI_EXPORTS_H__

#include <wx/textfile.h>

namespace Aurora
{
    // ----------------------------------------------------------------------------
    // STISpectrumExport declaration
    // ----------------------------------------------------------------------------
    class STISpectrumExports : public Aurora::Exports 
    { 
    public:
        enum 
        { 
            OPEN_PROMPT = 0, SAVE_PROMPT
        };

    private:
        Aurora::STI* m_pSti = nullptr;
        bool         m_bIsStereo = false;
            
    protected:
        bool BuildParametersTable(wxString& dataBuffer, 
                                  const int nDest = DataDestination::File,
                                  const int nFileType = FileFormat::Txt);
    
    private:
        bool LoadFromFile(wxTextFile& splDataFile);
        bool SaveToFile  (wxTextFile& splDataFile);

    public:
        bool OpenSavePrompt(const int nType = OPEN_PROMPT); 

        bool IsStereo() const { return m_bIsStereo; }
        void SetStereo(const bool bValue) { m_bIsStereo = bValue; }

        STISpectrumExports(Aurora::STIEffect* pAs, 
                           const bool bIsStereo = false);
    };

    // ----------------------------------------------------------------------------
    // STIShowExport declaration
    // ----------------------------------------------------------------------------
    class STIShowExports : public Aurora::Exports 
    {
        Aurora::STI* m_pSti = nullptr;
        bool       m_bIsStereo = false;

        wxString m_wxszIrName;

      protected:
        bool BuildParametersTable(wxString &dataBuffer, 
                                const int nDest = DataDestination::File, 
                                const int nFileType = FileFormat::Txt);

      public:
        bool SavePrompt();

        STIShowExports(Aurora::STIEffect* pAs, 
                       const bool nIsStereo = false);
    };

}
#endif // __AURORA_STI_EXPORTS_H__

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: c05d7383-e7cd-410e-b7b8-f45f47c9e283
