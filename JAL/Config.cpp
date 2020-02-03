#include "Config.h"


wchar_t* Config::title;
int Config::javaV;
wchar_t* Config::jvmArgs;
bool Config::uac;
wchar_t* Config::appjvm;
bool Config::debug;
int Config::javaVer = 8;
char* Config::jal_package = "net/xjboss/jal/";
wchar_t* Config::aria2ExeName = L"JAL_tmp_Aria2c.exe";
bool Config::force32 = false;
bool Config::force64 = false;
bool Config::isJavaw = false;
bool Config::loadConfig(wchar_t* execPath, bool debug) {
	INIReader* langinfo = NULL;
	byte* zip_header = new byte[4];
	int ziph_int = 0x04034b50;
	memcpy(zip_header, &ziph_int, 4);
	void* zipStream = NULL;
	void* handler = NULL;
	char* exec = wchar_t2Char(execPath, CP_UTF8);
	bool ret = false;
	bool done = false;
	if (mz_stream_os_create(&zipStream) && mz_zip_create(&handler)) {
		int ret;
		int index_ = 0;
		if ((ret = mz_stream_open(zipStream, exec, MZ_OPEN_MODE_READ)) == MZ_OK) {
			if (mz_zip_open(handler, zipStream, MZ_OPEN_MODE_READ) == MZ_OK) {
				bool ln_mem = false;
				char* lang_name = NULL;
				char* lang_name_p = NULL;
				char* lang_name_hl = NULL;
				INIReader* lang1 = NULL;
				INIReader* lang2 = NULL;
				INIReader* lang3 = NULL;
				bool foundPL = false;
				if ((ret = mz_zip_goto_first_entry(handler)) == MZ_OK) {
					do {
						mz_zip_file* file_inf;
						mz_zip_entry_get_info(handler, &file_inf);
						const char* addr = strstr(file_inf->filename, jal_package);
						if (addr != NULL && addr == file_inf->filename) {
							const char* fname = addr + strlen(jal_package);
							if (fname != NULL) {
								if (strlen(fname) == 7 && strstr(fname, "app.ini") == fname) {
									MessageBox(NULL, L"TEST", L"TEST", MB_OK);
									INIReader* inir2 = readIniInZip(handler, file_inf);
									INIReader inir_ = *inir2;
									std::string title_ = inir_.Get("App", "title", "Java Launcher");
									int ver = inir_.GetInteger("App", "javaVer", 8);
									std::string jvmArg = inir_.Get("App", "jvmArgs", "");
									std::string langs = inir_.Get("App", "lang", "");
									std::string sAjvm = inir_.Get("App", "appJvm", "");
									std::string forceJvm = inir_.Get("App", "forceJvmBit", "");
									if (forceJvm == "32") {
										force32 = true;
									}
									else if (forceJvm == "64") {
										force64 = true;
									}
									isJavaw = inir_.GetBoolean("App", "javaw", true);
									loadJavaDLURLs(inir_);
									if (sAjvm != "") {
										appjvm = char2Wchar_t(sAjvm.c_str());
									}
									bool enableUAC = inir_.GetBoolean("App", "uac", false);
									title = (char2Wchar_t(title_.c_str(), CP_UTF8));
									javaV = ver;
									if (!jvmArg.empty())
										jvmArgs = char2Wchar_t(jvmArg.c_str(), CP_UTF8);
									if (!langs.empty()) {
										lang_name = new char[langs.length() + 5];
										lang_name[0] = '\0';
										strcpy(lang_name, langs.c_str());
										strcat(lang_name, ".ini");
										if (strstr(lang_name, "zh") == lang_name) {
											lang_name_hl = "zh_CN";
										}
										else {
											lang_name_hl = "en_US";
										}
										lang_name_p = new char[3];
										strncpy(lang_name_p, lang_name, 2);
										ln_mem = true;
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
						}
					} while ((ret = mz_zip_goto_next_entry(handler)) == MZ_OK);
				}
				if (lang_name != NULL) {
					if ((ret = mz_zip_goto_first_entry(handler)) == MZ_OK) {
						do {
							mz_zip_file* file_inf;
							mz_zip_entry_get_info(handler, &file_inf);
							const char* addr = strstr(file_inf->filename, jal_package);
							if (addr != NULL && addr == file_inf->filename) {
								const char* fname = addr + strlen(jal_package);
								if (strlen(fname) >= 11 && strstr(fname, "lang/") == fname) {
									const char* Lfname = fname + 5;
									if (strstr(Lfname, lang_name) == Lfname) {
										const char* ext = Lfname + strlen(lang_name);
										if (strlen(ext) == 4 && strstr(ext, ".ini") == ext) {
											lang1 = readIniInZip(handler, file_inf);
											foundPL = true;
										}

									}
									else if (strstr(lang_name, lang_name_p) == lang_name && lang3 == NULL && strstr(Lfname, lang_name_p) == Lfname) {
										goto doing_lang3;
									}
									else if (strstr(Lfname, lang_name_hl) == Lfname) {
										const char* ext = Lfname + strlen(lang_name_hl);
										if (strlen(ext) == 4 && strstr(ext, ".ini") == ext) {
											lang2 = readIniInZip(handler, file_inf);
										}
									}
									else if (lang2 == NULL && lang3 == NULL && strstr(Lfname, lang_name_p) == Lfname) {
									doing_lang3:
										const char* ext = Lfname + strlen(lang_name_p);
										if (strlen(ext) == 4 && strstr(ext, ".ini") == ext) {
											lang3 = readIniInZip(handler, file_inf);
										}
										else if (strlen(ext) == 7 && strstr(ext + 3, ".ini") == ext + 3) {
											lang3 = readIniInZip(handler, file_inf);
										}
									}
								}
								if (lang1 != NULL)break;
							}
						} while ((ret = mz_zip_goto_next_entry(handler)) == MZ_OK);
					}
				}
				if (ln_mem) {
					delete[] lang_name;
					delete[] lang_name_p;
				}
				if (lang1 != NULL) {
					langinfo = lang1;
				}
				else if (lang_name != NULL && lang_name_p != NULL && strstr(lang_name, lang_name_p) == lang_name && lang3 != NULL) {
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
	delete[] exec;
	if (langinfo != NULL)return 0;
	return ret;
}
