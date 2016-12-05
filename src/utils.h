#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

using namespace std;

class Utils {
public:
    static bool isDir(string path);
    static bool fileExists(string file);
    static unsigned long millisecondsSinceEpoch();
    static string getCurrentPath();
    static bool createLogFile(string fileName, string header, fstream &logFile);
    static string getTimeString();
    static int numFilesInDir(string path);
    static long getFreeDiskSpace(string path);
};

#endif
