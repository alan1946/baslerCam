#include <iostream>
#include <dirent.h>
#include <fstream>
#include "utils.h"
#include <string>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <sys/statvfs.h>
#include <cstring>
#include "plog/Log.h"

using namespace std;

bool Utils::isDir(string path) {
    struct stat s;
    int err = stat(path.c_str(), &s);
    if (-1 == err) {
        if (ENOENT == errno) {
            LOGE << path << " is not a directory";
            return false;
        } else {
            LOGE << "Error running stat on path: " << path;
            return false;
        }
    } else {
        if (S_ISDIR(s.st_mode)) {
            return true;
        } else {
            LOGE << path << " exists but is not a directory";
            return false;
        }
    }
}

bool Utils::fileExists(string file) {
    struct stat buffer;
    return (stat(file.c_str(), &buffer) == 0);
}

unsigned long Utils::millisecondsSinceEpoch() {
    return chrono::system_clock::now().time_since_epoch() /
        chrono::milliseconds(1);
}

string Utils::getTimeString() {
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d_%H-%M-%S", tm_info);
    return string(buffer);
}

string Utils::getCurrentPath() {
    if (const char* env_p = getenv("DATA_ACQ")) {
        return string(env_p);
    } else {
        LOGE << "DATA_ACQ path must be set as env var";
        return "";
    }
}

bool Utils::createLogFile(string fileName, string header, fstream &logFile) {
    if (Utils::fileExists(fileName)) {
        LOGE << "Log file already exists " << fileName;
        return false;
    }
    logFile.open(fileName, fstream::out | fstream::app);
    if (!logFile) {
        LOGD << "creating log file '" << fileName << "'";
        logFile.open(fileName, fstream::out | fstream::trunc);
        if (!logFile) {
            LOGE << "cannot create log file";
            return false;
        }
        logFile << header << endl;
        LOGD << "log file '" << fileName << "' created successfully";
    } else {
        logFile << header << endl;
        LOGD << "log file '" << fileName << "' created successfully";
    }
    return true;
}

int Utils::numFilesInDir(string path) {
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(path.c_str());
    LOGE_IF(dirp == NULL) << "Could not open path: " << path;
    if (dirp == NULL) return -1;
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) {
            file_count++;
        }
    }
    int rc = closedir(dirp);
    LOGE_IF(rc == -1) << "Could not close dir: " << path;
    if (rc == -1) return -1;
    return file_count;
}

long Utils::getFreeDiskSpace(string path) {
    struct statvfs stat;
    if (statvfs(path.c_str(), &stat) != 0) {
        LOGE << "statvfs / error";
        return -1;
    }
    return stat.f_bsize * stat.f_bavail;
}
