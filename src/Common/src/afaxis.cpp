/********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common Libraries

  afaxis.cpp

  Simone Campanini

**********************************************************************/
#include <aurora.h>


void Aurora::RulerBar::SetCustomMajorLabels(const TranslatableStrings &labels,
                                            int start,
                                            int step)
{
    SetCustomMode( true );
    mpCache = std::make_unique<Cache>();
    auto &cache = *mpCache;
    auto &mMajorLabels = cache.mMajorLabels;

    const auto numLabel = labels.size();
    mMajorLabels.resize( numLabel );

    for(size_t i = 0; i<numLabel; i++) {
       mMajorLabels[i].text = labels[i];
       mMajorLabels[i].pos  = start + i*step;
    }
}

void Aurora::RulerBar::SetCustomMinorLabels(const TranslatableStrings &labels,
                                            int start,
                                            int step)
{
    SetCustomMode( true );
    mpCache = std::make_unique<Cache>();
    auto &cache = *mpCache;
    auto &mMinorLabels = cache.mMinorLabels;

    const auto numLabel = labels.size();
    mMinorLabels.resize( numLabel );

    for(size_t i = 0; i<numLabel; i++) {
       mMinorLabels[i].text = labels[i];
       mMinorLabels[i].pos  = start + i*step;
    }
}

//----------------------------------------------------------------------------
// Aurora::AxisBase implementation
//----------------------------------------------------------------------------

Aurora::AxisBase::Label::Label(const wxString& text,
                               const wxFontInfo& fontInfo)
: label(text),
  font(fontInfo)
{ }


void Aurora::AxisBase::SetFormat(const int nFormat)
{
	wxASSERT_MSG(nFormat >= RF_INT && nFormat <= RF_LINEARdB,
			     wxT("Aurora::AxisBase: Wrong format."));

	m_nRulerFormat = nFormat;
    m_pRuler->SetFormat((Ruler::RulerFormat)m_nRulerFormat);
}

void Aurora::AxisBase::SetRulerFonts(const wxFont &minorFont, 
                                     const wxFont &majorFont, 
                                     const wxFont &minorMinorFont)
{
	m_pRuler->SetFonts(minorFont, majorFont, minorMinorFont);
}

void Aurora::AxisBase::SetBounds(wxDC& dc, wxRect& wxrectBox, int nGridLength)
{
    SetBounds( dc,
    		   wxrectBox.x,
    		   wxrectBox.y,
    		   wxrectBox.x + wxrectBox.width,
    		   wxrectBox.y + wxrectBox.height,
    		   nGridLength );
}

void Aurora::AxisBase::SetBounds(wxDC& dc, 
                                 int nLeft, int nTop, 
                                 int nRight, int nBottom, 
                                 int nGridLength)
{
	int nWidth  = nRight - nLeft,
        nHeight = nBottom - nTop;

	// calculate ruler box size
    m_pRuler->GetMaxSize(&m_rulerArea.width, &m_rulerArea.height);

    m_rulerArea.x = nLeft;
    m_rulerArea.y = nTop;

    // calculate title size
    wxSize titleSize(0,0);

    if(m_Title.bShow)
    {
    	dc.SetFont(m_Title.font);
        titleSize = dc.GetTextExtent(m_Title.label);
    }

    switch(m_nOrientation)
    {
        case wxHORIZONTAL:
        	m_rulerArea.width = nWidth;
    	    m_axisArea = m_rulerArea;
    	    m_axisArea.height += titleSize.GetHeight();
    	    break;

        case wxVERTICAL:
    	    m_rulerArea.height = nHeight;
    	    m_axisArea = m_rulerArea;
    	    m_axisArea.width += titleSize.GetHeight();
    	    m_rulerArea.x    += titleSize.GetHeight();
    	    break;
    }

    // If this is not a categories axis, set here the bounds
    if (!IsCategories())
    {
        m_pRuler->SetBounds(m_rulerArea.x,
                            m_rulerArea.y,
                            m_rulerArea.x + m_rulerArea.width,
                            m_rulerArea.y + m_rulerArea.height);
    }
	switch(m_nOrientation)
	{
		case wxHORIZONTAL:
		{
			if(m_Title.bShow)
			{
				m_Title.position.x = m_rulerArea.x + (m_rulerArea.width - titleSize.GetWidth())/2;
				m_Title.position.y = m_rulerArea.GetBottom();
			}

			if(m_Grid.bShow)
			{
				m_Grid.nLenght   = -nGridLength;        // m_nHeight - (m_nTBorder + m_nBBorder)
				m_Grid.offset.x = m_rulerArea.x;  // m_nLBorder
				m_Grid.offset.y = nTop;                // m_nTBorder + 1
			}
		}
		break;

		case wxVERTICAL :
		{
			if(m_Title.bShow)
			{
				m_Title.position.x = m_axisArea.x;
				m_Title.position.y = m_axisArea.y  + (m_axisArea.height + titleSize.GetWidth())/2;
			}
			if(m_Grid.bShow)
			{
				m_Grid.nLenght   = nGridLength; // (m_nWidth  - (m_nLBorder + m_nRBorder)) - 2,
				m_Grid.offset.x = m_rulerArea.x + m_rulerArea.width; // m_nLBorder + 1,
				m_Grid.offset.y = nTop;                                        // m_nTBorder
			}
		}
		break;
    }
}

