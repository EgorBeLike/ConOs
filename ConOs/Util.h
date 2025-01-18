#pragma once
#include "Define.h"

enum OSStatus {
    STARTED,
    CHECK,
    OK,
    CONFIG,
    FATALSCREEN
};

enum ConcfgLoadStatus {
    OK,
    InvalidType,
    RdErr,
    WrErr
};

using message = vector<char>;

string getABSPath(const char* path = "") {
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

message StringToMessage(string str) {
    message mess;
    for (auto& ch : str) {
        mess.push_back(ch);
    }
    return mess;
}

/* FileSystem Structure:
*  # | Name              | Type   | Size                      | Total Size                         | Comment
*  --+-------------------+--------+---------------------------+------------------------------------+-------------------------------------
*  1 | FSFileTypeMessage | String | 15 B                      | 15 B                               | File type constant for check
*  2 | FSFileVer         | UInt   | 10 B                      | 25 B                               | File version for know missing blocks
*  3 | UseReserved       | Bool   | 1 B                       | 26 B                               | Enable Reserved block (Always 0)
*  4 | FSSize            | ULL    | 20 B                      | 46 B                               | Filesystem size in bytes
*  5 | OSFiles           | FB     | 1073741824 B (1 GB)       | 1073741870 B (1 GB)                | OS files structure. Read-Only files
*  6 | Reserved          | MB     | 1073741824 B (1 GB)       | 2147483694 B (2 GB)                | Reserved. In future for OS Pause
*  7 | FS                | FB     | <35184372088832 B (32 TB) | <35186519572526 B (32770 GB/32 TB) | Disk File System
* 
*  FB Data Structure:
*  1              2    3 4
*  v              v    v v
*  --------------|----|-|---------------------------------
* 
*  FB Structure:
*  # | Name     | Type   | Size                           | Total Size                | Comment
*  --+----------+--------+--------------------------------+---------------------------+-------------------------------------
*  1 | FileName | String | 1024 B (1 KB)                  | 1024 B (1 KB)             | File name
*  2 | Size     | ULL    | 20 B                           | 1044 B (1 KB)             | File data size
*  3 | Flags    | Shrt   | 1 B                            | 1045 B (1 KB)             | File flags (read file documentation)
*  4 | FileData | String | <35184372087787 B (32767,9 GB) | <35184372088832 B (32 TB) | File data
* 
*  Shrt - short
*  UInt - unsigned int       (DWORD)
*  ULL  - unsigned long long (size_t)
*  FB   - Files Base
*  MB   - Memory Base
*/
namespace Constants {
    const message FSFileTypeMessage = StringToMessage("ConOSFileSystem");
    const size_t  FSFileTypeOffset = 0;
    const size_t  FSFileTypeSize = FSFileTypeMessage.size();
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

namespace Constants {
    const LPSTR   ConcfgFileType = ".concfg";
    const message ConcfgFileTypeMessage = StringToMessage("ConOSCfg");
    const size_t  ConcfgFileTypeOffset = 0;
    const size_t  ConcfgFileTypeSize = ConcfgFileTypeMessage.size();
    const size_t  ConcfgFileVerOffset = ConcfgFileTypeOffset + ConcfgFileTypeSize;
    const size_t  ConcfgFileVerSize = 10;
}

class Parent;
class Logger;
class Window;
class FileSystem;
class OS;
struct Drawable;
class DrawableSorter;
class DrawableSorter::iterator;