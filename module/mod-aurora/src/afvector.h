/**********************************************************************
 * 
 *  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 *                       plugin collection
 * 
 *  Common libraries
 * 
 *  afvector.h
 * 
 *  Simone Campanini 2011-20
 * 
 **********************************************************************/
#ifndef __AF_VECTOR_H__
#define __AF_VECTOR_H__

#include <kiss_fft.h>

#define MAX_INIT 1.0e-6
#define MIN_INIT 1.0e6

namespace Aurora
{
    template <class F>
    class VectorBase
    {
    protected:
        F* m_pData = nullptr; // base
        SampleCount  m_length = 0; // base

    public:

        void Clear(const SampleCount firstNSamples = 0); // base
        void ZeroPad(const SampleCount offset);

        virtual void Reverse();
        virtual void Reverse(VectorBase<F>& supportBuffer); // base
        virtual void ShiftLeft(const SampleCount amount);

        bool IsNull() const { return (m_pData == nullptr); }
        
        VectorBase& operator=(VectorBase&& Right) // base
        {
            if (&Right != this)
            {
                Destroy();
                m_pData = Right.m_pData;
                m_length = Right.m_length;
                Right.Destroy();
            }
            return *this;
        }

        VectorBase& operator=(const VectorBase& Right) = delete; // forcing move semantic

        // Retrieve item
        const F& operator[](const SampleCount nIdx) const { return m_pData[nIdx]; }
        const F& Item      (const SampleCount nIdx) const { return m_pData[nIdx]; }

        F& operator[](const SampleCount nIdx) { return m_pData[nIdx]; }
        F& Item      (const SampleCount nIdx) { return m_pData[nIdx]; }
        
        F GetItem   (const SampleCount nIdx) const { return m_pData[nIdx]; }
        void SetItem(const SampleCount nIdx, const F fValue) { m_pData[nIdx] = fValue; }

        F GetFirst() const { return m_pData[0]; }
        F GetLast()  const { return m_pData[m_length - 1]; }

        F& First() { return m_pData[0]; }
        F& Last () { return m_pData[m_length - 1]; }

        /**
         * TODO: specialized version to obtain exactly DC and Nyquist
         * for kiss_fft_cpx type vector, stored respectively at 
         *  indices 0 and  nfft/2 + 1
         */        
        F& DC() { return m_pData[0]; }
        F& Nyquist () { return m_pData[m_length - 1]; }
        
        // Retrieve vector
        F* Samples() { return m_pData; } // base      
        const F* CSamples() const { return m_pData; } // base 

        const F* GetData() const { return m_pData; } // base
        F* GetData(const SampleCount ofs) { return (m_pData + ofs); }

        virtual void Copy(const VectorBase& v) 
        { 
            Copy(v.m_pData, v.m_length); 
        }
        
        virtual void Copy(const VectorBase& src,
                          const SampleCount srcOfs,
                          const SampleCount destOfs)
        {
            if ( destOfs + src.m_length - srcOfs > m_length)
            {
                Resize(destOfs + src.m_length - srcOfs, false);
            }
            memcpy(    m_pData + destOfs, 
                   src.m_pData + srcOfs, 
                   sizeof(F) * (src.m_length - srcOfs)); 
        }

        virtual void Copy(const VectorBase& src,
                          const SampleCount srcOfs,
                          const SampleCount destOfs,
                          const SampleCount amount)
        {
            if (destOfs + amount > m_length)
            {
                Resize(destOfs + amount, false);
            }
            memcpy(    m_pData + destOfs, 
                   src.m_pData + srcOfs, 
                   sizeof(F) * amount); 
        }

        void Copy(const F* dataSrc,
                  const SampleCount length,
                  const SampleCount destOfs = 0)
        {
            if (destOfs + length > m_length)
            {
                Resize(destOfs + length, false);
            }
            memcpy(m_pData + destOfs, dataSrc, sizeof(F) * length); 
        }

