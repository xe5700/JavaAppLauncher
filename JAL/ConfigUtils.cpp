#include "ConfigUtils.h"

wchar_t* ConfigUtils::char2Wchar_t(const char* source, size_t encoding) {
	int l = MultiByteToWideChar(encoding, 0, source, strlen(source), NULL, 0);
	wchar_t* r = new wchar_t[l + 1];
	MultiByteToWideChar(encoding, 0, source, strlen(source), r, l);
	r[l] = '\0';
	return r;
}

char* ConfigUtils::wchar_t2Char(const wchar_t* source, size_t encoding) {
	int l = WideCharToMultiByte(encoding, 0, source, lstrlenW(source), NULL, 0, NULL, NULL);
	char* r = new char[l + 1];
	WideCharToMultiByte(encoding, 0, source, lstrlenW(source), r, l, NULL, NULL);
	r[l] = '\0';
	return r;
}

INIReader* ConfigUtils::readIniInZip(void* handler, mz_zip_file* file_inf) {
	char* buf = new char[128];
	char* mfile = new char[file_inf->uncompressed_size + 1];
	size_t total = 0;
	int ret;
	if ((ret = mz_zip_entry_read_open(handler, 0, NULL)) == MZ_OK) {	
		while ((ret = mz_zip_entry_read(handler, buf, 128)) > 0) {
			memcpy(mfile + total, buf, ret);
			total += ret;
		}
		mfile[total] = '\0';
		mz_zip_entry_close(handler);
	}
	INIReader* ini = new INIReader(mfile,total);
	delete[] buf;
	delete[] mfile;
	return ini;
}
