/****************************************************************
 *                                                              *
 *                 Multiple ConvolverBase class                 *
 *                                                              *
 * This class performs N by N matrix filtering using            *
 * partitioned convolution engine.                              *
 *                                                              *
 * ------------------------------------------------------------ *
 *                                                              *
 * by Simone Campanini 2009, 2020                               *
 *                                                              *
 * ------------------------------------------------------------ *
 *                                                              *
 * multivolver.cpp                                              *
 *                                                              *
 ***************************************************************/
#include <aurora.h>
#include "multivolver.h"

// -------------------------------------------
// Aurora::Multivolver implementation
// -------------------------------------------

void Aurora::Multivolver::ReverseFilter()
{
    m_filterMatrix.Reverse();
}

void Aurora::Multivolver::FindFilterPeakPosition()
{
    // This makes really sense only for 1 column filter matrixes....
    for (int row = 0; row < m_nRows; row++)
    {
        SamplesVector&  cell = m_filterMatrix.Cell(row, 0);
        
        if (! cell.IsNull())
        {
            m_filterPeakPositions[row] = FindPeakPosition(cell);
        }
    }
}

void Aurora::Multivolver::PreserveOutputLength()
{
    int row = 0;

    for(auto out = m_outputs.begin(); out != m_outputs.end(); ++out, ++row)
    {
        PreserveLength(*out,
                       m_filterMatrix.Cell(row, 0).Length(),
                       m_filterPeakPositions[row]);
    }
}

void Aurora::Multivolver::SetInputLength(const SampleCount length,
                                         const int idx)
{
    assert(length > 0 && m_nCols > 0);
    
    if (idx < 0)
    {
        m_inputs.clear();
    }

    if (m_inputs.empty())
    {
        m_inputs.resize(m_nCols);
    }
    m_inputs[idx].Resize(length);    
}

void Aurora::Multivolver::SetInputVectorItem(const Aurora::SamplesVector& v,
                                             const int index)
{
    m_inputs[index].Copy(v);
}

void Aurora::Multivolver::SetFilterMatrixItem(Aurora::SamplesVector& v,
                                              const int row, 
                                              const int col)
{
    assert(! v.IsNull());

    if (m_filterMatrix.IsEmpty())
    {
        m_filterMatrix.Destroy(); // not needed, but..
        m_filterMatrix.Resize(m_nRows, m_nCols, v.Length());
    }
    m_filterMatrix.SetCellItem(row, col, v);
}

Aurora::SamplesVector& Aurora::Multivolver::GetInputVectorItem(const int index)
{
    return m_inputs[index];
}   

Aurora::SamplesVector& Aurora::Multivolver::GetOutputVectorItem(const int index)
{
    return m_outputs[index];
}   

const Aurora::SamplesVector& Aurora::Multivolver::GetInputVectorItem(const int index) const
{
    return m_inputs[index];
}   

const Aurora::SamplesVector& Aurora::Multivolver::GetOutputVectorItem(const int index) const
{
    return m_outputs[index];
}   

void Aurora::Multivolver::GetOutputVectorsInterlaced(Aurora::Sample *p) const
{
    // TODO: unsafe method....
    for (Aurora::SampleCount i = 0; i < m_outputs[0].Length(); i++)
    {
        for (int row = 0; row < m_nRows; row++)
        {
            p[m_nRows*i + row] = m_outputs[row].Item(i);
        }
    }
}

void Aurora::Multivolver::PreProcess()
{
    if (m_options.reverseFilter)
    {
        ReverseFilter();
    }
    else if (m_options.preserveOutputLength)
    {
        FindFilterPeakPosition();
    }
}


