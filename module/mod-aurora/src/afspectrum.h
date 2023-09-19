/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common modules

  afspectrum.h

  Simone Campanini - 9/5/20

***********************************************************************/
#ifndef __AURORA_SPECTRUM_H__
#define __AURORA_SPECTRUM_H__

#include <array>
#include <map>

namespace Aurora
{
    /**
     * @brief The supported spectrum types
     */
    enum SpectrumType
    {
        Octave,
        ThirdOctave
    };

    /**
     * @brief The spectrum center band frequency indices. They should not
     * be used normally.
     */
    enum SpectrumBand
    {
        Octave_f31_5 = 0,
        Octave_f63,
        Octave_f125,
        Octave_f250,
        Octave_f500,
        Octave_f1k,
        Octave_f2k,
        Octave_f4k,
        Octave_f8k,
        Octave_f16k,

        Octave_fA,
        Octave_fLin,

        ThirdOctave_f25 = 0, ThirdOctave_f31_5,
        ThirdOctave_f40,     ThirdOctave_f50,    ThirdOctave_f63,
        ThirdOctave_f80,     ThirdOctave_f100,   ThirdOctave_f125,
        ThirdOctave_f160,    ThirdOctave_f200,   ThirdOctave_f250,
        ThirdOctave_f315,    ThirdOctave_f400,   ThirdOctave_f500,
        ThirdOctave_f630,    ThirdOctave_f800,   ThirdOctave_f1k,
        ThirdOctave_f1k25,   ThirdOctave_f1k6,   ThirdOctave_f2k,
        ThirdOctave_f2k5,    ThirdOctave_f3k15,  ThirdOctave_f4k,
        ThirdOctave_f5k,     ThirdOctave_f6k3,   ThirdOctave_f8k,
        ThirdOctave_f10k,    ThirdOctave_f12k5,  ThirdOctave_f16k,
        ThirdOctave_f20k,

        ThirdOctave_fA,
        ThirdOctave_fLin,
    };

    /**
     * @brief The supported spectrum averages.
     */
    enum SpectrumAverage

    {
        Aweighted = 0,
        Linear
    };

    /**
     * @brief The spectrum class. It should support any data type stored as
     * spectral values.
     */
    template <typename T>
    class Spectrum 
    {
      public:
        /**
         * @brief Encapsulates a float value and let the user do some
         * useful checks...to be extended....
         */
          class Band
          {
              float m_fcb = 0.0;
              static const float ms_aweightedBin;
              static const float ms_linearBin;

          public:

              /**
               * @brief Returns the (symbolic) frequency bin associated to
               * Aweighted band.
               */
              static float AweightedBin();

              /**
               * @brief Returns the (symbolic) frequency bin associated to
               * Linear band.
               */
              static float LinearBin();

              bool IsAweighted() const { return (m_fcb == ms_aweightedBin); }
              bool IsLinear()    const { return (m_fcb == ms_linearBin); }

              bool IsAverage()   const { return m_fcb <= 1.0f; }
              bool IsValid()     const { return m_fcb >= 0.0f && m_fcb <= 20000.0f; }

              /**
               * @brief Make a string containing a compact label for the
               * stored frequency (i.e. '2k' instead of 2000).
               * @param replaceDotsWithUnderscores uses '_' instead of the dot as
               * decimal separator.
               * @return The label as wxString.
               */
              wxString GetShortLabel(const bool replaceDotsWithUnderscores = false)
              {
                  if (IsAweighted())
                  {
                      return wxString("A");
                  }
                  if (IsLinear())
                  {
                      return wxString("Lin");
                  }
                  return Aurora::GetBandShortLabel(m_fcb, replaceDotsWithUnderscores);
              }

              /**
               * @brief Make a string containing a label for the stored frequency.
               * @param replaceDotsWithUnderscores uses '_' instead of the dot as
               * decimal separator.
               * @return The label as wxString.
               */
              wxString GetLabel(const bool replaceDotsWithUnderscores = false)
              {
                  if (IsAweighted())
                  {
                      return wxString("A");
                  }
                  if (IsLinear())
                  {
                      return wxString("Lin");
                  }
                  return Aurora::GetBandLabel(m_fcb, replaceDotsWithUnderscores);
              }