void Aurora::AxisBase::GetSize(int& nWidth, int& nHeight)
{
    nWidth  = m_axisArea.GetWidth();
    nHeight = m_axisArea.GetHeight();
}

wxSize Aurora::AxisBase::GetSize()
{
    wxSize sz(m_axisArea.GetWidth(),
    		  m_axisArea.GetHeight());
    return sz;
}

int  Aurora::AxisBase::GetWidth(wxDC& dc)
{
    m_pRuler->GetMaxSize(&m_rulerArea.width, &m_rulerArea.height);

    if(m_nOrientation == wxVERTICAL)
    {
        // calculate title size
        wxSize titleSize(0,0);

        if(m_Title.bShow)
        {
    	    dc.SetFont(m_Title.font);
    	    titleSize = dc.GetTextExtent(m_Title.label);
        }

  	    m_axisArea = m_rulerArea;
        m_axisArea.width += titleSize.GetHeight();
    }
	return m_axisArea.width;
}

int  Aurora::AxisBase::GetHeight(wxDC& dc)
{
    m_pRuler->GetMaxSize(&m_rulerArea.width, &m_rulerArea.height);

    if(m_nOrientation == wxHORIZONTAL)
    {
        // calculate title size
        wxSize titleSize(0,0);

        if(m_Title.bShow)
        {
    	    dc.SetFont(m_Title.font);
    	    titleSize = dc.GetTextExtent(m_Title.label);
        }

  	    m_axisArea = m_rulerArea;
        m_axisArea.height += titleSize.GetHeight();
    }
	return m_axisArea.height;
}

void Aurora::AxisBase::DrawTitles(wxDC& dc)
{
	if(m_Title.bShow)
	{
		dc.SetFont(m_Title.font);
		switch(m_nOrientation)
		{
		    case wxHORIZONTAL:
	            dc.DrawText( m_Title.label,
	                         m_Title.position.x,
	                         m_Title.position.y );
	            break;

		    case wxVERTICAL :
	            dc.DrawRotatedText( m_Title.label,
	            		            m_Title.position.x,
	            		            m_Title.position.y,
	                                90.0 );
	            break;
	    }
	}
}

void Aurora::AxisBase::DrawGrid(wxDC& dc)
{
    if(m_Grid.bShow)
    	m_pRuler->DrawGrid( dc,
    			            m_Grid.nLenght,
                            true,
                            true,
                            m_Grid.offset.x,
                            m_Grid.offset.y );
}

void Aurora::AxisBase::Draw(wxDC& dc)
{
	DrawTitles(dc);
    DrawGrid(dc);
    m_pRuler->Draw(dc);
}

Aurora::AxisBase::AxisBase(const int nOrientation)
  : m_nOrientation(nOrientation),
    m_axisArea(0,0,100,100),  m_rulerArea(0,0,100,100), // x,y,w,h
    m_nRulerFormat(RF_INT),
    m_nameStr("Name"),
    m_measureUnitStr(" "),
    m_Title("Axis title",
            wxFontInfo(AXIS_TITLE_FONT_SIZE).FaceName("Helvetica")),
#ifdef __APPLE__
    m_majorFont(wxFontInfo(RULER_FONT_SIZE).FaceName("Helvetica").Bold()),
    m_minorFont(wxFontInfo(RULER_FONT_SIZE).FaceName("Helvetica")),
    m_minorMinorFont(wxFontInfo(RULER_FONT_SIZE - 1).FaceName("Helvetica"))
