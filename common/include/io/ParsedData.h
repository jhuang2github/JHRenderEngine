//
//  ParsedData.h
//
//  Created by Jingshu Huang on 12/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _ParsedData_h
#define _ParsedData_h

#include "platformDef.h"


enum ObjectType {
    UNKNOWN_TYPE = 0x0000,
    OBJECT_TYPE = 0x0001,
    MAP_TYPE = 0x0002,
    ARRAY_TYPE = 0x0004,
    STRING_TYPE = 0x0008,
    ENUM_TYPE = 0x0010,
    BOOL_TYPE = 0x0020,
    INT_TYPE = 0x0040,
    LONG_TYPE = 0x0080,
    FLOAT_TYPE = 0x0100,
    DOUBLE_TYPE = 0x0200,
    LONGLONG_TYPE = 0x0400
};

const int PRIMITIVE_TYPE_MASK = BOOL_TYPE | INT_TYPE | LONG_TYPE | FLOAT_TYPE | DOUBLE_TYPE | LONGLONG_TYPE | STRING_TYPE;


class ParsedData {
public:
    typedef unordered_map<string, ParsedData*> DataMap;
    typedef vector<ParsedData*> DataList;
    
    ParsedData(const bool val) { mType = BOOL_TYPE;  mBool = val; }
    ParsedData(const int val) { mType = INT_TYPE;  mInt = val; }
    ParsedData(const long val) { mType = LONG_TYPE;  mLong = val; }
    ParsedData(const float val) { mType = FLOAT_TYPE;  mFloat = val; }
    ParsedData(const double val) { mType = DOUBLE_TYPE;  mDouble = val; }
    ParsedData(const long long val) { mType = LONGLONG_TYPE;  mLongLong = val; }
    ParsedData(const char* val) { mType = STRING_TYPE; mStr = string(val); }
//    ParsedData(const char* val) {
//        mType = STRING_TYPE;
//        size_t stringLen = strlen(val);
//        char* str = (char*)malloc(stringLen + 1);
//        strncpy(str, val, stringLen);
//        str[stringLen] = '\0';
//    }
    ParsedData(DataMap* map) { mType = MAP_TYPE;  mMap = map; }
    ParsedData(DataList* list) { mType = ARRAY_TYPE;  mArray = list; }
    ParsedData() : mType(UNKNOWN_TYPE), mMap(NULL) { }

    ~ParsedData() {
        switch(mType) {
            case MAP_TYPE:
                if (mMap) {
                    for (DataMap::iterator it = mMap->begin(); it != mMap->end(); ++it) {
                        if (it->second) {
                            delete it->second;  // recursively delete.
                        }
                    }
                }
                break;
            case ARRAY_TYPE:
                if (mArray ) {
                    for (DataList::iterator it = mArray->begin(); it != mArray->end(); ++it) {
                        if (*it) {
                            delete *it;
                        }
                    }
                }
                break;
            case STRING_TYPE:
//                if (mStr) {
//                    free(mStr);
//                    mStr = NULL;
//                }
                mStr = "";
                break;
            default:
                break;
        }
    }

    inline bool isPrimitiveType() {
        return mType & PRIMITIVE_TYPE_MASK;
    }

    inline bool isMapType() {
        return mType == MAP_TYPE;
    }

    inline bool isArrayType() {
        return mType == ARRAY_TYPE;
    }
    
    inline ParsedData& operator [] (const string& key) {
        if (mType == MAP_TYPE && mMap != NULL) {
            if (mMap->find(key) != mMap->end()) {
                ParsedData* val = mMap->at(key);
                return *val;
            }
        }

        static ParsedData kEmpty;
        return kEmpty;
    }

    inline ParsedData& operator [] (int index) {
        if (mType == ARRAY_TYPE && mArray != NULL) {
            if (index < mArray->size()) {
                return *(*mArray)[index];
            }
        }

        static ParsedData kEmpty;
        return kEmpty;
    }

    inline bool operator == (const ParsedData& other) const {
        if (mType != other.mType) {
            return false;
        }

        switch(mType) {
            case BOOL_TYPE:
                return mBool == other.mBool;
            case INT_TYPE:
                return mInt == other.mInt;
            case LONG_TYPE:
                return mLong == other.mLong;
            case FLOAT_TYPE:
                return mFloat == other.mFloat;
            case DOUBLE_TYPE:
                return mDouble == other.mDouble;
            case LONGLONG_TYPE:
                return mLongLong == other.mLongLong;
            case STRING_TYPE:
                return string(mStr) == string(other.mStr);
            case MAP_TYPE:
            {
                int count = mMap->size();
                if (mMap && count == other.mMap->size()) {
                    DataMap::iterator it = mMap->begin();
                    DataMap::iterator ot = other.mMap->begin();
                    for (; it != mMap->end() && ot != other.mMap->end(); ++it, ++ot) {
                        if (it->first != ot->first) { return false; }
                        if (it->second != ot->second) { return false; }
                    }
                    return true;
                }
                return false;
            }
            case ARRAY_TYPE:
            {
                int count = mArray->size();
                if (mArray && count == other.mArray->size()) {
                    for (int i = 0; i < count; ++i) {
                        if ((*mArray)[i] != (*other.mArray)[i]) {
                            return false;
                        }
                    }
                    return true;
                }
                return false;
            }
            default:
                return false;
        }
        return false;

    }

    inline bool empty() {
        static ParsedData kEmpty;
        return *this == kEmpty;
    }

    void debugPrint(int numSpace = 0) {
        if (isPrimitiveType()) {
            return;
        }
//        if (mType == STRING_TYPE) {
//            printf("%s\n", mStr.c_str());
//        } else
        if (mType == MAP_TYPE) {
            ParsedData::DataMap& m = *mMap;
            for (ParsedData::DataMap::iterator it = m.begin(); it != m.end(); ++it) {
                for (int i = 0; i < numSpace; ++i) {printf(" ");}
                printf("%s : {\n", it->first.c_str());
                it->second->debugPrint(numSpace + 2);
                for (int i = 0; i < numSpace; ++i) {printf(" ");}
                printf("}\n");
            }
        } else {
            assert(mType == ARRAY_TYPE);
            for (int i = 0; i < numSpace; ++i) {printf(" ");}
            printf("array size = %d\n", (int)mArray->size());
        }
    }

public:
    ObjectType mType;

    string mStr;  // TODO(jhuang): cannot put string into union.
    union {
        DataMap* mMap;
        DataList* mArray;
        bool mBool;
        int mInt;
        long mLong;
        long long mLongLong;
        float mFloat;
        double mDouble;
    };
};


#endif
