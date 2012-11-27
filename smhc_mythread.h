/***************************************************************
 * Name:      smhc_mythread.h
 * Purpose:   Thread definition
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-15
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#ifndef MYTHREAD_H_INCLUDED
#define MYTHREAD_H_INCLUDED

#include <memory>
#include <wx/thread.h>
#include <wx/textfile.h>
#include <wx/notifmsg.h>
#include "smhc_mycurl.h"

class MyThread : public wxThread {
public:
    explicit MyThread();
    virtual ~MyThread();
    virtual void* Entry();

    static void TransferVariables();
    static int GetGood()    {return num_good;}
    static int GetBad()     {return num_bad;}
    static int GetOverall() {return num_overall;}
    static int GetCurrent() {return num_current;}

    static wxString Read();
    static bool Check();
    static void CloseFiles();

private:
    std::unique_ptr<MyCURL> mt_curl;

    static wxCriticalSection mt_critsect;
    static wxTextFile file_src;
    static wxTextFile file_good;
    static int num_good;
    static int num_bad;
    static int num_current;
    static int num_overall;

    static wxString notif_str;
    static std::unique_ptr<wxNotificationMessage> m_notif_msg;
};


#endif

