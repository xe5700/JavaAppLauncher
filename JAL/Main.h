#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif
#include "windows.h"
#include "INIReader.h"
#include "direct.h"
#include  <io.h>
#include <mz.h>
#include <mz_os.h>
#include <mz_zip.h>
#include <mz_strm_os.h>
#include <mz_strm.h>
#include <mz_zip_rw.h>
//#pragma comment(lib,"zlibstat.lib")
extern void debugRet(int ret);