/*************s*********************************************************
 *
 *  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 *                       plugin collection
 *
 *  Common libraries
 *
 *  afcomplex.cpp
 *
 *  Simone Campanini 2011-20
 *
 **********************************************************************/
#include <aurora.h>

// Specialized methods
void Aurora::ComplexVectorBase::Dump(const char* filename) const
{
    char path[256];
    sprintf(path, "Z:\\dump\\%s.m", filename);

    FILE* f = fopen(path, "w");

    if (f)
    {
        fprintf(f, "%s_ = [", filename);

        for (Aurora::SampleCount i = 1; i < m_length; i++)
        {
            fprintf(f, " %.4f %+.4fi ", m_pData[i].r, m_pData[i].i);
        }
        fprintf(f, "];\n");
        fclose(f);
    }
}



Aurora::ComplexVectorBase& Aurora::ComplexVectorBase::operator=(Aurora::ComplexVectorBase&& Right)
{
    if (&Right != this)
    {
        Destroy();

        m_pData    = Right.m_pData;
        m_length   = Right.m_length;

        Right.m_pData = nullptr;
    }
    return *this;
}

// -------------------------------------------------- Template class definitions


inline const Aurora::ComplexVector& Aurora::ComplexVector::Mac(const Aurora::ComplexVector& Left, 
                                                               const Aurora::ComplexVector& Right,
                                                               const Aurora::SampleCount length, 
                                                               const Aurora::SampleCount from)
{
    // core methods that uses current class as accumulator in wich is stored the operands products.
    for(Aurora::SampleCount i = 0; i < length; i++)
    {
        Re(m_pData[i + from]) +=   (Re(Left.m_pData[i]) * Re(Right.m_pData[i])
                                  - Im(Left.m_pData[i]) * Im(Right.m_pData[i]));
        Im(m_pData[i + from]) +=   (Re(Left.m_pData[i]) * Im(Right.m_pData[i])
                                  + Re(Left.m_pData[i]) * Im(Right.m_pData[i]));
    }
    return *this;
}

inline const Aurora::ComplexVector& Aurora::ComplexVector::Mac(const Aurora::ComplexVector& Left, 
                                                               const Aurora::ComplexVector& Right)
{
    // core methods that uses current class as accumulator in wich is stored the operands products.
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) +=   (Re(Left.m_pData[i]) * Re(Right.m_pData[i])
                           - Im(Left.m_pData[i]) * Im(Right.m_pData[i]));
        Im(m_pData[i]) +=   (Re(Left.m_pData[i]) * Im(Right.m_pData[i])
                           + Re(Left.m_pData[i]) * Im(Right.m_pData[i]));
    }
    return *this;
}

inline const Aurora::ComplexVector& Aurora::ComplexVector::Mac(const float fLeft, 
                                                               const Aurora::ComplexVector& Right)
{
    // core methods that uses current class as accumulator in wich is stored the operands products.
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) += ( fLeft * Re(Right.m_pData[i]) );
        Im(m_pData[i]) += ( fLeft * Im(Right.m_pData[i]) );
    }
    return *this;
}

// operators
Aurora::ComplexVector Aurora::ComplexVector::operator+(const Aurora::ComplexVector& Right)
{
    Aurora::ComplexVector V(m_length);

    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(V.m_pData[i]) = Re(m_pData[i]) + Re(Right.m_pData[i]);
        Im(V.m_pData[i]) = Im(m_pData[i]) + Im(Right.m_pData[i]);
    }
    return V;
}

Aurora::ComplexVector Aurora::ComplexVector::operator-(const Aurora::ComplexVector& Right)
{
    Aurora::ComplexVector V(m_length);

    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(V.m_pData[i]) = Re(m_pData[i]) - Re(Right.m_pData[i]);
        Im(V.m_pData[i]) = Im(m_pData[i]) - Im(Right.m_pData[i]);
    }
    return V;
}

