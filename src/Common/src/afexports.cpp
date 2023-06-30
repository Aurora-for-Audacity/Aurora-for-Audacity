/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries

  afexports.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::Exports
\brief Data exports manager parent class

  This class manages exports to/from files and clipboard.

*//*******************************************************************/
#include <aurora.h>

wxString Aurora::Exports::Extension(const Aurora::Exports::FileFormat fmt)
{
    switch(fmt)
    {
        case Aurora::Exports::FileFormat::Txt:
            return wxString("txt");
            
        case Aurora::Exports::FileFormat::Csv:
            return wxString("csv");
            
        case Aurora::Exports::FileFormat::Prn:
            return wxString("prn");
    }
    assert(1);
    return wxString();
}

wxString Aurora::Exports::FieldsSeparator(const Aurora::Exports::Separator sep)
{
    switch(sep)
    {
        case Aurora::Exports::Separator::Tab:
            return wxString("\t");
            
        case Aurora::Exports::Separator::Comma:
            return wxString(",");
            
        case Aurora::Exports::Separator::Colon:
            return wxString(":");
            
        case Aurora::Exports::Separator::Semicolon:
            return wxString(";");
    }
    assert(1);
    return wxString();
}

void Aurora::Exports::CopyEntireTableToClipboard(const bool messageOnSuccess)
{
   wxClipboardLocker locker;
    
   if( !locker ) 
   {
      Aurora::EffectBase::MessageBox("Cannot open clipboard!",
    		                         Aurora::MessageType::Error);
      return;
   }
   wxTheClipboard->Clear();

   //Copy results to clipboard
   wxString data;
    
   if(!BuildParametersTable(data, Aurora::Exports::DataDestination::Clipboard))
   {
       Aurora::EffectBase::MessageBox("Error building table or table is too big (> 65535 rows)",
                                      Aurora::MessageType::Error);
       return;
   }
    
   if ( !wxTheClipboard->AddData(new wxTextDataObject(data)) )
   {
      Aurora::EffectBase::MessageBox("Can't copy data to the clipboard!",
                                     Aurora::MessageType::Error);
   }
   else if (messageOnSuccess)
   {
       Aurora::EffectBase::MessageBox("Data successifully copied to the clipboard!",
                                      Aurora::MessageType::Info);
   }
}

