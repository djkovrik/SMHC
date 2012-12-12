/***************************************************************
 * Name:      smhc_mycurl.cpp
 * Purpose:   Code for custom CURL-based Class
 * Author:    Sergey V. (DJ_Kovrik) (djkovrik at gmail.com)
 * Created:   2012-10-11
 * Copyright: Sergey V. (DJ_Kovrik)
 **************************************************************/

#include "smhc_mycurl.h"

wxString MyCURL::url = "http://accountservices.passport.net/pp1400/memberexists.srf";
size_t WriteData(void *buffer, size_t size, size_t nmemb, void *userp) {return 0;}


MyCURL::MyCURL() {
    curl = curl_easy_init();
    InnerInit();
}

MyCURL::~MyCURL() {
    curl_easy_cleanup(curl);
}

void MyCURL::InnerInit() {
    const char* myurl = url.ToUTF8();
    curl_easy_setopt(curl, CURLOPT_URL, myurl);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1229.79 Safari/537.4");
    curl_easy_setopt(curl, CURLOPT_HEADER, false);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
}

void MyCURL::SetLogin(wxString login) {

    wxString temp = "MSPMemberExists=" + login;
    const char* mycookie = temp.ToUTF8();
    curl_easy_setopt(curl, CURLOPT_COOKIE, mycookie);

}

bool MyCURL::IsAvailable() {

    struct curl_slist *cookies_list;
    CURLcode res;
    wxString str = wxEmptyString;

    curl_easy_perform(curl);
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies_list);

    if (res != CURLE_OK)
        return false;

    if (cookies_list)
        str = cookies_list->data;

    return !(str.AfterLast('\t') == "MemberExists" || str.AfterLast('\t') == "OtherError" || str.AfterLast('\t') == "ForbiddenWord");
}