Aurora::ComplexVector Aurora::ComplexVector::operator*(const Aurora::ComplexVector& Right)
{
    Aurora::ComplexVector V(m_length);
    
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(V.m_pData[i]) =   Re(m_pData[i]) * Re(Right.m_pData[i])
                           - Im(m_pData[i]) * Im(Right.m_pData[i]);
        Im(V.m_pData[i]) =   Re(m_pData[i]) * Im(Right.m_pData[i])
                           + Re(m_pData[i]) * Im(Right.m_pData[i]);
    }
    return V;
}

Aurora::ComplexVector Aurora::ComplexVector::operator/(const Aurora::ComplexVector& Right)
{
    Aurora::ComplexVector V(m_length);
    
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        float den = SqrMod(Right.m_pData[i]);

        if(den != 0.0)
        {
            Re(V.m_pData[i]) = (  Re(m_pData[i])*Re(Right.m_pData[i])
                                + Im(m_pData[i])*Im(Right.m_pData[i])) / den;
            Im(V.m_pData[i]) = (  Im(m_pData[i])*Re(Right.m_pData[i])
                                - Re(m_pData[i])*Im(Right.m_pData[i])) / den;
        }
        else
        {
            //A.Farina: if smpDen == 0, cannot perform division. We assume the result is 0.
            Re(V.m_pData[i]) = 0;
            Im(V.m_pData[i]) = 0;
        }
    }
    return V;
}

Aurora::ComplexVector Aurora::ComplexVector::operator+(const float fRight)
{
    Aurora::ComplexVector V(m_length);

    for (Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(V.m_pData[i]) = Re(m_pData[i]) + fRight;
    }
    return V;
}

Aurora::ComplexVector Aurora::ComplexVector::operator-(const float fRight)
{
    Aurora::ComplexVector V(m_length);

    for (Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(V.m_pData[i]) = Re(m_pData[i]) - fRight;
    }
    return V;
}

Aurora::ComplexVector Aurora::ComplexVector::operator*(const float fRight)
{
    Aurora::ComplexVector V(m_length);

    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(V.m_pData[i]) = Re(m_pData[i]) * fRight;
        Im(V.m_pData[i]) = Im(m_pData[i]) * fRight;
    }
    return V;
}

Aurora::ComplexVector Aurora::ComplexVector::operator/(const float fRight)
{
    Aurora::ComplexVector V(m_length);

    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(V.m_pData[i]) = Re(m_pData[i]) / fRight;
        Im(V.m_pData[i]) = Im(m_pData[i]) / fRight;
    }
    return V;
}

Aurora::ComplexVector& Aurora::ComplexVector::operator+=(const Aurora::ComplexVector& Right)
{
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) = Re(m_pData[i]) + Re(Right.m_pData[i]);
        Im(m_pData[i]) = Im(m_pData[i]) + Im(Right.m_pData[i]);
    }
    return *this;
}

Aurora::ComplexVector& Aurora::ComplexVector::operator-=(const Aurora::ComplexVector& Right)
{
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) = Re(m_pData[i]) - Re(Right.m_pData[i]);
        Im(m_pData[i]) = Im(m_pData[i]) - Im(Right.m_pData[i]);
    }
    return *this;
}

Aurora::ComplexVector& Aurora::ComplexVector::operator*=(const Aurora::ComplexVector& Right)
{
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) =   Re(m_pData[i]) * Re(Right.m_pData[i])
                         - Im(m_pData[i]) * Im(Right.m_pData[i]);
        Im(m_pData[i]) =   Re(m_pData[i]) * Im(Right.m_pData[i])
                         + Re(m_pData[i]) * Im(Right.m_pData[i]);
    }
    return *this;
}

