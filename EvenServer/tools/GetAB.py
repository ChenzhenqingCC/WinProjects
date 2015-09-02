#!/usr/bin/env python
#coding=utf-8
import os
import sys
        

#if __name__ == "__main__":
commandtxt = "call ShippingConfig.bat"
os.system(commandtxt)
commandtxt = "ab logon"
os.system(commandtxt)
commandtxt = "mkdir %RESOURCE_ROOT_PATH%\ACB_FINALRELEASE"
os.system(commandtxt)
commandtxt = "mkdir %RESOURCE_ROOT_PATH%\ACB_FINALRELEASE\Binaries\Win32"
os.system(commandtxt)
commandtxt = "mkdir %RESOURCE_ROOT_PATH%\ACB_FINALRELEASE\Engine"
os.system(commandtxt)
commandtxt = "mkdir %RESOURCE_ROOT_PATH%\ACB_FINALRELEASE\FengYunOLGame"
os.system(commandtxt)
commandtxt = "del %CODE_ROOT_PATH%\Binaries\Win32\FengYunOLGame.exe"
os.system(commandtxt)
commandtxt = "del %CODE_ROOT_PATH%\Binaries\Win32\FengYunOLGame.exp"
os.system(commandtxt)
commandtxt = "del %CODE_ROOT_PATH%\Binaries\Win32\FengYunOLGame.lib"
os.system(commandtxt)
commandtxt = "del %CODE_ROOT_PATH%\Binaries\Win32\FengYunOLGame.pdb"
os.system(commandtxt)
commandtxt = "ab getlatest %RESOURCE_AB_PATH%\Engine -overwritewritable replace -localpath %CODE_ROOT_PATH%\Engine"
os.system(commandtxt)
commandtxt = "ab getlatest %RESOURCE_AB_PATH%\FengYunOLGame -overwritewritable replace -localpath %CODE_ROOT_PATH%\FengYunOLGame"
os.system(commandtxt)
commandtxt = "ab getlatest %RESOURCE_AB_PATH%\ShippingDLL -overwritewritable replace -localpath %CODE_ROOT_PATH%\ShippingDLL"
os.system(commandtxt)
commandtxt = "ab getlatest %DATA_ECA_PATH% -overwritewritable replace -localpath %LOC_TRIGGER_PATH%"
os.system(commandtxt)
commandtxt = "ab getlatest %DATA_AB_PATH% -overwritewritable replace -localpath %CONFIG_ROOT_PATH%"
os.system(commandtxt)
commandtxt = "ab getlatest %RESOURCE_AB_PATH%\TriggerXml -overwritewritable replace -localpath %LOC_TRIGGER_PATH%\TriggerXml"
os.system(commandtxt)
commandtxt = "python Internal/ConfigDataExporter/tools/GetABFileTime.py %CODE_ROOT_PATH%"
os.system(commandtxt)
commandtxt = "ab logoff"
os.system(commandtxt)
commandtxt = "attrib -r %CODE_ROOT_PATH%\Binaries\*.* /S /D"
os.system(commandtxt)
commandtxt = "attrib -r %CODE_ROOT_PATH%\Engine\*.* /S /D"
os.system(commandtxt)
commandtxt = "attrib -r %CODE_ROOT_PATH%\FengYunOLGame\*.* /S /D"
os.system(commandtxt)



        
