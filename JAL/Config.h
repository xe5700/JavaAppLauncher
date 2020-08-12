#pragma once
#include "Main.h"
#include "JavaDLUrl.h"
#include "ConfigUtils.h"
namespace Config {
	using namespace ConfigUtils;
	using namespace JAVA_URLS;
	extern string title;
	extern int javaV;
	extern string jvmArgs;
	extern bool uac;
	extern string appjvm;
	extern string appjvm64;
	extern bool debug;
	extern int javaVer;
	extern string jal_package;
	extern string aria2ExeName;

	extern bool force32;
	extern bool force64;
	extern bool isJavaw;
	extern bool awaitJava;
	extern bool workdir;
	bool endsWith(string str, char* with);
	bool loadConfig(wstring execPath, bool debug);
}