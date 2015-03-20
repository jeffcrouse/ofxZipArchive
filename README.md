# ofxZipArchive
Primarily for dealing with existing zip archives, but a zipping method is provided for convenience. Extract, read individual files from archive to ofBuffer.

substantial help from https://github.com/danoli3/ofxZipUtils

## IMPORTANT

This addon is a little picky about the way you compress files. For instance, on OSX, it doesn't seem to like the right-click>Compress compression - whatever it is. Instead, use the command line
`zip -r archive.zip archive/`  If you have any idea why, please let me know!
