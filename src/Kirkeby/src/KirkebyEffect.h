/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  KirkebyEffect.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_KIRKEBY_MODULE_H__
#define __AURORA_KIRKEBY_MODULE_H__

#include "KirkebyBase.h"

class WaveTrack;

namespace Aurora
{
    class KirkebyEffect: public Aurora::EffectBase,
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
        
#if AUDACITY_HEX_VERSION < 0x020400
        bool PromptUser(wxWindow* parent) override;
#else
        bool ShowInterface(wxWindow& parent,
                           const EffectDialogFactory& factory,
                           bool forceModal = false) override;
#endif
      protected:
        bool Process() override;

        ComponentInterfaceSymbol GetSymbol() override;
#if AUDACITY_HEX_VERSION < 0x020400
        wxString GetDescription() override;
#else
        TranslatableString GetDescription() override;
#endif
        PluginPath GetPath() override;
        EffectType GetType() override;
     
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

