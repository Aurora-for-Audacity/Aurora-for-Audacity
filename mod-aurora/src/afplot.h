/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries
  
  afplot.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_AFPLOT_H__
#define __AURORA_AFPLOT_H__

#ifndef __STAND_ALONE__
   #include <widgets/Ruler.h>
#endif

// Some defines...
#define DECAY_PLOT_X_SIZE 540 // To be removed
#define DECAY_PLOT_Y_SIZE 250 // To be removed

#ifdef __WXOSX__
   #define PLOT_TITLE_FONT_SIZE  12
   #define AXIS_TITLE_FONT_SIZE  10
   #define LEGEND_FONT_SIZE      10
   #define DECAY_RULER_FONT_SIZE 10
#elif defined __WXMSW__
   #define PLOT_TITLE_FONT_SIZE  10
   #define AXIS_TITLE_FONT_SIZE   8
   #define LEGEND_FONT_SIZE       8
   #define DECAY_RULER_FONT_SIZE  8
#else
// In GTK a smaller font looks better..
   #define PLOT_TITLE_FONT_SIZE  8
   #define AXIS_TITLE_FONT_SIZE  6
   #define LEGEND_FONT_SIZE      6
   #define DECAY_RULER_FONT_SIZE 6
#endif

namespace Aurora
{
    //----------------------------------------------------------------------------
    // LegendEntry
    /// \brief A plot legend item.
    /// This class encapsulate a plot legend entry.
    //----------------------------------------------------------------------------
    class LegendEntry
    {
      private:
        wxString m_label;  ///< The legend label
        wxColour m_colour; ///< The plot color for this entry

      public:
        void SetLabel (wxString& label)      { m_label = label; }
        void SetLabel (const wxChar* pwxchLabel) { m_label = pwxchLabel; }
        void SetColour(const wxColour& wxcol)    { m_colour = wxcol; }

        wxString& GetLabel()  { return m_label; }
        wxColour& GetColour() { return m_colour; }

        LegendEntry& operator=(const LegendEntry& l)
        {
            m_label  = l.m_label;
            m_colour = l.m_colour;
            return *this;
        }
        
        /// The default constructor
        LegendEntry() {}

        LegendEntry(const LegendEntry& l)
          : m_label(l.m_label), m_colour(l.m_colour)
        { }
        
        /// A constructor for complete legend entry creation.
        LegendEntry(wxString& label, wxColour& colour)
          : m_label(label), m_colour(colour) 
        { }

        /// Another constructor for complete legend entry creation.
        LegendEntry(const wxChar* label, wxColour& colour)
           : m_label(label), m_colour(colour) 
        { }
    };
}
//WX_DECLARE_OBJARRAY(Aurora::LegendEntry, TArrayOfLegendEntry);

typedef std::vector<Aurora::LegendEntry> TArrayOfLegendEntry;

//----------------------------------------------------------------------------
// Legend
/// \brief The plot legend.
/// This class manage the legend entries and draw itself on the plot canvas.
//----------------------------------------------------------------------------
// sizes in pixel
#define LGD_MARGIN     5
#define LGD_BORDER     10
#define LGD_LINE_WIDTH 10

namespace Aurora
{
    class AxisBase;

    class Legend
    {
      public:
        /// The four possible legend positions in the plot canvas.
        enum 
        { 
            TOP_LEFT = 100, 
            TOP_RIGHT, 
            BOTTOM_RIGHT, 
            BOTTOM_LEFT 
        };

        private:
        int    m_nPosition;            ///< The position

        wxRect m_boundingBox;           ///< The bounding box
        wxSize m_maxLabelSizes; ///< Maximum sizes

        wxFont m_font;           ///< The legend font
        bool   m_bShow;               ///< Visibility flag

        TArrayOfLegendEntry m_aEntries; ///< Legend items array

        /// Given the device context, this methods finds maximum width
        /// and height between all legend entries then stores them.
        void FindMaxLabelsDims(wxDC& dc);

