# ofxZipArchive
openFrameworks addon for dealing with zip archives. compress, extract, add to archive, read individual files from archive.

based on https://github.com/danoli3/ofxZipUtils

## IMPORTANT

This addon is a little picky about the way you compress files. For instance, on OSX, it doesn't seem to like the right-click>Compress compression - whatever it is. Instead, use the command line
`zip -r archive.zip archive/`

## Functions

### 