void Aurora::Exports::CopyGridSelectionToClipboard(wxGrid *pGrid,
                                                   const bool messageOnSuccess)
{
    wxClipboardLocker locker;
    
    if( !locker )
    {
        Aurora::EffectBase::MessageBox("Cannot open clipboard!",
                                       Aurora::MessageType::Error);
        return;
    }

   wxTheClipboard->Clear();

   //Copy results to clipboard  
   size_t i;
   int nRow, nCol;
   wxString data;
   int nRows = pGrid->GetNumberRows();
   int nCols = pGrid->GetNumberCols();
   
   const wxGridCellCoordsArray topLeftGrd  = pGrid->GetSelectionBlockTopLeft();
   const wxGridCellCoordsArray btmRightGrd = pGrid->GetSelectionBlockBottomRight();
   const wxGridCellCoordsArray sparseCellsGrd = pGrid->GetSelectedCells();
   const wxArrayInt rows = pGrid->GetSelectedRows();
   const wxArrayInt cols = pGrid->GetSelectedCols();

   bool bThereAreBoxes  = false;
   bool bThereAreRows   = false;
   bool bThereAreCols   = false;
   bool bThereAreSparse = false;
   
   int nMinRow = nRows -1; 
   int nMaxRow = 0;
   int nMinCol = nCols - 1; 
   int nMaxCol =0; 
   
   //find minimum grid subset containing all selections
   if((topLeftGrd.GetCount() > 0) && (btmRightGrd.GetCount() > 0))
   {   
      bThereAreBoxes = true;
      for(i = 0; i < topLeftGrd.GetCount(); i++)
      {
          nMinRow = (topLeftGrd[i].GetRow() < nMinRow)  ? topLeftGrd[i].GetRow()  : nMinRow;
          nMinCol = (topLeftGrd[i].GetCol() < nMinCol)  ? topLeftGrd[i].GetCol()  : nMinCol;
          nMaxRow = (btmRightGrd[i].GetRow() > nMaxRow) ? btmRightGrd[i].GetRow() : nMaxRow;
          nMaxCol = (btmRightGrd[i].GetCol() > nMaxCol) ? btmRightGrd[i].GetCol() : nMaxCol;
      }
   }
   if(rows.GetCount() > 0)
   {
       bThereAreRows = true;
       for(i = 0; i < rows.GetCount(); i++)
       {
           nMinRow = (rows[i] < nMinRow) ? rows[i] : nMinRow;
           nMaxRow = (rows[i] > nMaxRow) ? rows[i] : nMaxRow;
       }
       nMinCol = 0;
       nMaxCol = nCols -1;
   }
   if(cols.GetCount() > 0)
   {
       bThereAreCols = true;
       for(i = 0; i < cols.GetCount(); i++)
       {
           nMinCol = (cols[i] < nMinCol) ? cols[i] : nMinCol;
           nMaxCol = (cols[i] > nMaxCol) ? cols[i] : nMaxCol;
       }
       nMinRow = 0;
       nMaxRow = nRows - 1;
   }
   if(sparseCellsGrd.GetCount() > 0)
   {
       bThereAreSparse = true;
       for(i = 0; i < sparseCellsGrd.GetCount(); i++)
       {
          nMinRow = (sparseCellsGrd[i].GetRow() < nMinRow) ? sparseCellsGrd[i].GetRow()
                                                           : nMinRow;
          nMinCol = (sparseCellsGrd[i].GetCol() < nMinCol) ? sparseCellsGrd[i].GetCol()
                                                           : nMinCol;
          nMaxRow = (sparseCellsGrd[i].GetRow() > nMaxRow) ? sparseCellsGrd[i].GetRow()
                                                           : nMaxRow;
          nMaxCol = (sparseCellsGrd[i].GetCol() > nMaxCol) ? sparseCellsGrd[i].GetCol()
                                                           : nMaxCol;
           
       }
   }
   // Now the matrix size is (nMaxRow - nMinRow + 1) x (nMaxCol - nMinCol + 1) 
   bool bFound = false;
    
   for(nRow = nMinRow; nRow < (nMaxRow+1); nRow++) 
   {
       for(nCol = nMinCol; nCol < (nMaxCol+1); nCol++) 
       {
          if(bThereAreBoxes) 
          {
             for(i = 0; (i < topLeftGrd.GetCount()) &&  (!bFound); i++)
             {
                if( (nCol >= topLeftGrd[i].GetCol()) && (nCol <= btmRightGrd[i].GetCol()) &&
                    (nRow >= topLeftGrd[i].GetRow()) && (nRow <= btmRightGrd[i].GetRow()) )
                {
                    bFound = true;
                }
             }
          }
           
          if(bThereAreCols) 
          {
             for(i = 0; (i < cols.GetCount()) && (!bFound); i++)
             {
                 if( nCol == cols[i] )
                 {
                     bFound = true;
                 }
             }
          }
          if(bThereAreRows) 
          {
              for(i = 0; (i < rows.GetCount()) && (!bFound); i++)
              {
                  if( nRow == rows[i] )
                  {
                      bFound = true;
                  }
              }
          }
           
          if(bThereAreSparse) 
          {
              for(i = 0; (i < sparseCellsGrd.GetCount()) && (!bFound); i++)
              {
                if( (nCol == sparseCellsGrd[i].GetCol()) &&
                   (nRow == sparseCellsGrd[i].GetRow()) )
                {
                    bFound = true;
                }
              }
          }
          
          if(bFound) data << pGrid->GetCellValue(nRow, nCol);
          data << "\t";
          bFound = false;
       }
       data << LINE_BREAK;
   }
   
    if ( !wxTheClipboard->AddData(new wxTextDataObject(data)) )
    {
         Aurora::EffectBase::MessageBox("Can't copy data to the clipboard!",
                                        Aurora::MessageType::Error);
    }
    else if (messageOnSuccess)
    {
        Aurora::EffectBase::MessageBox("Data successifully copied to the clipboard!",
                                       Aurora::MessageType::Info);
    }
}

Aurora::Exports::Exports()
{
   // under X we usually want to use the primary selection by default (which
   // is shared with other apps)
   wxTheClipboard->UsePrimarySelection();
}
