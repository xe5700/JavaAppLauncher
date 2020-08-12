#include "Config.h"


string Config::title;
int Config::javaV;
string Config::jvmArgs;
bool Config::uac;
string Config::appjvm;
bool Config::debug;
int Config::javaVer = 8;
string Config::jal_package = "net/xjboss/jal/";
string Config::aria2ExeName = "JAL_tmp_Aria2c.exe";
bool Config::force32 = false;
bool Config::force64 = false;
bool Config::isJavaw = false;
bool Config::awaitJava = false;
bool Config::workdir = false;

bool Config::endsWith(string str, char* with) {
	return strlen(with) == str.length() - str.find(with);
}

bool Config::loadConfig(wstring execPath, bool debug) {
	INIReader* langinfo = NULL;
	byte* zip_header = new byte[4];
	int ziph_int = 0x04034b50;
	memcpy(zip_header, &ziph_int, 4);
	void* zipStream = NULL;
	void* handler = NULL;
	bool ret = false;
	bool done = false;
	char* execP = wchar_t2Char(execPath.c_str());
	if (mz_stream_os_create(&zipStream) && mz_zip_create(&handler)) {
		int ret;
		int index_ = 0;
		if ((ret = mz_stream_open(zipStream, execP, MZ_OPEN_MODE_READ)) == MZ_OK) {
			if (mz_zip_open(handler, zipStream, MZ_OPEN_MODE_READ) == MZ_OK) {
				string lang_name = "";
				string lang_name_p = "";
				string lang_name_hl = "";
				INIReader* lang1 = NULL;
				INIReader* lang2 = NULL;
				INIReader* lang3 = NULL;
				bool foundPL = false;
				if ((ret = mz_zip_goto_first_entry(handler)) == MZ_OK) {
					do {
						mz_zip_file* file_inf;
						mz_zip_entry_get_info(handler, &file_inf);
						auto fn = string(file_inf->filename);
						if (fn.find(jal_package)==0) {
							fn = fn.substr(fn.length() - jal_package.length());
							if (fn == "app.ini") {
								INIReader* inir2 = readIniInZip(handler, file_inf);
								INIReader inir_ = *inir2;
								title = inir_.Get("App", "title", "Java Launcher");
								int ver = inir_.GetInteger("App", "javaVer", 8);
								jvmArgs = inir_.Get("App", "jvmArgs", "");
								auto langs = inir_.Get("App", "lang", "");
								appjvm = inir_.Get("App", "appJvm", "");
								//std::string sAjvm64 = inir_.Get("App", "appJvm64", "");
								auto forceJvm = inir_.Get("App", "forceJvmBit", "");
								awaitJava = inir_.GetBoolean("App", "await", false);
								if (forceJvm == "32") {
									force32 = true;
								}
								else if (forceJvm == "64") {
									force64 = true;
								}
								isJavaw = inir_.GetBoolean("App", "javaw", true);
								loadJavaDLURLs(inir_);
								bool enableUAC = inir_.GetBoolean("App", "uac", false);
								javaV = ver;
								if (!langs.empty()) {
									lang_name = langs;
									lang_name += ".ini";
									if (lang_name.find("zh")==0) {
										lang_name_hl = "zh_CN";
									}
									else {
										lang_name_hl = "en_US";
									}
								}
								else {
									LANGID lang = GetUserDefaultUILanguage();
									short plang = lang << 5 >> 5;
									short slang = lang >> 9;
									if (plang == LANG_CHINESE) {
										lang_name_p = "zh";
										switch (slang)
										{
										case SUBLANG_CHINESE_SIMPLIFIED:
											lang_name = "zh_CN";
											break;
										case SUBLANG_CHINESE_TRADITIONAL:
											lang_name = "zh_TW";
											break;
										case SUBLANG_CHINESE_HONGKONG:
											lang_name = "zh_HK";
											break;
										case SUBLANG_CHINESE_MACAU:
											lang_name = "zh_MO";
											break;
										case SUBLANG_CHINESE_SINGAPORE:
											lang_name = "zh_SG";
											break;
										default:
											lang_name = "zh_CN";
										}
										lang_name_hl = "zh_CN";
									}
									else if (plang == LANG_ENGLISH) {
										lang_name_p = "en";
										switch (plang)
										{
										case SUBLANG_ENGLISH_US:
											lang_name = "en_US";
											break;
										case SUBLANG_ENGLISH_UK:
											lang_name = "en_UK";
											break;
										default:
											lang_name = "en_US";
											break;
										}
									}

								}
								uac = enableUAC;
								delete inir2;
							}
						}
					} while ((ret = mz_zip_goto_next_entry(handler)) == MZ_OK);
				}
				if (!lang_name.empty()) {
					if ((ret = mz_zip_goto_first_entry(handler)) == MZ_OK) {
						do {
							mz_zip_file* file_inf;
							mz_zip_entry_get_info(handler, &file_inf);
							auto fn = string(file_inf->filename);
							if (fn.find(jal_package) == 0) {
								fn = fn.substr(fn.length() - jal_package.length());
								if (fn.find("lang/") == 0) {
									fn = fn.substr(5);
									if (fn.find(lang_name) == 0 && endsWith(lang_name, ".ini")) {
										lang1 = readIniInZip(handler, file_inf);
										foundPL = true;

									}
									else if (lang_name.find(lang_name_p)==0 && lang3 == NULL && fn.find(lang_name_p)==0) {
										goto doing_lang3;
									}
									else if (fn.find(lang_name_hl) == 0) {
										if (endsWith(lang_name, ".ini")) {
											lang2 = readIniInZip(handler, file_inf);
										}
									}
									else if (lang2 == NULL && lang3 == NULL && fn.find(lang_name_p) == 0) {
									doing_lang3:
										if (endsWith(lang_name, ".ini")) {
											lang3 = readIniInZip(handler, file_inf);
										}
									}
								}
								if (lang1 != NULL)break;
							}
						} while ((ret = mz_zip_goto_next_entry(handler)) == MZ_OK);
					}
				}
				if (lang1 != NULL) {
					langinfo = lang1;
				}
				else if (!lang_name.empty() && lang_name_p.empty() && lang_name.find(lang_name_p)==0 && lang3 != NULL) {
					langinfo = lang3;
				}
				else if (lang2 != NULL) {
					langinfo = lang2;
				}
				else {
					langinfo = lang3;
				}
				if (lang1 != langinfo && lang1 != NULL) {
					delete lang1;
				}
				if (lang2 != langinfo && lang2 != NULL) {
					delete lang2;
				}
				if (lang3 != langinfo && lang3 != NULL) {
					delete lang3;
				}
				if (langinfo != NULL) {

				}
				ret = true;
			}
		}
		else {
			debugRet(ret);
		}
	}
	else {
		wchar_t tmp[16];
		_itow(ret, tmp, 10);
		OutputDebugString(tmp);
	}

	if (zipStream != NULL && mz_stream_is_open(zipStream)) {
		mz_stream_close(zipStream);
	}
	if (handler != NULL && mz_zip_reader_is_open(handler)) {
		mz_stream_close(handler);
	}
	delete[] execP;
	if (langinfo != NULL)return 0;
	return ret;
}