Aurora::ComplexVector& Aurora::ComplexVector::operator/=(const Aurora::ComplexVector& Right)
{
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        auto den = SqrMod(Right.m_pData[i]);

        if(den != 0.0f)
        {
            Re(m_pData[i]) =   (Re(m_pData[i])*Re(Right.m_pData[i])
                              + Im(m_pData[i])*Im(Right.m_pData[i])) / den;
            Im(m_pData[i]) =   (Im(m_pData[i])*Re(Right.m_pData[i])
                              - Re(m_pData[i])*Im(Right.m_pData[i])) / den;
        }
        else
        {
            //A.Farina: if smpDen == 0, cannot perform division. We assume the result is 0.
            Re(m_pData[i]) = 0;
            Im(m_pData[i]) = 0;
        }
    }
    return *this;
}

inline Aurora::ComplexVector& Aurora::ComplexVector::operator+=(const float fRight)
{
    for (Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) = Re(m_pData[i]) + fRight;
    }
    return *this;
}

inline Aurora::ComplexVector& Aurora::ComplexVector::operator-=(const float fRight)
{
    for (Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) = Re(m_pData[i]) - fRight;
    }
    return *this;
}

inline Aurora::ComplexVector& Aurora::ComplexVector::operator*=(const float fRight)
{
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) = Re(m_pData[i]) * fRight;
        Im(m_pData[i]) = Im(m_pData[i]) * fRight;
    }
    return *this;
}

inline Aurora::ComplexVector& Aurora::ComplexVector::operator/=(const float fRight)
{
    for(Aurora::SampleCount i = 0; i < m_length; i++)
    {
        Re(m_pData[i]) = Re(m_pData[i]) / fRight;
        Im(m_pData[i]) = Im(m_pData[i]) / fRight;
    }
    return *this;
}

float Aurora::ComplexVector::Module(const Aurora::SampleCount idx) const 
{ 
    return Mod(m_pData[idx]);
}
        
float Aurora::ComplexVector::Argument(const Aurora::SampleCount idx) const 
{ 
    return Phase(m_pData[idx]);
}
        
float Aurora::ComplexVector::SquareModule(const Aurora::SampleCount idx) const 
{ 
    return  SqrMod(m_pData[idx]);
}
        
float Aurora::ComplexVector::SquareReal(const Aurora::SampleCount idx) const
{
    return  Re(m_pData[idx]) * Re(m_pData[idx]);
}

float Aurora::ComplexVector::SquareImag(const Aurora::SampleCount idx) const
{
    return  Re(m_pData[idx]) * Re(m_pData[idx]);
}

float Aurora::ComplexVector::FindAbsMax(Aurora::SampleCount& maxIndex) const
{
    float mod = 0.0f;
    float max = 1e-12f;
    
    for (size_t i = 0; i < m_length; i++)
    {
        mod = Mod(m_pData[i]);
        
        if (mod > max)
        {
            max = mod;
            maxIndex = i;
        }
    }
    return max;
}

void Aurora::ComplexVector::ForEach(const Aurora::Range& range,
                                    std::function<void(const size_t i, float& real, float& imag)> handler)
{
    if (range.first == range.second)
    {
        return;
    }
    assert(range.second > range.first && range.second <= m_length);

    for (Aurora::SampleCount i = range.first; i <= range.second; ++i)
    {
        handler(i, m_pData[i].r, m_pData[i].i);
    }
}

void Aurora::ComplexVector::StoreModule(float* dest)
{
    for (size_t i = 0; i < m_length; i++)
    {
        dest[i] = Mod(m_pData[i]);
    }
}

void Aurora::ComplexVector::StoreModule(float* dest,
                                        const Aurora::SampleCount length,
                                        const Aurora::SampleCount from)
{
    for (size_t i = from; i < from + length; i++)
    {
        dest[i] = Mod(m_pData[i]);
    }
}

void Aurora::ComplexVector::StorePhase(float* dest)
{
    for (size_t i = 0; i < m_length; i++)
    {
        dest[i] = Phase(m_pData[i]);
    }
}

void Aurora::ComplexVector::StorePhase(float* dest,
                                       const Aurora::SampleCount length,
                                       const Aurora::SampleCount from)
{
    for (size_t i = from; i < from + length; i++)
    {
        dest[i] = Phase(m_pData[i]);
    }
}