        public:
        /// Clear the legend item array
        void Clear()    { m_aEntries.clear(); }

        /// Add new item in the legend array
        void AddLabel   (wxString& label, const wxColour& brush = wxNullColour);

        /// Add new item in the legend array
        void AddLabel   (const wxChar* label, 
                            const wxColour& brush = wxNullColour);

        /// Removes an existing item in the legend array
        void RemoveLabel(const int nIdx);

        /// Visibility check
        bool IsShown() const { return m_bShow; }

        /// Set visibility flag
        void Show(const bool bValue = true) { m_bShow = bValue; }

        /// Set legend position
        /// \param nPos - position, one of TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT
        void SetPosition(const int nPos) {m_nPosition = nPos; }

        /// Draws the legend.
        /// \param dc - the device context
        /// \param wxrectClientArea - plot canvas bounding box
        void Draw(wxDC& dc, wxRect& wxrectClientArea);

        /// The constructor.
        /// \param nPosition - legend position in the plot canvas.
        Legend(const int nPosition = TOP_RIGHT);
    };

    //----------------------------------------------------------------------------
    // PlotBase
    //
    /// \brief The Aurora plot base class.
    /// All Aurora plots are based on this class, so it's designed to be quite
    /// flexible and complete. It has contextual menu in the plot area with
    /// clipboard copy, drag zoom, autorange and contextual menus in both horizontal
    /// and vertical axes.\n
    /// Anyway, this class shouldn't be instantiated directly, use, instead,
    /// Scope(), SScope(), Chart(), BarPlot().
    //----------------------------------------------------------------------------
    class PlotBase: public wxControl
    {
     public:
       /// Events IDs.
        enum 
        { 
            ID_COPY_BITMAP = 0x3000, 
            ID_COPY_VECTORIAL,
            ID_SHOW_LEGEND_MCHK,
            ID_SET_X_AXIS_RANGE,
            ID_SET_Y_AXIS_RANGE,
            ID_X_AUTORANGE_MCHK,
            ID_Y_AUTORANGE_MCHK,
            ID_SET_X_AXIS_DLG,
            ID_SET_Y_AXIS_DLG 
        };

     protected:
        wxWindow* m_pParent = nullptr;  /**< The plot parent window, usually a 
                                             wxPanel or wxScrolledWindow.*/

        wxRect m_wxEnvRect;        ///< The entire plot bounding box (Maybe quite unused...).

        struct 
        { 
            bool     bShow;
            wxString label;
            wxFont   font; 
            
        } m_Title; ///< Plot title properties
        
        wxColour m_canvasColor,     ///< Canvas color.
                 m_backgroundColor, ///< Drawing area background color.
                 m_bordersColor;    ///< Borders color.

     protected:
        std::unique_ptr<AxisBase> m_pHorzAxis;  ///< The horizontal axis.
        std::unique_ptr<AxisBase> m_pVertAxis;  ///< The vertial axis.

        std::unique_ptr<Legend>   m_pLegend;    ///< The legend.

        wxRect  m_clientArea; ///< The drawing area bounding box.

        int m_nWidth,               ///< The entire plot width
            m_nHeight;              ///< The entire plot height

        // Borders contain rulers and titles
        int m_nTBorder,            ///< Top border width.
            m_nBBorder,            ///< Bottom border width.
            m_nLBorder,            ///< Left border width.
            m_nRBorder;            ///< Right border width.

        bool m_bIsTBorderOn,       ///< Top border flag.
             m_bIsRBorderOn;       ///< Right border flag.

        bool m_bIsHorzAutorangeOn, ///< Horizontal axis autorange flag.
             m_bIsVertAutorangeOn; ///< Vertical axis autorange flag.
        
        bool m_bIsHorzRangeMutable, ///< False if horizontal is a categories axis.
             m_bIsVertRangeMutable; ///< False if vertical is a categories axis.