        void CopyTo(F* dest, 
                    const SampleCount length, 
                    const SampleCount srcOfs = 0) const;

        void Store(VectorBase& Dest);
        void Store(VectorBase& Dest, 
                   const SampleCount length, 
                   const SampleCount unFrom);

        // length set-getters
        SampleCount Length()    const { return m_length; }
        SampleCount GetLength() const { return m_length; }
        
        void Resize   (const SampleCount length, const bool saveData = false); // base
        void Destroy(); // base
                
        // 'ctors
        VectorBase() { }

        VectorBase(const SampleCount length) : m_length(length)
        { Resize(length); }

        VectorBase(const VectorBase& v)  = delete;

        VectorBase(VectorBase&& v)
         : m_pData(v.m_pData),
           m_length(v.m_length)
        {  
            v.m_pData = nullptr;
            v.m_length = 0;
        }
        
        ~VectorBase()
        { Destroy(); }
    };    

    // ---------------------------------------------- Template class definitions

    template<class F>
    void VectorBase<F>::Clear(const SampleCount firstNSamples)
    {
        if (m_pData)
        {
            memset(m_pData, 0, sizeof(F) * (firstNSamples > 0 ? firstNSamples 
                                                              : m_length));
        }
    }

    template<class F>
    void VectorBase<F>::ZeroPad(const SampleCount offset)
    {
        if (m_pData)
        {
            memset(m_pData + offset, 0, sizeof(F) * (m_length - offset));
        }
    }

    template<class F>
    void VectorBase<F>::Reverse()
    {
        VectorBase<F> tmp(m_length);
        F* supportBuffer = tmp.m_pData;

        for (Aurora::SampleCount i = 0; i < m_length; i++)
        {
            supportBuffer[i] = m_pData[m_length - 1 - i];
        }
        memcpy(m_pData, supportBuffer, m_length * sizeof(F));
    }

    template<class F>
    void VectorBase<F>::Reverse(VectorBase<F>& supportBuffer) //base
    {
        supportBuffer.Resize(m_length);
        
        for (Aurora::SampleCount i = 0; i < m_length; i++)
        {
            supportBuffer[i] = m_pData[m_length - 1- i];
        }
        memcpy(m_pData, supportBuffer.Samples(), m_length * sizeof(F));
    }

    template<class F>
    inline void VectorBase<F>::ShiftLeft(const SampleCount ofs)
    {
        assert(m_pData != nullptr && ofs > 0);
        memmove(m_pData, m_pData + ofs, sizeof(F) * (m_length - ofs));
        memset(m_pData + (m_length - ofs), 0, sizeof(F) * ofs);
    }

    template<class F>
    void VectorBase<F>::Resize(const SampleCount length, const bool saveData)
    {
        assert(length > 0);
        
        if(m_pData == nullptr)
        {
            m_length = length;

            if (m_pData)
            {
                delete[] m_pData;
            }
            m_pData = new F [m_length];
            Clear();
        }
        else
        {
            assert(m_length > 0);

            // create a temporary vector for storing purposes
            F* p = new F [length];

            if (saveData)
            {
                memcpy(p, m_pData, sizeof(F) * (m_length < length ? m_length
                                                                  : length));
            }

            delete [] m_pData;
            m_pData = p;
            // set new length attribute
            m_length = length;

            if (!saveData)
            {
                Clear();
            }
        }
    }

    template<class F>
    void VectorBase<F>::CopyTo(F* dest, 
                               const SampleCount length, 
                               const SampleCount srcOfs) const
    {
        const size_t l = (length > m_length - srcOfs) ? m_length - srcOfs
                                                      : length;
        memcpy(dest, m_pData + srcOfs, sizeof(F) * l);
    }
    
    template<class F>
    inline void VectorBase<F>::Store(VectorBase& dest)
    {
        CopyTo(dest.m_pData, dest.m_length);
    }

