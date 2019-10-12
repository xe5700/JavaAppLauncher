#include "Main.h"
#include "Lang.h"
#include "JavaDLUrl.h"
#include "Config.h"
using namespace JAVA_URLS;
using namespace Config;
using namespace ConfigUtils;
void debugRet(int ret) {
	wchar_t tmp[16];
	_itow(ret, tmp, 10);
	OutputDebugStringA("\n\nErrorCode:");
	OutputDebugString(tmp);
}
OSVERSIONINFO verinf;
LANG* lang;
bool isXP() {
	return verinf.dwMajorVersion == 5;
}
BOOL wow64 = 233;
bool is64bitSys() {
	if (wow64 == 233) {
		IsWow64Process(GetCurrentProcess(), &wow64);
	}
	return wow64;
}
bool autoSetupJava(wchar_t* title,bool wow64,int jver,wchar_t* execPath) {
	wchar_t tmpPath[MAX_PATH];
	GetTempPath(MAX_PATH,tmpPath);
	void* zipStream = NULL;
	void* handler = NULL;
	char* exec = wchar_t2Char(execPath, CP_UTF8);
	bool ret = false;
	bool aria = false;
	wchar_t mpath[MAX_PATH];
	lstrcpy(mpath, tmpPath);
	lstrcat(mpath, aria2ExeName);
	if (mz_stream_os_create(&zipStream) && mz_zip_create(&handler)) {
		if ((ret = mz_stream_open(zipStream, exec, MZ_OPEN_MODE_READ)) == MZ_OK) {
			if (mz_zip_open(handler, zipStream, MZ_OPEN_MODE_READ) == MZ_OK) {
				if ((ret = mz_zip_goto_first_entry(handler)) == MZ_OK) {
					do {
						mz_zip_file* file_inf;
						mz_zip_entry_get_info(handler, &file_inf);
						const char* addr = strstr(file_inf->filename, jal_package);
						if (addr != NULL && addr == file_inf->filename) {
							const char* fname = addr + strlen(jal_package);
							if (fname != NULL) {
								if (strlen(fname) == 10 && strstr(fname, "aria2c.exe") == fname) {
									FILE* ariaF = NULL;
									if (_wfopen_s(&ariaF, mpath, L"wb") == 0) {
										char* buf = new char[128];
										int ret;
										if ((ret = mz_zip_entry_read_open(handler, 0, NULL)) == MZ_OK) {
											while ((ret = mz_zip_entry_read(handler, buf, 128)) > 0) {
												fwrite(buf, 1, ret, ariaF);
											}
											mz_zip_entry_close(handler);
										}
										delete buf;
										fclose(ariaF);
										aria = true;
										break;
									}
								}
							}
						}
					} while ((ret = mz_zip_goto_next_entry(handler)) == MZ_OK);
				}
			}
			mz_stream_close(zipStream);
		}
	}
	if (!aria)return false;
	std::wstring args = L"-s256 -x256 -oJALTmp_javaSetup.exe -k1M --file-allocation=falloc ";
	wchar_t javaSetup[MAX_PATH];
	lstrcpy(javaSetup, tmpPath);
	lstrcat(javaSetup, L"JALTmp_javaSetup.exe");
	if (jver == 8) {
		if (isXP()&&!is64bitSys()) {
			args += Txpjre8;
		} else {
			if (wow64) {
				args += Tjre8_64;
			} else {
				args += Tjre8_32;
			}
		}
	} else if (jver == 7) {
		if (wow64) {
			args += Tjre7_64;
		}
		else {
			args += Tjre7_32;
		}
	} else if (jver > 8) {
		BOOL wow642 = false;
		if (verinf.dwMajorVersion < 6 || !IsWow64Process(GetCurrentProcess(), &wow642) || !wow642) {
			WCHAR* Nmsg = new WCHAR[lstrlen(lang->notsupportedOS) + 20];
			wsprintf(Nmsg, lang->notsupportedOS, jver);
			MessageBoxW(NULL, Nmsg, title, MB_OK);
			delete[] Nmsg;
			return false;
		}
		switch (jver)
		{
		case 9:
			args += Tjre9;
			break;
		case 10:
			args += Tjre10;
			break;
		case 11:
			args += Tjre11;
			break;
		case 12:
			args += Tjre12;
			break;
		case 13:
			args += Tjre13;
			break;
		case 14:
			args += Tjre14;
			break;
		case 15:
			args += Tjre15;
			break;
		case 16:
			args += Tjre16;
			break;
		default:
			args += Tjre16;
			break;
		}
	}

	{
		SHELLEXECUTEINFO shell;
		shell.cbSize = sizeof(SHELLEXECUTEINFO);
		shell.fMask = SEE_MASK_NOCLOSEPROCESS;
		shell.hwnd = NULL;
		shell.lpVerb = L"open";
		shell.lpParameters = args.c_str();
		shell.lpDirectory = tmpPath;
		shell.nShow = SW_SHOW;
		shell.lpFile = mpath;
		shell.hInstApp = NULL;
		ShellExecuteEx(&shell);
		HWND aria = NULL;
		while (WaitForSingleObject(shell.hProcess, 0)!= 0xFFFFFFFF && ((aria=FindWindow(L"ConsoleWindowClass", mpath))==NULL)) {

		}
		
		SetWindowText(aria, lang->TDLTITLE);
		WaitForSingleObject(shell.hProcess, INFINITE);
	}
	Sleep(2000);
	DeleteFile(mpath);
	if (_waccess(javaSetup, 0) == -1) {
		return false;
	}
	{
		args = L"/c \"title ";
		args += lang->waitSetupJava;
		args += L" & JALTmp_javaSetup.exe /s\"";
		SHELLEXECUTEINFO shell;
		shell.cbSize = sizeof(SHELLEXECUTEINFO);
		shell.fMask = SEE_MASK_NOCLOSEPROCESS;
		shell.hwnd = NULL;
		shell.lpVerb = L"open";
		shell.lpParameters = args.c_str();
		shell.lpDirectory = tmpPath;
		shell.nShow = SW_SHOW;
		shell.lpFile = L"cmd.exe";
		shell.hInstApp = NULL;
		ShellExecuteEx(&shell);
		WaitForSingleObject(shell.hProcess, INFINITE);
	}
	Sleep(5000);
	DeleteFile(javaSetup);
	return true;
}
int WinMain2(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
		ZeroMemory(&verinf, sizeof(OSVERSIONINFO));
		verinf.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx(&verinf)) {
			verinf.dwMajorVersion = 5;
		}
		wchar_t exePath[MAX_PATH];
		wchar_t* lpwCmdLine = char2Wchar_t(lpCmdLine);
		bool uac = false;
		INIReader* langini=NULL;
		GetModuleFileName(NULL, exePath, MAX_PATH);
		OutputDebugStringW(exePath);
		wchar_t appPath[MAX_PATH];
		lstrcpy(appPath, exePath);
		(wcsrchr(appPath, '\\'))[1] = '\0';
		if (loadConfig(exePath,strstr(lpCmdLine,"debugLoadResourceFuckingMode")==lpCmdLine)!=0) {
			MessageBox(NULL, L"Wrong configuration", L"Error", MB_OK);
			return -1;
		}
		std::wstring javaHome;
		if (appjvm == NULL) {
			HKEY key=NULL;
			long result;
			WCHAR* jre_key_name;
			//Check version
			if (javaVer < 9) {
				jre_key_name = L"SOFTWARE\\JavaSoft\\Java Runtime Environment";
			} else {
				jre_key_name = L"SOFTWARE\\JavaSoft\\JRE";
			}
			if (force32) {
				result = 88888;
			} else {
				result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, jre_key_name, 0, KEY_WOW64_64KEY | KEY_READ, &key);
			}
			if (result != ERROR_SUCCESS) {
				if(key!=NULL&& !force64)
				RegCloseKey(key);
				if(!force64)
				result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, jre_key_name, 0, KEY_WOW64_32KEY | KEY_READ, &key);
				if (result != ERROR_SUCCESS) {
					startSetup:
					int re = MessageBox(NULL, lang->noJava, title, MB_YESNO);
					RegCloseKey(key);
					if (re == IDYES) {
						bool is64 = wow64;
						if (force32) {
							is64 = false;
						}
						if (force64 && !wow64){
							WCHAR* Nmsg = new WCHAR[lstrlen(lang->notsupportedOS) + 20];
							std::wstring bitname = L" ";
							bitname += lang->win64;
							wsprintf(Nmsg, lang->notsupportedOS, javaVer,lang->win64);
							MessageBoxW(NULL, Nmsg, title, MB_OK);
							delete[] Nmsg;
							return false;
						}
						if (autoSetupJava(title,is64, javaVer,exePath)) {
							SHELLEXECUTEINFO shell;
							shell.cbSize = sizeof(SHELLEXECUTEINFO);
							shell.fMask = NULL;
							shell.hwnd = NULL;
							shell.lpVerb = NULL;
							shell.lpParameters = lpwCmdLine;
							shell.lpDirectory = NULL;
							shell.nShow = SW_SHOW;
							shell.lpFile = exePath;
							shell.hInstApp = NULL;
							ShellExecuteEx(&shell);
						} else {
							MessageBox(NULL, lang->cantDLJava, title, MB_ICONERROR);
							return -1;
						}
					}
					return 0;
				}
			}
			DWORD size;
			RegQueryValueEx(key, L"CurrentVersion", NULL, NULL, NULL, &size);
			BYTE * buf = new BYTE[size];
			RegQueryValueEx(key, L"CurrentVersion", NULL, NULL, buf, &size);
			wchar_t* ver = (wchar_t*)buf;
			//delete[] buf;
			OutputDebugString(ver);
			OutputDebugString(L"\n");
			wchar_t findJVerStr[25];
			wchar_t wJVer[10];
			_itow(javaVer, wJVer, 10);
			if (javaVer < 9) {
				lstrcpy(findJVerStr, L"1.");
				lstrcat(findJVerStr, wJVer);
				lstrcat(findJVerStr, L".0");
			} else {
				lstrcpy(findJVerStr, wJVer);
			}
			double last_jver=0;
			HKEY jre_ver_key;
			searchJavaHome:
			if (wcsstr(ver, findJVerStr) == ver) {
				result = RegOpenKeyEx(key, ver, 0, KEY_READ, &jre_ver_key);
			} else {
				delete[] ver;
				ver = new wchar_t[lstrlen(findJVerStr) + 1];
				lstrcpy(ver, findJVerStr);
				goto startFindingJava;
			}
			bool again=false;
			if (result == ERROR_SUCCESS) {
				RegQueryValueEx(jre_ver_key, L"JavaHome", NULL, NULL, NULL, &size);
				buf = new BYTE[size];
				int result=RegQueryValueEx(jre_ver_key, L"JavaHome", NULL, NULL, buf, &size);
				if (result != ERROR_SUCCESS) {
					if (again) {
						return -1;
					}
					delete[] buf;
					DWORD keylen;
					DWORD keyMAXL;
					startFindingJava:
					result=RegQueryInfoKey(key, NULL, NULL, NULL, &keylen, &keyMAXL, NULL, NULL, NULL, NULL, NULL, NULL);
					keyMAXL++;
					if (result != ERROR_SUCCESS) {
						return -1;
					}
					for (DWORD i = 0; i < keylen; i++) {
						double current_jver = 0;
						wchar_t* name;
						name = new wchar_t[keyMAXL];
						size = keyMAXL;
						result = RegEnumKeyEx(key, i, name, &size, NULL, NULL, NULL, NULL);
						current_jver=_wtof(name + lstrlen(findJVerStr) + 1);
						if (last_jver >= current_jver) {
							continue;
						}
						last_jver = current_jver;
						if (result == ERROR_SUCCESS) {
							if (wcsstr(name, findJVerStr) == name&&size!=wcslen(findJVerStr)) {
								if (ver != NULL) {
									delete[] ver;
									ver = NULL;
								}
								ver = name;
								result=RegOpenKeyEx(key, name, 0, KEY_READ, &jre_ver_key);
								again = true;
							}
						}
					}
					if (last_jver == 0) {
						return -1;
					} else {
						goto searchJavaHome;
					}
				}
				javaHome = (wchar_t*)buf;
				delete[] buf;
			}

			RegCloseKey(jre_ver_key);
			RegCloseKey(key);
		}else{
			javaHome = appPath;
			javaHome += L"\\";
			javaHome += appjvm;
			javaHome += L"\\";
			if (wow64) {
				javaHome += L"64";
			} else {
				javaHome += L"32";
			}
		}
		//find javaw
		wchar_t* java_exe = new wchar_t[javaHome.length() + 15];
		lstrcpy(java_exe, javaHome.c_str());
		lstrcat(java_exe, L"\\bin\\");
		if (isJavaw) {
			lstrcat(java_exe, L"javaw.exe");
		} else {
			lstrcat(java_exe, L"java.exe");
		}
		SHELLEXECUTEINFO shell;
		shell.cbSize = sizeof(SHELLEXECUTEINFO);
		shell.fMask = SEE_MASK_NOCLOSEPROCESS;
		shell.hwnd = NULL;
		shell.lpVerb = NULL;
		wchar_t java_args[MAX_PATH];
		java_args[0] = '\0';
		if (jvmArgs != NULL && jvmArgs[0] != '\0') {
			lstrcpy(java_args, jvmArgs);
			lstrcat(java_args, L" ");
		}
		lstrcat(java_args, L"-jar \"");
		lstrcat(java_args, exePath);
		lstrcat(java_args, L"\"");
		if (lpCmdLine[0] != '\0') {
			lstrcat(java_args, L" ");
			lstrcat(java_args, lpwCmdLine);
			delete lpwCmdLine;
		}
		shell.lpParameters = java_args;
		shell.lpDirectory = appPath;
		shell.nShow = SW_SHOW;
		shell.lpFile = java_exe;
		shell.hInstApp = NULL;
		if (uac&&verinf.dwMajorVersion > 5) {
			shell.lpVerb = L"runas";
		}
		ShellExecuteEx(&shell);
		if (!isJavaw) {
			HWND javaH=NULL;
			while (WaitForSingleObject(shell.hProcess, 0) != 0xFFFFFFFF && ((javaH = FindWindow(L"ConsoleWindowClass", java_exe)) == NULL)) {

			}
			if (javaH != 0) {
				SetWindowText(javaH, title);
			}
		}
		delete[] java_exe;
		return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int r= WinMain2(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	DeleteFileW(aria2ExeName);
	return r;
}