bool Aurora::Multivolver::Process()
{
    PreProcess();

    m_outputs.resize(m_nRows);

    // init progress meter
    int counter = 0;

    // Start processing.
    
    if (m_matrixOptions.diagonalMode)
    {
        std::vector<int> ranges = { m_nRows, 100 };
        ProgressMeterWrapper::Show("Computing convolution...", ranges, true);
        int row = 0;
        
        for (auto in = m_inputs.begin(); in != m_inputs.end(); ++in, ++row)
        {
            if (in->IsNull())
            {
                continue;
            }
            // if a m_psmpFilterMatrix item is null, the convolution result
            // will be null, so it doesn't make sense to do any sort of
            // calculation...
            Aurora::SamplesVector& ht = m_filterMatrix.Cell(row, row);
            
            if(! ht.IsNull())
            {
                SetFilter(ht);  // this FIRST!
                SetVector(*in); // ...then this.
                               
                m_outputs[row].Resize(GetResultVector().Length());

                if (! Convolve())
                {
                    ProgressMeterWrapper::Destroy();
                    return (m_bDone = false);
                }
                m_outputs[row].Copy(GetResultVector());
            
               // Update first block Autorange.
                if (m_options.firstBlockAutorange)
               {
                   m_fbsMax[row] = m_fbMax;
               }
            }
            ProgressMeterWrapper::Update(counter++, 0);
        }
    }
    else
    {
        // matrix mode
        std::vector<int> ranges = { m_nRows * m_nCols, 100 };
        ProgressMeterWrapper::Show("Computing convolution...", ranges, true);

        for (int row = 0; row < m_nRows; ++row)
        {
            int col = 0;

            for (auto in = m_inputs.begin(); in != m_inputs.end(); ++in, col++)
            {
                if (in->IsNull())
                {
                    continue;
                }
                // if a m_psmpFilterMatrix item is null, the convolution result
                // will be null, so it doesn't make sense to do any sort of
                // calculation...
                Aurora::SamplesVector& ht = m_filterMatrix.Cell(row, col);
                
                if(! ht.IsNull())
                {
                    SetFilter(ht);  // this FIRST!
                    SetVector(*in); // ...then this.
                                   
                    m_outputs[row].Resize(GetResultVector().Length());

                    if (! Convolve())
                    {
                        ProgressMeterWrapper::Destroy();
                        return (m_bDone = false);
                    }
                    AddVector(GetResultVector(), m_outputs[row]);
                
                   // Update first block Autorange.
                    if (m_options.firstBlockAutorange)
                   {
                       m_fbsMax[row] = m_fbMax;
                   }
                }
                ProgressMeterWrapper::Update(counter++, 0);
            }
        }
    }
    m_bDone = true;
    
    PostProcess();
    
    // delete progress meter
    ProgressMeterWrapper::Destroy();

    return m_bDone;
}
//----------------------------------------------- arrived from bottom here!!
void Aurora::Multivolver::PostProcess()
{
//   int row;
    if(m_matrixOptions.firstBlockAutorange)
   {
       VectorFBAutorange();
          
   } 
    else if(m_options.firstBlockAutorange)
   {
        for (auto& out : m_outputs)
        {
            Autorange(out);
        }
          
   } 
    else if(m_matrixOptions.autorange)
   {       
        VectorAutorange();
            
   }
    else if(m_options.autorange)
   {
       for (auto& out : m_outputs)
        {
            Autorange(out);
        }
   }
   
    if (m_options.removeDC)
   {
       VectorRemoveDC();
   }

    if (m_options.applyGain)
   {
       VectorBoost();
   }
   
    if (m_options.preserveOutputLength)
   {
       PreserveOutputLength();
   }
}

void Aurora::Multivolver::InitProgressMeter(const int total)
{
    // override the single channel progress meter method: it will be used
    // to init the second meter gauge.
    ProgressMeterWrapper::SetRange(total, 1);
}

bool Aurora::Multivolver::UpdateProgressMeter(const int step)
{
    // override the single channel progress meter method: it will be used
    // to update the second meter gauge.
    return ProgressMeterWrapper::Update(step, 1);
}

void Aurora::Multivolver::AddVector(const Aurora::SamplesVector& src,
                                          Aurora::SamplesVector& dst)
{
    dst += src;
}

bool Aurora::Multivolver::VectorRemoveDC()
{
    if (!m_bDone) 
    { 
        return false; 
    }

    for (auto& out : m_outputs)
    {
        RemoveDC(out);
    }
    return true;
}

