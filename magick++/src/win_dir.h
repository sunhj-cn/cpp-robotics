#pragma once
#include <iostream>
#include <string>
#include <windows.h>

// ���Ŀ¼�Ƿ����
bool DirectoryExists(const std::string& dirPath);
// ɾ��Ŀ¼�е������ļ�
void DeleteFilesInDirectory(const std::string& dirPath);
// ����Ŀ¼����������ڣ�
bool CreateDirectoryIfNotExists(const std::string& dirPath);