/***************************************************************
 * Name:      smhc_main.cpp
 * Purpose:   Code for Application frame
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#include <wx/string.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>

#if defined(__WXMSW__)
    #include <wx/notifmsg.h>
    #include <wx/taskbar.h>
#endif

#include "smhc_main.h"
#include "smhc_app.h"
#include "smhc_mythread.h"
#include "smhc_mydroptarget.h"

#include "msn.xpm"


DECLARE_APP(MyApp)

//{Id Init
const long MyFrame::ID_TEXTCTRL_SRC = wxNewId();
const long MyFrame::ID_BUTTON_BROWSE = wxNewId();
const long MyFrame::ID_SPINCTRL = wxNewId();
const long MyFrame::ID_BUTTON_START = wxNewId();
const long MyFrame::ID_BUTTON_STOP = wxNewId();
const long MyFrame::ID_BUTTON_EXIT = wxNewId();
const long MyFrame::ID_STATICTEXT1 = wxNewId();
const long MyFrame::ID_STATICTEXT_THALIVE = wxNewId();
const long MyFrame::ID_STATICTEXT2 = wxNewId();
const long MyFrame::ID_STATICTEXT_LINES = wxNewId();
const long MyFrame::ID_STATICTEXT3 = wxNewId();
const long MyFrame::ID_STATICTEXT_BAD = wxNewId();
const long MyFrame::ID_STATICTEXT4 = wxNewId();
const long MyFrame::ID_STATICTEXT_GOOD = wxNewId();
const long MyFrame::ID_GAUGE_MAIN = wxNewId();
const long MyFrame::ID_PANELMAIN = wxNewId();
//}

BEGIN_EVENT_TABLE(MyFrame,wxFrame)

END_EVENT_TABLE()

wxString MyFrame::filename_source = wxEmptyString;
wxString MyFrame::filename_good = "good.txt";

MyFrame::MyFrame(wxWindow* parent,wxWindowID id) :
    n_threads_num(0),
    n_alive_threads(0),
    n_good(0),
    n_bad(0),
    n_overall(0),
    n_notify_str(wxEmptyString)
    #if defined(__WXMSW__)
        ,n_taskbarIcon(new wxTaskBarIcon()),
        n_notify(new wxNotificationMessage("SMHC", wxEmptyString, this))
    #endif
    {
#if defined(__WXMSW__)
    n_taskbarIcon->SetIcon(wxIcon(msn_xpm), "Hotmail checker");
    n_notify->UseTaskBarIcon(n_taskbarIcon.get());
#endif
    //n_notify->SetMessage("Program started!");
    //n_notify->Show();

    // UI generation
    // Initialize sizers
    wxStaticBoxSizer* StaticBoxSizerSrc;
    wxBoxSizer* BoxSizerCommon;
    wxFlexGridSizer* FlexGridSizerInfo;
    wxBoxSizer* BoxSizerRight;
    wxBoxSizer* BoxSizerMain;
    wxBoxSizer* BoxSizerLeft;
    wxStaticBoxSizer* StaticBoxSizerCtrl;
    wxBoxSizer* BoxSizerThreads;
    wxStaticBoxSizer* StaticBoxSizerInfo;
    wxStaticBoxSizer* StaticBoxSizerThreads;
    wxBoxSizer* topSizer;

    // Created with wxSmith
    Create(parent, wxID_ANY, "SMHC by DJ_Kovrik", wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxCLIP_CHILDREN, _T("wxID_ANY"));
    topSizer = new wxBoxSizer(wxHORIZONTAL);
    PanelMain = new wxPanel(this, ID_PANELMAIN, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANELMAIN"));
    BoxSizerMain = new wxBoxSizer(wxVERTICAL);
    BoxSizerCommon = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerLeft = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizerSrc = new wxStaticBoxSizer(wxHORIZONTAL, PanelMain, _("Source:"));
    TextCtrlSrc = new wxTextCtrl(PanelMain, ID_TEXTCTRL_SRC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL_SRC"));
    StaticBoxSizerSrc->Add(TextCtrlSrc, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    ButtonBrowse = new wxButton(PanelMain, ID_BUTTON_BROWSE, _("Browse"), wxDefaultPosition, wxSize(55,25), 0, wxDefaultValidator, _T("ID_BUTTON_BROWSE"));
    StaticBoxSizerSrc->Add(ButtonBrowse, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerLeft->Add(StaticBoxSizerSrc, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerThreads = new wxBoxSizer(wxHORIZONTAL);
    StaticBoxSizerThreads = new wxStaticBoxSizer(wxHORIZONTAL, PanelMain, _("Threads:"));
    SpinCtrlThreads = new wxSpinCtrl(PanelMain, ID_SPINCTRL, _T("50"), wxDefaultPosition, wxSize(50,20), 0, 1, 100, 10, _T("ID_SPINCTRL"));
    //SpinCtrlThreads->SetValue(_T("50"));
    StaticBoxSizerThreads->Add(SpinCtrlThreads, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerThreads->Add(StaticBoxSizerThreads, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    StaticBoxSizerCtrl = new wxStaticBoxSizer(wxHORIZONTAL, PanelMain, wxEmptyString);
    ButtonStart = new wxButton(PanelMain, ID_BUTTON_START, _("Start"), wxDefaultPosition, wxSize(55,25), 0, wxDefaultValidator, _T("ID_BUTTON_START"));
    StaticBoxSizerCtrl->Add(ButtonStart, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    ButtonStop = new wxButton(PanelMain, ID_BUTTON_STOP, _("Stop"), wxDefaultPosition, wxSize(55,25), 0, wxDefaultValidator, _T("ID_BUTTON_STOP"));
    ButtonStop->Disable();
    StaticBoxSizerCtrl->Add(ButtonStop, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    ButtonExit = new wxButton(PanelMain, ID_BUTTON_EXIT, _("Exit"), wxDefaultPosition, wxSize(55,25), 0, wxDefaultValidator, _T("ID_BUTTON_EXIT"));
    StaticBoxSizerCtrl->Add(ButtonExit, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerThreads->Add(StaticBoxSizerCtrl, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerLeft->Add(BoxSizerThreads, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizerCommon->Add(BoxSizerLeft, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerRight = new wxBoxSizer(wxHORIZONTAL);
    StaticBoxSizerInfo = new wxStaticBoxSizer(wxHORIZONTAL, PanelMain, _("Info:"));
    FlexGridSizerInfo = new wxFlexGridSizer(4, 2, 0, 0);
    StaticText1 = new wxStaticText(PanelMain, ID_STATICTEXT1, _("Threads:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizerInfo->Add(StaticText1, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
    StaticTextNumThAlive = new wxStaticText(PanelMain, ID_STATICTEXT_THALIVE, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_THALIVE"));
    FlexGridSizerInfo->Add(StaticTextNumThAlive, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
    StaticText2 = new wxStaticText(PanelMain, ID_STATICTEXT2, _("Overall:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizerInfo->Add(StaticText2, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
    StaticTextNumLines = new wxStaticText(PanelMain, ID_STATICTEXT_LINES, _("0"), wxDefaultPosition, wxSize(60,13), 0, _T("ID_STATICTEXT_LINES"));
    FlexGridSizerInfo->Add(StaticTextNumLines, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 2);
    StaticText3 = new wxStaticText(PanelMain, ID_STATICTEXT3, _("Bad:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizerInfo->Add(StaticText3, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
    StaticTextBad = new wxStaticText(PanelMain, ID_STATICTEXT_BAD, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_BAD"));
    FlexGridSizerInfo->Add(StaticTextBad, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
    StaticText4 = new wxStaticText(PanelMain, ID_STATICTEXT4, _("Good:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizerInfo->Add(StaticText4, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
    StaticTextGood = new wxStaticText(PanelMain, ID_STATICTEXT_GOOD, _("0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_GOOD"));
    FlexGridSizerInfo->Add(StaticTextGood, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
    StaticBoxSizerInfo->Add(FlexGridSizerInfo, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerRight->Add(StaticBoxSizerInfo, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
    BoxSizerCommon->Add(BoxSizerRight, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    BoxSizerMain->Add(BoxSizerCommon, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    GaugeMain = new wxGauge(PanelMain, ID_GAUGE_MAIN, 100, wxDefaultPosition, wxSize(100,10), 0, wxDefaultValidator, _T("ID_GAUGE_MAIN"));
    BoxSizerMain->Add(GaugeMain, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    PanelMain->SetSizer(BoxSizerMain);
    BoxSizerMain->Fit(PanelMain);
    BoxSizerMain->SetSizeHints(PanelMain);
    topSizer->Add(PanelMain, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints(this);
    TextCtrlSrc->SetDropTarget(new MyDropTarget(TextCtrlSrc));

    // Connect handlers
    Connect(ID_BUTTON_BROWSE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnButtonBrowseClick);
    Connect(ID_BUTTON_START,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnButtonStartClick);
    Connect(ID_BUTTON_STOP,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnButtonStopClick);
    Connect(ID_BUTTON_EXIT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnQuit);
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyFrame::OnIdle));
}

MyFrame::~MyFrame() {

    {
        wxCriticalSectionLocker locker(wxGetApp().myapp_critsect);

        // Check for running threads
        const wxArrayThread& threads = wxGetApp().myapp_threads;
        size_t tcount = threads.GetCount();

        // If there is no running threads - return immediately
        if ( !tcount )
            return;

        // Set signaling flag - all threads should exit
        wxGetApp().myapp_shut_down = true;
    }

    // Wait for terminate
    wxGetApp().myapp_semaphore_done.Wait();
}


void MyFrame::OnButtonBrowseClick(wxCommandEvent& event) {

    wxString filename = wxFileSelector("Please select source file:",
                                       wxEmptyString,
                                       wxEmptyString,
                                       wxEmptyString,
                                       "Text files (*.txt)|*.txt",
                                       wxFD_OPEN|wxFD_FILE_MUST_EXIST,
                                       this);

    if (!filename.empty()) {
        TextCtrlSrc->SetValue(filename);
    }

    event.Skip();
}

void MyFrame::OnButtonStartClick(wxCommandEvent& event) {

    wxString temp = TextCtrlSrc->GetValue();

    if (temp == wxEmptyString) {
        return;
    }

    filename_source = TextCtrlSrc->GetValue();
    n_threads_num   = SpinCtrlThreads->GetValue();

    MyThread::TransferVariables();

    GaugeMain->SetRange( MyThread::GetOverall() );

    wxArrayThread threads;
    size_t n;

    // Create N threads
    for ( n = 0; n < n_threads_num; n++ ) {
        wxThread *thr = CreateThread();
        threads.Add(thr);
    }

    // ...and then start them
    for ( n = 0; n < n_threads_num; n++ ) {
        threads[n]->Run();
    }

    event.Skip();
}

void MyFrame::OnButtonStopClick(wxCommandEvent& event) {

    wxCriticalSectionLocker enter(wxGetApp().myapp_critsect);
    wxGetApp().myapp_shut_down = true;

    ButtonStop->Disable();

    MyThread::CloseFiles();

    event.Skip();
}

void MyFrame::OnQuit(wxCommandEvent& event) {

    wxCriticalSectionLocker enter(wxGetApp().myapp_critsect);

    // myapp_threads should always be empty...
    if (wxGetApp().myapp_threads.IsEmpty()) {
        Close(true);
    }
    // ... so this check is a little bit superfluously
    else {
        wxGetApp().myapp_threads.Empty();
        Close(true);
        }

    event.Skip();
}


MyThread* MyFrame::CreateThread() {

    MyThread* thread = new MyThread;


    if ( thread->Create() != wxTHREAD_NO_ERROR ) {
        // If logger not defined (release build)
        // it would be standard messagebox.
        wxLogMessage("Can't create thread!");
    }

    wxCriticalSectionLocker enter(wxGetApp().myapp_critsect);
    wxGetApp().myapp_threads.Add(thread);

    return thread;
}

void MyFrame::OnIdle(wxIdleEvent &event) {

    // Gauge progress
    GaugeMain->SetValue ( MyThread::GetCurrent() );

    {
        wxCriticalSectionLocker enter(wxGetApp().myapp_critsect);

        size_t nRunning = 0;
        size_t nCount = wxGetApp().myapp_threads.Count();

        for ( size_t n = 0; n < nCount; n++ ) {
            if ( wxGetApp().myapp_threads[n]->IsRunning() )
                nRunning++;
        }

        if (n_alive_threads != nRunning) {
            n_alive_threads = nRunning;

            wxString temp;
            temp << n_alive_threads;
            StaticTextNumThAlive->SetLabel(temp);
        }

    }


    if (n_alive_threads > 0 ) {
            ButtonStop->Enable();
            ButtonExit->Disable();
            ButtonStart->Disable();
    }

    else {
        ButtonStop->Disable();
        ButtonExit->Enable();
        ButtonStart->Enable();
    }


    if (n_good != MyThread::GetGood()) {
        n_good = MyThread::GetGood();

        wxString temp;
        temp << n_good;
        StaticTextGood->SetLabel(temp);
    }

    if (n_bad != MyThread::GetBad()) {
        n_bad = MyThread::GetBad();

        wxString temp;
        temp << n_bad;
        StaticTextBad->SetLabel(temp);
    }

    if (n_overall != MyThread::GetOverall()) {
        n_overall = MyThread::GetOverall();

        wxString temp;
        temp << n_overall;
        StaticTextNumLines->SetLabel(temp);
    }
#if defined(__WXMSW__)
    if (n_notify_str != MyThread::GetNotifyStr()) {
        n_notify_str = MyThread::GetNotifyStr();

        n_notify->SetMessage(n_notify_str);
        n_notify->Show();

    }
#endif
    event.Skip();
}
