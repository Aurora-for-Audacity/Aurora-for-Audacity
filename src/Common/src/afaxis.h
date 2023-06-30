/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common Libraries

  afaxis.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_AXIS_H__
#define __AURORA_AXIS_H__

#ifndef __STAND_ALONE__
   #include <widgets/Ruler.h>
#endif

#ifdef __WXOSX__
   #define AXIS_TITLE_FONT_SIZE  10
   #define RULER_FONT_SIZE 10
#elif defined __WXMSW__
   #define PLOT_TITLE_FONT_SIZE  10
   #define AXIS_TITLE_FONT_SIZE   8
   #define RULER_FONT_SIZE  8
#else
// In GTK a smaller font looks better..
   #define AXIS_TITLE_FONT_SIZE  6
   #define RULER_FONT_SIZE 6
#endif

class PlotBase;

//----------------------------------------------------------------------------
// Aurora::AxisBase
/// \brief The Aurora plot axis base class
/// This class manages all phisical axis properties. It encapsulates an
/// Audacity Ruler object.
//----------------------------------------------------------------------------

namespace Aurora
{

    /**
     * @brief Derived class because since 3.0.0 some code was dropped from
     * Aduacity main stream.
     */
    class RulerBar : public Ruler
    {
      public:
        RulerBar() : Ruler() { }
        
        // If this is the case, you should provide an array of labels, start
        // label position, and labels step. The range eventually specified will be
        // ignored.
        void SetCustomMajorLabels(
           const TranslatableStrings &labels, int start, int step);
        void SetCustomMinorLabels(
           const TranslatableStrings &labels, int start, int step);

    };

    // -------------------------------------------------------------------------
    class AxisBase
    {
      public:
    	/// Orientation macros
    	enum Orientation
        { 
            RF_HORIZONTAL = wxHORIZONTAL,
    	    RF_VERTICAL   = wxVERTICAL 
        };

    	/// Rulers types
    	enum Type
        { 
            RF_INT        = RulerBar::IntFormat,
    	    RF_REAL       = RulerBar::RealFormat,
    	    RF_REALLOG    = RulerBar::RealLogFormat,
    	    RF_TIME       = RulerBar::TimeFormat,
    	    RF_LINEARdB   = RulerBar::LinearDBFormat/*,
    	    RF_CATEGORIES*/
    	};

      protected:
    	std::unique_ptr<RulerBar> m_pRuler;         ///< The ruler (taken from the Audacity widget, Ruler)

        int    m_nOrientation;   ///< Stores the axis orientation

        wxRect m_axisArea;  ///< The entire axis bounding box
        wxRect m_rulerArea; ///< The ruler bounding box

        int  m_nRulerFormat;     ///< Stores the ruler format

        struct 
        { 
            bool    bShow   = false;
            int     nLenght = 0;
            wxPoint offset  = { 0 ,0 }; 
        } m_Grid;   ///< Grid properties

        struct Label
        { 
            bool     bShow = false;
            wxString label;
            wxFont   font;
            wxPoint  position = { 0 ,0 };
            
            Label() { }
            Label(const wxString& text,
                  const wxFontInfo& fontInfo);
        };

        Label m_Title; ///< Axis title properties
        
        wxString m_nameStr;         ///< The axis name
        wxString m_measureUnitStr;  ///< The axis measure unit

        wxFont   m_majorFont;     ///< Font for major labels
        wxFont   m_minorFont;       ///< Font for minor labels
        wxFont   m_minorMinorFont;  ///< Font for minor minor labels

      public:

        /// Get a pointer to the axis ruler
        /// \return A pointer to Audacity Ruler object
        Ruler* GetRuler() { return m_pRuler.get(); }

        /// True, if is a category axis
        virtual bool IsCategories() { return false; }

        /// Check if the axis title is shown
        bool IsTitleOn()  const { return m_Title.bShow; }

