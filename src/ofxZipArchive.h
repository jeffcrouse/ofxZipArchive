//
//  ofxZipArchive.h
//  zipTest
//
//  Created by Jeffrey Crouse on 3/19/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#ifdef OF_USING_POCO
#include <Poco/Zip/ZipArchive.h>
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/Zip/ZipStream.h>
#include <Poco/Path.h>
#include <Poco/Delegate.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>


// ----------------------------------------------------------
class ofxZipArchive {
public:
    ofxZipArchive():bOpened(false){}

	bool open(string zipPath, bool binary = false);
    vector<string> list();
    ofBuffer getFile(string fileName);
    bool unzipTo(string destination);
    
    static bool compress(string folderPath, string zipPath, bool recursive=true, bool excludeRoot=true, Poco::Zip::ZipCommon::CompressionLevel cl=Poco::Zip::ZipCommon::CL_NORMAL);
protected:
    ifstream infile;
    bool bOpened;
};



// ----------------------------------------------------------
typedef std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> ZipErrorInfo;
typedef std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> ZipOkInfo;
typedef const Poco::Zip::ZipLocalFileHeader ZipDoneInfo;



// ----------------------------------------------------------
class ofxZipArchiveHandler {
public:
    ofxZipArchiveHandler() {
        isSuccessful = false;
    }
    ~ofxZipArchiveHandler(){
    }
    
    void onError(const void*, ZipErrorInfo& info){
        ofLogError("ofxZipArchive") << "ofxZipUtils: Failed to Unzip: " + info.second;
        isSuccessful = false;
    }
    
    void onOk(const void*, ZipOkInfo& info){
        ofLogNotice("ofxZipArchive") << "ofxZipUtils: Unzipped: " << info.second.toString();
        isSuccessful = true;
    }
    
    void onDone(const void*, ZipDoneInfo& header){
        ofLogNotice("ofxZipArchive") << "Zipped " << header.getFileName() << " was " << header.getUncompressedSize() << " now " << header.getCompressedSize();
        isSuccessful = true;
    }
    
    bool isSuccessful;
};

#else
#warning *** ofxZipUtils Requires Poco Library
#endif