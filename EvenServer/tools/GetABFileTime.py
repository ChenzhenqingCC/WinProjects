#!/usr/bin/env python
#coding=utf-8
from __future__ import with_statement
import os
import sys
import codecs
import time

if __name__ == "__main__":
        path = sys.argv[1]
        path = path + r'\FengYunOLGame\Localization\INT\resouceversion.txt'
        wf = codecs.open(path,"w", "gbk")
        vs = time.strftime("%Y-%m-%d %X",time.localtime())
        wf.write(vs)
        wf.close()

