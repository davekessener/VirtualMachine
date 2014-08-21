#include "FileSystem.h"
#include <boost/filesystem.hpp>

size_t FileSystem::getFileSize(const std::string& fn)
try
{
	return boost::filesystem::file_size(fn);
}
catch(const boost::filesystem::filesystem_error& e)
{
	throw std::string(e.what());
}

void FileSystem::copyFile(const std::string& fn1, const std::string& fn2)
try
{
	boost::filesystem::copy_file(fn1, fn2);
}
catch(const boost::filesystem::filesystem_error& e)
{
	throw std::string(e.what());
}

void FileSystem::removeFile(const std::string& fn)
try
{
	boost::filesystem::remove(fn);
}
catch(const boost::filesystem::filesystem_error& e)
{
	throw std::string(e.what());
}

std::string FileSystem::getPath(const std::string& p)
{
	if(p.empty()) return "";
	size_t l = p.find_last_of('/');
	if(l == std::string::npos) return "";
	if(l == 0) return "/";
	return p.substr(0, l) + "/";
}

std::string FileSystem::getFile(const std::string& p)
{
	return p.empty() ? "" : p.substr(p.find_last_of('/') + 1);
}

std::string FileSystem::getFileName(const std::string& p)
{
	if(p.empty()) return "";
	std::string fn(getFile(p));
	size_t l = fn.find_last_of('.');
	return l == std::string::npos ? fn : fn.substr(0, l);
}

std::string FileSystem::getFileExtension(const std::string& p)
{
	return p.empty() ? "" : p.substr(p.find_last_of('.') + 1);
}