              operator float() { return m_fcb; }

              Band& operator=(const float fcb)
              {
                  m_fcb = fcb;
                  return *this;
              }

              void Set(const float fcb) { m_fcb = fcb; }

              void Set(const wxString& fcbLabel)
              {
                  std::string s = fcbLabel.ToStdString();
                  double value = 0.0;

                  if (s[0] == 'A')
                  {
                      value = ms_aweightedBin;
                  }
                  else if (s[0] == 'L')
                  {
                      value = ms_linearBin;
                  }
                  else
                  {
                      double mult = 1.0;
                      
                      if (s.back() == 'k')
                      {                        
                          mult = 1000.0;
                          s.pop_back();
                      }
                      value = std::stod(s);
                      assert(value > 0.0);
                      value *= mult;
                  }
                  m_fcb = (float)value;
              }

              Band() { }
              Band(const float fcb) : m_fcb(fcb) { }
              Band(const wxString& fcbLabel) { Set(fcbLabel);  }
        };

      protected:
         static const std::vector<float> m_octaveBandsFcb;         
         static const std::vector<float> m_thirdOctaveBandsFcb; 
         static const std::vector<std::string> m_averageTypes;
        
        int m_type = SpectrumType::Octave;
        
        std::pair<size_t, size_t>  m_range  = { SpectrumBand::Octave_f31_5, 
                                                SpectrumBand::Octave_f16k };

        std::map<float, T>        m_values;
        std::map<std::string, T>  m_averages;  // A, Lin
        
      public:
                
        /**
         * @brief Destroy the table values and re-initialize it with the given
         * settings.
         */
        virtual void Reset()
        {
            m_values.clear();

            if (m_type == SpectrumType::ThirdOctave)
            {
                m_range.first  = SpectrumBand::ThirdOctave_f25;
                m_range.second = SpectrumBand::ThirdOctave_f20k;
            }
            else
            {
                m_range.first  = SpectrumBand::Octave_f31_5;
                m_range.second = SpectrumBand::Octave_f16k;
            }
            Clear();
        }
        
        /**
         * @brief Initialize to zero all values.
         */
        void Clear()
        {
            const auto& fcbs = Frequencies();
    
            for (auto fcb : fcbs)
            {
                m_values[fcb] = T();
            }

            for (auto& at : m_averageTypes)
            {
                m_averages[at] = T();
            }
        }
        
        /**
         * @brief Return the first center band frequency of the spectrum.
         */
        float Begin() const
        {
            const auto& fcbs = Frequencies();
            return fcbs[m_range.first];
        }

        /**
         * @brief Return the last center band frequency of the spectrum.
         */
        float  End()   const
        {
            const auto& fcbs = Frequencies();
            return fcbs[m_range.second];    
        }
        
        /**
         * @brief Return the number of bands of the spectrum.
         */
        size_t Length() const
        {
            return m_range.second - m_range.first + 1;
        }

        /**
         * @brief Perform a loop over the spectrum bands.
         * @param includeAverages Include or not the Aweighted and Linear averages
         * in the loop
         * @param handler The function that will be called for every spectrum
         * band; it takes as parameters the band spectrum index \c i, the center 
         * band frequency \c fcb and a reference to the spectrum value.
         */
        void ForEach(const bool includeAverages,
                    std::function<void(const size_t i, const float fcb, T& value)> handler)
        {
            const auto& fcbs = Frequencies();
    
            for (size_t i = m_range.first; i <= m_range.second; ++i)
            {
                handler(i, fcbs[i], m_values[ fcbs[i] ]);
            }
            
            if (includeAverages)
            {
                size_t i = fcbs.size();
                
                handler(i,     0.0, m_averages["A"]); // A
                handler(i + 1, 1.0, m_averages["Lin"]); // Lin
            }
        }

