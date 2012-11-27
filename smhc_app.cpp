/***************************************************************
 * Name:      smhc_app.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#include <wx/app.h>
#include <wx/image.h>

#include "smhc_app.h"
#include "smhc_main.h"
#include "msn.xpm"

IMPLEMENT_APP(MyApp)

MyApp::MyApp() {
    myapp_shut_down = false;
}

bool MyApp::OnInit() {


    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK ) {
        MyFrame* Frame = new MyFrame(0);
        Frame->SetIcon(wxIcon(msn_xpm));
        Frame->Show();
        SetTopWindow(Frame);
    }

    return wxsOK;
}

