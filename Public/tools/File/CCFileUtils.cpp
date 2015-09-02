/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CCFileUtils.h"
#include "CCData.h"
//#include "ccMacros.h"
//#include "CCDirector.h"
//#include "CCSAXParser.h"
//#include "unzip.h"
//#include "ServerMacros.h"
#include "ccMacros.h"
#include <stack>
//#include "stdio.h"

using namespace std;


/* The subclass FileUtilsApple should override these two method. */
ValueMap FileUtils::getValueMapFromFile(const std::string& filename) {return ValueMap();}
ValueVector FileUtils::getValueVectorFromFile(const std::string& filename) {return ValueVector();}
bool FileUtils::writeToFile(ValueMap& dict, const std::string &fullPath) {return false;}

//#endif /* (CC_TARGET_PLATFORM != CC_PLATFORM_IOS) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC) */


FileUtils* FileUtils::s_sharedFileUtils = nullptr;


void FileUtils::destroyInstance()
{
    CC_SAFE_DELETE(s_sharedFileUtils);
}

FileUtils::FileUtils()
{
}

FileUtils::~FileUtils()
{
}


bool FileUtils::init()
{
    _searchPathArray.push_back(_defaultResRootPath);
    _searchResolutionsOrderArray.push_back("");
    return true;
}

void FileUtils::purgeCachedEntries()
{
    _fullPathCache.clear();
}

static bool writeData(const std::string& filename, const Data & tagData)
{
	const char* mode = "wb";
	std::string fullPath = filename;
	FILE ff;
	FILE *fp = &ff;
	fopen_s(&fp, fullPath.c_str(), mode);
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_SET);
		fwrite(tagData.getBytes(), tagData.getSize(), 1, fp);
		fclose(fp);
		return true;
	}
	return false;
}

static Data getData(const std::string& filename, bool forString)
{
    //CCASSERT(!filename.empty(), "Invalid filename!");
    
    Data ret;
    unsigned char* buffer = nullptr;
    ssize_t size = 0;
    const char* mode = nullptr;
    if (forString)
        mode = "rt";
    else
        mode = "rb";
    
    do
    {
        // Read the file from hardware
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
		FILE ff;
		FILE *fp = &ff;
		fopen_s(&fp,fullPath.c_str(), mode);
        CC_BREAK_IF(!fp);
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        
        if (forString)
        {
            buffer = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
            buffer[size] = '\0';
        }
        else
        {
            buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
        }
        
        size = fread(buffer, sizeof(unsigned char), size, fp);
        fclose(fp);
    } while (0);
    
    if (nullptr == buffer || 0 == size)
    {
        std::string msg = "Get data from file(";
        msg.append(filename).append(") failed!");
        //CCLOG("%s", msg.c_str());
    }
    else
    {
        ret.fastSet(buffer, size);
    }
    
    return ret;
}

std::string FileUtils::getStringFromFile(const std::string& filename)
{
    Data data = getData(filename, true);
    if (data.isNull())
    	return "";
    std::string ret((const char*)data.getBytes());
    return ret;
}

Data FileUtils::getDataFromFile(const std::string& filename)
{
    return getData(filename, false);
}

bool FileUtils::writeDataToFile(const std::string& filename, const Data & tagData)
{
	return writeData(filename, tagData);
}

//unsigned char* FileUtils::getFileData(const std::string& filename, const char* mode, ssize_t *size)
//{
//    unsigned char * buffer = nullptr;
//    //CCASSERT(!filename.empty() && size != nullptr && mode != nullptr, "Invalid parameters.");
//    *size = 0;
//    do
//    {
//        // read the file from hardware
//        const std::string fullPath = fullPathForFilename(filename);
//        FILE *fp = fopen(fullPath.c_str(), mode);
//        CC_BREAK_IF(!fp);
//        
//        fseek(fp,0,SEEK_END);
//        *size = ftell(fp);
//        fseek(fp,0,SEEK_SET);
//        buffer = (unsigned char*)malloc(*size);
//        *size = fread(buffer,sizeof(unsigned char), *size,fp);
//        fclose(fp);
//    } while (0);
//    
//    if (! buffer)
//    {
//        std::string msg = "Get data from file(";
//        msg.append(filename).append(") failed!");
//        
//        //CCLOG("%s", msg.c_str());
//    }
//    return buffer;
//}

