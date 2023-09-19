/**********************************************************************
 *
 *  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 *                       plugin collection
 *
 *  Common libraries
 *
 *  afvector.cpp
 *
 *  Simone Campanini 2011
 *
 **********************************************************************/

#include <aurora.h>

// ---------------------------------------------------------------------------
// Aurora::ScalarVector Specialized methods
// ---------------------------------------------------------------------------
template<>
Aurora::SampleCount Aurora::ScalarVector<float>::GetAbsMaxPosition()
{
    return (fabsf(GetMax()) > fabsf(GetMin())) ? m_maxPosition
                                               : m_minPosition;
}

template<>
Aurora::SampleCount Aurora::ScalarVector<double>::GetAbsMaxPosition()
{
    return (fabs(GetMax()) > fabs(GetMin())) ? m_maxPosition
                                             : m_minPosition;
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
