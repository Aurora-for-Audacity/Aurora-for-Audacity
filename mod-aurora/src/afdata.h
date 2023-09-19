/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries
  
  afdata.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_AFDATA_H__
#define __AURORA_AFDATA_H__

#include <wxcmn.h>
#include <commdefs.h>

#include "afcomplex.h"

namespace Aurora
{
    //----------------------------------------------------------------------------
    // DataHolder
    /// Data container base class.
    //----------------------------------------------------------------------------
    template <class T>
    class DataHolder : public std::vector<T>
    {
      private:
        bool     m_bIsShown;
        wxString m_label;
        wxColour m_penColor;
        wxColour m_brushColor;
        
      protected:
                             
        /**
         * @brief Make room and copy data.
         * @param dataSrc Source data.
         * @param length Data length.
         */
        virtual void Copy(const T* dataSrc, const size_t length)
        { 
            this->resize(length);
            memcpy(this->data(), dataSrc, length * sizeof(T)); 
        }
        
      public:
        virtual const size_t  GetLength()  { return this->size(); }

        bool IsShown() const                { return m_bIsShown; }
        void Show(const bool bValue = true) { m_bIsShown = bValue; }
        
        void ToggleVisibility() { m_bIsShown = !m_bIsShown; }
        
        wxString& GetLabel() { return m_label; }
        void SetLabel(const wxString& label) { m_label = label; }
        void SetLabel(const wxChar* pwxchLabel)  { m_label = pwxchLabel; }

        void SetLineColour(const wxColour& colour)  { m_penColor = colour; }
        void SetLineColour( const unsigned char r,
                            const unsigned char g,
                            const unsigned char b ) { m_penColor.Set(r,g,b); }

        void SetFillColour(const wxColour& colour)  { m_brushColor = colour; }
        void SetFillColour( const unsigned char r,
                            const unsigned char g,
                            const unsigned char b ) { m_brushColor.Set(r,g,b); }

        const wxColour& GetLineColour() const { return m_penColor; }
        const wxColour& GetFillColour() const { return m_brushColor; }

        DataHolder& operator=(DataHolder&& dh)
        {
           std::vector<T>::operator=(std::move(dh));
           m_bIsShown = dh.m_bIsShown;
           m_penColor = dh.m_penColor;       
           m_brushColor = dh.m_brushColor;
           return *this;
        }

        DataHolder& operator=(const DataHolder& dh)
        {
           std::vector<T>::operator=(dh);
           m_bIsShown = dh.m_bIsShown;
           m_penColor = dh.m_penColor;       
           m_brushColor = dh.m_brushColor;
           return *this;
        }
        
        DataHolder()
         : m_bIsShown(true),
           m_label(wxT("NoName")),
           m_penColor(0,0,0),         // default border: black
           m_brushColor(255,255,255)  // default fill: white;
        { }

        DataHolder(DataHolder&& dh)
         : std::vector<T>(std::move(dh)),
           m_bIsShown(dh.m_bIsShown),
           m_penColor(dh.m_penColor),       
           m_brushColor(dh.m_brushColor)
        { }
        
        DataHolder(const DataHolder& dh)
         : std::vector<T>(dh),
           m_bIsShown(dh.m_bIsShown),
           m_penColor(dh.m_penColor),       
           m_brushColor(dh.m_brushColor)
        { }
        
        DataHolder(const size_t unLength)
         : std::vector<T>(unLength),
           m_bIsShown(true),
           m_penColor(0,0,0),          // default border: black
           m_brushColor(255,255,255)  // default fill: white;
        { }

        virtual ~DataHolder() { }
    };

    //----------------------------------------------------------------------------
    // DataSerie
    //----------------------------------------------------------------------------
    class DataSerie : public DataHolder<double>
    {
    public:
        /**
         * @brief Make room and copy data.
         * @param data Source data.
         * @param length Data length.
         */
        void Copy(const float*  data, const size_t length)
        {
            resize(length);

            for (size_t un = 0; un < this->size(); un++)
            {
                at(un) = double(data[un]);
            }
        }
       
        void Copy(const double* dataSrc, const size_t length)
        {
            resize(length);
            memcpy(data(), dataSrc, length * sizeof(double));
        }

        DataSerie(const size_t unLength)
          : DataHolder<double>(unLength)
        { /* wxASSERT_MSG(m_unLength > 0, wxT("DataSerie: Zero length array!")); */ }
/*
        DataSerie(const size_t unLength, const double* pdbSerie)
          : DataHolder<double>(unLength)
        { // wxASSERT_MSG(m_unLength > 0, wxT("DataSerie: Zero length array!"));
          SetData(pdbSerie); }

        DataSerie(const size_t unLength, const float* pflSerie)
          : DataHolder<double>(unLength)
        { SetData(pflSerie);  }
  */      
        DataSerie& operator=(DataSerie&& ds) 
        { 
            DataHolder::operator=(std::move(ds));
            return *this;
        }

         DataSerie& operator=(const DataSerie& ds) 
        { 
            DataHolder::operator=(ds);
            return *this;
        }
        
        DataSerie() {}
        DataSerie(DataSerie&& ds) : DataHolder<double>(std::move(ds))  { }
        DataSerie(const DataSerie& ds) : DataHolder<double>(ds)  { }
        
    };

    //----------------------------------------------------------------------------
    // TTimeDataSerie template
    //----------------------------------------------------------------------------
    template <class T>
    class TTimeDataSerie : public DataSerie
    {
      protected:
        std::vector<T> m_timeSamples;
        int    m_nHandlerType;
        double m_dbRate;    

      public:

        virtual void CalculateDataSerie(const size_t nWindowLength, 
                                        double& tlo,
                                        double& thi) = 0;

        virtual bool IsOk(const double tlo, const double thi) = 0;        

        void CopyTimeData(const T* data, const size_t length)
        {
            m_timeSamples.resize(length);
            memcpy(m_timeSamples.data(), data, length * sizeof(T));
        }

        void CopyTimeData(const Vector<T>& src)
        {
            m_timeSamples.resize(src.Length());
            memcpy(m_timeSamples.data(), src.CSamples(), src.Length() * sizeof(T));
        }

        void ResizeTimeData(const size_t length) 
        { 
            m_timeSamples.resize(length); 
        }

        void SetRate(const double dbRate)   { m_dbRate = dbRate; }

        const size_t GetSamplesCount()  { return m_timeSamples.size(); }
        double GetRate() const { return m_dbRate; }

        void SetHandler(const int nValue) { m_nHandlerType = nValue; }
        int  GetHandler() const { return m_nHandlerType; }

        virtual double GetMax() const 
        { 
            return double(m_timeSamples.size()) / m_dbRate; 
        }

        TTimeDataSerie& operator=(TTimeDataSerie&& tds)
        {
            DataSerie::operator=(std::move(tds));
            m_timeSamples     = std::move(tds.m_timeSamples);
            m_nHandlerType = tds.m_nHandlerType;
            m_dbRate       = tds.m_dbRate;
            return *this;
        }

        TTimeDataSerie& operator=(const TTimeDataSerie& tds)
        {
            DataSerie::operator=(tds);
            m_timeSamples      = tds.m_timeSamples;
            m_nHandlerType = tds.m_nHandlerType;
            m_dbRate       = tds.m_dbRate;
            return *this;
        }

        TTimeDataSerie() { }

        TTimeDataSerie(TTimeDataSerie&& tds)
         : DataSerie(std::move(tds)),
           m_timeSamples(std::move(tds.m_timeSamples)),
           m_nHandlerType(tds.m_nHandlerType),
           m_dbRate(tds.m_dbRate)
        { }

        TTimeDataSerie(const TTimeDataSerie& tds)
         : DataSerie(tds),
           m_timeSamples(tds.m_timeSamples),
           m_nHandlerType(tds.m_nHandlerType),
           m_dbRate(tds.m_dbRate)
        { }
        
        TTimeDataSerie(const double dbRate, 
                       const int nHandlerType)
         : DataSerie(),
           m_nHandlerType(nHandlerType),
           m_dbRate(dbRate)
        { }

        TTimeDataSerie(const size_t unVectorLength, 
                       const double dbRate, 
                       const int nHandlerType)
        : DataSerie(),
          m_timeSamples(unVectorLength, 0.0),
          m_nHandlerType(nHandlerType),
          m_dbRate(dbRate)
        { }
    };

    //----------------------------------------------------------------------------
    // TSpectralDataSerie template
    //----------------------------------------------------------------------------
    class TSpectralDataSerie : public DataSerie
    {
      protected:
        ComplexSpectrum m_spectrum;
        int             m_nHandlerType;

      public:
        virtual bool IsOk(const double fLo, const double fHi) = 0;

        virtual void CalculateDataSerie(const size_t nWindowLength, 
                                        const double dbLo, 
                                        const double dbHi, 
                                        const bool bIsLog) = 0;

        void SetRate(const double fs) { m_spectrum.SetNyquistFrequency(fs/2.0); }

        ComplexSpectrum& Y() { return m_spectrum; }
        double GetRate() const { return m_spectrum.GetSamplerate(); }

        virtual void SetHandlerId(const int nTypeId) { m_nHandlerType = nTypeId; }
        int  GetHandlerId() const { return m_nHandlerType; }

        double GetMax() const { return m_spectrum.GetNyquistFrequency(); }

        TSpectralDataSerie& operator=(TSpectralDataSerie&& sds)
        {
            DataSerie::operator=(std::move(sds));
            m_spectrum     = std::move(sds.m_spectrum);
            m_nHandlerType = sds.m_nHandlerType;
            return *this;
        }

        TSpectralDataSerie& operator=(const TSpectralDataSerie& sds)
        {
            DataSerie::operator=(sds);
            m_spectrum.Copy(sds.m_spectrum);
            m_nHandlerType = sds.m_nHandlerType;
            return *this;
        }

    // 'ctors ---------
        TSpectralDataSerie() { }
        
        TSpectralDataSerie(const TSpectralDataSerie& sds)
         : DataSerie(sds),
           m_nHandlerType(sds.m_nHandlerType)
        { 
            m_spectrum.Copy(sds.m_spectrum);
        }
        
        TSpectralDataSerie(TSpectralDataSerie&& sds)
         : DataSerie(std::move(sds)),
           m_spectrum(std::move(sds.m_spectrum)),
           m_nHandlerType(sds.m_nHandlerType)
        { }
        
        TSpectralDataSerie(const int nHandlerType)
         : DataSerie(),
           m_nHandlerType(nHandlerType)
        {}

        TSpectralDataSerie(const size_t spectrumLength,
                           const int nHandlerType)
        : DataSerie(),
          m_nHandlerType(nHandlerType)
        {
            m_spectrum.Resize(spectrumLength);
        }

        TSpectralDataSerie(const ComplexSpectrum& spectrum,
                           const int nHandlerType)
        : DataSerie(),        
          m_nHandlerType(nHandlerType)
        { 
            m_spectrum.Copy(spectrum);
        }

    };
}
#endif  // __AURORA_DATA_H__