//unsigned char* FileUtils::getFileDataFromZip(const std::string& zipFilePath, const std::string& filename, ssize_t *size)
//{
//    unsigned char * buffer = nullptr;
//    unzFile file = nullptr;
//    *size = 0;
//
//    do 
//    {
//        CC_BREAK_IF(zipFilePath.empty());
//
//        file = unzOpen(zipFilePath.c_str());
//        CC_BREAK_IF(!file);
//
//        int ret = unzLocateFile(file, filename.c_str(), 1);
//        CC_BREAK_IF(UNZ_OK != ret);
//
//        char filePathA[260];
//        unz_file_info fileInfo;
//        ret = unzGetCurrentFileInfo(file, &fileInfo, filePathA, sizeof(filePathA), nullptr, 0, nullptr, 0);
//        CC_BREAK_IF(UNZ_OK != ret);
//
//        ret = unzOpenCurrentFile(file);
//        CC_BREAK_IF(UNZ_OK != ret);
//
//        buffer = (unsigned char*)malloc(fileInfo.uncompressed_size);
//        int CC_UNUSED readedSize = unzReadCurrentFile(file, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
//        //CCASSERT(readedSize == 0 || readedSize == (int)fileInfo.uncompressed_size, "the file size is wrong");
//
//        *size = fileInfo.uncompressed_size;
//        unzCloseCurrentFile(file);
//    } while (0);
//
//    if (file)
//    {
//        unzClose(file);
//    }
//
//    return buffer;
//}

std::string FileUtils::getNewFilename(const std::string &filename)
{
    std::string newFileName;
    
    // in Lookup Filename dictionary ?
    auto iter = _filenameLookupDict.find(filename);

    if (iter == _filenameLookupDict.end())
    {
        newFileName = filename;
    }
    else
    {
        newFileName = iter->second.asString();
    }
    return newFileName;
}

std::string FileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath)
{
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }
    
    // searchPath + file_path + resourceDirectory
    std::string path = searchPath;
    path += file_path;
    path += resolutionDirectory;
    
    path = getFullPathForDirectoryAndFilename(path, file);
    
    //CCLOG("getPathForFilename, fullPath = %s", path.c_str());
    return path;
}


std::string FileUtils::fullPathForFilename(const std::string &filename)
{
    if (isAbsolutePath(filename))
    {
        return filename;
    }

    // Already Cached ?
    auto cacheIter = _fullPathCache.find(filename);
    if( cacheIter != _fullPathCache.end() )
    {
        return cacheIter->second;
    }
    
    // Get the new file name.
    const std::string newFilename( getNewFilename(filename) );
    
	std::string fullpath;
    
    for (auto searchIt = _searchPathArray.cbegin(); searchIt != _searchPathArray.cend(); ++searchIt) {
        for (auto resolutionIt = _searchResolutionsOrderArray.cbegin(); resolutionIt != _searchResolutionsOrderArray.cend(); ++resolutionIt) {
            
            fullpath = this->getPathForFilename(newFilename, *resolutionIt, *searchIt);
            
            if (fullpath.length() > 0)
            {
                // Using the filename passed in as key.
                _fullPathCache.insert(std::pair<std::string, std::string>(filename, fullpath));
                return fullpath;
            }
        }
    }
    
    //CCLOG("cocos2d: fullPathForFilename: No file found at %s. Possible missing file.", filename.c_str());

    // XXX: Should it return nullptr ? or an empty string ?
    // The file wasn't found, return the file name passed in.
    return filename;
}