       /// Find maximum and minimum in a given data serie.
       /// \param serie - the data serie vector
       /// \param dbMin - the minimum value
       /// \param dbMax - the maximum value
       /// \param bIsLog - logarithmic flag...actually not working!!!
        virtual void Autorange(const std::vector<double>& serie,
                               double& dbMin,
                               double& dbMax,
                               const bool bIsLog = false);

       /// Find maximum and minimum in a given data serie.
       /// \param pdbSerie - the data serie vector
       /// \param unLength - data serie vector length
       /// \param dbMin - the minimum value
       /// \param dbMax - the maximum value
       /// \param bIsLog - logarithmic flag...actually not working!!!
       virtual void Autorange(const double* pdbSerie, 
                              const size_t unLength,
    		                  double& dbMin, 
                              double& dbMax, 
                              const bool bIsLog = false);

       /// Called in Draw(), if something has been changed since the last redraw,
       /// it have to recalculate vertical axis range.
       virtual void UpdateVerticalAxis  () = 0;

       /// Called in Draw(), if something has been changed since the last redraw,
       /// it have to recalculate horizontal axis range.
       virtual void UpdateHorizontalAxis() = 0;

       // --- Event handlers
       void OnPaint(wxPaintEvent& event);
       void OnRightButtonClick(wxMouseEvent& evt);
    #if wxUSE_METAFILE
       void OnCopyVectorial(wxCommandEvent& evt);
    #endif
       void OnShowSetXAxisRange(wxCommandEvent& evt);
       void OnShowSetYAxisRange(wxCommandEvent& evt);
       void OnXAutorangeMenuCheck(wxCommandEvent& evt);
       void OnYAutorangeMenuCheck(wxCommandEvent& evt);
       void OnShowLegendMenuCheck(wxCommandEvent& evt);

       void OnCopyBitmap(wxCommandEvent& evt);
       void OnChar(wxKeyEvent& evt);

       void OnMouseStartSelection(wxMouseEvent& evt);
       void OnMouseSelection(wxMouseEvent& evt);
       void OnMouseEndSelection(wxMouseEvent& evt);

       /// This pure virtual method should contains
       /// all the plot routines and MUST be implemented!
       virtual void Draw(wxDC& dc) = 0;

      private:
       /// This method inits the device context and calls Draw()
       /// \param dc - the device context
       /// \param sz - the window size.
       void UpdatePlot(wxDC& dc, const wxSize& sz);

       wxRect m_selectionArea; ///< The ruler selection region.
       bool   m_bIsSelectionOn;  ///< True when the user starts a drag selection action.

       /// Draws the selection region in the rulers.
       void   DrawSelectionRegion(wxDC& dc);

      public:

        // Plot dimensions
        int   GetWidth()  const { return m_nWidth; }
        int   GetHeight() const { return m_nHeight; }
        
        int   GetClientAreaWidth() const 
        { 
            return (m_nWidth  - (m_nLBorder + m_nRBorder)); 
        }
        
        int   GetClientAreaHeight() const 
        { 
            return (m_nHeight - (m_nTBorder + m_nBBorder)); 
        }
        
        void  GetClientAreaSize(wxSize& size) const 
        { 
            size = wxSize(GetClientAreaWidth(),
                          GetClientAreaHeight()); 
        }
        
        // Get client area top left coordinate
        int  GetClientAreaHorzOffset()         const { return m_nLBorder; }
        int  GetClientAreaVertOffset()         const { return m_nTBorder; }
        void GetClientAreaOffset(wxPoint& ofs) const { ofs.x = m_nLBorder;
                                                        ofs.y = m_nTBorder;}

        bool IsTopBorderOn()   const { return m_bIsTBorderOn; }
        bool IsRightBorderOn() const { return m_bIsRBorderOn; }

        void SetTopBorder(const bool bValue)    { m_bIsTBorderOn = bValue; }
        void SetRightBorder(const bool bValue)  { m_bIsRBorderOn = bValue; }

