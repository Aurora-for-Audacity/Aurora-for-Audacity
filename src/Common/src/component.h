/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
					   plugin collection

  Common libraries

  component.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_COMPONENT_H__
#define __AURORA_COMPONENT_H__

namespace Aurora
{
    /**
     * Class derived from this one, will encapsulate a nice multiple progress
     * meter (and all Aurora modules should do)
     */
    class ProgressMeterWrapper
    {
       protected:
        std::unique_ptr<GTProgressIndicator> m_progInd;
        
        virtual wxString GetCaptionString() const { return wxString("Aurora"); }
        
       public:
        void Show(const wxString& msg,
                  const int  range = 100,
                  const bool bCancellable = false);
        
        void Show(const wxString& msg,
                  const std::vector<int>& ranges,
                  const bool bCancellable = false);

        bool Update  (const int pos,   const int nGauge = 0);
        void SetRange(const int range, const int nGauge = 0);
        
        void SetMessage(const wxString& msg);

        void Destroy();

        ProgressMeterWrapper() { }
    };

    /**
     * Base class for every Audacity module
     */
    class Component : public ComponentInterface
    {
        public:
        virtual PluginPath GetPath() override;

        // this should be overridden (default: "AuroraComponent")
        virtual ComponentInterfaceSymbol GetSymbol() override;

        virtual VendorSymbol GetVendor() override;
        virtual wxString    GetVersion() override;

            // this should be overridden
#if AUDACITY_HEX_VERSION < 0x020400
        virtual wxString GetDescription() override;
#else
        virtual TranslatableString GetDescription() override;
#endif
    };

    // ------------------------------------------------------------------------
    // class AuroraEffect implementation
    // ------------------------------------------------------------------------
    class EffectBase : public Effect
    {
      public:
        // this should be overridden (default: "AuroraEffect")
        virtual ComponentInterfaceSymbol GetSymbol() override; // tbo
#if AUDACITY_HEX_VERSION < 0x020400
        virtual wxString GetDescription() override; // tbo
#else
        virtual TranslatableString GetDescription() override;
#endif
        virtual PluginPath GetPath() override; // tbo

        // EffectDefinitionInterface implementation

        // default: EffectTypeAnalyiyze  -> tbo if needed
        virtual EffectType GetType() override; 

        // EffectClientInterface implementation

        virtual unsigned GetAudioInCount () override;
        virtual unsigned GetAudioOutCount() override;
        virtual size_t ProcessBlock(float **inBlock, 
                                        float **outBlock, 
                                        size_t blockLen) override;
        virtual bool DefineParams(ShuttleParams & S) override;
        virtual bool GetAutomationParameters(CommandParameters & parms) override;
        virtual bool SetAutomationParameters(CommandParameters & parms) override;
        virtual bool LoadFactoryDefaults() override { return false;  }

        // Effect implementation

        virtual bool Init() override { return false; } // tbo
        virtual void Preview(bool dryOnly) override { }
        virtual void PopulateOrExchange(ShuttleGui & S) override { }
        virtual bool TransferDataToWindow() override { return false;  }
        virtual bool TransferDataFromWindow() override { return false; }
        
        // my own
        static bool MessageBox(const wxString& msg, const Aurora::MessageType type);
    };
} // namespace Aurora
#endif // __AURORA_COMPONENT_H__