std::string FileUtils::fullPathFromRelativeFile(const std::string &filename, const std::string &relativeFile)
{
    return relativeFile.substr(0, relativeFile.rfind('/')+1) + getNewFilename(filename);
}

void FileUtils::setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder)
{
    bool existDefault = false;
    _fullPathCache.clear();
    _searchResolutionsOrderArray.clear();
    for(auto iter = searchResolutionsOrder.cbegin(); iter != searchResolutionsOrder.cend(); ++iter)
    {
        std::string resolutionDirectory = *iter;
        if (!existDefault && resolutionDirectory == "")
        {
            existDefault = true;
        }
        
        if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != '/')
        {
            resolutionDirectory += "/";
        }
        
        _searchResolutionsOrderArray.push_back(resolutionDirectory);
    }
    if (!existDefault)
    {
        _searchResolutionsOrderArray.push_back("");
    }
}

void FileUtils::addSearchResolutionsOrder(const std::string &order)
{
    _searchResolutionsOrderArray.push_back(order);
}

const std::vector<std::string>& FileUtils::getSearchResolutionsOrder()
{
    return _searchResolutionsOrderArray;
}

const std::vector<std::string>& FileUtils::getSearchPaths() const
{
    return _searchPathArray;
}

void FileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool existDefaultRootPath = false;
    
    _fullPathCache.clear();
    _searchPathArray.clear();
    for (auto iter = searchPaths.cbegin(); iter != searchPaths.cend(); ++iter)
    {
        std::string prefix;
        std::string path;
        
        if (!isAbsolutePath(*iter))
        { // Not an absolute path
            prefix = _defaultResRootPath;
        }
        path = prefix + (*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!existDefaultRootPath && path == _defaultResRootPath)
        {
            existDefaultRootPath = true;
        }
        _searchPathArray.push_back(path);
    }
    
    if (!existDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        _searchPathArray.push_back(_defaultResRootPath);
    }
}

void FileUtils::addSearchPath(const std::string &searchpath)
{
    std::string prefix;
    if (!isAbsolutePath(searchpath))
        prefix = _defaultResRootPath;

    std::string path = prefix + searchpath;
    if (path.length() > 0 && path[path.length()-1] != '/')
    {
        path += "/";
    }
    _searchPathArray.push_back(path);
}

void FileUtils::setFilenameLookupDictionary(const ValueMap& filenameLookupDict)
{
    _fullPathCache.clear();    
    _filenameLookupDict = filenameLookupDict;
}

void FileUtils::loadFilenameLookupDictionaryFromFile(const std::string &filename)
{
    const std::string fullPath = fullPathForFilename(filename);
    if (fullPath.length() > 0)
    {
        ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
        if (!dict.empty())
        {
            ValueMap& metadata =  dict["metadata"].asValueMap();
            int version = metadata["version"].asInt();
            if (version != 1)
            {
                //CCLOG("cocos2d: ERROR: Invalid filenameLookup dictionary version: %d. Filename: %s", version, filename.c_str());
                return;
            }
            setFilenameLookupDictionary( dict["filenames"].asValueMap());
        }
    }
}

std::string FileUtils::getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename)
{
    // get directory+filename, safely adding '/' as necessary 
    std::string ret = directory;
    if (directory.size() && directory[directory.size()-1] != '/'){
        ret += '/';
    }
    ret += filename;
    
    // if the file doesn't exist, return an empty string
    if (!isFileExist(ret)) {
        ret = "";
    }
    return ret;
}

bool FileUtils::isAbsolutePath(const std::string& path) const
{
    return (path[0] == '/');
}

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_popupNotify = true;

void FileUtils::setPopupNotify(bool notify)
{
    s_popupNotify = notify;
}

bool FileUtils::isPopupNotify()
{
    return s_popupNotify;
}


