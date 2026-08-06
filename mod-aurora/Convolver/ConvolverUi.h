/**********************************************************************
 
 Aurora
 
 ConvolverUi.h
 
 UI for Aurora Convolver
 
 **********************************************************************/
#pragma once

#include "SilenceBase.h"
#include "effects/StatefulEffectUIServices.h"
#include <wx/wx.h>
#include <wx/valgen.h>
#include <Aurora/aurora.h>
#include "../AuroraPlot/AuroraPlot.h"

class BUILTIN_EFFECTS_API ConvolverUi final : public Generator, public StatefulEffectUIServices
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    ConvolverUi();
    virtual ~ConvolverUi();
    
    std::unique_ptr<EffectEditor> PopulateOrExchange(
                                                     ShuttleGui & S, EffectInstance &instance,
                                                     EffectSettingsAccess &access, const EffectOutputs *pOutputs) override;
    bool TransferDataToWindow(const EffectSettings &settings) override;
    bool TransferDataFromWindow(EffectSettings &settings) override;
    
    
    // ComponentInterface implementation
    ComponentInterfaceSymbol GetSymbol() const override;
    TranslatableString GetDescription() const override;
    ManualPageID ManualPage() const override;
    
    // EffectDefinitionInterface implementation
    EffectType GetType() const override;


protected:
   // Generator implementation

   bool GenerateTrack(const EffectSettings& settings, WaveTrack& tmp) override;

private:
    void OnToAudio(wxCommandEvent &event);
    void OnToFilter(wxCommandEvent &event);
    void OnU(wxCommandEvent &event);
    void OnR(wxCommandEvent &event);
    void OnD(wxCommandEvent &event);
    void OnL(wxCommandEvent &event);
    void OnRemove(wxCommandEvent &event);
    
    bool CloseUI() const override;

private:
    
    // UI Elememnts
    // Step 1:
    wxBitmap mAuroraLogo;
    wxBitmap mConvolverLogo;
    
    wxListBox* mTrackListCtrl;
    wxListBox* mAudioListCtrl;
    wxListBox* mFilterListCtrl;
    
    wxButton* mToAudioButton;
    wxButton* mToFilterButton;
    
    wxButton* mUButton;
    wxButton* mLButton;
    wxButton* mRButton;
    wxButton* mDButton;
    
    wxButton* mRemoveButton;
    wxCheckBox* mMatrixModeCheckBox;
    
    wxButton *mCloseButton;
        
    // Step 2:
    AuroraPlot *mPlot;
    
    wxCheckBox* mFullAutorangeCheckBox;
    wxCheckBox* mFirstBlockAutorangeCheckBox;
    wxTextCtrl* mGainCtrl;
    
    wxCheckBox* mTimeRevCheckBox;
    wxCheckBox* mCrossTalkCheckBox;
    wxCheckBox* mPreserveLengthCheckBox;

    // State Variables
    
    Aurora::ConvolverController mConvolver;
};
