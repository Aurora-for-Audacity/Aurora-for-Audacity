/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries

  afconfig.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::Config
\brief Configuration file manager class

  This class manages configuration files.

*//*******************************************************************/

#include <aurora.h>

double Aurora::Config::StringToDouble(wxString& s) 
{
   double dbVal = 0.0;
   if(s.ToDouble(&dbVal)) return dbVal;
   return 0.0;
}

Aurora::Sample Aurora::Config::StringToSample(wxString& s) 
{
   double dbVal = 0.0;
   if(s.ToDouble(&dbVal)) return Aurora::Sample(dbVal);
   return Aurora::Sample(0.0);
}

Aurora::SampleCount Aurora::Config::StringToSampleCount(wxString& s) 
{
   long lnVal = 0.0;
   if(s.ToLong(&lnVal)) return Aurora::SampleCount(lnVal);
   return Aurora::SampleCount(0);
}

bool Aurora::Config::StringToBool(wxString& s) 
{
   if(s.CmpNoCase(wxT("true")) == 0) return true;
   return false;
}

Aurora::SampleCount Aurora::Config::Read(const wxString& key, 
                                         const Aurora::SampleCount& defValue)
{
   wxString value;
   if(!wxConfig::Read(key, &value)) return defValue;
   return StringToSampleCount(value);
}

double Aurora::Config::Read(const wxString& key, const double& dbDefaultVal)
{
    wxString value;
    if(!wxConfig::Read(key, &value)) return dbDefaultVal;
    return StringToDouble(value);
}

Aurora::Sample Aurora::Config::Read(const wxString& key, 
                                    const Aurora::Sample& defValue)
{
    wxString value;
    if(!wxConfig::Read(key, &value)) return defValue;
    return StringToSample(value);
}

bool Aurora::Config::Read(const wxString& key, double* pvalue)
{
   wxString s;
   if(!wxConfig::Read(key, &s)) return false;
   *pvalue = StringToDouble(s);
   return true;
}

bool Aurora::Config::Read(const wxString& key, Aurora::SampleCount* value)
{
   wxString s;
   if(!wxConfig::Read(key, &s)) return false;
   *value = StringToSampleCount(s);
   return true;
}

bool Aurora::Config::Read(const wxString& key, Aurora::Sample* psmpValue)
{
   wxString s;
   if(!wxConfig::Read(key, &s)) return false;
   *psmpValue = StringToSample(s);
   return true;
}

bool Aurora::Config::Read(const wxString& key, bool* pbValue)
{
   wxString s;
   if(!wxConfig::Read(key, &s)) return false;
   *pbValue = StringToBool(s);
   return true;
}

bool Aurora::Config::Read(const wxString& key, int* pnValue)
{
   wxString s;
   if(!wxConfig::Read(key, &s)) return false;
   *pnValue = StringToInt(s);
   return true;
}

bool Aurora::Config::Write(const wxString& key, const Aurora::SampleCount value)
{
   wxString s;
   s << value; 
   return wxConfig::Write(key, s);
}

bool Aurora::Config::Write(const wxString& key, const double value)
{
   wxString s;
   s << value;
   return wxConfig::Write(key, s);
}

bool Aurora::Config::Write(const wxString& key, const Aurora::Sample smpValue)
{
   wxString s;
   s << smpValue;
   return wxConfig::Write(key, s);
}

bool Aurora::Config::Write(const wxString& key, const bool bValue)
{
   wxString s;
   if(bValue) s << wxT("true");
   else       s << wxT("false");
   return wxConfig::Write(key, s);
}

bool Aurora::Config::Write(const wxString& key, const int nValue)
{
    wxString s;
	s << nValue;
	return wxConfig::Write(key, s);
}

bool Aurora::Config::SetAudacityVersionNo(wxString& value)
{
   return wxConfig::Write(AUDACITY_VERSION_KEY, value); 
}

const wxChar* Aurora::Config::GetAudacityVersionNo()
{
    wxString s;
    if(!wxConfig::Read(AUDACITY_VERSION_KEY, &s))
    {
        s = wxT("1.3.14-alpha-Sep 22 2011");
        SetAudacityVersionNo(s);
    }
    return s.c_str();
}

Aurora::Config::Config()
  : wxConfig(wxAURORA_NAME)
{}

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

