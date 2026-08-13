# ![Aurora](img/logos/aurora_logo_inv.png#gh-dark-mode-only) ![Aurora](img/logos/aurora_logo.png#gh-light-mode-only) for Audacity

![Acoustic Parameters](img/logos/ap_logo.png#gh-light-mode-only) ![Acoustic Parameters](img/logos/ap_logo_inv.png#gh-dark-mode-only) ![Convolver](img/logos/conv_logo.png#gh-light-mode-only) ![Convolver](img/logos/conv_logo_inv.png#gh-dark-mode-only) ![Kirkeby Filter Inversion](img/logos/kirk_logo.png#gh-light-mode-only) ![Kirkeby Filter Inversion](img/logos/kirk_logo_inv.png#gh-dark-mode-only) ![Sine Sweep Generator](img/logos/ssg_logo.png#gh-light-mode-only) ![Sine Sweep Generator](img/logos/ssg_logo_inv.png#gh-dark-mode-only) ![Speech Transmission Index](img/logos/sti_logo.png#gh-light-mode-only) ![Speech Transmission Index](img/logos/sti_logo_inv.png#gh-dark-mode-only) ![Time History Analyzer](img/logos/tha_logo.png#gh-light-mode-only) ![Time History Analyzer](img/logos/tha_logo_inv.png#gh-dark-mode-only) ![XFunctions](img/logos/xfn_logo.png#gh-light-mode-only) ![XFunctions](img/logos/xfn_logo_inv.png#gh-dark-mode-only)

***

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.16759727.svg)](https://doi.org/10.5281/zenodo.16759727)

Angelo Farina's Aurora for Audacity audio processing plugin suite.

## About

A module version of the the [Aurora plug-ins](https://www.aurora-plugins.com/) using [libAurora](https://github.com/mhamilt/libAurora) and the [Audacity DAW](https://github.com/audacity/audacity).

This project has been uploaded to github to rebring it life, but it needs some love.
It was compiled last time in the summer 2021 with Audacity 2.4.1 on Windows, macOs and linux platforms. 
It compiled but it worked properly only on windows.


## Plug-in Status

| Unit                      | Present | Building | Working Example |
| ------------------------- | :-----: | :------: | :-------------: |
| Acoustical Parameters     |   ✅    |    ✅    |       ✅        |
| Convolver                 |   ✅    |    ❌    |       ❌        |
| Kirkeby Filter Inversion  |   ❌    |    ❌    |       ❌        |
| SineSweepGenerator        |   ✅    |    ❌    |       ❌        |
| Speech Transmission Index |   ❌    |    ❌    |       ❌        |
| TimeHistoryAnalyzer       |   ❌    |    ❌    |       ❌        |
| XFunctions                |   ❌    |    ❌    |       ❌        |

## Note from Simone
 
> The Audacity code was patched because the module support was partially dropped, so some
> code was commented (but it was still there).
> 
> I did't do any cmake or similar because I don't have time to learn it. The very first
> releases (in 2009-2011) relies on scons, so the original scripts can be found in the
> code but I think that nowadays this is mere archeology. In 2021 I prepared a Visual Studio
> solution, a Xcode project and a linux Makefile.
> 
> The Aurora UI was designed using [`wxFormBuilder`](https://github.com/wxFormBuilder/wxFormBuilder) a very useful tool that saved me a lot of
> time. It can be found in its github repository. The directories 'gui-design' contain the
> wxFormBuilder fbp file projects.
> 
> But first of all, download Audacity source code and compile it!
> 
> Good luck.
> 
> Simone.

## Contributing

If you would like to contirbute then check out the [build-notes](https://github.com/Aurora-for-Audacity/Aurora-for-Audacity/blob/main/build-notes.md). This is just rough log of where we are at, some observations and potential places to look for updating the code. The [`src-update.md`](https://github.com/Aurora-for-Audacity/Aurora-for-Audacity/blob/main/src-updates.md) also contains some notes about how this Audacity source has changed and what may need updating and where to look.

Audacity is linked as a submodule, so you will need to clone this repo with:

```sh
git clone --recurse-submodules https://github.com/Aurora-for-Audacity/Aurora-for-Audacity.git
```

Please feel free to also check out the [issues]([./issues](https://github.com/Aurora-for-Audacity/Aurora-for-Audacity/issues)https://github.com/Aurora-for-Audacity/Aurora-for-Audacity/issues) to see what we are working on.



