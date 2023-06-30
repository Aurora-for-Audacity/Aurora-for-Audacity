/**********************************************************************
 * 
 *  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 *                       plugin collection
 * 
 *  Common libraries
 * 
 *  afcomplex.h
 * 
 *  Simone Campanini 2011-20
 * 
 **********************************************************************/
#ifndef __AF_COMPLEX_H__
#define __AF_COMPLEX_H__

#include <commdefs.h>


#include <kiss_fft.h>

#ifndef _FFT_MACRO_
#define _FFT_MACRO_

#  define Re(c) c.r         // real part of c kiss_fft_cpx
#  define Im(c) c.i         // imag part of c kiss_fft_cpx
#  define Mod(c)    std::sqrt(c.r*c.r + c.i*c.i) // Module, where 'c' is a kiss_fft_cpx
#  define SqrMod(c)          (c.r*c.r + c.i*c.i)  // Square module, 
                                                  // where 'c' is a kiss_fft_cpx
#  define Magnitude(c)    std::sqrt(c.r*c.r + c.i*c.i)  // Magnitude and Mod should be alias.
#  define SqrMagnitude(c)          (c.r*c.r + c.i*c.i)
#  define Phase(c)        (-std::atan2(c.i, c.r))
#  define DegPhase(c)     (-(std::atan2(c.i, c.r) * 180.0f) / M_PI)

 // Macros for complex reinterpretation, Mag,Phase instead of Re,Im
#  define MagPol(c) c.r
#  define PhPol(c)  c.i
#  define RePol(c) c.r * std::cos(c.i)
#  define ImPol(c) c.r * std::sin(c.i)
#endif

 #include "afvector.h"

namespace Aurora
{
    typedef kiss_fft_cpx ComplexNumber;

    class ComplexVectorBase : public VectorBase<kiss_fft_cpx>
    {
      public:
        void Dump(const char* filename) const;
          
        // Retrieve item

        virtual float Real(const SampleCount idx) const = 0;
        virtual float Imag(const SampleCount idx) const = 0;
        virtual float Magn(const SampleCount idx) const = 0;
        virtual float Phas(const SampleCount idx) const = 0;

        virtual float GetReal(const SampleCount idx) const = 0;
        virtual float GetImag(const SampleCount idx) const = 0;
        virtual float GetMagn(const SampleCount idx) const = 0;
        virtual float GetPhas(const SampleCount idx) const = 0;
        
        virtual float Module      (const SampleCount idx) const = 0;
        virtual float Argument    (const SampleCount idx) const = 0;
        virtual float SquareModule(const SampleCount idx) const = 0;
        virtual float SquareReal  (const SampleCount idx) const = 0;
        virtual float SquareImag  (const SampleCount idx) const = 0;

        // checkers
        virtual bool IsPolar()       const = 0;
        virtual bool IsRectangular() const = 0;

        ComplexVectorBase& operator=(ComplexVectorBase&& Right);
        ComplexVectorBase& operator=(const ComplexVectorBase& Right) = delete; // forcing move semantic

        void Copy(const ComplexVectorBase& src,
                  const SampleCount srcOfs,
                  const SampleCount destOfs)
        {
             VectorBase::Copy(src, srcOfs, destOfs);
        }

        void Copy(const ComplexVectorBase& src,
                  const SampleCount srcOfs,
                  const SampleCount destOfs,
                  const SampleCount amount)
        {
            VectorBase::Copy(src, srcOfs, destOfs, amount);
        }

        void Copy(const ComplexVectorBase& v)
        { 
            VectorBase::Copy(v.m_pData, v.m_length);
        }

        // init/destroy
        void Destroy()
        {
            VectorBase::Destroy();
        }
        
        // 'ctors
        ComplexVectorBase() { }

        ComplexVectorBase(const SampleCount length)
        : VectorBase(length)
        { }
        
        ComplexVectorBase(ComplexVectorBase&& v)
        : VectorBase(std::move(v))
        { }

        ~ComplexVectorBase()
        { Destroy(); }

    private:        
        ComplexVectorBase(const ComplexVectorBase& v) = delete;
    };

    /**
     * @brief Derived class used to store and manipulate complex numbers in rectangular
     * rapresentation.
     */
    class ComplexVector : public ComplexVectorBase
    {
      public:
        virtual bool IsPolar()       const override { return false; };
        virtual bool IsRectangular() const override { return true; };