        // Plot attributes
        bool IsTitleOn () const                    { return m_Title.bShow; }
        void ShowTitle (const bool bValue = true)  { m_Title.bShow     = bValue; }
        void SetTitle  (const wxString& wxszTitle) { m_Title.label = wxszTitle; }
        void SetTitle  (const wxChar* pwxchTitle)  { m_Title.label = pwxchTitle; }

        void SetCanvasColour(wxColour& col)     { m_canvasColor = col; }
        void SetBackgroundColour(wxColour& col) { m_backgroundColor = col; }
        void SetBorderColour(wxColour& col)     { m_bordersColor = col; }

        /// Assign an horizontal axis to plot that owns it.
        void SetHorizontalAxis(AxisBase* pab) { m_pHorzAxis.reset(pab); }

        /// Assign a vertical axis to plot that owns it.
        void SetVerticalAxis  (AxisBase* pab) { m_pVertAxis.reset(pab); }

        /// Return a pointer to horizontal axis.
        AxisBase* GetHorizontalAxis() const { return m_pHorzAxis.get(); }

        /// Return a pointer to vertical axis.
        AxisBase* GetVerticalAxis  () const { return m_pVertAxis.get(); }

        bool IsHorzAxisAutorangeOn() const { return m_bIsHorzAutorangeOn; }
        bool IsVertAxisAutorangeOn() const { return m_bIsVertAutorangeOn; }
        
        void SetHorzAxisAutorange(const bool bValue) 
        { 
           m_bIsHorzAutorangeOn = (m_bIsHorzRangeMutable) ? bValue : false; 
        }
       
        void SetVertAxisAutorange(const bool bValue) 
        { 
            m_bIsVertAutorangeOn = (m_bIsVertRangeMutable) ? bValue : false; 
        }
        

        /// Return a pointer to the legend object.
        Legend* GetLegend ();

        /// Set legend visibility flag and redraw.
        void      ShowLegend(const bool bValue = true);

        /// The constructor.\n
        /// \param parent - parent window
        /// \param id - window identifier
        /// \param pos - window position
        /// \param size - window size
        /// \param style - window style.
        PlotBase(wxWindow* parent, 
                 wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 const long style   = wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE);
        ~PlotBase();
    };


    //----------------------------------------------------------------------------
    // SetAxisRangeDialog
    /// \brief The dialog invoked from rulers contextual menu.
    /// This dialog let the user choose the new axis extremes.
    //----------------------------------------------------------------------------
    class SetAxisRangeDialog : public wxDialog
    {
      private:
        enum
        {
            ID_MIN_TC = 1000,
            ID_MIN_MU_ST,
            ID_MAX_MU_ST,
        };

        AxisBase* m_pAxis = nullptr;

        int    m_nId;

        double m_min;
        double m_max;

        wxPanel*      m_pUpperPanel;
        wxTextCtrl*   m_pMinText;
        wxTextCtrl*   m_pMaxText;
        wxPanel*      m_pLowerPanel;
        wxButton*     m_pCancelButton;
        wxButton*     m_pOkButton;

        protected:

        void OnCancel ( wxCommandEvent& event );
        void OnOk     ( wxCommandEvent& event );

        public:
        /// The constructor.
        /// \param parent - parent window
        /// \param id - window identifier
        /// \param pAxis - the axis to be modified
        /// \param title - the dialog title
        /// \param pos - window position
        /// \param size - window size
        /// \param style - window style.
        SetAxisRangeDialog(wxWindow* parent,
                           wxWindowID id,
                           AxisBase* pAxis,
                           const wxString& title = wxT("Set Axis Range"), 
                           const wxPoint&    pos = wxDefaultPosition,
                           const wxSize&    size = wxDefaultSize,
                           long style = wxDEFAULT_DIALOG_STYLE);
    };
} // namespace Aurora

#endif  // __AURORA_PLOT_H__

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