/* ----------------------------------------------------------------------------
 * ComplexSpectrum class implementation
 * -------------------------------------------------------------------------- */

Aurora::ComplexSpectrum Aurora::ComplexSpectrum::operator*(const Aurora::ComplexSpectrum& Right)
{
    Aurora::ComplexSpectrum V(m_length);

    // At DC, and Fnyquist, we have only real part
    V.SetDC     ( DC()*Right.DC() );
    V.SetNyquist( Ny()*Right.Ny() );
    
    for(Aurora::SampleCount i = 1; i < (m_length-1); i++)
    {
        Re(V.m_pData[i]) = Re(m_pData[i]) * Re(Right.m_pData[i]) - Im(m_pData[i]) * Im(Right.m_pData[i]);
        Im(V.m_pData[i]) = Re(m_pData[i]) * Im(Right.m_pData[i]) + Re(m_pData[i]) * Im(Right.m_pData[i]);
    }
    return V;
}

Aurora::ComplexSpectrum Aurora::ComplexSpectrum::operator/(const Aurora::ComplexSpectrum& Right)
{
    Aurora::ComplexSpectrum V(m_length);
    
    // At DC, and Fnyquist, we have only real part
    V.SetDC     ( (Right.DC() != 0) ? DC() / Right.DC() : DC() / std::exp(-32.0f)) ;
    V.SetNyquist( (Right.Ny() != 0) ? Ny() / Right.Ny() : Ny() / std::exp(-32.0f)) ;
    
    for(Aurora::SampleCount i = 1; i < (m_length-1); i++)
    {
        float den = SqrMod(Right.m_pData[i]);

        if(den != 0.0)
        {
            Re(V.m_pData[i]) = (  Re(m_pData[i])*Re(Right.m_pData[i])
                                + Im(m_pData[i])*Im(Right.m_pData[i])) / den;
            Im(V.m_pData[i]) = (  Im(m_pData[i])*Re(Right.m_pData[i])
                                - Re(m_pData[i])*Im(Right.m_pData[i])) / den;
        }
        else
        {
            //A.Farina: if smpDen == 0, cannot perform division. We assume the result is 0.
            Re(V.m_pData[i]) = 0;
            Im(V.m_pData[i]) = 0;
        }
    }
    return V;
}

Aurora::ComplexSpectrum& Aurora::ComplexSpectrum::operator/=(const Aurora::ComplexSpectrum& Right)
{
    // At DC, and Fnyquist, we have only real part
    SetDC     ( (Right.DC() != 0) ? DC() / Right.DC() : DC() / std::exp(-32.0f) );
    SetNyquist( (Right.Ny() != 0) ? Ny() / Right.Ny() : Ny() / std::exp(-32.0f) );
    
    for(Aurora::SampleCount i = 1; i < (m_length-1); i++)
    {
        auto den = SqrMod(Right.m_pData[i]);

        if(den != 0.0f)
        {
            Re(m_pData[i]) = (Re(m_pData[i])*Re(Right.m_pData[i]) + Im(m_pData[i])*Im(Right.m_pData[i])) / den;
            Im(m_pData[i]) = (Im(m_pData[i])*Re(Right.m_pData[i]) - Re(m_pData[i])*Im(Right.m_pData[i])) / den;
        }
        else
        {
            //A.Farina: if smpDen == 0, cannot perform division. We assume the result is 0.
            Re(m_pData[i]) = 0;
            Im(m_pData[i]) = 0;
        }
    }
    return *this;
}

void Aurora::ComplexSpectrum::ForEachSpectralBin(std::function<void(const size_t i,
                                                                    float& real,
                                                                    float& imag)> handler)
{
    // apply handler to every sample except for DC and Nyquist frequency bin
    
    for (size_t i = 1; i < m_length - 1; ++i)
    {
        handler(i, m_pData[i].r, m_pData[i].i);
    }
}

float Aurora::ComplexSpectrum::GetSamplerate() const
{
    assert(m_nyFrequency > 0);
    return 2.0f * m_nyFrequency;
}