#else
    m_majorFont(RULER_FONT_SIZE,
                wxFONTFAMILY_SWISS,
                wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD),

    m_minorFont(RULER_FONT_SIZE,
                wxFONTFAMILY_SWISS,
                wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_NORMAL),

    m_minorMinorFont(RULER_FONT_SIZE-1,
                     wxFONTFAMILY_SWISS,
                     wxFONTSTYLE_NORMAL,
                     wxFONTWEIGHT_NORMAL)
#endif
{
	m_Grid.bShow     = false;
	m_Grid.nLenght   = 0;
	m_Grid.offset.x = 0;
	m_Grid.offset.y = 0;
#if 0
	m_Title.bShow = false;
	m_Title.label = "Axis title";
    m_Title.font = *wxSWISS_FONT;
    m_Title.font.SetPointSize(AXIS_TITLE_FONT_SIZE);
	m_Title.position.x = 0;
	m_Title.position.y = 0;

    m_majorFont.SetPointSize(RULER_FONT_SIZE);
    m_minorFont.SetPointSize(RULER_FONT_SIZE);
    m_minorMinorFont.SetPointSize(RULER_FONT_SIZE - 1);
#endif
    
	m_pRuler.reset(new RulerBar());
	m_pRuler->SetOrientation(m_nOrientation); // required
	m_pRuler->SetBounds(0, 0, 100, 100);      // required
	m_pRuler->SetRange(0.0, 100.0);           // required

    if (m_nOrientation == wxHORIZONTAL)
    {
        m_pRuler->SetFlip(true); // usually the x ruler stays on bottom
    }

	m_pRuler->SetFonts(m_minorFont, m_majorFont, m_minorMinorFont);
}

Aurora::AxisBase::~AxisBase()
{
	
}

//----------------------------------------------------------------------------
// Aurora::Axis implementation
//----------------------------------------------------------------------------
void Aurora::Axis::Update()
{
    if (m_nOrientation == wxVERTICAL)
    {
        m_pRuler->SetRange(m_dbMax, m_dbMin);
    }
    else
    {
        m_pRuler->SetRange(m_dbMin, m_dbMax);
    }

	m_pRuler->SetLog(m_bIsLog);
}

void Aurora::Axis::SetPixelRange(const int nMin, const int nMax)
{
	if(m_bIsLog)
    {
        double dbLength = (m_nOrientation == wxVERTICAL) ? double(m_rulerArea.height)
                                                         : double(m_rulerArea.width);
        double dbMin = pow(m_dbMax/m_dbMin, nMin/dbLength) * m_dbMin;
        m_dbMax = pow(m_dbMax/m_dbMin, nMax/dbLength) * m_dbMin;
        m_dbMin = dbMin;
    }
    else
	{
        double dbScale = (m_nOrientation == wxVERTICAL) ? (m_dbMax-m_dbMin)/m_rulerArea.height
                                                        : (m_dbMax-m_dbMin)/m_rulerArea.width;
        double dbMin = m_dbMin;
        m_dbMin = nMin * dbScale + dbMin;
		m_dbMax = nMax * dbScale + dbMin;
	}
}

void Aurora::Axis::SetRange(const double dbMin, const double dbMax)
{
    m_dbMin = dbMin;
    m_dbMax = dbMax;
}

void Aurora::Axis::SetRange(const double dbMin, const double dbMax, const int nFormat, const bool bIsLog)
{
    m_dbMin = dbMin;
    m_dbMax = dbMax;
    
	if(nFormat >= 0)
		SetFormat(nFormat);
	SetLog(bIsLog);
}

void Aurora::Axis::SetLog(const bool bValue)
{
	m_bIsLog = bValue;
	if(m_bIsLog && (m_dbMin == 0.0))
	{
		// This is necessary, otherwise there is the
		// possibility of division by 0 error
		m_dbMin = 1.0;
        while (m_dbMin >= m_dbMax) 
        { 
            m_dbMin /= 10.0; 
        }
	}
}

void Aurora::Axis::Draw(wxDC& dc)
{
	Update();
	DrawTitles(dc);
    DrawGrid(dc);
    m_pRuler->Draw(dc);
}

