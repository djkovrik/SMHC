/***************************************************************
 * Name:      mycurl.h
 * Purpose:   Defines for CURL-based custom class
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#ifndef SMHC_MYCURL_H_INCLUDED
#define SMHC_MYCURL_H_INCLUDED

#include <wx/string.h>
#include <curl/curl.h>
#include <curl/easy.h>

class MyCURL {
public:
    MyCURL();
    ~MyCURL();
    void SetLogin(wxString login);
    bool IsAvailable();
private:
    void InnerInit();

    static wxString url;
    CURL* curl;
};

#endif
