/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  trprop.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_CONVOLVER_TRPROP_H__
#define __AURORA_CONVOLVER_TRPROP_H__

#include <wx/dynarray.h>

namespace Aurora
{
    class ConvolverTrackInfoItem 
    {
      private:
        int      mIndex = 0;
        int      mRow = -1;
        int      mChnl = 0;
        bool     mIsIr = false;
        bool     mIsAudioData = false;
        wxString mName;
        wxString mLabel;

      public:

        void SetIndex(int value) { mIndex = value; }
        void SetRow(int value) { mRow = value; }
        void SetChannel(int value) { mChnl = value; }
        void SetIr(bool value) { mIsIr = value; }
        void SetAudioData(bool value) { mIsAudioData = value; }
        void SetAsIr() { mIsIr = true;  mIsAudioData = false; }
        void SetAsAudioData() { mIsIr = false; mIsAudioData = true; }
        void SetName(wxString value) { mName = value; }

        int GetIndex()     const { return mIndex; }
        int GetRow()       const { return mRow; }
        int GetChannel()   const { return mChnl; }
        bool IsIr()        const { return mIsIr; }
        bool IsInput()     const { return mIsAudioData; }
        wxString GetName() const { return mName; }
        wxString GetLabel();

        ConvolverTrackInfoItem() { }
    };
}

// This defines a new type: ArrayOfConvolverTrackInfoItems
WX_DECLARE_OBJARRAY(Aurora::ConvolverTrackInfoItem, ArrayOfConvolverTrackInfoItems);

namespace Aurora
{
    class ConvolverTrackProperties 
    { 
      private:
       int mAudioDataRows = 0;
       int mFilterRows    = 0;
       ArrayOfConvolverTrackInfoItems   mTrackInfo;
       
      public: 
       void AddTrackInfoItem(ConvolverTrackInfoItem &info_item)             {  mTrackInfo.Add(info_item); }
       void InsertTrackInfoItem(ConvolverTrackInfoItem &info_item, int idx) { mTrackInfo.Insert(info_item, idx); }
       void DeleteTrackInfoItem(int idx)                                  { mTrackInfo.RemoveAt(idx); }
       ConvolverTrackInfoItem *GetTrackInfoItem(int idx) { if(idx < int(mTrackInfo.Count())) return &mTrackInfo[idx];
                                                    return 0; }
                                                    
       int GetTrackInfoCount()  const { return mTrackInfo.Count(); }
    //   void SortConvolverTrackInfoList()     { mTrackInfo.Sort(AFTrackProperties::CompareConvolverTrackInfoItems); } // Maybe not used...
       
       static int CompareTrackInfoItems(ConvolverTrackInfoItem *first, 
                                        ConvolverTrackInfoItem *second);
       
       int  GetInputRowsCount() const  { return mAudioDataRows; }
       int  GetFilterRowsCount()    const  { return mFilterRows; }
       void SetAudioDataRowsCount(const int value) { mAudioDataRows = value; }
       void SetFilterRowsCount(const int value)    { mFilterRows = value; }
       
       ConvolverTrackProperties(int n = 0) 
       {
          if(!n) mTrackInfo.Alloc(n);
          printf("Now array has %d (%d) elements\n", n, int(mTrackInfo.Count()));
       }
    };
}


#endif //__AURORA_CONVOLVER_TRPROP_H__

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