bool Aurora::Multivolver::VectorFBAutorange()
{
    if (!m_bDone) 
    { 
        return false; 
    }
    Aurora::Sample max = 0.0;
    
    //Find max for every result
    for (int row = 0; row < m_nRows; row++)
    {
        max = (m_fbsMax[row] > max) ? m_fbsMax[row] : max; 
    }
    // Normalize @ -6 dB FS 
    // m_gain = 0.5/max; //printf("Gain: %2.4f\n", m_gain); // *** DEBUG
    VectorBoost(1.0/2.0*max);
    m_gain = m_gain - dB20(max);
    return true;
}

bool Aurora::Multivolver::VectorAutorange()
{
    if (!m_bDone) 
    { 
        return false; 
    }
    Aurora::Sample max = 0.0;
    
    //Find max for every result
    for (auto& out : m_outputs)
    {
        Aurora::Sample m = out.FindAbsMax();

        if (m > max)
        {
            max = m;
        }
    }

    // Normalize @ -6 dB FS
#ifdef __AUDEBUG__            
    printf("multivolver: max =  %.4f\n", max); 
#endif
    m_gain = dB20(0.5/max);
    VectorBoost();
#ifdef __AUDEBUG__
    printf("multivolver: Gain =  %.4f\n", m_gain); 
#endif
    return true;
}

bool Aurora::Multivolver::VectorBoost()
{
    if (!m_bDone) 
    { 
        return false; 
    }
    Aurora::Sample G = pow(10.0, m_gain/20.0);
    
    //Find max for every result  
    for (auto& out : m_outputs)
    {
        out *= G;
    }
    return true;
}

    
bool Aurora::Multivolver::VectorBoost(const Aurora::Sample g)
{
    if (!m_bDone) 
    { 
        return false; 
    }

    //Find max for every result
    for (auto& out : m_outputs)
    {
        out *= g;
    }
    return true;
}

void Aurora::Multivolver::ResizeFilterAndOutputs(const Aurora::SampleCount filterLength)
{
    m_filterMatrix.Resize(m_nRows, m_nCols, filterLength);

    for(int row = 0; row < m_nRows; row++)
    {        
        m_fbsMax.push_back(0);      
        m_filterPeakPositions.push_back(0);

        if (!m_inputs.empty())
        {
            Aurora::SampleCount outLen = m_inputs[row].Length() + filterLength;
            m_outputs.push_back(Aurora::SamplesVector(outLen));
        }
    }
}


void Aurora::Multivolver::Init(const int rows, 
                               const int cols,
                               const SampleCount in_len, 
                               const SampleCount f_len) 
{
    assert(rows > 0 && cols > 0);
    
    Destroy();
    
    m_nRows = rows;
    m_nCols = cols;
   
    m_inputs.clear();
    m_outputs.clear();
    m_fbsMax.clear();
    m_filterPeakPositions.clear();
       
    if (in_len > 0)
    {
        SetInputLength(in_len, -1);
    }

    if (f_len > 0)
    {
        ResizeFilterAndOutputs(f_len);
    }
}

void Aurora::Multivolver::Destroy() 
{  
    m_inputs.clear();
    m_outputs.clear();
    
    m_filterMatrix.Destroy();

    m_fbsMax.clear();
    m_filterPeakPositions.clear();
    
    m_filterMatrix.Destroy();

    m_nRows = 0;
    m_nCols = 0;

    m_bDone = false;
    m_bAborted = false;
}

// ---- ctors

Aurora::Multivolver::Multivolver(const int N, 
                                 const Aurora::SampleCount in_len, 
                                 const Aurora::SampleCount f_len) 
: Aurora::ConvolverBase()
{
    Init(N, N, in_len, f_len);
}


Aurora::Multivolver::Multivolver(const int rows, 
                                 const int cols,
                                 const Aurora::SampleCount in_len, 
                                 const Aurora::SampleCount f_len)
  : Aurora::ConvolverBase(), 
    m_nRows(rows), m_nCols(cols)
{
    Init(rows, cols, in_len, f_len);
}


Aurora::Multivolver::~Multivolver() 
{
   Destroy();
}
