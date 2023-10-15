/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
					   plugin collection

  Common libraries

  component.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_COMPONENT_H__
#define __AURORA_COMPONENT_H__

#include <PluginManager.h>
#include <Track.h>

#include "GTProgressIndicator.h"

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
//    class Component : public ComponentInterface
//    {
//        public:
//        virtual PluginPath GetPath() ;
//
//        // this should be overridden (default: "AuroraComponent")
//        virtual ComponentInterfaceSymbol GetSymbol() ;
//
//        virtual VendorSymbol GetVendor() ;
//        virtual wxString    GetVersion() ;
//
//        virtual TranslatableString GetDescription() ;
//    };

    // ------------------------------------------------------------------------
    // class AuroraEffect implementation
    // ------------------------------------------------------------------------
//    class EffectBase : public Effect
//    {
//      public:
//        // this should be overridden (default: "AuroraEffect")
//        virtual ComponentInterfaceSymbol GetSymbol() ; // tbo
//        virtual TranslatableString GetDescription() ;
//        virtual PluginPath GetPath() ; // tbo
//
//        // EffectDefinitionInterface implementation
//
//        // default: EffectTypeAnalyiyze  -> tbo if needed
//        virtual EffectType GetType() ;
//
//        // EffectClientInterface implementation
//
//        virtual unsigned GetAudioInCount () ;
//        virtual unsigned GetAudioOutCount() ;
//        virtual size_t ProcessBlock(float **inBlock, 
//                                        float **outBlock, 
//                                        size_t blockLen) ;
//        //virtual bool DefineParams(ShuttleParams & S) ;
//        virtual bool GetAutomationParameters(CommandParameters & parms) ;
//        virtual bool SetAutomationParameters(CommandParameters & parms) ;
//        virtual bool LoadFactoryDefaults()  { return false;  }
//
//        // Effect implementation
//
//        virtual bool Init()  { return false; } // tbo
//        virtual void Preview(bool dryOnly)  { }
//        virtual void PopulateOrExchange(ShuttleGui & S) { }
//        virtual bool TransferDataToWindow()  { return false;  }
//        virtual bool TransferDataFromWindow()  { return false; }
//        
//    };
bool MessageBox(const wxString& msg, const Aurora::MessageType type);


} // namespace Aurora
#endif // __AURORA_COMPONENT_H__


