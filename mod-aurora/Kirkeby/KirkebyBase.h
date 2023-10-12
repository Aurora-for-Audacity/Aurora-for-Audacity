/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  afkirk.h

  Angelo Farina - Simone Campanini

***********************************************************************/
#ifndef __AURORA_KIRKEBY_AFKIRK_H__
#define __AURORA_KIRKEBY_AFKIRK_H__

#include <aurora.h>

#if 1
#include <tools/kiss_fftr.h>


#else
#include <fftw3.h>
#include <commdefs.h>

// Vector related stuffs
#include "afcpxmatrix.h"
#include "afmatrix.h"
typedef AFVector<Sample>                       TimeVector_t;
typedef AFComplexSpectrum<fftwf_complex, Sample> FreqVector_t;
typedef AFMatrix<Sample>                       TimeMatrix_t;
typedef AFComplexMatrix<fftwf_complex, Sample> FreqMatrix_t;

// Hazardous vector macros...
#define VectorItemPtr(ptr,r,c, lrow, lcol)   (ptr + r*lrow + c*lcol)
#define VectorRowPtr(ptr, r, lrow)           (ptr + r*lrow)

#define InItem(r,c)   VectorItemPtr(m_psmpIn,r,c,m_smpcInRowLength,m_smpcInColLength)
#define InRow(r)      VectorRowPtr(m_psmpIn,r,m_smpcInRowLength)
#define OutItem(r,c)  VectorItem(m_psmpOut,r,c,m_smpcOutRowLength,m_smpcOutColLength)
#define OutRow(r)     VectorRowPtr(m_psmpOut,r,m_smpcOutRowLength)
#endif

namespace Aurora
{
   typedef Matrix<SamplesVector>    TSampleMatrix;
   typedef Matrix<ComplexSpectrum>  TComplexMatrix;

   class KirkebyBase
   {
         enum  ComplexComponent
         { 
            Real      = 0, 
            Imag      = 1, 
            Magnitude = 0, 
            Phase     = 1,
            Both      = 2
         }; 
       
      public: 
         enum FilterType  
         { 
            Mono = 0, 
            Stereo, 
            Dipole, 
            Matrix 
         };

         enum AverageMode 
         { 
            None = 0, 
            ReIm, 
            MagPh, 
            Cepstrum 
         };

         enum AverageType     
         { 
            Linear = 0, 
            Octave 
         };
         
      private:
         double m_dbRate = 44100.0;
         int    m_nRows  = 0; //alias channels...
         int    m_nCols  = 0;
         
         TSampleMatrix   m_Ht;
         TComplexMatrix  m_Hf;

         kiss_fftr_cfg m_fftForwardCfg  = nullptr;
         kiss_fftr_cfg m_fftBackwardCfg = nullptr;

         SampleCount m_inputDataLength = 0;
         SampleCount m_colLength = 0;
         SampleCount m_rowLength = 0;
         
         int    m_nFilterType      = FilterType::Mono;
         bool   m_bCancelCrosstalk = false;
         double m_dbInRP           = 0.001;
         double m_dbOutRP          = 1.0;
         double m_dbLowCutFrq      = 80.0;
         double m_dbHighCutFrq     = 16000.0; // Hz
         double m_dbTransWidth     = 0.333;   // oct
         SampleCount m_fadeInLength  = 0;       // samples
         SampleCount m_fadeOutLength = 0;       // samples
         
         int    m_nAvgMode   = AverageMode::None;
         int    m_nAvgType   = AverageType::Linear;
         double m_dbAvgWidth = 5.0;  // spectral points (int??)

         bool   m_bAutorange = true;
         double m_dbGain     = 1.0;
         
         bool   m_bActiveNoiseControl = false;
         
         // --- methods
         void InitVectors();
         void DeleteVectors();
         void InitFft();
         void DeleteFft();
         
         void FFT (ComplexSpectrum& out, const SamplesVector& in);
         void IFFT(SamplesVector& out, const ComplexSpectrum& in);
         
         void ApplyGain();
         
         /** @brief It embeds Farina's myMedia2 and MediaLplus functions,
           * processing in the same loop real (magnitude) and imaginary
           * (phase) parts.
           */
         void ComputeMovingAverage (ComplexVectorBase& Hf,
                                    const ComplexComponent realOrImag = ComplexComponent::Both); // -> MediaLplus

         void ComputeOctaveSmoothing(ComplexVectorBase& Hf);
         void ComputeOctaveSmoothing(ComplexVectorBase& Hf,
                                     const ComplexComponent realOrImag);
         
         void ComputeAverage (ComplexSpectrum& Hf);
         void ComputeCepstrum(ComplexSpectrum& Hf);
             
         void ComputeKirkebyModulusInversion     (ComplexSpectrum& Hf);
         void ComputeKirkebyRatioModulusInversion(ComplexSpectrum& Uf,
                                                  ComplexSpectrum& Wf);

         bool ComputeMonoInverseFilter(TSampleVector& inOut);
         bool ComputeQuadInverseFilter(TSampleMatrix& Ht);

