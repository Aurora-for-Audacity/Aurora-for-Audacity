/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  kirk.h

  Angelo Farina - Simone Campanini

***********************************************************************/
#ifndef __AURORA_KIRKEBY_KIRK_H__
#define __AURORA_KIRKEBY_KIRK_H__


#include <fftw3.h>
#include <commdefs.h>

// Hazardous vector macros...
#define VectorItem(ptr,idx,r,c,lrow, lcol) ptr[r*lrow + c*lcol + idx]
#define VectorRowPtr(ptr, r, lrow)         (ptr + r*lrow)

#define InItem(idx,r,c)   VectorItem(mIn,idx,r,c,mInRowLength,mInColLength)
#define InRow(r)          VectorRowPtr(mIn,r,mInRowLength)
#define OutItem(idx,r,c)  VectorItem(mOut,idx,r,c,mOutRowLength,mOutColLength)
#define OutRow(r)         VectorRowPtr(mOut,r,mOutRowLength)

class Kirkeby {

   private:
      double mRate;
      int    mRows; //alias channels...
      int    mCols;
      
      AFSample* mIn;
      AFSample* mOut;
      
      AFSampleCount mInColLength;
      AFSampleCount mInRowLength;
      AFSampleCount mOutColLength;
      AFSampleCount mOutRowLength;
      
    public:
      AFSample* GetInputTrack() const { return mIn; }
      AFSample* GetInputTrackItem(const int row, const int col) const { return In(row, col); }
      AFSampleCout GetInputTrackLength()  const { return mInputTrackLength; }
      AFSampleCout GetOutputTrackLength() const { return mOutputTrackLength; }
        
      void SetInputTrackLength(const AFSampleCount value) { mIrLength = value; }
      
      Kirkeby(const int chnls);
      Kirkeby(const int rows, const int cols);
};

#endif // __AURORA_KIRKEBY_KIRK_H__

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

