#pragma once
#include "Main.h"
class LANG {
public:
	wchar_t *dlFail, *noticeYou, *win32,*win64, *noJava, *dlJavaSelf, *cantDLJava, *waitSetupJava, *TDLTITLE,*notsupportedOS;
	LANG(INIReader* langini);
};