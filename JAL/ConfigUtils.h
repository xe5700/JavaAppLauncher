#pragma once
#include "Main.h"
namespace ConfigUtils {
	wchar_t* char2Wchar_t(const char* source, size_t encoding = CP_THREAD_ACP);
	char* wchar_t2Char(const wchar_t* source, size_t encoding = CP_THREAD_ACP);
	INIReader* readIniInZip(void* handler, mz_zip_file* file_inf);
}