#include "main.h"

#ifndef LSG_FILE_SYSTEM_H
#define LSG_FILE_SYSTEM_H

class LSG_FileSystem
{
private:
	LSG_FileSystem() {}
	~LSG_FileSystem() {}

public:
	static void        CreateDirInPath(const std::string& dir, const std::string& path);
	static LSG_File    GetFile(const std::string& filePath);
	static LSG_Strings GetFilesFromDir(const std::string& dirPath);
	static std::string GetUserDir(const std::string& appName);
};

#endif
