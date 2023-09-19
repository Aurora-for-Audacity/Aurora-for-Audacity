/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries

  afconfig.h

  Simone Campanini

************************************************************************/
#ifndef __AURORA_AFCONFIG_H__
#define __AURORA_AFCONFIG_H__

#define AUDACITY_VERSION_KEY wxT("/Audacity/Version")

namespace Aurora
{
    class Config : public wxConfig 
    {
      private:
         void WriteAudacityVersionNo(); 
        
         double   StringToDouble(wxString& wxszStr);
         Sample StringToSample(wxString& wxszStr);
         bool     StringToBool(wxString& wxszStr);
         SampleCount StringToSampleCount(wxString& wxszStr);
         
      public:
         SampleCount Read(const wxString& wxszKey, const SampleCount& smpcDefaultVal);
         double   Read(const wxString& wxszKey, const double& dbDefaultVal);     
         Sample Read(const wxString& wxszKey, const Sample& smpDefaultVal);
         bool     Read(const wxString& wxszKey, SampleCount* psmpcValue);
         bool     Read(const wxString& wxszKey, double* pdbValue);
         bool     Read(const wxString& wxszKey, Sample* psmpValue);
         bool     Read(const wxString& wxszKey, bool* pbValue);
         bool     Read(const wxString& wxszKey, int* pnValue);
         
         bool Write(const wxString& wxszKey, const SampleCount smpcValue);
         bool Write(const wxString& wxszKey, const double dbValue);
         bool Write(const wxString& wxszKey, const Sample smpValue);
         bool Write(const wxString& wxszKey, const bool bValue);
         bool Write(const wxString& wxszKey, const int nValue);
        
         bool SetAudacityVersionNo(wxString& wxszValue);
         const wxChar* GetAudacityVersionNo();
         
         Config();
    };
}
       

#endif // __AURORA_AFCONFIG_H__

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
