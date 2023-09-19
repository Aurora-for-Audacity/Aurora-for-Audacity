/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries

  afexports.h

  Angelo Farina - Simone Campanini

************************************************************************/
#ifndef __AURORA_AFEXPORTS_H__
#define __AURORA_AFEXPORTS_H__

#define MAX_CLIPBOARD_COLS 32767
#define MAX_CLIPBOARD_ROWS 32767

namespace Aurora
{
    class Exports 
    {
        public:
           enum DataDestination
           {
               File,
               Clipboard
           };
        
           enum FileFormat
           {
               Txt = 0,
               Csv,
               Prn
           };
        
           enum Separator
           {
               Tab = 0,
               Comma,
               Colon,
               Semicolon
           };
        
        protected:
           wxString m_fileName;
           int      m_nSeparatorType = Separator::Tab;

           static wxString Extension(const FileFormat fmt);
           static wxString FieldsSeparator(const Separator sep);
        
           virtual bool BuildParametersTable(wxString& dataBuffer,
                                             const int nDest = DataDestination::File,
                                             const int nFileType = FileFormat::Txt) = 0;
           
        public:
          virtual void CopyEntireTableToClipboard(const bool messageOnSuccess = false);
          virtual void CopyGridSelectionToClipboard(wxGrid* grd,
                                                    const bool messageOnSuccess = false);

//          void GetFileName(wxString* fn) { *fn = m_fileName; }
//          void SetFileName(wxString* fn) { m_fileName = *fn; }

    	  Exports();
          virtual ~Exports() {}
    };
}
       

#endif // __AURORA_AFEXPORTS_H__
