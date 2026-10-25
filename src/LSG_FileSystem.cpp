#include "LSG_FileSystem.h"

void LSG_FileSystem::CreateDirInPath(const std::string& dir, const std::string& path)
{
	if (dir.empty() || path.empty())
		throw std::invalid_argument("'dir' and 'path' can not be empty.");

	auto subDir  = (dir.starts_with('/') || dir.starts_with('\\') ? dir.substr(1) : dir);
	auto baseDir = (path.ends_with('/')  || path.ends_with('\\')  ? path.substr(0, path.size() - 1) : path);

	#if defined _windows
        auto result = _wmkdir(std::format(L"{}\\{}", LSG_TextUtils_ToWide(baseDir), LSG_TextUtils_ToWide(subDir)).c_str());
    #else
        auto result = mkdir(std::format("{}/{}", baseDir, subDir).c_str(), (S_IRWXU | S_IRWXG));
    #endif

    if ((result != 0) && (errno != EEXIST))
        throw std::runtime_error(std::format("Failed to create dir '{}' in path '{}'.", subDir, baseDir));
}

size_t LSG_FileSystem::GetDirSize(const std::string& dirPath)
{
    auto directory = std::filesystem::recursive_directory_iterator(dirPath);

    size_t size = 0;

    for (const auto& dirEntry : directory) {
        if (dirEntry.is_regular_file())
            size += dirEntry.file_size();
    }

    return size;
}

LSG_File LSG_FileSystem::GetFile(const std::string& filePath)
{
    LSG_File file = { .filePath = filePath };

	auto lastSeparator = file.filePath.rfind('/');

    if (lastSeparator == std::string::npos)
        lastSeparator = file.filePath.rfind('\\');

    if (lastSeparator != std::string::npos) {
	    file.pathSep = file.filePath[lastSeparator];
	    file.path    = file.filePath.substr(0, lastSeparator);
        file.file    = file.filePath.substr(lastSeparator + 1);
    } else {
        file.file = file.filePath;
    }

    auto extension = file.file.rfind('.');

    if (extension != std::string::npos) {
        file.name = file.file.substr(0, extension);
        file.ext  = LSG_TextUtils_ToLower(file.file.substr(extension + 1));
    } else {
        file.name = file.file;
    }

    return file;
}

LSG_Strings LSG_FileSystem::GetFilesFromDir(const std::string& dirPath)
{
    if (!std::filesystem::is_directory(dirPath))
        return {};

    LSG_Strings files;

    auto directory = std::filesystem::recursive_directory_iterator(dirPath);

    for (const auto& dirEntry : directory) {
        if (dirEntry.is_regular_file())
            files.push_back(LSG_TextUtils_ToUTF8(dirEntry.path().wstring()));
    }

    return files;
}

std::string LSG_FileSystem::GetUserDir(const std::string& appName)
{
	if (appName.empty())
		throw std::invalid_argument("'appName' can not be empty.");

    #if defined _DEBUG
        auto prefPath = SDL_GetPrefPath(nullptr, std::format("{}-dev", appName).c_str());
    #else
        auto prefPath = SDL_GetPrefPath(nullptr, appName.c_str());
    #endif

    if (!prefPath)
        throw std::runtime_error(std::format("Failed to get an app-specific location where files can be written: {}", SDL_GetError()));

    auto userDir = std::string(prefPath);

    SDL_free(prefPath);

	return userDir;
}