        /**
         * @brief Perform a breakable loop over the spectrum bands.
         * @param includeAverages Include or not the Aweighted and Linear averages
         * in the loop
         * @param handler The function that will be called for every spectrum
         * band; it takes as parameters the band spectrum index \c i, the center 
         * band frequency \c fcb and a reference to the spectrum value. If the
         * handler returns \c false, the loop will be interrupted and the
         * function returns.
         */
        void ForEachBreakable(const bool includeAverages,
                              std::function<bool(const size_t i, const float fcb, T& value)> handler)
        {
            const auto& fcbs = Frequencies();
    
            for (size_t i = m_range.first; i <= m_range.second; ++i)
            {
                if (! handler(i, fcbs[i], m_values[ fcbs[i] ]))
                { 
                    return;
                }
            }
            
            if (includeAverages)
            {
                size_t i = fcbs.size();
                
                if (! handler(i, 0.0, m_averages["A"])) // A
                {
                    return;
                }
                
                if (! handler(i + 1, 1.0, m_averages["Lin"])) // Lin
                {
                    return;
                }
            }
        }
        
        /**
         * @brief Returns a vector containing a subset of the full center band
         * frequency vector obtained applying the chosen range.
         */
        std::vector<float> RangedFrequencies() const
        {
            std::vector<float> subset;
            const auto& fcbs = Frequencies();
            
            for (size_t i = m_range.first; i <= m_range.second; ++i)
            {
                subset.push_back(fcbs[i]);
            }    
            return subset;
        }
        
        /**
         * @brief Returns a vector with the full center band frequency range for
         * the chosen spectrum type.
         */
        const std::vector<float>& Frequencies() const
        {
            return m_type == Aurora::SpectrumType::ThirdOctave ? m_thirdOctaveBandsFcb
                                                               : m_octaveBandsFcb;
        }

        /**
         * @brief Returns a vector containing the available average types.
         */
        const std::vector<std::string>& AverageTypes() const
        {
            return m_averageTypes;
        }

        /**
         * @brief Return corresponding band value.
         * @param fcb Center band frequency. No control on value consistence.
         */
        T& operator[](const float fcb)
        {
            assert(fcb > 1.0);
            return m_values.at(fcb);
        }

        /**
         * @brief Return corresponding band value (Read only)
         * @param fcb Center band frequency. No control on value consistence.
         */
        const T& operator[](const float fcb) const
        {
            assert(fcb > 1.0);
            return m_values.at(fcb);
        }

        /**
         * @brief Return band value that corresponds to given index
         * @param index Band index.
         */
        T& operator[](const SpectrumBand index)
        {
            const auto& fcbs = Frequencies();
            return m_values.at(fcbs[index]);
        }

        /**
         * @brief Return corresponding band value.
         * @param fcb Center band frequency. If fcb = 0.0, it will returns A weighted,
         * if fcb = 1.0, it will returns linear.
         */
        const T& GetValue(const float fcb) const
        {
            Band b(fcb);

            if (b.IsAweighted())
            {
                return m_averages.at("A");
            }
            
            if (b.IsLinear())
            {
                return m_averages.at("Lin");
            }
            const auto& fcbs = Frequencies();
            
            assert(fcb >= fcbs[m_range.first] || fcb >= fcbs[m_range.second]);
            return m_values.at(fcb);
        }

        /**
         * @brief Set corresponding band value.
         * @param fcb Center band frequency. If fcb = 0.0, it will set A weighted
         * value, if fcb = 1.0, it will returns set linear value.
         */
        void SetValue(const float fcb, const T& value)
        {
            Band b(fcb);

            if (b.IsAweighted())
            {
                m_averages.at("A") = value; 
                return;
            }
            
            if (b.IsLinear())
            {
                m_averages.at("Lin") = value; 
                return;
            } 

            const auto& fcbs = Frequencies();
            assert(fcb >= fcbs[m_range.first] && fcb <= fcbs[m_range.second]);
            m_values[fcb] = value;
        }

