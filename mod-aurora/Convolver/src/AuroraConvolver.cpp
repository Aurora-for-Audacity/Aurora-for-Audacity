/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  auconv.cpp

  Simone Campanini

**********************************************************************/
#include <aurora.h>
#include "AuroraConvolver.h"

bool Aurora::ConvolverController::ResizeFilterTrack(const int idx,
                                                    const Aurora::SampleCount length)
{
    assert(FilterRows() > 0);
    
    if (mFilters.empty())
    {
        mFilters.resize(FilterRows());
    }
    mFilters[idx].Resize(length);
    return true;
}

bool Aurora::ConvolverController::ResizeInputTrack(const int idx,
                                         const Aurora::SampleCount length)
{
    assert(FilterColumns() > 0);
    mAfm->SetInputLength(length, idx);
    return true;
}

Aurora::SamplesVector& Aurora::ConvolverController::GetInputTrack(const int idx)
{ 
    return mAfm->GetInputVectorItem(idx);
}

Aurora::SamplesVector& Aurora::ConvolverController::GetOutputTrack(const int idx)
{ 
    return mAfm->GetOutputVectorItem(idx);
}

int Aurora::ConvolverController::FilterRows() const
{
    return mAfm->Rows();
}

int Aurora::ConvolverController::FilterColumns() const
{
    return mAfm->Columns();
}

bool Aurora::ConvolverController::CheckSamplerate(const double value)
{
   return (mRate != value) ? false : true;   // rate mismatch
}

Aurora::SampleCount Aurora::ConvolverController::GetFFTLength()
{
    Aurora::SampleCount l = 2;
    while(l  < mFilters[0].GetLength() ) l *= 2;
    return l * 2;
}

void Aurora::ConvolverController::SetFilterMatrixDimensions(const int nInputs,
                                                            const int nFilters)
{
    assert(nFilters > 0 && nInputs > 0);
    
    // Farina's logic assumes that 
    //  nOutputs = n. of rows of the filtering matrix
    //  nInputs  = n. of columns of the filtering matrix

    // if multitrack mode (normal) selected, the filtering matrix is
    // always square and diagonal and its size is the minimum between 
    // rows and cols.
    // If one for all is selected, the filters on the diagonal are all
    // equal.

    if (mMatrixMode)
    {
        assert(nInputs > 1);
        // Filter tracks must be splitted in nInputs chunks in order
        // to obtain the matrix column elements
        mAfm->Init(nFilters, nInputs, 0, 0);
    }
    else
    {
        int dim = (nInputs < nFilters ? nInputs : nFilters);
        
        if (nFilters == 1)
        {
            // nFilters == 1 is the "oneForAll" condition, otherwise it will do a
            // "one-to-one" convolution
            mUseThisFilterIndex = 0;
            dim = nInputs;
        }
        mAfm->Init(dim, 0, 0);
    }        
}
   
void Aurora::ConvolverController::SetMatrixMode(const bool value)
{ 
    mMatrixMode = value;
} 

void Aurora::ConvolverController::SetCancelCrossTalk(const bool value)
{ 
    mCancelCrossTalk = value; 
}

void Aurora::ConvolverController::SetFullAutorange(const bool value)
{ 
    if((mFullAutorange = value)) 
    {
        mFirstBlockAutorange = false; 
    }
}

void Aurora::ConvolverController::SetFirstBlockAutorange(const bool value)
{ 
    if((mFirstBlockAutorange = value)) 
    { 
        mFullAutorange = false; 
    }
}

void Aurora::ConvolverController::SetTimeReversedIr(const bool value)
{ 
    mTimeReversedIr = value; 
}

void Aurora::ConvolverController::SetPreserveLength(const bool value)
{ 
    mPreserveLength = value; 
}

// *** processing !!!

void Aurora::ConvolverController::DoMatrixConvolution()
{
   // Class setup
    mAfm->GetOptions().reverseFilter = mTimeReversedIr;
   
   if(IsFullAutorange())
   {
       mAfm->GetOptions().removeDC  = true;
       mAfm->GetOptions().autorange = true;
       mAfm->GetMatrixOptions().autorange = true;
   }
   else if(mGain != 0.0) 
   {
       mAfm->GetOptions().applyGain = true;
       mAfm->SetGainValue(mGain);
   }
   
   // Load vectors
   int row, col;
   
   for(row = 0; row < FilterRows(); row++)
   {
      for(col = 0; col < FilterColumns(); col++)
      {
          mAfm->SetFilterMatrixItem(mFilters[row], row, col);
      }
   }   
   mAfm->Process();
      
   mGain = double(mAfm->GetGainValue()); // for storing purposes...
}

