//
//  ofxZipArchive.cpp
//  zipTest
//
//  Created by Jeffrey Crouse on 3/19/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#include "ofxZipArchive.h"

typedef Poco::Delegate<ofxZipArchiveHandler, ZipErrorInfo> error_handler;
typedef Poco::Delegate<ofxZipArchiveHandler, ZipOkInfo> ok_handler;
typedef Poco::Delegate<ofxZipArchiveHandler, ZipDoneInfo> done_handler;


// ------------------------------------------------------
bool ofxZipArchive::compress(string folderPath, string zipPath, bool recursive, bool excludeRoot, Poco::Zip::ZipCommon::CompressionLevel cl) {
    
    folderPath = ofToDataPath(folderPath);
    zipPath = ofToDataPath(zipPath);
    
    ofLogNotice() << "Compressing " << folderPath << " to " << zipPath;
    
    std::ofstream outfile(zipPath.c_str(), ios::binary);
    if(!outfile.good())  {
        ofLogError("ofxZipArchive") << "Couldn't open " << zipPath;
        return false;
    }
    
    Poco::Zip::Compress c(outfile, true);
    
    ofxZipArchiveHandler handler;
    c.EDone += done_handler(&handler, &ofxZipArchiveHandler::onDone);
    
    c.addRecursive(Poco::Path(folderPath), cl, excludeRoot);
    c.close();
    c.EDone -= done_handler(&handler, &ofxZipArchiveHandler::onDone);
    
    return handler.isSuccessful;
}


// ----------------------------------------------------------
bool ofxZipArchive::open(string zipPath, bool binary) {
    zipPath = ofToDataPath(zipPath);
    ofLogNotice("ofxZipArchive") << "Opening " << zipPath;
    
	ios_base::openmode mode = binary ? ifstream::binary : ios_base::in;
	infile.open(zipPath.c_str(), std::ios_base::binary);
    if(!infile.good()) {
        ofLogError("ofZipArchive") << "Couldn't open " << zipPath;
        return false;
    }
    bOpened = true;
    return true;
}

// ----------------------------------------------------------
vector<string> ofxZipArchive::list() {
    vector<string> files;
    if(!bOpened) {
        ofLogWarning("ofxZipArchive") << "Archive not opened";
        return files;
    }
    
    infile.clear() ;
    infile.seekg(0, ios::beg);
    Poco::Zip::ZipArchive arch(infile);
    Poco::Zip::ZipArchive::FileInfos::const_iterator it;
    for (it = arch.fileInfoBegin(); it != arch.fileInfoEnd(); it++) {
        string fname = it->first;
        Poco::Zip::ZipFileInfo info = it->second;
        files.push_back( fname );
        ofLogNotice("ofZipArchive") << fname;
    }
    return files;
}


// ----------------------------------------------------------
ofBuffer ofxZipArchive::getFile(string fileName) {
    if(!bOpened) {
        ofLogWarning("ofxZipArchive") << "Archive not opened";
        return ofBuffer();
    }
    infile.clear() ;
    infile.seekg(0, ios::beg);
    
    // Find a file within the archive
    Poco::Zip::ZipArchive arch(infile);
    Poco::Zip::ZipArchive::FileHeaders::const_iterator it = arch.findHeader(fileName);
    if(it == arch.headerEnd()) {
        ofLogError() << fileName << " doesn't exist in archive";
        return ofBuffer();
    }
    
    ofLogNotice() << "Uncompressing " << it->second.getFileName() << " size = " << it->second.getUncompressedSize();
    
    Poco::Zip::ZipInputStream zipin (infile, it->second);
    
    ofBuffer buf(zipin);
    return buf;
    
}

// ----------------------------------------------------------
bool ofxZipArchive::unzipTo(string destination) {
    destination = ofToDataPath(destination);
    
    if(!bOpened) {
        ofLogWarning("ofxZipArchive") << "Archive not opened";
        return false;
    }
    infile.clear() ;
    infile.seekg(0, ios::beg);
    
    ofLogNotice("ofxZipArchive") << "Unzipping archive to " << destination;
    
    Poco::Path d(destination);
    Poco::Zip::Decompress de(infile, d);
    
    ofxZipArchiveHandler handler;
    
    de.EError += error_handler(&handler, &ofxZipArchiveHandler::onError);
    de.EOk += ok_handler(&handler, &ofxZipArchiveHandler::onOk);
    
    de.decompressAllFiles();
    
    de.EError -= error_handler(&handler, &ofxZipArchiveHandler::onError);
    de.EOk -= ok_handler(&handler, &ofxZipArchiveHandler::onOk);
    
    return handler.isSuccessful;
    
}