        /**
         * @brief Returns one of the available average values.
         * @param type The average to return ("A" or "Lin").
         */
        const T& GetAverageValue(const std::string& type) const
        {
            assert(type == "A" || type == "Lin");
            return m_averages.at(type);
        }
                
        /**
         * @brief Set one of the available average values.
         * @param type The average to set ("A" or "Lin").
         * @param value The value to assign.
         */
        void  SetAverageValue(const std::string& type, const T& value)
        {
            assert(type == "A" || type == "Lin");
            m_averages[type] = value;
        }

        /**
         * @brief Set the spectrum range subset and re-initializes it.
         * @param fcbStart The first center band frequency of the spectrum.
         * @param fcbEnd The last center band frequency of the spectrum.
         */
        void SetRange(const float fcbStart, const float fcbEnd)
        {
            assert(fcbStart < fcbEnd);
            const auto& fcbs = Frequencies();
            size_t i = 0;
            
            for( ; i < fcbs.size(); ++i)
            {
                if (fcbs[i] == fcbStart)
                {
                    m_range.first = i;
                    break;
                }
            }
            assert(i < fcbs.size());

            for( ; i < fcbs.size(); ++i)
            {
                if (fcbs[i] == fcbEnd)
                {
                    m_range.second = i;
                    break;
                }
            }
            assert(m_range.first < m_range.second);
            
            Clear();
        }
        
        /**
         * @brief Set spectru, type and re-initializes it.
         * @param type One of the supported types: \c Type::Octave or 
         * \c Type::ThirdOctave.
         */
        void SetType(const SpectrumType type)
        {
            m_type = type;
            Reset();
        }

        /**
         * @brief Copy operator.
         */
        Spectrum& operator=(const Spectrum& s)
        {
            m_type     = s.m_type;
            m_range    = s.m_range;
            m_values   = s.m_values;
            m_averages = s.m_averages;
            return *this;
        }
        
        /**
         * @brief The default constructor.
         */
        Spectrum()
        {
            Reset(); 
        }
        
        /**
         * @brief The copy constructor.
         */
        Spectrum(const Spectrum& s)
        :  m_type(s.m_type),
            m_range(s.m_range),
            m_values(s.m_values),
            m_averages(s.m_averages)
        { }
        
        /**
         * @brief Another constructor...
         * @param type One of the supported spectrum types: \c Type::Octave or 
         * \c Type::ThirdOctave.
         */
        Spectrum(const SpectrumType type)
        {
            SetType(type);
        }
    };

    // --- Static variables initialization ------------------------------------

    template<typename T>
    const std::vector<float> Spectrum<T>::m_octaveBandsFcb =
    {
          31.5f,    63.0f,  125.0f,  250.0f,   500.0f,
        1000.0f,  2000.0f, 4000.0f, 8000.0f, 16000.0f
    };

    template<typename T>
    const std::vector<float> Spectrum<T>::m_thirdOctaveBandsFcb = 
   {
           25.0f,     31.5f,    40.0f,    50.0f,    63.0f,    80.0f,   100.0f,
          125.0f,    160.0f,   200.0f,   250.0f,   315.0f,   400.0f,   500.0f,
          630.0f,    800.0f,  1000.0f,  1250.0f,  1600.0f,  2000.0f,  2500.0f,
         3150.0f,   4000.0f,  5000.0f,  6300.0f,  8000.0f, 10000.0f, 12500.0f,
        16000.0f,  20000.0f
   };

    template<typename T>
    const std::vector<std::string> Spectrum<T>::m_averageTypes = { "A", "Lin" };


    template<typename T>
    const float Spectrum<T>::Band::ms_aweightedBin = 0.0f;

    template<typename T>
    const float Spectrum<T>::Band::ms_linearBin = 1.0f;

    template<typename T>
    float Spectrum<T>::Band::AweightedBin() { return ms_aweightedBin; }

    template<typename T>
    float Spectrum<T>::Band::LinearBin() { return ms_linearBin; }


} // namespace Aurora

#endif // __AURORA_SPECTRUM_H__
