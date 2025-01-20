#pragma once
#ifndef CONOS_UTIL
#define CONOS_UTIL 1

#include "Define.h"

/* �������� OSStatus:
*   - STARTED - ���� ������� � �������� ������������
*/
struct OSStatus {
    enum Values {
        STARTED,
        CHECK,
        OK,
        CONFIG,
        FATALSCREEN
    };
};

struct ConcfgLoadStatus {
    enum Values {
        OK,
        InvalidType,
        RdErr,
        WrErr
    };
};

struct LoggerMessageLevel {
    enum Values {
        DEBUG = -1,
        INFO,
        WARN,
        ERR,
        FATAL,
        STARTED,
        STOPPED
    };
};

inline string LoggerEnumToStr(LoggerMessageLevel::Values level) {
    switch (level)
    {
    case LoggerMessageLevel::DEBUG:
        return "DEBUG";
    case LoggerMessageLevel::INFO:
        return "INFO";
    case LoggerMessageLevel::WARN:
        return "WARN";
    case LoggerMessageLevel::ERR:
        return "ERROR";
    case LoggerMessageLevel::FATAL:
        return "FATAL";
    case LoggerMessageLevel::STARTED:
        return "START-INFO";
    case LoggerMessageLevel::STOPPED:
        return "STOP-INFO";
    default:
        return "UNKNOWN";
    }
}

using message = vector<char>;

inline string getABSPath(const char* path = "") {
    vector<char> pathBuf;
    DWORD copied = 0;
    do {
        pathBuf.resize(pathBuf.size() + MAX_PATH);
        copied = GetModuleFileNameA(nullptr, &pathBuf.at(0), pathBuf.size());
    } while (copied >= pathBuf.size());
    pathBuf.resize(copied);
    string path_(pathBuf.begin(), pathBuf.end());
    return fs::path(path_).parent_path().string() + "\\" + string(path);
}

inline message StringToMessage(string str) {
    message mess;
    for (auto& ch : str) {
        mess.push_back(ch);
    }
    return mess;
}

inline string MessageToString(message mess) {
    string str;
    for (auto& ch : mess) {
        str+=ch;
    }
    return str;
}

/* ��������� .confs:
*  # | ��������          | ���    | ������                    | ����� ������                       | ��������
*  --+-------------------+--------+---------------------------+------------------------------------+------------------------------------------------------------------
*  1 | FSFileTypeMessage | String | 15 �                      | 15 �                               | ��������� ��� �������� ���� �����
*  2 | FSFileVer         | UInt   | 10 �                      | 25 �                               | ������ ����� ��� ������� ������
*  3 | UseReserved       | Bool   | 1 �                       | 26 �                               | ���� ������������ ������������� ���������� ����� (������ 0)
*  4 | FSSize            | ULL    | 20 �                      | 46 �                               | ������ �������� ������� � ����� (����. �������� - 35184372088832)
*  5 | OSFiles           | FS     | 1073741824 � (1 ��)       | 1073741870 � (1 ��)                | ��������� ����� (������ ������ �� ������ ����)
*  6 | Reserved          | MS     | 1073741824 � (1 ��)       | 2147483694 � (2 ��)                | ����������������� ����.
*  7 | FS                | FS     | <35184372088832 � (32 ��) | <35186519572526 � (32770 ��/32 ��) | �������� �������
* 
*  ��������� FS:
*  # | �������� | ���    | ������                         | ����� ������              | ��������
*  --+----------+--------+--------------------------------+---------------------------+----------------------------------------
*  1 | FileName | String | 1024 � (1 ��)                  | 1024 � (1 ��)             | ��� �����
*  2 | Size     | ULL    | 20 �                           | 1044 � (1 ��)             | ������ �����
*  3 | Flags    | Short  | 1 �                            | 1045 � (1 ��)             | ����� ����� (��������, "������ ������")
*  4 | FileData | String | <35184372087787 � (32767,9 ��) | <35184372088832 � (32 ��) | ������ �����
* 
*  UInt - unsigned int       (DWORD)
*  ULL  - unsigned long long (size_t)
*  FS   - �������� �������
*  MS   - ������� ������
*/
namespace Constants {
    const message FSFileTypeMessage = StringToMessage("ConOSFileSystem");
    const size_t  FSFileTypeOffset = 0;
    const size_t  FSFileTypeSize = 15;
    const size_t  FSFileVerOffset = FSFileTypeOffset + FSFileTypeSize;
    const size_t  FSFileVerSize = 10;
    const size_t  UseReservedOffset = FSFileVerOffset + FSFileVerSize;
    const size_t  UseReservedSize = 1;
    const size_t  FSSizeOffset = UseReservedOffset + UseReservedSize;
    const size_t  FSSizeSize = 20;
    const size_t  OSFilesOffset = FSSizeOffset + FSSizeSize;
    const size_t  OSFilesSize = 1073741824;
    const size_t  ReservedOffset = OSFilesOffset + OSFilesSize;
    const size_t  ReservedSize = 1073741824;
    const size_t  FSOffset = ReservedOffset + ReservedSize;
    const size_t  FSSize = 35184372088832;
}


/* ��������� .concfg:
*  # | Name                  | Type   | Size                      | Total Size                         | Comment
*  --+-----------------------+--------+---------------------------+------------------------------------+-------------------------------------
*  1 | ConcfgFileTypeMessage | String | 8 B                       | 8 B                                | File type constant for check
*  2 | ConcfgFileVer         | UInt   | 10 B                      | 18 B                               | File version for know missing blocks
*  3 | LoadOsPath            | String | 255 B                     | 273 B                              | Path to disk for load OS
*/
namespace Constants {
    const message ConcfgFileTypeMessage = StringToMessage("ConOSCfg");
    const size_t  ConcfgFileTypeOffset = 0;
    const size_t  ConcfgFileTypeSize = 8;
    const size_t  ConcfgFileVerOffset = ConcfgFileTypeOffset + ConcfgFileTypeSize;
    const size_t  ConcfgFileVerSize = 10;
    const size_t  LoadOsPathOffset = ConcfgFileVerOffset + ConcfgFileVerSize;
    const size_t  LoadOsPathSize = 255;
}

class  Parent;
class  Logger;
class  DrawableSorter;
class  Window;
class  FileSystem;
class  OS;

#endif