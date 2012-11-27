/***************************************************************
 * Name:      mydroptarget.cpp
 * Purpose:   Class that enables file D&D support for wxTextCtrl
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#ifndef SMHC_MYDROPTARGET_H_INCLUDED
#define SMHC_MYDROPTARGET_H_INCLUDED

#include <wx/textctrl.h>
#include <wx/dnd.h>

class MyDropTarget : public wxFileDropTarget {
public:
    MyDropTarget(wxTextCtrl* owner) : my_owner(owner) {}
    virtual bool OnDropFiles (wxCoord x, wxCoord y, const wxArrayString &filenames);
private:
    wxTextCtrl* my_owner;
};

#endif
