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


typedef std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> error_info;
typedef std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> ok_info;
typedef const Poco::Zip::ZipLocalFileHeader done_info;


bool ofxZipFolder(string folderPath, string zipPath, bool recursive=true, bool excludeRoot=true, Poco::Zip::ZipCommon::CompressionLevel cl=Poco::Zip::ZipCommon::CL_NORMAL);
bool ofxUnzip(string zipPath, string destination);
vector<string> ofxListZip(string zipPath);
ofBuffer ofxZipGetFile(string zipPath, string file);


//--------------------------------------------
class ofxZipArchiveHandler
{
public:
    ofxZipArchiveHandler() {
        isSuccessful = false;
    }
    ~ofxZipArchiveHandler(){
    }
    
    void onError(const void*, error_info& info){
        ofLogError("ofxZipArchiveHandler") << "ofxZipUtils: Failed to Unzip: " + info.second;
        isSuccessful = false;
    }
    
    void onOk(const void*, ok_info& info){
        ofLogNotice("ofxZipArchiveHandler") << "ofxZipUtils: Unzipped: " << info.second.toString();
        isSuccessful = true;
    }
    
    void onDone(const void*, done_info& header){
        ofLogNotice() <<"ofxZipUtils: Zipped";
        isSuccessful = true;
    }
    
    bool isSuccessful;
    ofLogLevel logLevel;
};

#else
#warning *** ofxZipUtils Requires Poco Library
#endif