void Aurora::ConvolverController::DoCancelCrossTalkConvolution()
{
   // Class setup
    mAfm->GetOptions().reverseFilter = mTimeReversedIr;
    mAfm->GetOptions().firstBlockAutorange       = true;
    mAfm->GetMatrixOptions().firstBlockAutorange = true;

   if(mFullAutorange)
   {
      mAfm->GetOptions().removeDC        = true;
      mAfm->GetOptions().autorange       = true;
      mAfm->GetMatrixOptions().autorange = true;
   } 
   else if(mGain != 0.0) 
   {
       mAfm->GetOptions().applyGain = true;
       mAfm->SetGainValue(mGain);
   }
   
   // Load vectors follows this layout:
   /*
      |hl hr|   |xl|
      |hr hl| * |xr| = |xlhl+xrhr xlhr+xrhl| = |yl yr|
   */
   
   mAfm->SetFilterMatrixItem(mFilters[CH_LEFT],  0, 0);
   mAfm->SetFilterMatrixItem(mFilters[CH_LEFT],  1, 1);
   mAfm->SetFilterMatrixItem(mFilters[CH_RIGHT], 0, 1);
   mAfm->SetFilterMatrixItem(mFilters[CH_RIGHT], 1, 0);

   mAfm->Process();
      
   mGain = double(mAfm->GetGainValue()); // for storing purposes...
}

void Aurora::ConvolverController::DoMultitrackConvolution()
{
   // Class setup
    mAfm->GetOptions().reverseFilter = mTimeReversedIr;
    mAfm->GetOptions().firstBlockAutorange       = mFirstBlockAutorange;
    mAfm->GetMatrixOptions().firstBlockAutorange = mFirstBlockAutorange;
   
   // This is the only case where preserve length has sense...
    mAfm->GetOptions().preserveOutputLength = mPreserveLength;
   
   if(mFullAutorange)
   {
      mAfm->GetOptions().removeDC  = true;
      mAfm->GetOptions().autorange = true;
      mAfm->GetMatrixOptions().autorange = true;
   }
   else 
   {
      mAfm->GetOptions().applyGain = true;
      mAfm->SetGainValue(mGain);
   }
   
   // It loads vectors following this layout:
   /*
                          |h1  0  0 ...  0|   |y1|
                          | 0 h2  0 ...  0|   |y2|
                          | 0  0 h3 ...  0|   |y3| 
      |x1 x2 x3 ... xn| * | .  .  .  .   .| = | .|
                          | .  .  .  .   .|   | .|
                          | .  .  .  .   .|   | .|
                          | 0  0  0 ... hn|   |yn|
   */
    mAfm->GetMatrixOptions().diagonalMode = true;
    
   for(int row = 0; row < FilterRows(); row++)
   {
       auto& filter = mFilters[mUseThisFilterIndex < 0 ? row
                                                       : mUseThisFilterIndex];
       mAfm->SetFilterMatrixItem(filter, row, row);
   }
   mAfm->Process();
   
   mGain = double(mAfm->GetGainValue()); // for storing purposes...
}

bool Aurora::ConvolverController::DoConvolution()
{
   if(mMatrixMode)
   {
      DoMatrixConvolution();
   } 
   else 
   {
       if (mCancelCrossTalk && (FilterRows() == 2) && (FilterColumns() == 2))
       {
           DoCancelCrossTalkConvolution();
       }
       else
       {
           DoMultitrackConvolution();
       }
   }

   return true; 
}
 
void Aurora::ConvolverController::ReadConfigurationValues()
{
   // Read parameter from stored configuration
    Aurora::Config cfg;
    
    double dbValue;

    if (cfg.Read("/Aurora/Convolver/Gain", &dbValue))
    {
       mGain = dbValue;
    }
    bool bValue;

    if (cfg.Read("/Aurora/Convolver/FullAutorange", &bValue))
    {
       SetFullAutorange(bValue);
    }

    if (cfg.Read("/Aurora/Convolver/FirstBlockAutorange", &bValue))
    {
       SetFirstBlockAutorange(bValue);
    }

    if (cfg.Read("/Aurora/Convolver/CancelCrossTalk", &bValue))
    {
       SetCancelCrossTalk(bValue);
    }

    if (cfg.Read("/Aurora/Convolver/TimeReversedIr", &bValue))
    {
       SetTimeReversedIr(bValue);
    }

    if (cfg.Read("/Aurora/Convolver/PreserveLength", &bValue))
    {
       SetPreserveLength(bValue);
    }
}

void Aurora::ConvolverController::StoreConfigurationValues()
{
    Aurora::Config cfg;
    
    cfg.Write("/Aurora/Convolver/Gain", mGain);
    cfg.Write("/Aurora/Convolver/FullAutorange", mFullAutorange);
    cfg.Write("/Aurora/Convolver/FirstBlockAutorange", mFirstBlockAutorange);
    cfg.Write("/Aurora/Convolver/CancelCrossTalk", mCancelCrossTalk);
    cfg.Write("/Aurora/Convolver/TimeReversedIr", mTimeReversedIr);
    cfg.Write("/Aurora/Convolver/PreserveLength", mPreserveLength);
}
   
void Aurora::ConvolverController::Reset()
{
    mAfm.reset(new Aurora::Multivolver);
    
   // read parameters values from stored configuration
   ReadConfigurationValues();
}

void Aurora::ConvolverController::Destroy()
{
    mRate                = 0.0;
    mMatrixMode          = false;
    mCancelCrossTalk     = false;
    mFullAutorange       = true;
    mFirstBlockAutorange = false;
    mTimeReversedIr      = false;
    mPreserveLength      = false;
    mGain                = 0.0; // dB
    mUseThisFilterIndex  = -1;
    
    mFilters.clear();
    mAfm.reset(nullptr);
}
