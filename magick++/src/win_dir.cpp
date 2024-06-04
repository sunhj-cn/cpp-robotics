#include "win_dir.h"

// 检查目录是否存在
bool DirectoryExists(const std::string& dirPath) {
    DWORD fileAttr = GetFileAttributesA(dirPath.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        return false;  // 文件或目录不存在
    }
    if (fileAttr & FILE_ATTRIBUTE_DIRECTORY) {
        return true;   // 存在并且是目录
    }
    return false;      // 存在但不是目录
}

// 删除目录中的所有文件
void DeleteFilesInDirectory(const std::string& dirPath) {
    std::string searchPath = dirPath + "\\*";
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to find files in directory: " << dirPath << std::endl;
        return;
    }

    do {
        const std::string fileOrDir = findFileData.cFileName;
        if (fileOrDir == "." || fileOrDir == "..") {
            continue; // 跳过当前目录和父目录
        }

        std::string fullPath = dirPath + "\\" + fileOrDir;
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 如果是子目录，递归删除
            DeleteFilesInDirectory(fullPath);
            RemoveDirectoryA(fullPath.c_str());
        }
        else {
            // 如果是文件，删除文件
            if (!DeleteFileA(fullPath.c_str())) {
                std::cerr << "Failed to delete file: " << fullPath << std::endl;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// 创建目录（如果不存在）
bool CreateDirectoryIfNotExists(const std::string& dirPath) {
    if (!DirectoryExists(dirPath)) {
        if (CreateDirectoryA(dirPath.c_str(), NULL)) {
            std::cout << "Directory created successfully: " << dirPath << std::endl;
            return true;
        }
        else {
            std::cerr << "Failed to create directory: " << dirPath << std::endl;
            return false;
        }
    }
    else {
        std::cout << "Directory already exists: " << dirPath << std::endl;
        // 目录已存在，删除其中的所有文件
        DeleteFilesInDirectory(dirPath);

        return true;
    }
}