    template<class F>
    inline void VectorBase<F>::Store(VectorBase& Dest, 
                                     const SampleCount length, 
                                     const SampleCount unFrom)
    {
        memcpy(Dest.m_pData, m_pData + unFrom, sizeof(F) * length);
    }

    template<class F>
    void VectorBase<F>::Destroy()
    {
        if(m_pData)
        { 
            delete [] m_pData; 
        }
        m_pData = nullptr; 
        m_length = 0;
    }

    /**
     * @brief Extension of the \c Vector class to support some linear array operations
     */
    template <class F>
    class Vector : public Aurora::VectorBase<F>
    {

      public:        
        float Module(const SampleCount nIdx) const 
        { 
            return std::fabs(this->m_pData[nIdx]); 
        }

        float Abs(const SampleCount nIdx) const 
        { 
            return std::fabs(this->m_pData[nIdx]); 
        }
        
        float Sqr(const SampleCount nIdx) const 
        { 
            return this->m_pData[nIdx] * this->m_pData[nIdx]; 
        }

        // operators
        const Vector& operator<<(const SampleCount ofs);

        Vector operator+(const Vector& Right);
        Vector operator-(const Vector& Right);
        Vector operator*(const Vector& Right);
        Vector operator/(const Vector& Right);

        Vector operator+(const F right);
        Vector operator-(const F right);
        Vector operator*(const F right);
        Vector operator/(const F right);

        Vector& operator+=(const Vector& Right);
        Vector& operator-=(const Vector& Right);
        Vector& operator*=(const Vector& Right);
        Vector& operator/=(const Vector& Right);

        Vector& operator+=(const F right);
        Vector& operator-=(const F right);
        Vector& operator*=(const F right);
        Vector& operator/=(const F right);
                 
        // 'ctors
        Vector() { }
        Vector(const SampleCount length) :  VectorBase<F>(length){ }
        Vector(Vector&& v) : VectorBase<F>(std::move(v)) { }
        Vector(const Vector& v) = delete;
    };
    
    // operators
    template<class F>
    inline const Vector<F>& Vector<F>::operator<<(const SampleCount amount)
    {
        assert(this->m_pData > 0 && amount > 0);
        memcpy(this->m_pData, this->m_pData + amount, sizeof(F) * (this->m_length - amount));
        memmove(this->m_pData + (this->m_length - amount), 0, sizeof(F) * amount);
        return *this;
    }

    template<class F>
    Vector<F> Vector<F>::operator+(const Vector& Right)
    {
        Vector V(this->m_length);

        for (SampleCount i = 0; i < this->m_length; i++)
        {
            if constexpr (std::is_same<F, kiss_fft_cpx>::value)
            {
                V.m_pData[i].r = this->m_pData[i].r + Right.m_pData[i].r;
                V.m_pData[i].i = this->m_pData[i].i + Right.m_pData[i].i;
            }
            else
            {
                V.m_pData[i] = this->m_pData[i] + Right.m_pData[i];
            }
        }
        return V;
    }

