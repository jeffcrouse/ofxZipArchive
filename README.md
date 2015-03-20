# ofxZipArchive
Primarily for dealing with existing zip archives. Features include: 
- list - list the files inside a zip archive
- unzip - unzip the entire archive to a destination
- read individual files to ofBuffer
- compress - compress folders and files with variable compression

substantial help from https://github.com/danoli3/ofxZipUtils

## IMPORTANT

This addon is a little picky about the way you compress files. For instance, on OSX, it doesn't seem to like the right-click>Compress compression - whatever it is. Instead, use the command line
`zip -r archive.zip archive/`  

If you have any idea why this is, please let me know!
