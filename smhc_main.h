/***************************************************************
 * Name:      smhc_main.h
 * Purpose:   Defines Application frame
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#ifndef SMHC_MAIN_H_INCLUDED
#define SMHC_MAIN_H_INCLUDED

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/textfile.h>


class MyThread;

class MyFrame: public wxFrame
{
public:
    MyFrame(wxWindow* parent,wxWindowID id = -1);
    virtual ~MyFrame();

    // Functions for filenames transfer
    static wxString GetSourceName() {return filename_source;}
    static wxString GetGoodsName()  {return filename_good;  }

private:
    static wxString    filename_source;
    static wxString    filename_good;
    size_t      n_threads_num;

    size_t      n_alive_threads;
    int         n_good;
    int         n_bad;
    int         n_overall;

    // IDs
    static const long ID_TEXTCTRL_SRC;
    static const long ID_BUTTON_BROWSE;
    static const long ID_SPINCTRL;
    static const long ID_BUTTON_START;
    static const long ID_BUTTON_STOP;
    static const long ID_BUTTON_EXIT;
    static const long ID_STATICTEXT1;
    static const long ID_STATICTEXT_THALIVE;
    static const long ID_STATICTEXT2;
    static const long ID_STATICTEXT_LINES;
    static const long ID_STATICTEXT3;
    static const long ID_STATICTEXT_BAD;
    static const long ID_STATICTEXT4;
    static const long ID_STATICTEXT_GOOD;
    static const long ID_GAUGE_MAIN;
    static const long ID_PANELMAIN;

    //Declarations
    wxStaticText* StaticTextBad;
    wxStaticText* StaticTextNumLines;
    wxButton* ButtonExit;
    wxStaticText* StaticText2;
    wxGauge* GaugeMain;
    wxButton* ButtonBrowse;
    wxStaticText* StaticTextNumThAlive;
    wxStaticText* StaticText1;
    wxStaticText* StaticText3;
    wxTextCtrl* TextCtrlSrc;
    wxPanel* PanelMain;
    wxSpinCtrl* SpinCtrlThreads;
    wxButton* ButtonStart;
    wxButton* ButtonStop;
    wxStaticText* StaticText4;
    wxStaticText* StaticTextGood;

    // Handlers

    // For buttons
    void OnButtonBrowseClick(wxCommandEvent& event);
    void OnButtonStartClick(wxCommandEvent& event);
    void OnButtonStopClick(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);

    void OnIdle(wxIdleEvent &event);


    // --- Utilites

    // Creates a new thread (NOT RUNS)
    MyThread *CreateThread();

DECLARE_EVENT_TABLE()
};

#endif