        /// Set title visibility status
        void ShowTitle(const bool bValue = true) { m_Title.bShow = bValue; }

        /// Toggle title visibility status
        void ToggleTitle()  { m_Title.bShow = !m_Title.bShow; }

        /// Set axis title
        /// \param label - The title string
        void SetTitle(const wxString& label)  { m_Title.label = label; }

        /// Set axis title
        /// \param label - The title string
        void SetTitle(const wxChar* label)   { m_Title.label = label; }

        /// Set axis title font
        /// \param font - The choosed title font
        void SetTitleFont(wxFont& font)      { m_Title.font = font; }

        /// Set ruler format
        /// \param nFormat - The selected format, one of RF_INT, RF_REAL, RF_REALLOG, RF_TIME, RF_LINEARdB
        virtual void SetFormat(const int nFormat);
        int  GetType()   const { return m_nRulerFormat; } // TO BE REMOVED

        /// Get current ruler format
        int  GetFormat() const { return m_nRulerFormat; }

        /// Check grid visibilty
        bool IsGridOn()   const { return m_Grid.bShow; }
        /// Set grid visibility
        void ShowGrid(const bool bValue = true)   { m_Grid.bShow = bValue; }

        /// Set ruler fontset
        void SetRulerFonts(const wxFont &minorFont,
                           const wxFont &majorFont,
                           const wxFont &minorMinorFont);

        /// Set axis bounding box doing complete axis sizing.
        /// \param dc - axis device context
        /// \param wxrectBox - axis external edges
        /// \param nGridLength - length of grid lines (0 to turn off grid)
        void  SetBounds(wxDC& dc, wxRect& wxrectBox, int nGridLength = 0);

        /// Set axis bounding box doing complete axis sizing.
        /// \param dc - axis device context
        /// \param nLeft nTop, nRight, nBottom - axis external edges
        /// \param nGridLength - length of grid lines (0 to turn off grid)
        void  SetBounds(wxDC& dc, 
                        int nLeft,  
                        int nTop, 
                        int nRight, 
                        int nBottom, 
                        int nGridLength = 0);

        /// Get axis bounding box
        wxRect& GetBounds     () { return m_axisArea; }

        /// Get ruler bounding box (contained in the axis bounding box)
        wxRect& GetRulerBounds() { return m_rulerArea; }

        void   GetSize(int& nWidth, int& nHeight);
        wxSize GetSize();

        int  GetWidth (wxDC& dc);
        int  GetHeight(wxDC& dc);
        int  GetWidth () const { return m_axisArea.width;  }
        int  GetHeight() const { return m_axisArea.height; }

        const wxString& GetName()    { return m_nameStr; }
        void SetName(wxString& name) { m_nameStr = name; }
        void SetName(const wxChar* name) { m_nameStr = name; }
        
        wxString& GetMeasureUnit()            { return m_measureUnitStr; }
        void SetMeasureUnit(wxString& mu)     { m_measureUnitStr = mu; }
        void SetMeasureUnit(const wxChar* mu) { m_measureUnitStr = mu; }

      protected:
        /// Draws axis titles
        void DrawTitles(wxDC& dc);

        /// Draws axis grid
        void DrawGrid(wxDC& dc);

      public:
        /// Empty method
        virtual void SetPixelRange(const int nMin, const int nMax) {}

        /// Empty method
        virtual void GetRange(double& dbMin, double& dbMax) {}

        /// Empty method
        virtual void SetRange(const double dbMin, const double dbMax) {}

        /// Draws everything.
        virtual void Draw(wxDC& dc);

    public:
        /// The constructor.
        /// \param nOrientation - The axis orientation.
    	AxisBase(const int nOrientation = RF_HORIZONTAL);
    	virtual ~AxisBase();

    };

    //----------------------------------------------------------------------------
    // Aurora::Axis
    /// \brief Generic continuous numbers axis.
    /// This axis is designed to represent continuous set of real numbers, like a time
    /// axis, for example. The axis can be linear or logarithmic as well.
    //----------------------------------------------------------------------------
    class Axis : public AxisBase
    {
      protected:

        double m_dbMin,  ///< Lower number set extreme
               m_dbMax;  ///< Higher number set extreme
        bool   m_bIsLog; ///< Logarithmic flag

      private:
        void Update();

      public:
        /// This method takes two coordinates, convert them to the
        /// corresponding values represented in the ruler and set
        /// these values as new rulers extremes. It's a scaling
        /// function.
        /// \param nMin - lower coordinate
        /// \param nMax - higher coordinate
        void SetPixelRange(const int nMin, const int nMax);

        /// Set axis extremes.
        /// \param dbMin - lower coordinate
        /// \param dbMax - higher coordinate
        void SetRange(const double dbMin, const double dbMax);

        /// Set axis extremes and some other propertes.
        /// \param dbMin - lower coordinate
        /// \param dbMax - higher coordinate
        /// \param nFormat - ruler format, one of RF_INT, RF_REAL, RF_REALLOG, RF_TIME, RF_LINEARdB
        /// \param bIsLog - set logarithmic flag
        void SetRange(const double dbMin, 
                      const double dbMax, 
                      const int nFormat, 
                      const bool bIsLog = false);

        /// Get current axis range.
        void GetRange(double& dbMin, double& dbMax) { dbMin = m_dbMin; dbMax = m_dbMax; }

        /// Get axis lower extreme
        double& GetMin() { return m_dbMin; }

        /// Get axis higher extreme
        double& GetMax() { return m_dbMax; }

        bool IsLog() const { return m_bIsLog; }
        void SetLog(const bool bValue);

        void Draw(wxDC& dc);

        /// The constructor.
        /// \param nOrientation - The axis orientation.
    	Axis(const int nOrientation = RF_HORIZONTAL);
    };

    //----------------------------------------------------------------------------
    // Aurora::CategoriesAxis
    /// \brief Generic category axis.
    /// This class is designed to manage a finite set of values, or categories,
    /// like a spreadsheet chart.
    //----------------------------------------------------------------------------
    class CategoriesAxis : public Aurora::AxisBase
    {
      protected:

    	/// Categories properties.
        struct
        {

#if AUDACITY_HEX_VERSION < 0x020400
            wxArrayString aLabels;
        	int  nCount = 0;
#else
            TranslatableStrings aLabels;
#endif
            int  nStart = 0;
            int  nStep = 0;
         } m_Categories;

      public:
        /// Of course always true (Deprecated)
        bool IsCategories() { return true; }

        /// Empty: in this class format setting has no effect.
        void SetFormat(const int nFormat) {}

        /// Add categories as array of wxChar.
        /// \param aCat - the array
        /// \param unCount - the number of array's elements
        void AddCategories(const wxChar* aCat[], const size_t unCount);

        /// Add categories as array of wxChar.
        /// \param aCat - the array
        /// \param unCount - the number of array's elements
        void AddCategories(const wxString aCat[], const size_t unCount);

        /// Add categories as array of wxString
        /// \param aCat - the array
        void AddCategories(const wxArrayString& aCat);

        /// Get the number of stored categories
#if AUDACITY_HEX_VERSION < 0x020400
        int GetCategoriesCount() const { return m_Categories.nCount; }
#else
        int GetCategoriesCount() const { return (int)m_Categories.aLabels.size(); }
#endif
        /// Get the position of the first categories element in the axis
        int GetCategoriesStart() const { return m_Categories.nStart; }

        /// Get the step between categories elements in the axis
        int GetCategoriesStep () const { return m_Categories.nStep; }

        virtual void Draw(wxDC& dc);

        /// The constructor.\n
        /// In the current implementation this type of axis can be only horizontal.
    	CategoriesAxis();
    	~CategoriesAxis() {}
    };
} // namespace Aurora

#endif /* __AURORA_AXIS_H__ */