    template<class F>
    Vector<F> Vector<F>::operator-(const Vector& Right)
    {
        Vector V(this->m_length);
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            V.m_pData[i] = this->m_pData[i] - Right.m_pData[i];
        }
        return V;
    }

    template<class F>
    Vector<F> Vector<F>::operator*(const Vector& Right)
    {
        Vector V(this->m_length);
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            V.m_pData[i] = this->m_pData[i] * Right.m_pData[i];
        }
        return V;
    }

    template<class F>
    Vector<F> Vector<F>::operator/(const Vector& Right)
    {
        Vector V(this->m_length);
        for (SampleCount i = 1; i < (this->m_length - 1); i++)
        {
            V.m_pData[i] = this->m_pData[i] / Right.m_pData[i];
        }
        return V;
    }

    template<class F>
    Vector<F> Vector<F>::operator+(const F fRight)
    {
        Vector V(this->m_length);
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            V.m_pData[i] = this->m_pData[i] + fRight;
        }
        return V;
    }

    template<class F>
    Vector<F> Vector<F>::operator-(const F fRight)
    {
        Vector V(this->m_length);
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            V.m_pData[i] = this->m_pData[i] - fRight;
        }
        return V;
    }

    template<class F>
    Vector<F> Vector<F>::operator*(const F fRight)
    {
        Vector V(this->m_length);
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            V.m_pData[i] = this->m_pData[i] * fRight;
        }
        return V;
    }

    template<class F>
    Vector<F> Vector<F>::operator/(const F fRight)
    {
        Vector V(this->m_length);
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            V.m_pData[i] = this->m_pData[i] / fRight;
        }
        return V;
    }

    
    template<class F>
    inline Vector<F>& Vector<F>::operator+=(const Vector& Right)
    {
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] = this->m_pData[i] + Right.m_pData[i];
        }
        return *this;
    }

    template<class F>
    inline Vector<F>& Vector<F>::operator-=(const Vector& Right)
    {
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] = this->m_pData[i] - Right.m_pData[i];
        }
        return *this;
    }

    template<class F>
    inline Vector<F>& Vector<F>::operator*=(const Vector& Right)
    {
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] = this->m_pData[i] * Right.m_pData[i];
        }
        return *this;
    }

    template<class F>
    inline Vector<F>& Vector<F>::operator+=(const F fRight)
    {
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] += fRight;
        }
        return *this;
    }

    template<class F>
    inline Vector<F>& Vector<F>::operator-=(const F fRight)
    {
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] -= fRight;
        }
        return *this;
    }

    template<class F>
    inline Vector<F>& Vector<F>::operator*=(const F fRight)
    {
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] *= fRight;
        }
        return *this;
    }

    template<class F>
    inline Vector<F>& Vector<F>::operator/=(const F fRight)
    {
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] /= fRight;
        }
        return *this;
    }


    /**
     * @brief Extension of the \c Vector class to support some linear array operations
     */
    template <class F>
    class ScalarVector : public Vector<F>
    {
      protected:
        F m_fMax = (F)MAX_INIT;
        F m_fMin = (F)MIN_INIT;
        SampleCount m_maxPosition = 0;
        SampleCount m_minPosition = 0;

      public:
        enum WindowType
        {
            Rectangular,
            Hanning,
            BlackmannHarris,
            FlatTop,
            Triangular,
            HalfSine
        };
          
        void MakeWindow(const WindowType type);
        void RemoveDC();
        void Normalize(F max = 0);
        void Rotate(const SampleCount numSamples);
          
        /**
         * @brief Apply a raised cosine fade in to the vector
         * @param duration The duration of the fade in
         * @param startSample The sample from which the fade in should start
         * No boundary check will be performed.
         */
        void FadeIn (const SampleCount duration, const int startSample  = 0);
        
        /**
         * @brief Apply a raised cosine fade out to the vector
         * @param duration The duration of the fade out
         * @param startSample The sample from which the fade out should start
         * a value of -1 means that the fade out will be applied to the tail
         * of the vector. No boundary check will be performed.
         */
        void FadeOut(const SampleCount duration, const int startSample = -1);

        // Multiply-ACcumulate
        const ScalarVector& Mac(const ScalarVector& Left, 
                                const ScalarVector& Right, 
                                const SampleCount length, 
                                const SampleCount unFrom = 0);
        
        const ScalarVector& Mac(const ScalarVector& Left, 
                                const ScalarVector& Right);

        const ScalarVector& Mac(const F fLeft, 
                                const ScalarVector& Right);

        F Abs(const SampleCount nIdx) const 
        { 
            return std::fabs(this->m_pData[nIdx]); 
        }
        
        F Sqr(const SampleCount nIdx) const 
        { 
            return this->m_pData[nIdx] * this->m_pData[nIdx]; 
        }

        F FindAbsMax(const SampleCount firstNSamples = 0);

        F GetMax(const bool useStoredValue = false);
        F GetMin(const bool useStoredValue = false);
        F GetAbsMax(const bool useStoredValue = false);
        F GetAbsMax(SampleCount& maxIndex, const bool useStoredValue = false);

        SampleCount GetMaxPosition() const { return m_maxPosition; }
        SampleCount GetMinPosition() const { return m_minPosition; }
        SampleCount GetAbsMaxPosition();  

        /**
         * @brief Compute average value.
         */
        F Mean();

        /**
         * @brief Compute RMS.
         * @param length number of samples of the computation
         */
        F GetSquareRMS(const SampleCount length = 0) const;
        
        /**
         * @brief Compute RMS between two vectors. Used in XFunctions.
         * @param w the second vector.
         * @param length number of samples of the computation.
         */
        F GetCrossSquareRMS(const ScalarVector& w, const SampleCount length = 0) const;

       void Copy(const F* dataSrc,
                 const SampleCount length,
                 const SampleCount ofs = 0)
        {
            this->Resize(ofs + length, true);
            memcpy(this->m_pData + ofs, dataSrc, sizeof(F) * length);
        }

        ScalarVector& operator=(ScalarVector&& Right); 
        ScalarVector& operator=(const ScalarVector& Right) = delete; // forcing move semantic

        void Copy(const ScalarVector& src,
                  const SampleCount srcOfs,
                  const SampleCount destOfs)
        {
             VectorBase<F>::Copy(src, srcOfs, destOfs);
             m_fMax = src.m_fMax;
             m_fMin = src.m_fMin;
             m_maxPosition = src.m_maxPosition;
             m_minPosition = src.m_minPosition;
        }

        void Copy(const ScalarVector& src,
                  const SampleCount srcOfs,
                  const SampleCount destOfs,
                  const SampleCount amount)
        {
            VectorBase<F>::Copy(src, srcOfs, destOfs, amount);
            m_fMax = src.m_fMax;
            m_fMin = src.m_fMin;
            m_maxPosition = src.m_maxPosition;
            m_minPosition = src.m_minPosition;
        }

        void Copy(const ScalarVector& v)
        { 
            VectorBase<F>::Copy(v.m_pData, v.m_length); 
            m_fMax = v.m_fMax;
            m_fMin = v.m_fMin;
            m_maxPosition = v.m_maxPosition;
            m_minPosition = v.m_minPosition;
        }
        
        void Destroy()
        {
            VectorBase<F>::Destroy();
            m_fMax = 0;
            m_fMin = 0;
            m_maxPosition = 0;
            m_minPosition = 0;
        }

        // 'ctors
        ScalarVector() { }
        ScalarVector(const SampleCount length) :  Vector<F>(length){ }

        ScalarVector(ScalarVector&& v) : Vector<F>(std::move(v)),
                                         m_fMax(v.m_fMax),
                                         m_fMin(v.m_fMin),
                                         m_maxPosition(v.m_maxPosition),
                                         m_minPosition(v.m_minPosition)
        {   }

        ScalarVector(const ScalarVector& v) = delete;
        ~ScalarVector() { Destroy();  }
    };

    /**
     * @brief Useful shortuct to define a vector of samples.
     */
    typedef ScalarVector<Sample>  TSampleVector;  // TBR
    typedef ScalarVector<Sample>  TSamplesVector; // TBR
    typedef ScalarVector<Sample>  SamplesVector;
    
    template<class F>
    void ScalarVector<F>::MakeWindow(const WindowType wt)
    {
        assert(this->m_length > 0);
        
        const Sample len = (Sample)this->m_length;
        SampleCount k = 0;
        
        switch(wt)
        {

            case  Rectangular:
            {
                for(k = 0; k < this->m_length; k++) 
                {
                    this->m_pData[k] = 1.0;
                }
                break;
            }   
            case WT_HANNING:
            {            
                for(k = 0; k < this->m_length; k++) 
                {
                    this->m_pData[k] = (  0.5 
                                        - 0.5 * std::cos((2.0*M_PI*k)/len));
                }
                break;
            }
            
            case BlackmannHarris:
            {
                for(k = 0; k < this->m_length; k++) 
                {
                    const Sample arg = M_PI * k / len;                    
                    this->m_pData[k] = ((0.35875 
                                        - 0.48829 * std::cos(2.0 * arg)
                                        + 0.14128 * std::cos(4.0 * arg)
                                        - 0.01168 * std::cos(6.0 * arg)) 
                                      / 0.35875 );
                }
                break;
            }
            
            case FlatTop:
            {
                const SampleCount k0 = this->m_length / 4;
                const SampleCount k1 = 3 * k0;
                const Sample arg = 2.0 * M_PI * k / len;
                
                for(k = 0; k < k0; k++) 
                {                    
                    this->m_pData[k] = std::sin(arg * k);
                }
                
                for(k = k0; k < k1; k++) 
                {
                    this->m_pData[k] = 1.0;
                }
                
                for(k = k1; k < this->m_length; k++) 
                {
                    this->m_pData[k] = this->m_pData[this->m_length - k - 1];
                }
                break;
            }
            
            case Triangular:
            {
                const SampleCount k0 = this->m_length / 2;
                
                for(k = 0; k < k0; k++) 
                {
                    this->m_pData[k] = 2.0 * k / len;
                }
                for(k = k0; k < this->m_length; k++) 
                {
                    this->m_pData[k] = this->m_pData[this->m_length - k - 1];
                }
                break;
            }
        
            case HalfSine:
            {
                const Sample arg = 2.0 * M_PI * k / len;
                
                for(k = 0; k < this->m_length; k++) 
                {
                    this->m_pData[k] = std::sin(arg * k);
                }
                break;
            }
        }        
    }
    
    template<class F>
    void ScalarVector<F>::RemoveDC()
    {
        const F dc = Mean();
        this->operator-=(dc);
    }
    
    template<class F>
    void ScalarVector<F>::Normalize(F max)
    {
        if (max == 0)
        {
            max = GetAbsMax();
        }
        const F g = 1/max;
        
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] *= g;
        }
    }
    
    template<class F>
    void ScalarVector<F>::Rotate(const SampleCount numSamples)
    {
        assert(numSamples < this->m_length);
        SampleCount first = 0;
        SampleCount mid   = numSamples;
        SampleCount next  = mid;
        F tmp = 0;
        /*
          Algorithm from 
          http://www.cplusplus.com/reference/algorithm/rotate/
        */
        while (first != next)
        {
            // swap
            tmp = this->m_pData[next];
            this->m_pData[next]  = this->m_pData[first];;
            this->m_pData[first] = tmp;
            first++;
            next++;
             
            if (next == this->m_length)
            {
                next = mid;
            }
            else if (first == mid)
            {
                mid = next;
            }
        }
    }
    
    template<class F>
    void ScalarVector<F>::FadeIn(const SampleCount duration,
                                 const int startSample)
    {
        const int len = (int)((duration > this->m_length) ? this->m_length
                                                          : duration);

        for (int i = startSample; i < len; i++)
        {
            this->m_pData[i] *= (1.0 - std::cos(M_PI*i / F(len))) / 2.0;
        }
            
    }

    template<class F>
    void ScalarVector<F>::FadeOut(const SampleCount duration,
                                  const int startSample)
    {
        const int len = (int)((duration > this->m_length) ? this->m_length
                                                          : duration);
        const int from = (startSample < 0 ? (int)this->m_length - len
                                          : startSample);
        for (int i = 0; i < len; i++)
        {
            this->m_pData[from + i] *= (1.0 - std::cos(M_PI*i / F(len))) / 2.0;
        }
    }


    template<class F>
    inline const ScalarVector<F>& ScalarVector<F>::Mac(const ScalarVector& Left, 
                                                       const ScalarVector& Right, 
                                                       const SampleCount length, 
                                                       const SampleCount unFrom)
    {
        // core methods that uses current class as accumulator in wich is stored the operands products.
        for (SampleCount i = 0; i < length; i++)
        {
            this->m_pData[i + unFrom] += (Left.m_pData[i] * Right.m_pData[i]);
        }
        return *this;
    }

    template<class F>
    inline const ScalarVector<F>& ScalarVector<F>::Mac(const ScalarVector& Left, 
                                                       const ScalarVector& Right)
    {
        // core methods that uses current class as accumulator in wich is stored the operands products.
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] += (Left.m_pData[i] * Right.m_pData[i]);
        }
        return *this;
    }

    template<class F>
    inline const ScalarVector<F>& ScalarVector<F>::Mac(const F fLeft, 
                                                       const ScalarVector& Right)
    {
        // core methods that uses current class as accumulator in wich is stored the operands products.
        for (SampleCount i = 0; i < this->m_length; i++)
        {
            this->m_pData[i] += (fLeft * Right.m_pData[i]);
        }
        return *this;
    }

    template<class F>
    F ScalarVector<F>::FindAbsMax(const SampleCount firstNSamples)
    {
        F max = (F)1.0e-6;

        for(SampleCount i = 0; i < (firstNSamples > 0 ? firstNSamples 
                                                      : this->m_length); ++i)
        {
            if (std::fabs(this->m_pData[i]) > max)
            {
                max = std::fabs(this->m_pData[i]);
            }
        }
        return max;
    }

    template<class F>
    F ScalarVector<F>::GetMax(const bool useStoredValue)
    {
        if(m_fMax == MAX_INIT || ! useStoredValue)
        {
            m_fMax = (F)-1.0e12;
            
            for(SampleCount i = 0; i < this->m_length; i++)
            {
                if(this->m_pData[i] > m_fMax)
                {
                    m_fMax = this->m_pData[i];
                    m_maxPosition = i;
                }
            }
        }
        return m_fMax;
    }

    template<class F>
    F ScalarVector<F>::GetMin(const bool useStoredValue)
    {
        if (m_fMin == MIN_INIT || ! useStoredValue)
        {
            m_fMin = (F)1.0e12;
            
            for (SampleCount i = 0; i < this->m_length; i++)
            {
                if (this->m_pData[i] < m_fMin)
                {
                    m_fMin = this->m_pData[i];
                    m_minPosition = i;
                }
            }
        }
        return m_fMin;
    }

    template<class F>
    F ScalarVector<F>::GetAbsMax(const bool useStoredValue)
    {
        F max = GetMax(useStoredValue);
        F min = GetMin(useStoredValue);
        max = std::fabs(max);
        min = std::fabs(min);
        return (max > min) ? max : min;
    }
    
    template<class F>
    F ScalarVector<F>::GetAbsMax(SampleCount& maxIndex, const bool useStoredValue)
    {
        F max = GetMax(useStoredValue);
        F min = GetMin(useStoredValue);
        max = std::fabs(max);
        min = std::fabs(min);
        
        if (min > max) 
        {
            maxIndex = m_minPosition;
            return min;
        }
         maxIndex = m_maxPosition;
         return max;
    }
    
    template<class F>
    F ScalarVector<F>::Mean()
    {
        F avg = 0;

        for (SampleCount i = 0; i < this->m_length; i++)
        {
            avg += this->m_pData[i];
        }
        avg /= (Sample)this->m_length;
        return avg;
    }

    template<class F>
    F ScalarVector<F>::GetSquareRMS(const SampleCount length) const
    {
        SampleCount len = this->m_length;
        
        if (length > 0)
        {
            assert(length <= len);
            len = length;
        }
        
        F rms = 0;
        
        for (SampleCount i = 0; i < len; i++)
        {
            rms += this->m_pData[i] * this->m_pData[i];
        }
        
        return rms / (F)len;
    }

    template<class F>
    F ScalarVector<F>::GetCrossSquareRMS(const ScalarVector<F>& w, const SampleCount length) const
    {
        SampleCount len = (this->m_length < w.Length() ? this->m_length
                                                       : w.Length());
        if (length > 0)
        {
            assert(length <= len);
            len = length;
        }
        
        F rms = 0;
        
        for (SampleCount i = 0; i < len; i++)
        {
            rms += std::fabs(this->m_pData[i] * w.m_pData[i]);
        }
        
        return rms / (F)len;
    }
    
    template<class F>
    ScalarVector<F>& ScalarVector<F>::operator=(ScalarVector&& Right)
    {        
        if (&Right != this)
        {
            Destroy();

            this->m_pData  = Right.m_pData;
            this->m_length = Right.m_length;
            m_fMax   = Right.m_fMax;
            m_fMin   = Right.m_fMin;
            m_maxPosition = Right.m_maxPosition;
            m_minPosition = Right.m_minPosition;

            Right.m_pData = nullptr;                
        }
        return *this;
    }


    /**
     * @brief Extension of the \c Vector class to support matrix item storage
     * It's still a linear array, but there are methods that allows row,column
     * item access.
     */
    
    template <typename T>
    class Matrix
    {
      protected:
          int m_nRows = 0;
          int m_nCols = 0;
          SampleCount m_cellLength = 0;

          std::vector<T> m_array;
          
    public:
        bool IsEmpty() const { return (m_nRows == 0 && m_nCols == 0);  }

        int Rows      () const { return m_nRows; }
        int Columns   () const { return m_nCols; }
        int CellLength() const { return m_cellLength; }
        
        T& Cell(const int row, const int col) // TBR
        {
            return m_array[row * m_nCols + col];
        }
        
        const T& Cell(const int row, const int col) const // TBR
        {
            return m_array[row * m_nCols + col];
        }
        
        void SetCellItem(const int row, const int col, const T& src) // TBR
        {
            assert(row < m_nRows && col < m_nCols);
            T& dst = Cell(row, col);
            dst.Copy(src);
        }
        
        T& operator()(const int row, const int col)
        {
            assert(row >= 0 && row < m_nRows);
            assert(col >= 0 && col < m_nCols);
            return m_array[row * m_nCols + col];
        }

        const T& operator()(const int row, const int col) const
        {
            assert(row >= 0 && row < m_nRows);
            assert(col >= 0 && col < m_nCols);
            return m_array[row * m_nCols + col];
        }

        void ForEach(std::function< void(T&) > f)
        {
            for (auto& cell : m_array)
            {
                f(cell);
            }
        }

        void Clear()
        {
            for (auto it = m_array.begin(); it != m_array.end(); ++it)
            {
                it->Clear();
            }
        }
        
        void Reverse() 
        {
            T supportBuffer(m_cellLength);

            for (auto& cell : m_array)
            {
                cell.Reverse(supportBuffer);
            }
        }
        
        /**
         * @brief Resize the matrix.
         * This method does not preserve the data integrity!
         */
        void Resize(const int rows, 
                    const int columns, 
                    const SampleCount cellLength)
        {
            assert(rows > 0 && columns > 0 && cellLength > 0);

            m_nRows = rows;
            m_nCols = columns;
            m_cellLength = cellLength;

            m_array.resize(m_nRows * m_nCols);

            for (auto& item : m_array)
            {
                item.Resize(m_cellLength);
            }
        }
        
        void Destroy()
        {
            m_array.clear();
            m_nRows = 0;
            m_nCols = 0;
            m_cellLength = 0;
        }
        
        Matrix() { }
        Matrix(const int rows, const int columns, const SampleCount cellLength)
        { 
            Resize(rows, columns, cellLength); 
            
        }        
    };
} // namespace Aurora

#endif //__AF_VECTOR_H__

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
