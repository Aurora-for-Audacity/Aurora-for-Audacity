/**********************************************************************
 
 Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 plugin collection
 
 Common libraries
 
 audtypes.h
 
 Angelo Farina - Simone Campanini
 
 **********************************************************************/
#ifndef __AURORA_AUDTYPES_H__
#define __AURORA_AUDTYPES_H__

// This files works as wrap between Audacity types definitions
// and Aurora classes

#define _AF_USE_FLOAT_

// from Audacity
#include <SampleFormat.h>
#include <Sequence.h>

namespace Aurora
{
    typedef  float         Sample;
    typedef  sampleFormat  SampleFormat;
    typedef  samplePtr     SamplePtr;

    // now - Audacity 2.3.3 - sampleCount is a class!
    typedef  size_t        SampleCount;

    enum MessageType
    {
        Info,
        Warning,
        Error,
        YesNo
    };
}

#endif // __AURORA_AUDTYPES_H__
