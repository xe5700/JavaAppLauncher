#include "JavaDLUrl.h"
using namespace JAVA_URLS;
wchar_t* JAVA_URLS::Txpjre8, * JAVA_URLS::Tjre9,
* JAVA_URLS::Tjre10, * JAVA_URLS::Tjre11,
* JAVA_URLS::Tjre12, * JAVA_URLS::Tjre13,
* JAVA_URLS::Tjre14, * JAVA_URLS::Tjre15,
* JAVA_URLS::Tjre16, * JAVA_URLS::Tjre8_32,
* JAVA_URLS::Tjre8_64, * JAVA_URLS::Tjre7_64,
* JAVA_URLS::Tjre7_32 = NULL;
void JAVA_URLS::loadJavaDLURLs(INIReader& inir_) {
	std::string xpjre8 = inir_.Get("App", "xpjre8", "http://java.dstats.xyz/webapps/download/AutoDL?BundleId=225353_090f390dda5b47b9b721c7dfaa008135");
	std::string jre8_32 = inir_.Get("App", "jre8_32", "http://java.dstats.xyz/webapps/download/AutoDL?BundleId=234472_96a7b8442fe848ef90c96a2fad6ed6d1");
	std::string jre8_64 = inir_.Get("App", "jre8_64", "http://java.dstats.xyz/webapps/download/AutoDL?BundleId=234474_96a7b8442fe848ef90c96a2fad6ed6d1");
	std::string jre7_32 = inir_.Get("App", "jre7_32", "http://java.dstats.xyz/webapps/download/GetFile/1.7.0_80-b13/windows-i586/jre1.7.0_80-c.msi");
	std::string jre7_64 = inir_.Get("App", "jre7_64", "http://java.dstats.xyz/webapps/download/GetFile/1.7.0_80-b13/windows-x64/jre1.7.0_80-c.msi");
	std::string jre9 = inir_.Get("App", "jre9", "");
	std::string jre10 = inir_.Get("App", "jre10", "http://java.dstats.xyz/webapps/download/AutoDL?BundleId=234376_19aef61b38124481863b1413dce1855f");
	std::string jre11 = inir_.Get("App", "jre11", "");
	std::string jre12 = inir_.Get("App", "jre12", "");
	std::string jre13 = inir_.Get("App", "jre13", "");
	std::string jre14 = inir_.Get("App", "jre14", "");
	std::string jre15 = inir_.Get("App", "jre15", "");
	std::string jre16 = inir_.Get("App", "jre16", "");
	Txpjre8 = char2Wchar_t(xpjre8.c_str());
	Tjre7_32 = char2Wchar_t(jre7_32.c_str());
	Tjre7_64 = char2Wchar_t(jre7_64.c_str());
	Tjre8_32 = char2Wchar_t(jre8_32.c_str());
	Tjre8_64 = char2Wchar_t(jre8_64.c_str());
	Tjre9 = char2Wchar_t(jre9.c_str());
	Tjre10 = char2Wchar_t(jre10.c_str());
	Tjre11 = char2Wchar_t(jre11.c_str());
	Tjre12 = char2Wchar_t(jre12.c_str());
	Tjre13 = char2Wchar_t(jre13.c_str());
	Tjre14 = char2Wchar_t(jre14.c_str());
	Tjre15 = char2Wchar_t(jre15.c_str());
	Tjre16 = char2Wchar_t(jre16.c_str());
}
