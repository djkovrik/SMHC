/***************************************************************
 * Name:      smhc_mythread.cpp
 * Purpose:   Code for thread class
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-15
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#include "smhc_mythread.h"
#include "smhc_app.h"
#include "smhc_main.h"

DECLARE_APP(MyApp)

wxTextFile  MyThread::file_src;
wxTextFile  MyThread::file_good;
wxCriticalSection MyThread::mt_critsect;

int         MyThread::num_good = 0;
int         MyThread::num_bad = 0;
int         MyThread::num_current = 0;
int         MyThread::num_overall = 0;
wxString    MyThread::notif_str = wxEmptyString;

MyThread::MyThread() :
    wxThread(),
    mt_curl(new MyCURL) {

}

wxThread::ExitCode MyThread::Entry() {

    while (Check()) {
        {
            wxCriticalSectionLocker locker(wxGetApp().myapp_critsect);

            // If stop button pressed then return immediately
            if ( wxGetApp().myapp_shut_down ) {
                return NULL;
            }
        }
        // check if just this thread was asked to exit
        if ( TestDestroy() )
            break;

        wxString login = Read();

        //mt_curl->SetLogin(login.BeforeFirst('@'));
        mt_curl->SetLogin(login);

        if (mt_curl->IsAvailable()) {
            wxCriticalSectionLocker lock(mt_critsect);
            num_good++;
            file_good.AddLine(login);
            file_good.Write();
            notif_str += login;
            notif_str += '\n';
        }
        else {
            wxCriticalSectionLocker lock(mt_critsect);
            num_bad++;
        }
    }
    return NULL;
}

MyThread::~MyThread() {

    //delete mt_curl;

    wxCriticalSectionLocker locker(wxGetApp().myapp_critsect);

    wxArrayThread& threads = wxGetApp().myapp_threads;
    threads.Remove(this);

    if ( threads.IsEmpty() )
    {
        // signal that no threads is waiting
        if ( wxGetApp().myapp_shut_down )
        {
            wxGetApp().myapp_shut_down = false;

            wxGetApp().myapp_semaphore_done.Post();
        }
    }
}


void MyThread::TransferVariables() {

    wxString s = MyFrame::GetSourceName();
    wxString g = MyFrame::GetGoodsName();


    file_src.Open(s);

    if (file_src.IsOpened()) {
        num_overall = file_src.GetLineCount();
    }

    file_good.Create(g);
    file_good.Open();
}

wxString MyThread::Read()  {
    wxString line;
        {
            wxCriticalSectionLocker read_lock(mt_critsect);
            line = file_src[num_current];
            num_current++;
        }
    return line;
}

bool MyThread::Check() {
    wxCriticalSectionLocker read_lock(mt_critsect);
    return num_current < num_overall;
}

void MyThread::CloseFiles() {

    wxCriticalSectionLocker locker(wxGetApp().myapp_critsect);
    if ( wxGetApp().myapp_shut_down ) {
        file_src.Close();
        file_good.Close();
    }
}
