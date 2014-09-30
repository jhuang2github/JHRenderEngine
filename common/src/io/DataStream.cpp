//
//  DataStream.cpp
//
//  Created by Jingshu Huang on 12/30/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "DataStream.h"
#include "zlib.h"


#pragma mark - DataStream

DataStream::DataStream() {
    mSize = 0;
    mData = NULL;
}

DataStream::~DataStream() {
    if (mData) {
        delete mData;
    }
    mData = NULL;
}


#pragma mark - BufferDataStream

BufferDataStream::BufferDataStream(char *buffer_, size_t bufferSize_) {
	mData = buffer_;
	mSize = bufferSize_;
	currentPos = 0;
}

BufferDataStream::~BufferDataStream() {
	close();
}

bool BufferDataStream::open() {
	currentPos = 0;
	return mData && mSize > 0;
}

size_t BufferDataStream::read(void *data, size_t length) {
	size_t readLength = mSize - currentPos;
	if (readLength > length) {
		readLength = length;
	}
	memcpy(data, mData + currentPos, readLength);
	currentPos += readLength;
	return readLength;
}

void BufferDataStream::close() {
	mData = NULL;
	mSize = 0;
	currentPos = 0;
}


#pragma mark - FileDataStream

FileDataStream::FileDataStream() {
    fileName.clear();
    f = NULL;
}

FileDataStream::FileDataStream(const string& fileName_) {
    if (fileName_.empty()) {
        return;
    }
    fileName = fileName_;
    f = NULL;
    readAll();
}

FileDataStream::~FileDataStream() {
	close();
}

bool FileDataStream::open() {
	f = fopen(fileName.c_str(), "rb");
	if (!f) {
		LOGD("[FileDataStream open] Error: open %s failed", fileName.c_str());
		return false;
	}
	return true;
}

size_t FileDataStream::read(void *data, size_t length) {
	if (!f) {
		return 0;
	}
	return fread(data, length, 1, f);
}

size_t FileDataStream::read(void *data, size_t length, size_t repeats) {
	if (!f) {
		return 0;
	}
	return fread(data, length, repeats, f);
}

void FileDataStream::close() {
	if (f) {
		fclose(f);
	}
    f = NULL;
}

void FileDataStream::readAll() {
    if (!open()) {
        return;
    }

	fseek(f, 0, SEEK_END);
	mSize = ftell(f);
	fseek(f, 0, SEEK_SET);
	mData = (char *)malloc(mSize + 1);
	fread(mData, mSize, 1, f);
    mData[mSize] = '\0';

    close();
}


#pragma mark - ZipFileDataStream


zip* ZipFileDataStream::apk = NULL;


void ZipFileDataStream::openZip(const char *apkFileName) {
	closeZip();
	apk = zip_open(apkFileName, 0, NULL);
	if (!apk) {
		LOGD("[ZipFileDataStream openZip] Error: fail to open the zip(apk) %s", apkFileName);
	}
}


void ZipFileDataStream::closeZip() {
	if (apk) {
		zip_close(apk);
		apk = NULL;
	}
}


ZipFileDataStream::ZipFileDataStream() {
    fileName.clear();
    f = NULL;
    mFile = NULL;
}

ZipFileDataStream::ZipFileDataStream(const string& fileName_) {
    if (fileName_.empty()) {
        return;
    }
    if (!apk) {
        LOGD("[ZipFileDataStream ZipFileDataStream] Error: Zip file is not open "
             "yet. Cannot read the content.\n");
        return;
    }

    fileName = fileName_;
    f = NULL;
    mFile = NULL;

	zip_stat_init(&mZipStat);
	zip_stat(apk, fileName_.c_str(), ZIP_FL_UNCHANGED, &mZipStat);

    readAll();
}

ZipFileDataStream::~ZipFileDataStream() {
	close();
}

// Open the content (zipped file) inside a zip file.
bool ZipFileDataStream::open() {
	if (!apk || mZipStat.valid == 0) {
		return false;
	}
	zip_file *zf = zip_fopen_index(apk, mZipStat.index, ZIP_FL_UNCHANGED);
	if (!zf) {
        LOGD("[ZipFileDataStream open] Error: cannot open the content of the zipped file.\n");
		return false;
	}
	if (mFile) {
		zip_fclose(mFile);
	}
	mFile = zf;
	return true;
}

size_t ZipFileDataStream::read(void *data, size_t length) {
	if (!mFile) {
		return 0;
	}
	return zip_fread(mFile, data, length);
}

size_t ZipFileDataStream::read(void *data, size_t length, size_t repeats) {
    assert(false);
    return 0;
}

void ZipFileDataStream::close() {
	if (mFile) {
		zip_fclose(mFile);
		mFile = NULL;
	}
}

