/***************************************************************
 * Name:      smhc_app.h
 * Purpose:   Defines Application Class
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#ifndef SMHC_APP_H_INCLUDED
#define SMHC_APP_H_INCLUDED

#include <wx/app.h>
#include <wx/dynarray.h>

WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);

class MyApp : public wxApp {
public:
    MyApp();
    virtual ~MyApp() {}
    virtual bool OnInit();

    // Protects all below members
    wxCriticalSection myapp_critsect;

    // Array for alive threads
    wxArrayThread myapp_threads;

    // Semaphore uset for threads waiting
    wxSemaphore myapp_semaphore_done;

    // indicates that all threads should exit
    bool myapp_shut_down;
};


#endif
