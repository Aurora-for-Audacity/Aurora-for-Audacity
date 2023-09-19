/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  acpar.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_ACPAR_H__
#define __AURORA_ACPAR_H__

class AcParameters : public AFAcousticalParameters
{
	static const wxChar* const m_awxpchParamLabels[];

  private:
     bool m_bAppendDataToFile;
     
     wxString m_wxszOutputFileName;
     wxString m_wxszIrFn;    //TODO: remove it
     wxString m_wxszFilteredTrackLabel;
     wxArrayString m_awxszTrackName;

     AFConfig* m_pCfg;
     
     // ** Methods
     
  protected:
     void SetFilteredTrackLabel(const int nBand);
     void ShowErrorMessage(const int nErrNo);
     
     void InitProgressMeter(const int nTotal);
     bool UpdateProgressMeter(const int nStep);
     void DestroyProgressMeter();
     
     void PreProcess() {}
     void PostProcess();
   
  public:
     // configuration read & write methods
     void SetConfigurationSource(AFConfig* pCfg) { m_pCfg = pCfg; }

     void ReadConfigurationValues();
     void StoreConfigurationValues();
     void ReadConfigurationGReferenceLevels();
     void StoreConfigurationGReferenceLevels();
     
     bool   GetAppendDataToFile()  const { return m_bAppendDataToFile; }
     bool   IsAppendDataToFile()   const { return m_bAppendDataToFile; }

     wxString& GetFilteredTrackLabel() { return m_wxszFilteredTrackLabel; }

     wxString GetOutputFileName()  const { return m_wxszOutputFileName; } 
     
     void AddTrackName(wxString wxszTn) { m_awxszTrackName.Add(wxszTn); }
     
     const wxChar* GetParamLabel(int nIdx) const { return m_awxpchParamLabels[nIdx]; }
 
     void SetAppendDataToFile(const bool bVal) { m_bAppendDataToFile = bVal; }
     void SetAppendFileName(wxString* pwxszFn)   { m_wxszOutputFileName = *pwxszFn; }

     int GetParameterFromString(wxString& wxszLabel);

  private:
#ifdef __WX_NATIVE_PROG_DLG__
     int m_nProgMax;
     wxProgressDialog* m_pProgressDlg;

  public:
#else
     ProgressDialog*   m_pProgressDlg;

  public:
     void SetProgressDialog(ProgressDialog* pProgDlg) { m_pProgressDlg = pProgDlg; }
#endif

//     AcParameters();
     AcParameters(TArrayOfAPAudioTracks* paAudioTracks, AFConfig* pCfg = 0);
     ~AcParameters(); 
       
};



#endif //__AURORA_ACPAR_H__

