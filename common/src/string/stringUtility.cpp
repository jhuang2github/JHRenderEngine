//
//  stringUtility.cpp
//
//  Created by Jingshu Huang on 2/13/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <algorithm>
#include "stringUtility.h"

using namespace std;


namespace stringUtility {

string getFileBaseName(const string& name) {
    size_t pos = name.rfind(".");
    if (pos == string::npos) {
        return "";
    }
    return name.substr(0, pos);
}

string getFileExtension(const string& name) {
    int pos = name.rfind(".");
    if (pos == string::npos) {
        return "";
    }
    return name.substr(pos + 1);
}

// Split file name into base name and extension.
void splitBaseFilename(const string& fullName,
                       string& outBasename, string& outExtention) {
    size_t i = fullName.find_last_of(".");
    if (i == string::npos) {
        outExtention.clear();
        outBasename = fullName;
    } else {
        outExtention = fullName.substr(i + 1);
        outBasename = fullName.substr(0, i);
    }
}

// Split into a file name and a path.
void splitFilename(const string& name,
                   string& outFileName, string& outPath) {
    string path = name;
    // Replace \ with / first
    std::replace(path.begin(), path.end(), '\\', '/');
    // split based on final /
    size_t i = path.find_last_of('/');
    
    if (i == string::npos) {
        outFileName = name;
        outPath.clear();
    } else {
        outFileName = path.substr(i+1, path.size() - i - 1);
        outPath = path.substr(0, i+1);
    }
}

// Split into a file base name, a file extension and a path.
void splitFullFilename(const string& name,
                       string& outBasename, string& outExtention, string& outPath) {
    string fullName;
    splitFilename(name, fullName, outPath);
    splitBaseFilename(fullName, outBasename, outExtention);
}

void toLowerCase(string& str) {
#ifdef __APPLE__
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
#else
    std::transform(str.begin(), str.end(), str.begin(), ::towlower);
#endif
}

void toUpperCase(string& str) {
#ifdef __APPLE__
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
#else
    std::transform(str.begin(), str.end(), str.begin(), ::towupper);
#endif
}

bool startsWith(const string& str, const string& pattern, bool lowerCase) {
    size_t thisLen = str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0) {
        return false;
    }
    
    string startOfThis = str.substr(0, patternLen);
    if (lowerCase) {
        toLowerCase(startOfThis);
    }
    
    return startOfThis == pattern;
}

bool endsWith(const string& str, const string& pattern, bool lowerCase) {
    size_t thisLen = str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0) {
        return false;
    }
    
    string endOfThis = str.substr(thisLen - patternLen, patternLen);
    if (lowerCase) {
        toLowerCase(endOfThis);
    }
    
    return endOfThis == pattern;
}

const string replaceAll(const string& source, const string& replaceWhat,
                        const string& replaceWithWhat) {
    string result = source;
    string::size_type pos = 0;
    result.find(replaceWhat, pos);
    while(pos != string::npos) {
        result.replace(pos,replaceWhat.size(), replaceWithWhat);
        pos += replaceWithWhat.size();
        pos = result.find(replaceWhat, pos);
    }
    return result;
}

// Check if a string matches a given pattern.
bool match(const string& str, const string& pattern, bool caseSensitive) {
    string tmpStr = str;
    string tmpPattern = pattern;
    if (!caseSensitive) {
        toLowerCase(tmpStr);
        toLowerCase(tmpPattern);
    }
    
    string::const_iterator strIt = tmpStr.begin();
    string::const_iterator patIt = tmpPattern.begin();
    string::const_iterator lastWildCardIt = tmpPattern.end();
    while (strIt != tmpStr.end() && patIt != tmpPattern.end()) {
        if (*patIt == '*') {
            lastWildCardIt = patIt;
            // Skip over looking for next character
            ++patIt;
            if (patIt == tmpPattern.end()) {
                // Skip right to the end since * matches the entire rest of
                // the string
                strIt = tmpStr.end();
            } else {
                // scan until we find next pattern character
                while(strIt != tmpStr.end() && *strIt != *patIt)
                    ++strIt;
            }
        } else {
            if (*patIt != *strIt) {
                if (lastWildCardIt != tmpPattern.end()) {
                    // The last wildcard can match this incorrect sequence
                    // rewind pattern to wildcard and keep searching
                    patIt = lastWildCardIt;
                    lastWildCardIt = tmpPattern.end();
                } else {
                    // no wildwards left
                    return false;
                }
            } else {
                ++patIt;
                ++strIt;
            }
        }
    }
    
    // If we reached the end of both the pattern and the string, we succeeded.
    return patIt == tmpPattern.end() && strIt == tmpStr.end();
}
    
}
