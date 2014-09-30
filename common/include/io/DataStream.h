//
//  DataStream.h
//
//  Created by Jingshu Huang on 12/30/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _DataStream_h
#define _DataStream_h

#include <cstdio>
#include <fstream>
#include <sstream>

#include "platformDef.h"
#include "zip.h"


class DataStream {
public:
    DataStream();
    virtual ~DataStream();

    inline void setData(char* data) { mData = data; }
    inline char* getData() { return mData; }
    inline void setDataSize(size_t size) { mSize = size; }
    inline size_t getDataSize() { return mSize; }

	virtual bool open() = 0;
	virtual size_t read(void *data, size_t length) = 0;
	virtual void close() = 0;

protected:
    char*   mData;
    size_t  mSize;
};


class BufferDataStream : public DataStream {
public:
	BufferDataStream(char *buffer_, size_t bufferSize_);
	virtual ~BufferDataStream();
    
	bool open();
	size_t read(void *data, size_t length);
	void close();

protected:
	size_t currentPos;
};


class FileDataStream : public DataStream {    
public:
    FileDataStream();
	FileDataStream(const string& fileName_);
	virtual ~FileDataStream();

	virtual bool open();
	virtual size_t read(void *data, size_t length);
    virtual size_t read(void *data, size_t length, size_t repeats);
    virtual void readAll();
	virtual void close();

    inline string& getFileName() { return fileName; }

protected:
	string fileName;
	FILE *f;
};


class ZipFileDataStream : public FileDataStream {
public:
    // Zip file open/close.
	static void openZip(const char *zipFileName);
	static void closeZip();

    // Zipped file(s) (content of the zip file) open/close/read etc.
    ZipFileDataStream();
    ZipFileDataStream(const string& fileName_);
    virtual ~ZipFileDataStream();

	bool open();
	size_t read(void *data, size_t length);
    size_t read(void *data, size_t length, size_t repeats);
    virtual void readAll();
	void close();

protected:
    static zip *apk;   // Zip file

	struct zip_stat mZipStat;
	zip_file *mFile;   // Zipped file (content of the zip file).
};


//#pragma mark - ascii format output
//
//class StrDataStream : public DataStream {
//public:
//    StrDataStream();
//    StrDataStream(const string& fileName);
//    ~StrDataStream();
//
//    bool open();
//	size_t read(void *data, size_t length);
//    virtual void readAll();
//    void close();
//
//    inline std::string& getFileName() { return fileName; }
//    inline std::stringstream& getStrStream() { return strStream; }
//
//protected:
//    string       fileName;
//    fstream      fileStream;
//    stringstream strStream;
//};

#endif
