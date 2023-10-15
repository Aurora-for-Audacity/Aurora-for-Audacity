/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  trprop.cpp

  Simone Campanini

*******************************************************************//**

\class AFTrackInfoItem
\brief Base track properties list item

*//****************************************************************//**

\class AFTrackProperties
\brief Class that holds track informations, useful as data shuttle

*//*******************************************************************/
#include <aurora.h>

#include "TrackProperties.h"

wxString Aurora::ConvolverTrackInfoItem::GetLabel() 
{
    wxString label = mName;
    // from Audacity's Track.h: LeftChannel = 0,  RightChannel = 1,  MonoChannel = 2
    if(mChnl == 0)        label << _(" [1]");
    else if (mChnl == 1)  label << _(" [2]");
    else label << _(" [") << mIndex << _("]");
    return label;
}

#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(ArrayOfConvolverTrackInfoItems);

int Aurora::ConvolverTrackProperties::CompareTrackInfoItems(Aurora::ConvolverTrackInfoItem* first, 
                                                            Aurora::ConvolverTrackInfoItem* second)
{
   if(first->GetRow() > second->GetRow()) return 1;
   else if (first->GetRow() > second->GetRow()) return -1;
   return 0;
}

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
