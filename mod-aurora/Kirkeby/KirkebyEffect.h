/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  KirkebyEffect.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_KIRKEBY_MODULE_H__
#define __AURORA_KIRKEBY_MODULE_H__

#include <iostream>
#include <aurora.h>
#include <effects/StatefulPerTrackEffect.h>
#include "KirkebyBase.h"

namespace Aurora
{
    class KirkebyEffect: public StatefulPerTrackEffect,
//public wxDialogWrapper,
                         public Aurora::KirkebyBase
    {

      private:
        sampleCount m_start    = 0;
        sampleCount m_end      = 0;
       
        bool m_bProcess  = false;
        bool m_bIsStereo = false;
       
        bool LoadTracks();
       
      protected:        
        void ReadConfigurationValues();
        void StoreConfigurationValues();

    public:
        bool Init() override;
        
//        bool ShowInterface(wxWindow& parent,
//                           const EffectDialogFactory& factory,
//                           bool forceModal = false) override;
        std::unique_ptr<EffectEditor> PopulateOrExchange(
           ShuttleGui & S, EffectInstance &instance,
           EffectSettingsAccess &access, const EffectOutputs *pOutputs) override;
        bool TransferDataToWindow(const EffectSettings &settings) override;
        bool TransferDataFromWindow(EffectSettings &settings) override;
      protected:
//        bool Process() override;

        ComponentInterfaceSymbol GetSymbol() const override;
        TranslatableString GetDescription() const override;
//        PluginPath GetPath() const override;
        EffectType GetType() const override;
     
      public:
        KirkebyEffect() { }
        
        int GetSelectionLength() const;
        
    private:
        wxWindow* m_parent = nullptr;
        std::unique_ptr<GTProgressIndicator> m_pProgressDlg;

    };
} // namespace Aurora

#endif // __AURORA_KIRKEBY_MODULE_H__

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