Aurora::Axis::Axis(const int nOrientation)
  :  Aurora::AxisBase(nOrientation),
     m_dbMin(0), m_dbMax(100), m_bIsLog(false)
{
	m_pRuler->SetRange(m_dbMin, m_dbMax);
}


//----------------------------------------------------------------------------
// Aurora::CategoriesAxis implementation
//----------------------------------------------------------------------------

void Aurora::CategoriesAxis::AddCategories(const wxChar* aCat[], const size_t unCount)
{
#if AUDACITY_HEX_VERSION < 0x020400
	wxString cat;

	for(size_t un = 0; un < unCount; un++)
	{
		cat = aCat[un];
		m_Categories.aLabels.Add(cat);
	}
	m_Categories.nCount = int(m_Categories.aLabels.GetCount());
#else
    m_Categories.aLabels.clear();
    
    for(size_t un = 0; un < unCount; un++)
    {
        TranslatableString str{ aCat[un], {}};
        m_Categories.aLabels.push_back(str);
    }
#endif
}

void Aurora::CategoriesAxis::AddCategories(const wxString aCat[], const size_t unCount)
{
#if AUDACITY_HEX_VERSION < 0x020400
    wxString cat;

    for (size_t un = 0; un < unCount; un++)
    {
        cat = aCat[un];
        m_Categories.aLabels.Add(cat);
    }
    m_Categories.nCount = int(m_Categories.aLabels.GetCount());
#else
    m_Categories.aLabels.clear();
    
    for(size_t un = 0; un < unCount; un++)
    {
        TranslatableString str { aCat[un], {}};
        m_Categories.aLabels.push_back(str);
    }

#endif
}

void Aurora::CategoriesAxis::AddCategories(const wxArrayString& acat)
{
#if AUDACITY_HEX_VERSION < 0x020400
	for(size_t un = 0; un < acat.GetCount(); un++)
	{
		m_Categories.aLabels.Add(acat[un]);
	}
	m_Categories.nCount = int(m_Categories.aLabels.GetCount());
#else
    m_Categories.aLabels.clear();
    
    for(size_t un = 0; un < acat.GetCount(); un++)
    {
        TranslatableString str { acat[un], {}};
        m_Categories.aLabels.push_back(str);
    }
#endif
}

void Aurora::CategoriesAxis::Draw(wxDC& dc)
{
#if AUDACITY_HEX_VERSION < 0x020400
	wxASSERT_MSG(m_Categories.nCount > 0,
				 wxT("Aurora::CategoriesAxis: No categories present."));

    m_Categories.nStep  = GetWidth()/m_Categories.nCount;   // labels step
#else
    wxASSERT_MSG(! m_Categories.aLabels.empty(),
                 "Aurora::CategoriesAxis: No categories present.");

    m_Categories.nStep  = GetWidth()/m_Categories.aLabels.size(); // labels step

#endif
	m_Categories.nStart = m_Categories.nStep/2;// labels start coordinate

	//The following call is needed, because the Custom Ruler
    //functions are buggy: they never invalidata the device
    //context, so labels coordinates won't be updated.
	//A call to SetCustomMajorLabels reset everything but
	//don't force the label positions calculus. This
	//SetBounds, followed with another SetBounds with real
	//parameters cause the device context invalidation.
	//This problem cause to me a big waste of time and
	//some shame....[sc]
	m_pRuler->SetBounds(0, 0, GetWidth(), GetHeight());

	m_pRuler->SetCustomMode(true);
    
#if AUDACITY_HEX_VERSION < 0x020400
	m_pRuler->SetCustomMajorLabels( &m_Categories.aLabels,
					                 m_Categories.nCount,
									 m_Categories.nStart,
									 m_Categories.nStep );
#else
    m_pRuler->SetCustomMajorLabels(  m_Categories.aLabels,
                                     m_Categories.nStart,
                                     m_Categories.nStep );
#endif
    
    // l t r b
    m_pRuler->SetBounds(m_rulerArea.x,
    		            m_rulerArea.y,
    		            m_rulerArea.x + m_rulerArea.width,
    		            m_rulerArea.y + m_rulerArea.height);
	DrawTitles(dc);
	DrawGrid(dc);
	m_pRuler->Draw(dc);
}

// --- 'ctors
Aurora::CategoriesAxis::CategoriesAxis()
  : Aurora::AxisBase(wxHORIZONTAL)
{
    m_pRuler->SetMinor(false);
    m_pRuler->SetLog(false);
}

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