        float Real(const SampleCount idx) const override { return Re(m_pData[idx]); }
        float Imag(const SampleCount idx) const override { return Im(m_pData[idx]); }
        float Magn(const SampleCount idx) const override { return Mod(m_pData[idx]); }
        float Phas(const SampleCount idx) const override { return Phase(m_pData[idx]); }

        float GetReal(const SampleCount idx) const override { return Re(m_pData[idx]); }
        float GetImag(const SampleCount idx) const override { return Im(m_pData[idx]); }
        float GetMagn(const SampleCount idx) const override { return Mod(m_pData[idx]); }
        float GetPhas(const SampleCount idx) const override { return Phase(m_pData[idx]); }

        void SetReal(const SampleCount idx, const float fValue)  { Re(m_pData[idx]) = fValue; }
        void SetImag(const SampleCount idx, const float fValue)  { Im(m_pData[idx]) = fValue; }


        void StoreModule(float* Dest);
        void StoreModule(float* Dest, const SampleCount length, const SampleCount from = 0);
        void StorePhase(float* Dest);
        void StorePhase(float* Dest, const SampleCount length, const SampleCount from = 0);

        float Module      (const SampleCount idx) const override;
        float Argument    (const SampleCount idx) const override;
        float SquareModule(const SampleCount idx) const override;
        float SquareReal  (const SampleCount idx) const override;
        float SquareImag  (const SampleCount idx) const override;

        float FindAbsMax(SampleCount& maxIndex) const;

        void Set(const SampleCount idx,
                 const float fReal,
                 const float fImag)  { Re(m_pData[idx]) = fReal;
                                       Im(m_pData[idx]) = fImag; }

        // MultiplyACcumulate
        const ComplexVector& Mac(const ComplexVector& Left,
                                 const ComplexVector& Right,
                                 const SampleCount length,
                                 const SampleCount from = 0);
        
        const ComplexVector& Mac(const ComplexVector& Left, const ComplexVector& Right);
        const ComplexVector& Mac(const float fLeft, const ComplexVector& Right);
        
        // operators
        ComplexVector operator+(const ComplexVector& Right);
        ComplexVector operator-(const ComplexVector& Right);
        ComplexVector operator*(const ComplexVector& Right);
        ComplexVector operator/(const ComplexVector& Right);
        
        ComplexVector operator+(const float fRight);
        ComplexVector operator-(const float fRight);
        ComplexVector operator*(const float fRight);
        ComplexVector operator/(const float fRight);

        
        ComplexVector& operator+=(const ComplexVector& Right);
        ComplexVector& operator-=(const ComplexVector& Right);
        ComplexVector& operator*=(const ComplexVector& Right);
        ComplexVector& operator/=(const ComplexVector& Right);

        ComplexVector& operator+=(const float fRight);
        ComplexVector& operator-=(const float fRight);
        ComplexVector& operator*=(const float fRight);
        ComplexVector& operator/=(const float fRight);
        
        void ForEach(const Range& range,
                     std::function<void(const size_t i, float& real, float& imag)> handler);
        
        ComplexVector& operator=(ComplexVector&& Right)
        {
            return (ComplexVector&)ComplexVectorBase::operator=(std::move(Right));
        }
        
        ComplexVector& operator=(const ComplexVector& Right) = delete; // forcing move semantic


        ComplexVector() { }

        ComplexVector(const SampleCount length)
        : ComplexVectorBase(length)
        { }
        
        ComplexVector(ComplexVector&& v)
        : ComplexVectorBase(std::move(v))
        { }

    };

   
    /**
     * @brief Essentially a \c ComplexVector but it should effectively used
     * to store a spectrum, so it holds some useful informations like
     * Nyquist frequency, etc....
     */
    class ComplexSpectrum : public ComplexVector
    {
      private:
        float m_nyFrequency = 0.0f;
        
      public:
        ComplexSpectrum operator*(const ComplexSpectrum& Right);
        ComplexSpectrum operator/(const ComplexSpectrum& Right);
        
        ComplexSpectrum& operator/=(const ComplexSpectrum& Right);

        float DC() const { return Re(m_pData[0]); }
        float Ny() const { return Re(m_pData[m_length-1]); }

        float GetDC     () const { return Re(m_pData[0]); }
        float GetNyquist() const { return Re(m_pData[m_length-1]); }
        
