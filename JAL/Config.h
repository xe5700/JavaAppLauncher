#pragma once
#include "Main.h"
#include "JavaDLUrl.h"
#include "ConfigUtils.h"
namespace Config {
	using namespace ConfigUtils;
	using namespace JAVA_URLS;
	extern wchar_t* title;
	extern int javaV;
	extern wchar_t* jvmArgs;
	extern bool uac;
	extern wchar_t* appjvm;
	extern bool debug;
	extern int javaVer;
	extern char* jal_package;
	extern wchar_t* aria2ExeName;

	extern bool force32;
	extern bool force64;
	extern bool isJavaw;
	bool loadConfig(wchar_t* execPath, bool debug);
}