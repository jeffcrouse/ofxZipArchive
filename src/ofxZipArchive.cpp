//
//  ofxZipArchive.cpp
//  zipTest
//
//  Created by Jeffrey Crouse on 3/19/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#include "ofxZipArchive.h"

typedef Poco::Delegate<ofxZipArchiveHandler, error_info> error_handler;
typedef Poco::Delegate<ofxZipArchiveHandler, ok_info> ok_handler;
typedef Poco::Delegate<ofxZipArchiveHandler, done_info> done_handler;


// ------------------------------------------------------
bool ofxZipFolder(string folderPath, string zipPath, bool recursive, bool excludeRoot, Poco::Zip::ZipCommon::CompressionLevel cl) {
    
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

// ------------------------------------------------------
bool ofxUnzip(string zipPath, string destination) {
    zipPath = ofToDataPath(zipPath);
    destination = ofToDataPath(destination);
    
    ofLogNotice("ofxZipArchive") << "Unzipping " << zipPath ;
    
    ifstream infile(zipPath.c_str(), std::ios::binary);
    infile.clear() ;
    infile.seekg(0, ios::beg);
    
    if(!infile.good()) {
        ofLogError("ofZipArchive") << "Couldn't open " << zipPath;
        return false;
    }

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


// ------------------------------------------------------
vector<string> ofxListZip(string zipPath) {
    vector<string> files;
    zipPath = ofToDataPath(zipPath);
    
    ofLogNotice("ofxZipArchive") << "Listing " << zipPath ;
    
    ifstream infile(zipPath.c_str(), std::ios::binary);
    infile.clear() ;
    infile.seekg(0, ios::beg);
    
    if(!infile.good()) {
        ofLogError("ofZipArchive") << "Couldn't open " << zipPath;
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

// ------------------------------------------------------
ofBuffer ofxZipGetFile(string zipPath, string fileName) {
    zipPath = ofToDataPath(zipPath);
    
    ofLogNotice("ofxZipArchive") << "Opening " << zipPath ;
    
    ifstream infile(zipPath.c_str(), std::ios::binary);
    infile.clear() ;
    infile.seekg(0, ios::beg);
    
    if(!infile.good()) {
        ofLogError("ofZipArchive") << "Couldn't open " << zipPath;
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




