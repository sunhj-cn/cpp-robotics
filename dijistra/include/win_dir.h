#pragma once
#include <iostream>
#include <string>
#include <windows.h>

// 检查目录是否存在
bool DirectoryExists(const std::string& dirPath);
// 删除目录中的所有文件
void DeleteFilesInDirectory(const std::string& dirPath);
// 创建目录（如果不存在）
bool CreateDirectoryIfNotExists(const std::string& dirPath);