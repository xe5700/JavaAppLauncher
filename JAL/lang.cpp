#include "Main.h"
#include "Lang.h"
#include "ConfigUtils.h"
using namespace ConfigUtils;
LANG::LANG(INIReader* langini)
{
	this->dlFail = char2Wchar_t(langini->Get("lang", "auto_dl_fail", "").c_str(), CP_UTF8);
	this->noticeYou = char2Wchar_t(langini->Get("lang", "notice_you", "").c_str(), CP_UTF8);
	this->win64 = char2Wchar_t(langini->Get("lang", "win64", "").c_str(), CP_UTF8);
	this->win32 = char2Wchar_t(langini->Get("lang", "win32", "").c_str(), CP_UTF8);
	this->noJava = char2Wchar_t(langini->Get("lang", "you_not_installed_java", "").c_str(), CP_UTF8);
	this->dlJavaSelf = char2Wchar_t(langini->Get("lang", "dl_java_yourself", "").c_str(), CP_UTF8);
	this->cantDLJava = char2Wchar_t(langini->Get("lang", "can_not_download_java", "").c_str(), CP_UTF8);
	this->waitSetupJava = char2Wchar_t(langini->Get("lang", "wait_for_setup_java", "").c_str(), CP_UTF8);
	this->TDLTITLE = char2Wchar_t(langini->Get("lang", "download_title", "").c_str(), CP_UTF8);
	this->notsupportedOS = char2Wchar_t(langini->Get("lang", "system_is_not_support", "").c_str(), CP_UTF8);
}
