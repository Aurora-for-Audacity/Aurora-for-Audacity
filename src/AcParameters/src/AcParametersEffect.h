/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  AcParametersEffect.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_ACPARAM_EFFECT_H__
#define __AURORA_ACPARAM_EFFECT_H__

#include "AcParametersBase.h"
#include "AcParametersTrack.h"
#include "AcParametersExports.h"

// Audacity forwards
class WaveTrack;

namespace Aurora
{
    //-----------------------------------------------------------------------------
    // AcParametersEffect
    //
    /// \brief The Acoustical Parameters module main class.
    //
    /// This class is the interface with Audacity, so it is
    /// derived from Effect and implements all its methods.
    /// Its core is the instantiation of a AcParameter object that does
    /// all the calculations.
    /// See \sa AFAcousticalParameters to know what exactly
    /// it computes.
    //-----------------------------------------------------------------------------
    class AcParametersEffect: public Aurora::EffectBase,
                              public Aurora::AcousticalParameters
    {

    private:
        /// Process function return value
        bool m_bAborted = false;
        bool m_bProcess = false;
        wxString m_fileToAppend; 

        /// Used to display calculation results in the End method
        wxWindow* m_parent = nullptr;

        /// Append filtered/processed audio data to Audacity workspace
        /// as new track(s).
        void AddProcessedTracks();

        /// Load audio data from Audacity workspace.
        void LoadTracks();

    public:
        /**
         * @brief Set a filename to append data when the show results window
         * is closed.
         */
        void SetFileToAppend(const wxString& fn) { m_fileToAppend = fn; }
        
        /**
         * @brief Return a filename where data will be appended when the show 
         * results window is closed.
         */
        const wxString& GetFileToAppend() const { return m_fileToAppend; }

        // ---------------- AcParameters methods ------------------------------
        //static const wxChar* const ms_paramLabels[];

  private:
     
        wxString m_outputFileName;
        wxString m_filteredTrackLabel;
        wxArrayString m_trackNames;
        
        // ** Methods
        
    protected:
        void SetFilteredTrackLabel(const int nBand);
        void ShowErrorMessage(const int nErrNo) override;
                
        void PreProcess() override {}
        void PostProcess() override;
    
    public:
        void ReadConfigurationValues();
        void StoreConfigurationValues();
        void ReadConfigurationGReferenceLevels(Aurora::Config& cfg);
        void StoreConfigurationGReferenceLevels(Aurora::Config& cfg);
        
        void UpdateFilteredTrack(const int channel, const wxString& fcbLabel);

        wxString& GetFilteredTrackLabel() { return m_filteredTrackLabel; }
        wxString GetOutputFileName()  const { return m_outputFileName; } 
        
        void AddTrackName(wxString tn) { m_trackNames.Add(tn); }     

        void SetAppendFileName(wxString* fn)   { m_outputFileName = *fn; }

    private:
    #ifdef __WX_NATIVE_PROG_DLG__
        int m_nProgMax;
        std::unique_ptr<wxProgressDialog> m_pProgressDlg;

    public:
    #else
        ProgressDialog*   m_pProgressDlg;

    public:
        void SetProgressDialog(ProgressDialog* pProgDlg) { m_pProgressDlg = pProgDlg; }
#endif
    
    
    // ---------------- Standard Audacity Effects' methods ----------------
    
        bool Init() override;
        
#if AUDACITY_HEX_VERSION < 0x020400
        bool PromptUser(wxWindow* parent) override;
#else
        bool ShowInterface(wxWindow& parent,
                           const EffectDialogFactory& factory,
                           bool forceModal = false) override;
#endif
    protected:
        bool Process() override;
        void End() override;

        ComponentInterfaceSymbol GetSymbol() override;
#if AUDACITY_HEX_VERSION < 0x020400
        wxString GetDescription() override;
#else
        TranslatableString GetDescription() override;
#endif
        PluginPath GetPath() override;
    
    public:
        AcParametersEffect() { }
    };

} // namespace Aurora

#endif // __AURORA_ACPARAM_EFFECT_H__

