/***************************************************************
 * Name:      smhc_mydroptarget.cpp
 * Purpose:   Implementation of OnDropFiles method
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 ********
******************************************************/

#include "smhc_mydroptarget.h"

bool MyDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) {

    wxString name = filenames[0];

    if (name.AfterLast('.').Lower() == "txt") {
        my_owner->SetValue(name);
    }


    return true;
}
