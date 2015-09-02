#!/usr/bin/env python
#coding=utf-8
from __future__ import with_statement
import os
import sys
import codecs


# 返回svn本地工作拷贝的版本号
def getSvnWorkingCopyRevision(path):
	# 解析svn log
	version = 0
	call = "svn info %s" % path
	with os.popen(call) as tempFile:		
		SKIP_COUNT_NEED = 8
		skipCount = 0
		for line in tempFile:
			skipCount = skipCount + 1
			if skipCount <= SKIP_COUNT_NEED:
				continue
			firstBlankPos = -1
			for i in range(len(line)):
				if line[i] == ':':
					firstBlankPos = i
					break
			version = line[firstBlankPos+2:-1]
			break
	# 确保解析得到正确的版本号
	try:
		int(version)
	except ValueError:
		print "can't parse svn info, version(%s)" % version
		return 0
	return version

if __name__ == "__main__":
        path = sys.argv[1]
        path = path + r'\FengYunOLGame\Localization\INT\codeversion.txt'
        wf = codecs.open(r'E:\FYClient\trunk\FengYunOLGame\Localization\INT\codeversion.txt',"w", "gbk")
        codepath = sys.argv[1]
        codepath = codepath + r'\Development\Src'
        vs = getSvnWorkingCopyRevision(codepath)
        wf.write(vs)
        wf.close()

