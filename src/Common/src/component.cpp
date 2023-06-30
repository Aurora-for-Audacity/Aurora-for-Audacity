/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
					   plugin collection

  Common libraries

  component.cpp

  Simone Campanini

**********************************************************************/
#include <aurora.h>

#include <PluginManager.h>
#include <Track.h>


void Aurora::ProgressMeterWrapper::Show(const wxString& msg,
                                        const int range,
                                        const bool bCancellable)
{
    wxString caption = GetCaptionString();
    m_progInd.reset(new GTProgressIndicator(nullptr,
                                            caption,
                                            msg,
                                            bCancellable,
                                            1,
                                            false));
    SetRange(range, 0);
}

void Aurora::ProgressMeterWrapper::Show(const wxString& msg,
                                        const std::vector<int>& ranges,
                                        const bool bCancellable)
{
    wxString caption = GetCaptionString();
    assert(ranges.size() < 17);
    
    m_progInd.reset(new GTProgressIndicator(nullptr,
                                            caption,
                                            msg,
                                            bCancellable,
                                            (int)ranges.size(),
                                            false));
    
    for (int g = 0; g < (int)ranges.size(); ++g)
    {
        SetRange(ranges[g], g);
    }
}

bool Aurora::ProgressMeterWrapper::Update(const int pos, const int nGauge)
{
    assert (m_progInd);
    return m_progInd->SetProgress(pos, nGauge);
}

void Aurora::ProgressMeterWrapper::SetRange(const int range, const int nGauge)
{
    assert(m_progInd);
    m_progInd->SetRange(range, nGauge);
}

void Aurora::ProgressMeterWrapper::SetMessage(const wxString& msg)
{
    assert(m_progInd);
    m_progInd->SetMessage(msg);
}

void Aurora::ProgressMeterWrapper::Destroy()
{
    m_progInd.reset(nullptr);
}


PluginPath Aurora::Component::GetPath()
{
	return PluginPath("modules"); // PluginPath is a wxString
}

ComponentInterfaceSymbol Aurora::Component::GetSymbol()
{
	return ComponentInterfaceSymbol{ XO("AuroraComponent") };
}

VendorSymbol Aurora::Component::GetVendor()
{
	return VendorSymbol{ XO("Unipr") };
}

wxString Aurora::Component::GetVersion()
{
	return wxString(AURORA_VERSION_String);
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::Component::GetDescription()
{
	return _("Generic Aurora Component");
}
#else
TranslatableString Aurora::Component::GetDescription()
{
    return TranslatableString { XO("Generic Aurora Component") };
}
#endif

// ------------------------------------------------------------------------
// class Aurora::EffectBase implementation
// ------------------------------------------------------------------------
ComponentInterfaceSymbol Aurora::EffectBase::GetSymbol() 
{
	return ComponentInterfaceSymbol{ XO("AuroraEffect") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::EffectBase::GetDescription() 
{
	return _("Generic Aurora effect");
}
#else
TranslatableString Aurora::EffectBase::GetDescription()
{
    return TranslatableString { XO("Generic Aurora effect") };
}
#endif

PluginPath Aurora::EffectBase::GetPath()
{
	return PluginPath("Aurora/Effect");
}

// from EffectAmplify in Audacity source code...

// EffectDefinitionInterface implementation

EffectType Aurora::EffectBase::GetType()
{
	return EffectTypeAnalyze;
}

// EffectClientInterface implementation

unsigned Aurora::EffectBase::GetAudioInCount()
{
	return 1;
}

unsigned Aurora::EffectBase::GetAudioOutCount()
{
	return 1;
}

size_t Aurora::EffectBase::ProcessBlock(float **inBlock, float **outBlock, size_t blockLen)
{
	for (decltype(blockLen) i = 0; i < blockLen; i++)
	{
		outBlock[0][i] = inBlock[0][i];
	}

	return blockLen;
}

bool Aurora::EffectBase::DefineParams(ShuttleParams & S)
{
	return false;
}

bool Aurora::EffectBase::GetAutomationParameters(CommandParameters & parms)
{
	return false;
}

bool Aurora::EffectBase::SetAutomationParameters(CommandParameters & parms)
{
	return false;
}

bool Aurora::EffectBase::MessageBox(const wxString& msg,
                                   const Aurora::MessageType type)
{
    switch (type)
    {
        case Aurora::MessageType::Info:
            ::wxMessageBox(msg,
                           "Aurora for Audacity",
                           wxOK | wxICON_INFORMATION);
            break;
            
        case Aurora::MessageType::Warning:
            ::wxMessageBox(msg,
                           "Aurora for Audacity - Warning",
                           wxOK | wxICON_WARNING);
            break;
            
            case Aurora::MessageType::Error:
            ::wxMessageBox(msg,
                           "Aurora for Audacity - Error",
                           wxOK | wxICON_WARNING);
            break;
            
        case Aurora::MessageType::YesNo:
        {
            int ans = ::wxMessageBox(msg, "Aurora for Audacity",
                                     wxYES_NO | wxICON_INFORMATION);
            return (ans == wxYES);            
        }
    }
    return true;
}
