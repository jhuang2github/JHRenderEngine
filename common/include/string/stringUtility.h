//
//  stringUtility.h
//
//  Created by Jingshu Huang on 2/7/13.
//

#ifndef _stringUtility_h
#define _stringUtility_h

#include "platformDef.h"

using namespace std;


#define strHash(X) stringUtility::getStringHash(X)


namespace stringUtility {

    static hash<string> strHashFunc;
    inline size_t getStringHash(const string& str) {
        return strHashFunc(str);
    }

    string getFileBaseName(const string& name);
    string getFileExtension(const string& name);

    // Split file name into base name and extension.
	void splitBaseFilename(const string& fullName,
                           string& outBasename, string& outExtention);
    // Split into a file name and a path.
    void splitFilename(const string& name,
                       string& outFileName, string& outPath);
    // Split into a file base name, a file extension and a path.
	void splitFullFilename(const string& name,
                           string& outBasename, string& outExtention, string& outPath);

    inline void toLowerCase(string& str);
    inline void toUpperCase(string& str);

    bool startsWith(const string& str, const string& pattern, bool lowerCase);
    bool endsWith(const string& str, const string& pattern, bool lowerCase);
	const string replaceAll(const string& source, const string& replaceWhat,
                            const string& replaceWithWhat);
    // Check if a string matches a given pattern.
    bool match(const string& str, const string& pattern, bool caseSensitive);
};

#endif
