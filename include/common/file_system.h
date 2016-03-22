﻿/**
 * @file file_system.h
 * @brief 文件系统统一接口
 * Licensed under the MIT licenses.
 *
 * @version 1.0
 * @author OWenT
 * @date 2014.12.15
 *
 * @history
 *
 *
 */

#ifndef _UTIL_COMMON__FILESYSTEM_H
#define _UTIL_COMMON__FILESYSTEM_H

#pragma once

#include <string>
#include <vector>
#include <list>
#include <climits>
#include <cstdio>

#if defined(__CYGWIN__) // Windows Cygwin
#define UTIL_FS_POSIX_API
#elif defined(_WIN32) // Windows default, including MinGW
#define UTIL_FS_WINDOWS_API
#else
#define UTIL_FS_POSIX_API
#endif

#ifdef UTIL_FS_WINDOWS_API
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if (defined(_MSC_VER) && _MSC_VER >= 1600) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
#define UTIL_FS_OPEN(e, f, path, mode) errno_t e = fopen_s(&f, path, mode)
#define UTIL_FS_C11_API
#else
#include <errno.h>
#define UTIL_FS_OPEN(e, f, path, mode) \
    f = fopen(path, mode);             \
    int e = errno
#endif


namespace util {
    class file_system {
    public:
        static const char DIRECTORY_SEPARATOR =
#ifdef _WIN32
            '\\';
#else
            '/';
#endif

        static const size_t MAX_PATH_LEN =
#if defined(MAX_PATH)
            MAX_PATH;
#elif defined(_MAX_PATH)
            _MAX_PATH;
#elif defined(PATH_MAX)
                PATH_MAX;
#else
                260;
#endif

        struct dir_opt_t {
            enum type {
                EN_DOT_ABSP = 0x0001, // 转换为绝对路径
                EN_DOT_SELF = 0x0002, // 包含.和..
                EN_DOT_RLNK = 0x0004, // 解析符号链接
                EN_DOT_RECU = 0x0010, // 对目录递归扫描而不是列举出出目录名

                EN_DOT_TDIR = 0x0100,  // 包含目录
                EN_DOT_TREG = 0x0200,  // 包含文件
                EN_DOT_TLNK = 0x0400,  // 包含符号链接
                EN_DOT_TSOCK = 0x0800, // 包含Unix Sock
                EN_DOT_TOTH = 0x1000,  // 其他类型

                EN_DOT_DAFAULT = 0xFF00, // 默认规则
            };
        };

    public:
        /**
         * @brief 获取文件内容
         * @param out [OUT] 输出变量
         * @param file_path [IN] 文件路径
         * @param is_binary [IN] 是否是二进制
         * @return 成功返回true
         */
        static bool get_file_content(std::string &out, const char *file_path, bool is_binary = false);

        /**
         * @brief 获取文件内容
         * @param out [OUT] 输出变量
         * @param path [IN] 路径
         * @param compact [IN] 是否精简路径（这个功能会尽量移除路径中的.和..）
         * @return 成功返回true
         */
        static bool split_path(std::vector<std::string> &out, const char *path, bool compact = false);

        /**
         * @brief 检查文件是否存在
         * @param file_path [IN] 文件路径
         * @return 存在且有权限返回true
         */
        static bool is_exist(const char *file_path);

        /**
         * @brief 创建目录
         * @param dir_path [IN] 目录路径
         * @param recursion [IN] 是否递归创建
         * @param mode [IN] 目录权限（Windows下会被忽略）
         * @return 创建成功返回true
         */
        static bool mkdir(const char *dir_path, bool recursion = false, int mode = 0);

        /**
         * @brief 获取当前运行目录
         * @return 当前运行目录
         */
        static std::string get_cwd();

        /**
         * @brief 获取绝对路径
         * @param dir_path 相对路径
         * @return 当前运行目录
         */
        static std::string get_abs_path(const char *dir_path);

        /**
         * @brief 移动或重命名文件/目录
         * @param from 原始路径
         * @param to 目标路径
         * @return 成功返回true
         */
        static bool rename(const char *from, const char *to);

        /**
         * @brief 移除文件/目录
         * @param path 路径
         * @return 成功返回true
         */
        static bool remove(const char *path);

        /**
         * @brief 打开一个临时文件
         * @return 临时文件
         */
        static FILE *open_tmp_file();

        /**
         * @brief 列举目录下所有文件
         * @param dir_path 目录路径
         * @param out 录下所有文件路径
         * @return 成功返回0，错误返回错误码(不同平台错误码不同)
         */
        static int scan_dir(const char *dir_path, std::list<std::string> &out, int options = dir_opt_t::EN_DOT_DAFAULT);

        /**
         * @brief 判断是否是绝对路径
         * @param dir_path 目录路径
         * @return 是绝对路径返回true
         */
        static bool is_abs_path(const char *dir_path);
    };
}


#endif //_UTIL_COMMON__FILESYSTEM_H
