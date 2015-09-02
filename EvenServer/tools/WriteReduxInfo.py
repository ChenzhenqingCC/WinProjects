#!/usr/bin/env python
#coding=utf-8
import os
import sys
import codecs
import md5
from os.path import join, getsize

#print sys.argv[1]
#print sys.argv[2]

#计算md5
def sumfile(fobj):    
    m = md5.new()
    while True:
        d = fobj.read(8192)
        if not d:
            break
        m.update(d)
    return m.hexdigest()

def md5sum(fname):    
    if fname == '-':
        ret = sumfile(sys.stdin)
    else:
        try:
            f = file(fname, 'rb')
        except:
            return ''
        ret = sumfile(f)
        f.close()
    return ret

fileMd5 = md5sum(sys.argv[1])
fileSize = getsize(sys.argv[3])
writef = codecs.open(sys.argv[2], "w", "utf-8")
writelines = {}

writelines[0] = "[ReduxInfo]\r\n"
writelines[1] = "UnzipedFileDataMd5=" + fileMd5 + "\r\n"
writelines[2] = "UnzipedFileDataSize=" + str(fileSize / (1024*1024)) + "\r\n"

idx = 0
for wr in writelines:
	writef.write(writelines[idx])
	idx = idx + 1
	
writef.close()

