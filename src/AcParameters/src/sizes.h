/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  plot.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_ACPARAM_SIZES__
#define __AURORA_ACPARAM_SIZES__


//----------------------------------------------------------------------------
// AcParametersDialog enum & defines
//----------------------------------------------------------------------------

#ifdef __WXMAC__
// The following are sizes and positions for
//  MacOsX platform.
#define APD_BOX_X_SIZE        330
#define APD_dB_LBL_X_OFFSET    35
#define APD_dB_LBL_Y_OFFSET    26
#define APD_dB_LBL_X_SIZE      25
#define APD_dB_TEXT_X_SIZE     80
#define APD_CHECK_BOX_X_POS    42
#define APD_CHECK_BOX_X_SIZE  240
#define APD_TEXT_LBL_X_POS     10
#define APD_TEXT_LBL_X_SIZE   240
#define APD_TEXT_X_POS        250
#define APD_TEXT_X_SIZE        60
#define APD_RADIO_LBL_X_SIZE  250
#define APD_CHOICE_LBL_X_POS   40
#define APD_CHOICE_LBL_X_SIZE 120
#define APD_TEXT_DST_Y_POS     90
#define APD_TEXT_SPL_Y_POS    115
#define APD_CHOICE_X_POS      186
#define APD_CHOICE_X_SIZE     140
#define APD_FILE_TEXT_X_SIZE  248
#define APD_FILE_BTN_X_POS    265
#define APD_FILE_BTN_X_SIZE    30

#else
// These are sizes and positions for 
//  GTK & Windows platforms
#define APD_BOX_X_SIZE        300
#define APD_dB_LBL_X_OFFSET    20
#define APD_dB_LBL_Y_OFFSET    24
#define APD_dB_LBL_X_SIZE      25
#define APD_dB_TEXT_X_SIZE     80
#define APD_CHECK_BOX_X_POS    52
#define APD_CHECK_BOX_X_SIZE  230
#define APD_TEXT_LBL_X_POS     10
#define APD_TEXT_LBL_X_SIZE   220
#define APD_TEXT_DST_Y_POS     85
#define APD_TEXT_SPL_Y_POS    110
#define APD_TEXT_X_POS        230
#define APD_TEXT_X_SIZE        60
#define APD_CHOICE_X_SIZE     120
#define APD_RADIO_LBL_X_SIZE  230
#define APD_CHOICE_LBL_X_POS   40
#define APD_CHOICE_LBL_X_SIZE 100
#define APD_CHOICE_X_POS      166
#define APD_CHOICE_X_SIZE     120
#define APD_FILE_TEXT_X_SIZE  248
#define APD_FILE_BTN_X_POS    255
#define APD_FILE_BTN_X_SIZE    30

#endif


//----------------------------------------------------------------------------
// AcParametersShow enum & defines
//----------------------------------------------------------------------------

#ifdef __WXMAC__
// The following are sizes and positions for
//  MacOsX platform.
#define   SHOW_WINDOW_X_SIZE 760
#define   SHOW_WINDOW_Y_SIZE 630

#define APS_HC_BTN_X_SIZE      85
#define APS_HLP_BTN_X_POS      90
#define APS_PANEL_WDG_X_SIZE  190
#define APS_CH_RADIO_X_SIZE    80
#define APS_CH_RADIO_X_OFFSET  20
#define APS_USR_TEXT_X_SIZE   150
#define APS_TABLE_H_GAP        52
#define APS_RADIO_COL_X_POS   630

#else
// These are sizes and positions for 
//  GTK & Windows platforms
#define APS_HC_BTN_X_SIZE      70
#define APS_HLP_BTN_X_POS      80
#define APS_PANEL_WDG_X_SIZE  150
#define APS_CH_RADIO_X_SIZE    60
#define APS_CH_RADIO_X_OFFSET  15
#define APS_USR_TEXT_X_SIZE   120
#define APS_TABLE_H_GAP        50
#define APS_RADIO_COL_X_POS   600

#endif

#ifdef __WXGTK__
#define   SHOW_WINDOW_X_SIZE 720
#define   SHOW_WINDOW_Y_SIZE 610
#endif
#ifdef __WXMSW__
#define   SHOW_WINDOW_X_SIZE 720
#define   SHOW_WINDOW_Y_SIZE 630
#endif


//----------------------------------------------------------------------------
// Decay Plot window defines
//----------------------------------------------------------------------------
#define   DECAY_PLOT_X_SIZE 540
#define   DECAY_PLOT_Y_SIZE 250

#ifdef __WXMAC__
// MacOsX need bigger fonts for rulers
#define DECAY_RULER_FONT_SIZE 8
#else
#define DECAY_RULER_FONT_SIZE 6
#endif





#endif //__AURORA_ACPARAM_SIZES__

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