       public:
         bool Init();
           
         bool PreProcess();
         bool Process();
         bool PostProcess();
         
         // -- checkers
         bool IsCancelCrosstalkSet()    const { return m_bCancelCrosstalk; }
         bool IsAverageSet()            const { return (m_nAvgMode > AverageMode::None); }
         bool IsAutorangeSet()          const { return m_bAutorange; }
         bool IsActiveNoiseControlSet() const { return m_bActiveNoiseControl; }
         
         bool IsFilterMono  () const { return (m_nFilterType == FilterType::Mono); }
         bool IsFilterStereo() const { return (m_nFilterType == FilterType::Stereo); }
         bool IsFilterDipole() const { return (m_nFilterType == FilterType::Dipole); }
         bool IsFilterMatrix() const { return (m_nFilterType == FilterType::Matrix); }

         // -- set-getters
         double GetSamplerate() const { return m_dbRate; } 
         
         int GetRows()     const { return m_nRows; }
         int GetCols()     const { return m_nCols; } 
         int GetChannels() const { return m_nRows; }
         
         TSampleVector& GetInputTrackItem (const int nRow, const int nCol) { return m_Ht.Cell(nRow, nCol); }
         TSampleVector& GetOutputTrackItem(const int nRow, const int nCol) { return m_Ht.Cell(nRow, nCol); }

         SampleCount GetRowLength()     const { return m_rowLength; }
         SampleCount GetColumnLength()  const { return m_colLength; }
         SampleCount GetColumnWidth()   const { return m_colLength; }
         SampleCount GetFftLength()     const { return m_colLength; }
         SampleCount GetInverseFilterLength() const { return m_colLength; }
         
         int    GetFilterType() const { return m_nFilterType; }
         double GetInBandRegularisationParameter()  const { return m_dbInRP; }
         double GetOutBandRegularisationParameter() const { return m_dbOutRP; }
         double GetLowerCutFrequency()   const { return m_dbLowCutFrq; }
         double GetHigherCutFrequency()  const { return m_dbHighCutFrq; }
         double GetTransitionWidth()     const { return m_dbTransWidth; }
         SampleCount GetFadeInLength()   const { return m_fadeInLength; }
         SampleCount GetFadeOutLength()  const { return m_fadeOutLength; }
         
         int    GetAverageMode()  const { return m_nAvgMode; }
         int    GetAverageType()  const { return m_nAvgType; }
         double GetAverageWidth() const { return m_dbAvgWidth; } // TODO Check it!!

         double GetGaindB() const { return dB(m_dbGain); }
         double GetGain()   const { return m_dbGain; }

         void SetSamplerate(const double dbValue) { m_dbRate = dbValue; }

         void SetRows    (const int nr);
         void SetCols    (const int nc);
         void SetChannels(const int nch);

         void SetCancelCrosstalk   (const bool bValue) { m_bCancelCrosstalk = bValue; }
         void SetAutorange         (const bool bValue) { m_bAutorange = bValue; }
         void SetActiveNoiseControl(const bool bValue) { m_bActiveNoiseControl = bValue; }

         void SetInputRowLength  (const SampleCount irl);
         void SetInputTrackLength(const SampleCount itl);

         void SetFftLength          (const SampleCount N);                                     
         void SetInverseFilterLength(const SampleCount N);

         void SetFilterType                    (const int ft)      { m_nFilterType = ft; }
         void SetInBandRegularisationParameter (const double ibrp) { m_dbInRP  = ibrp; }
         void SetOutBandRegularisationParameter(const double obrp) { m_dbOutRP = obrp; }
         void SetLowerCutFrequency             (const double lcf)  { m_dbLowCutFrq  = lcf; }
         void SetHigherCutFrequency            (const double hcf)  { m_dbHighCutFrq = hcf; }
         void SetTransitionWidth               (const double tw)   { m_dbTransWidth = tw; }

         void SetFadeInLength (const  SampleCount fil) { m_fadeInLength = fil; }
         void SetFadeOutLength(const  SampleCount fol) { m_fadeOutLength = fol; }
         
         void SetAverageMode (const int am)      { m_nAvgMode = am; }
         void SetAverageType (const int at)      { m_nAvgType = at; }
         void SetAverageWidth(const double aw) { m_dbAvgWidth = aw; }  // TODO Check it!!

         void SetGaindB(const double G) { m_dbGain = undB(G); }
         void SetGain(const double g)   { m_dbGain = g; }
         
         void Init(const int nRows, const int nCols = 1);
         void Destroy();

   protected:
       //wxString GetCaptionString() const override { return wxString("Aurora Kirkeby Inversion"); }

   public:
         // --- 'ctors
         KirkebyBase() { }
         KirkebyBase(const int nRows, const int nCols = 1)
          : m_nRows(nRows), m_nCols(nCols)
         { }

         ~KirkebyBase() { Destroy(); }
   };
} // end of Aurora namspace

#endif // __AURORA_KIRKEBY_AFKIRK_H__

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



