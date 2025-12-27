#include "dir.h"

using namespace std;

// Check if directory exists
bool Dir::Exists(const string& path) 
{
    struct stat info;
    return (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

// Recursively create directories
bool Dir::CreateDirectories(const string& path) 
{
    if (path.empty()) return false;

    // If already exists, done
    if (Dir::Exists(path)) return true;

    // Find parent directory
    size_t pos = path.find_last_of("/\\");
    if (pos != string::npos) 
    {
        string parent = path.substr(0, pos);
        if (!parent.empty() && !Dir::Exists(parent))
        {
            if (!Dir::CreateDirectories(parent))
                return false;
        }
    }
    return MKDIR(path.c_str()) == 0 || Dir::Exists(path);
}

// Get parent path from full path
string Dir::GetParentPath(const string& fullPath) 
{
    size_t pos = fullPath.find_last_of("/\\");
    if (pos == string::npos) return "";
    return fullPath.substr(0, pos);
}