        void SetDC     (const float dc) { Re(m_pData[0]) = dc; }
        void SetNyquist(const float ny) { Re(m_pData[m_length-1]) = ny; }
        
        float GetSamplerate() const;
        float GetNyquistFrequency() const;
        float GetFrequencyBinsWidth() const;
        
        void SetNyquistFrequency(const float fny) { m_nyFrequency = fny; }
        
        void ForEachSpectralBin(std::function<void(const size_t i,
                                                   float& real,
                                                   float& imag)> handler);

        void Copy(const ComplexSpectrum& src,
                  const SampleCount srcOfs,
                  const SampleCount destOfs)
        {
            m_nyFrequency = src.m_nyFrequency;
            VectorBase::Copy(src, srcOfs, destOfs);
        }

        void Copy(const ComplexSpectrum& src,
                  const SampleCount srcOfs,
                  const SampleCount destOfs,
                  const SampleCount amount)
        {
            m_nyFrequency = src.m_nyFrequency;
            VectorBase::Copy(src, srcOfs, destOfs, amount);
        }

        void Copy(const ComplexSpectrum& s)
        {
            m_nyFrequency = s.m_nyFrequency;
            VectorBase::Copy(s.m_pData, s.m_length);
        }

        ComplexSpectrum& operator=(ComplexSpectrum&& Right)
        {
            m_nyFrequency = Right.m_nyFrequency;
            return (ComplexSpectrum&)ComplexVectorBase::operator=(std::move(Right));
        }
        
        ComplexSpectrum& operator=(const ComplexSpectrum& Right) = delete; // forcing move semantic


        ComplexSpectrum() { }

        ComplexSpectrum(const SampleCount length)
        : ComplexVector(length)
        { }
        
        ComplexSpectrum(ComplexSpectrum&& s)
        : ComplexVector(std::move(s)), m_nyFrequency(s.m_nyFrequency)
        { }

    };


    /**
    * @brief Derived class used to store and manipulate complex numbers in polar
    * rapresentation.
    */
    class PolarComplexVector : public ComplexVectorBase
    {
        
      public:
        virtual bool IsPolar()       const override { return true; };
        virtual bool IsRectangular() const override { return false; };

        // conversion methods
        void ToRectangular(ComplexSpectrum& dest);
        void ToPolar(const ComplexSpectrum& src);

        void UnwrapPhase();

        float Real(const SampleCount idx) const override { return RePol(m_pData[idx]); }
        float Imag(const SampleCount idx) const override { return ImPol(m_pData[idx]); }
        float Magn(const SampleCount idx) const override { return MagPol(m_pData[idx]); }
        float Phas(const SampleCount idx) const override { return PhPol(m_pData[idx]); }

        float GetReal(const SampleCount idx) const override { return RePol(m_pData[idx]); }
        float GetImag(const SampleCount idx) const override { return ImPol(m_pData[idx]); }
        float GetMagn(const SampleCount idx) const override { return MagPol(m_pData[idx]); }
        float GetPhas(const SampleCount idx) const override { return PhPol(m_pData[idx]); }

        void SetMagn(const SampleCount idx, const float fValue)  { MagPol(m_pData[idx]) = fValue; }
        void SetPhas(const SampleCount idx, const float fValue)  { PhPol(m_pData[idx])  = fValue; }

        void StoreModule(float* Dest);
        void StoreModule(float* Dest, const SampleCount length, const SampleCount from = 0);
        void StorePhase(float* Dest);
        void StorePhase(float* Dest, const SampleCount length, const SampleCount from = 0);

        float Module      (const SampleCount idx) const override;
        float Argument    (const SampleCount idx) const override;
        float SquareModule(const SampleCount idx) const override;
        float SquareReal  (const SampleCount idx) const override;
        float SquareImag  (const SampleCount idx) const override;
        
        float FindAbsMax(SampleCount& maxIndex) const;

        PolarComplexVector& operator=(PolarComplexVector&& Right)
        {
            return (PolarComplexVector&)ComplexVectorBase::operator=(std::move(Right));
        }
        
        PolarComplexVector& operator=(const PolarComplexVector& Right) = delete; // forcing move semantic


        PolarComplexVector() {}
        
        PolarComplexVector(PolarComplexVector&& pcv)
        : ComplexVectorBase(std::move(pcv))
        { }
        
        PolarComplexVector(const ComplexSpectrum& cv)
        {
            ToPolar(cv);
        }
    };

}


#endif //___COMPLEX_H__

