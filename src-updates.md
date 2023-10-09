# Audacity Source Code Updates

There have been a few changes to the Audacity source code since 2.4.1. Bleow is a list of the name and  behaviour changes which should hopefully aid the transition.



```cpp
bool ShowInterface(wxWindow& parent, const EffectDialogFactory& factory, bool forceModal = false) override;
```

now in `EffectUIServices`

```cpp
int ShowClientInterface(const EffectPlugin &plugin, wxWindow &parent, wxDialog &dialog, EffectEditor *pEditor, bool forceModal) const override;
```

***

```cpp
 PluginPath GetPath() override;
```

Add

```cpp
static const ComponentInterfaceSymbol Symbol;
```

***

```cpp
EffectBase::Init()
```

to 

```cpp
StatefulEffectBase::Init()
```

***

```cpp
enum Type
{
    RF_INT        = RulerBar::IntFormat,
    RF_REAL       = RulerBar::RealFormat,
    RF_REALLOG    = RulerBar::RealLogFormat,
    RF_TIME       = RulerBar::TimeFormat,
    RF_LINEARdB   = RulerBar::LinearDBFormat/*RF_CATEGORIES*/
};
```

are now classes.

### Plug-in registration


To register a plug-in you really just need to inclide this line in the `.cpp` of the plug-in class.

```cpp
namespace{ BuiltinEffectsModule::Registration< Aurora::SineSweepGeneratorEffect > reg; }
```

NOTE: This will likely have an impact on the "path" of the plug-in. At the moment if you take this approach the plug-in will simply be registered as "Built-in". This is a smal cosmetic problem until we have the rest of the source code working.

## UI

UI elements may need to us `wxDialogWrapper`

```
A debugging check in this application has failed.
/Users/<user>/Documents/GitHub/Aurora-for-Audacity/audacity/src/effects/EffectUI.cpp(270):assert    "mClosed"   failedin    ~EffectUlHost().
```

`mClosed` is found in `EffectUIHost` set in the `OnClose` method and asserted in the destructor `~EffectUIHost`

In the `ShowClientInterface` method the `wxDialog &dialog` is handled internally by Audacity. Substituting another `wxDialog` seems to be casuing this problem.

NOTE: Maybe check how the "Plot Spectrum" tool works

```cpp
class FrequencyPlotDialog final : public wxDialogWrapper,
                                  public PrefsListener
```

Look at `Change Pitch` effect for some cues on formatting

## Inheritance

### Generator

```py
Generator
|
StatefulEffect
├───────────────────────┬───────────────────────────────┐
StatefulEffectBase      Effect                          StatefulEffectUIServices
                        |                               ├───────────────────────┐
                        |                               wxEvtHandler            BasicEffectUIServices
                        |                                                       |
                        |                                                       EffectUIServices
                        |
                        EffectBase
                        |
                        EffectPlugin
                        |
                        EffectInstanceFactory
                        |
                        EffectSettingsManager
                        |
                        EffectDefinitionInterface
                        |
                        ComponentInterface

```
