#include "win_dir.h"

// ���Ŀ¼�Ƿ����
bool DirectoryExists(const std::string& dirPath) {
    DWORD fileAttr = GetFileAttributesA(dirPath.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        return false;  // �ļ���Ŀ¼������
    }
    if (fileAttr & FILE_ATTRIBUTE_DIRECTORY) {
        return true;   // ���ڲ�����Ŀ¼
    }
    return false;      // ���ڵ�����Ŀ¼
}

// ɾ��Ŀ¼�е������ļ�
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
            continue; // ������ǰĿ¼�͸�Ŀ¼
        }

        std::string fullPath = dirPath + "\\" + fileOrDir;
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // �������Ŀ¼���ݹ�ɾ��
            DeleteFilesInDirectory(fullPath);
            RemoveDirectoryA(fullPath.c_str());
        }
        else {
            // ������ļ���ɾ���ļ�
            if (!DeleteFileA(fullPath.c_str())) {
                std::cerr << "Failed to delete file: " << fullPath << std::endl;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// ����Ŀ¼����������ڣ�
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
        // Ŀ¼�Ѵ��ڣ�ɾ�����е������ļ�
        DeleteFilesInDirectory(dirPath);

        return true;
    }
}
