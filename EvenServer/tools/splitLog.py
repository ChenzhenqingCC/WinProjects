import struct
import sys
import os
import string

def Split( fileName,splitSize ):
	lastSplit = 0
	splitCount = 1
	
	try:
		fileSize =  os.path.getsize( fileName + ".bin" )
	except :
		print( "Error File" )
		sys.exit(0)
		
	cur = 0
	messageCount = 0
	f = open( fileName + ".bin",'rb')
	
	newFile = "%s.%d.bin" % (fileName,splitCount)
	showInfo =  "Generate %s" % (newFile) 
	splitFile = open( newFile,'wb' )
	while True:
		header = f.read( 9 )
		splitFile.write( header )	
		length = struct.unpack( 'I',header[5:9] )[0]
		splitFile.write( f.read( length ) )	
		messageCount = messageCount + 1
		cur += 9 + length
		if ( cur == fileSize ):
			splitFile.close()
			print ( "%s ... (%d/%d)" % (showInfo,cur,fileSize) )
			print ( "Split Complete" )
			break
		if ( cur - lastSplit >= splitSize ):
			lastSplit = cur
			splitCount = splitCount + 1
			splitFile.close()
			print ( "%s ... (%d/%d)" % (showInfo,cur,fileSize) )
			newFile = "%s%d.bin" % (fileName,splitCount)
			showInfo =  "Generate %s" % (newFile) 
			splitFile = open( newFile,'wb' )	


if len(sys.argv)!=2 and len(sys.argv)!=3:
	print "Usage:"
	print "\t./splitLog.py <fileName>"
	print "\t./xml2dat.py <fileName> <splitSize>"	
	sys.exit(0)


FILE_NAME=sys.argv[1]
if len(sys.argv)==3:
	try:
		FILE_SIZE = string.atoi(sys.argv[2])
	except:
		print "Usage:"
		print "\t./splitLog.py <fileName>"
		print "\t./xml2dat.py <fileName> <splitSize>"	
		sys.exit(0)
else:
	FILE_SIZE = 50

FILE_SIZE = FILE_SIZE*1024*1024	
Split( FILE_NAME,FILE_SIZE )