float Aurora::ComplexSpectrum::GetNyquistFrequency() const
{
    assert(m_nyFrequency > 0);
    return m_nyFrequency;
}

float Aurora::ComplexSpectrum::GetFrequencyBinsWidth() const
{
    assert(m_nyFrequency > 0);
    return (float)m_length / m_nyFrequency;
}

/* ----------------------------------------------------------------------------
 * ComplexVectorPhase class implementation
 * -------------------------------------------------------------------------- */
 void Aurora::PolarComplexVector::ToPolar(const Aurora::ComplexSpectrum& src)
 {
     /*
      * Here the complex type is reinterpretated from Re,Im, to Mag,Phase
      */
     Resize(src.Length());
     
     for(Aurora::SampleCount i = 0; i < src.Length(); i++)
     {
         MagPol(m_pData[i]) =  src.Magn(i);
         PhPol(m_pData[i])  = -src.Phas(i); // why '-' ????
     }
 }

 void Aurora::PolarComplexVector::ToRectangular(Aurora::ComplexSpectrum& dest)
 {
     /*
      * Here the complex type is reinterpretated from Re,Im, to Mag,Phase
      * Note: this functions comes from InvertKirkeby, so it's tailored on
      * the plugins' needings
      */
     
     // Devo verificare se la fase della componente DC o Fnyq è pi greca,
     // o multiplo intero se la fase non è un multiplo intero di 2pi allora
     // la assumo negativa.
     dest.Resize(m_length);
     
     const float pi = (float)M_PI;

     //a freq. zero ho solo componente reale
     dest.SetDC(0.0);

     if(    (std::fmod(PhPol(m_pData[0]), pi) > 1.0)
         && (std::fmod(PhPol(m_pData[0]), pi) < 3.0) )
     {
         dest.SetDC(-MagPol(m_pData[0])); //cioe' se il resto e' non zero,
     }
     else
     {
         dest.SetDC(MagPol(m_pData[0]));
     }
     
     //alla freq. di Nyquist, ho solo valore reale, la Fnyq e' il secondo
     // elemento di h e l'ultimo di hpm
     
     dest.SetNyquist(0.0);

     if(    (std::fmod(PhPol(m_pData[m_length-1]), pi) > 1.0)
         && (std::fmod(PhPol(m_pData[m_length-1]), pi) < 3.0) )
     {
         dest.SetNyquist(-MagPol(m_pData[m_length-1])); // cioe' se il resto
                                                        // non e' zero,
     }
     else
     {
         dest.SetNyquist(MagPol(m_pData[m_length-1]));
     }
                   
     for(Aurora::SampleCount i = 1; i < (m_length-1); i++)   // altre frequenze, conto fino ad L2
     {
         dest.SetReal(i, MagPol(m_pData[i]) * std::cos(PhPol(m_pData[i])));
         dest.SetImag(i, MagPol(m_pData[i]) * std::sin(PhPol(m_pData[i])));
     }
 }


