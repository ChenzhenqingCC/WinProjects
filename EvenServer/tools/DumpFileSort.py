import os

## soure directory and dest directory
mailDir = "./mailDir/"
destDir = "./destDir/"

##sort name
findSubStr = "GameVersion"
endStr = "-"

#set clear "True" to clear the source directory, else "False"
clear = True

def copyFile(oldFile, newFile):
    f1 = open(oldFile, "rb")
    f2 = open(newFile, "wb")
##    while 1:
##        text = f1.read(512)
##        if text == "":
##            break
##        f2.write(text)
    f2.write(f1.read())
    f1.close()
    f2.close()
    return

def parseMailVersion():
    if not os.path.isdir(mailDir) :
        print("there's no such dir: ", mailDir)
        return
    else :
        mailList = os.listdir(mailDir)
        for mail in mailList :
            startPos = mail.find(findSubStr)
            endPos = mail.find(endStr, startPos)
            if startPos != -1 and endPos != -1:
                version = mail[startPos + len(findSubStr):endPos]

                ## creat version dir
                newDir = destDir + version
                if not os.path.isdir(newDir) :
                    os.mkdir(newDir)

                ## copy mail to new dir
                oldFile = mailDir + mail
                newFile = newDir + '/' + mail
                copyFile(oldFile, newFile)
                if clear :
                    os.remove(oldFile)
    return



if __name__ == '__main__':
    parseMailVersion()
