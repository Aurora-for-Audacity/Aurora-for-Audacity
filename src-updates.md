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