void Aurora::PolarComplexVector::UnwrapPhase()
{
    
    //Aurora::SampleCount smpcN = length/2; // Farina's
    
    // W conterrà i valori della fase modulo 2pi
    
    float* fUnwrappedPhase = new float [m_length];
    memset(fUnwrappedPhase, 0, m_length * sizeof(float));
    
    const float fTwoPi = float(2.0*M_PI);
    Aurora::SampleCount i;
    
    //Ricerco il minimo del vettore
    float fMinPhase = PhPol(m_pData[0]);   // TODO: Is this the correct initialization??

    for (i = 0; i < m_length; i++)
    {
        fMinPhase = (PhPol(m_pData[i]) < fMinPhase) ? PhPol(m_pData[i])
                                                    : fMinPhase;
    }

    for(i = 0; i < m_length; i++)
    {
        auto fPhaseDiff = (PhPol(m_pData[i]) - fMinPhase)/fTwoPi;
        
        if (fPhaseDiff > 0)   //se e' positivo, allora devo usare il floor, cioe' l'int + piccolo
        {
            fUnwrappedPhase[i] = float(PhPol(m_pData[i]) - fTwoPi * floor(fPhaseDiff));
        }
        else if (fPhaseDiff < 0) //se e' negativo, devo usare ceil, cioe' l'int +grande
        {
            fUnwrappedPhase[i] = float(PhPol(m_pData[i]) - fTwoPi * ceil(fPhaseDiff));
        }
        else
        {
            fUnwrappedPhase[i] = fMinPhase; // cioe' sono nel caso "zero"
        }
    }
    
    //Calcolo le differenze tra termini cosecutivi, cioe': [X(2)-X(1)  X(3)-X(2) ... X(n)-X(n-1)]
    float fHop = 0, fPrevHop = 0;
    
    if (fUnwrappedPhase[0] > fTwoPi)
    {
        fPrevHop = -fTwoPi;
    }
    else if (fUnwrappedPhase[0] < fTwoPi)
    {
        fPrevHop = fTwoPi;
    }
    PhPol(m_pData[0]) = fUnwrappedPhase[0] + fPrevHop;
    
    for(i = 1; i < m_length; i++)
    {
        //Cerco i salti, costruisco l'array dei salti,
        //sommo la fase alla correzione e
        //sovrascrivo il vettore in uscita
        
        if ((fUnwrappedPhase[i] - fUnwrappedPhase[i - 1]) > fTwoPi)
        {
            fHop = fPrevHop - fTwoPi;
        }
        else if ((fUnwrappedPhase[i] - fUnwrappedPhase[i - 1]) < fTwoPi)
        {
            fHop = fPrevHop + fTwoPi;
        }
        PhPol(m_pData[i]) = fUnwrappedPhase[i] + fHop;
        fPrevHop = fHop;
    }
}

void Aurora::PolarComplexVector::StoreModule(float* dest)
{
    for (size_t i = 0; i < m_length; i++)
    {
        dest[i] = MagPol(m_pData[i]);
    }
}

void Aurora::PolarComplexVector::StoreModule(float* dest,
                                             const Aurora::SampleCount length,
                                             const Aurora::SampleCount from)
{
    for (size_t i = from; i < from + length; i++)
    {
        dest[i] = MagPol(m_pData[i]);
    }
}

void Aurora::PolarComplexVector::StorePhase(float* dest)
{
    for (size_t i = 0; i < m_length; i++)
    {
        dest[i] = PhPol(m_pData[i]);
    }
}

void Aurora::PolarComplexVector::StorePhase(float* dest,
                                            const Aurora::SampleCount length,
                                            const Aurora::SampleCount from)
{
    for (size_t i = from; i < from + length; i++)
    {
        dest[i] = PhPol(m_pData[i]);
    }
}

float Aurora::PolarComplexVector::Module(const Aurora::SampleCount idx) const
{
    return MagPol(m_pData[idx]);
}
        
float Aurora::PolarComplexVector::Argument(const Aurora::SampleCount idx) const
{
    return  PhPol(m_pData[idx]);
}
        
float Aurora::PolarComplexVector::SquareModule(const Aurora::SampleCount idx) const
{
    return MagPol(m_pData[idx]) * MagPol(m_pData[idx]);
}
       
float Aurora::PolarComplexVector::SquareReal(const Aurora::SampleCount idx) const
{
    const float re = RePol(m_pData[idx]);
    return  re * re;
}

float Aurora::PolarComplexVector::SquareImag(const Aurora::SampleCount idx) const
{
    const float im = ImPol(m_pData[idx]);
    return  im * im;
}

float Aurora::PolarComplexVector::FindAbsMax(Aurora::SampleCount& maxIndex) const
{
    float mod = 0.0f;
    float max = -1e12f;
    
    for (size_t i = 0; i < m_length; i++)
    {
        mod = MagPol(m_pData[i]);
        
        if (mod > max)
        {
            max = mod;
            maxIndex = i;
        }
    }
    return max;
}