void ZipFileDataStream::readAll() {
    if (!open()) {
        return;
    }
    
    mSize = mZipStat.size;
	mData = (char *)malloc(mSize + 1);
	zip_fread(mFile, mData, mSize);
    mData[mSize] = '\0';

    close();
}


//#pragma mark - StrDataStream
//
//StrDataStream::StrDataStream() {
//}
//
//StrDataStream::StrDataStream(const string& name) {
//    fileName = name;
//    readAll();
//}
//
//StrDataStream::~StrDataStream() {
//    close();
//}
//
//bool StrDataStream::open() {
//    fileStream.open(fileName.c_str());
//    if (!fileStream) {
//        LOGD("open %s failed", fileName.c_str());
//        return false;
//    }
//    return true;
//}
//
//void StrDataStream::close() {
//    fileStream.close();
//}
//
//void StrDataStream::readAll() {
//    open();
//    fileStream.seekg(0, ios::end);
//    mSize = fileStream.tellg();
//    fileStream.seekg(0, ios::beg);
//    mData = (char *)malloc(mSize + 1);
//    fileStream.read(mData, mSize);
//    close();
//
//    mData[mSize] = '\0';
//    strStream.str(mData);
//}
//
//size_t StrDataStream::read(void *data, size_t length) {
//    if (!fileStream) {
//        return 0;
//    }
//
//    char* buffer = (char*)malloc(length);
//    fileStream.read(buffer, length);
//    memcpy(data, buffer, length);
//    delete buffer;
//
//    return length;
//}

//istream& StrDataStream::readLine() {
//    return std::getline(fileStream, lineStream);
//}
//
//string StrDataStream::readAllIntoString() {
//    string contents;
//    contents.clear();
//
//    std::ifstream in(fileName.c_str(), ios::in | ios::binary);
//    if (in) {
//        in.seekg(0, ios::end);
//        contents.resize(in.tellg());
//        in.seekg(0, ios::beg);
//        in.read(&contents[0], contents.size());
//        in.close();
//    }
//    return contents;
//}


/*

static const uint zbufferSize = 131072;
static byte *zbuffer = new byte[zbufferSize];

void *ResourceReader::deflateData(void *data, uint dataSize, uint& zipSize) {
	if (!data || dataSize == 0) {
		return NULL;
	}
    
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
		return NULL;
	}
    
	strm.avail_in = dataSize;
	strm.next_in = (byte *)data;
    
	byte *out = NULL;
	if (zipSize > 0) {
		out = new byte[zipSize];
		strm.avail_out = zipSize;
		strm.next_out = out;
		if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR) {
			delete out;
			deflateEnd(&strm);
			return NULL;
		}
	} else {
	    uint have = 0;
		zipSize = 0;
		do {
			strm.avail_out = zbufferSize;
			strm.next_out = zbuffer;
			if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR) {
				if (out) {
					delete out;
				}
				deflateEnd(&strm);
				return NULL;
			}
			have = zbufferSize - strm.avail_out;
			if (have > 0) {
				if (out) {
					byte *tmp = new byte[zipSize + have];
					memcpy(tmp, out, zipSize);
					delete out;
					out = tmp;
				} else {
					out = new byte[have];
				}
				memcpy(out + zipSize, zbuffer, have);
				zipSize += have;
			}
		} while (strm.avail_out == 0);
	}
    
	deflateEnd(&strm);
	return out;
}

void *ResourceReader::inflateData(void *zipData, uint zipSize, uint& dataSize) {
	if (!zipData || zipSize == 0) {
		return NULL;
	}
    
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    if (inflateInit(&strm) != Z_OK) {
        return NULL;
    }
    
	strm.avail_in = zipSize;
	strm.next_in = (byte *)zipData;
    
	byte *out = NULL;
	if (dataSize > 0) {
		out = new byte[dataSize];
		strm.avail_out = dataSize;
		strm.next_out = out;
		int ret = inflate(&strm, Z_FINISH);
		if (ret != Z_OK && ret != Z_STREAM_END) {
			inflateEnd(&strm);
			delete out;
			return NULL;
		}
	} else {
	    uint have = 0;
		dataSize = 0;
		int ret;
		do {
			strm.avail_out = zbufferSize;
			strm.next_out = zbuffer;
			ret = inflate(&strm, Z_NO_FLUSH);
			if (ret != Z_OK && ret != Z_STREAM_END) {
				delete out;
				inflateEnd(&strm);
				return NULL;
			}
			have = zbufferSize - strm.avail_out;
			if (have > 0) {
				if (out) {
					byte *tmp = new byte[dataSize + have];
					memcpy(tmp, out, dataSize);
					delete out;
					out = tmp;
				} else {
					out = new byte[have];
				}
				memcpy(out + dataSize, zbuffer, have);
				dataSize += have;
			}
		} while (strm.avail_out == 0);
	}
    
    inflateEnd(&strm);
    return out;
}

*/
