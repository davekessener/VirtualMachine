#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

class FileSystem
{
	public:
		static size_t getFileSize(const std::string&);
		static void copyFile(const std::string&, const std::string&);
		static void removeFile(const std::string&);
		static std::string getPath(const std::string&);
		static std::string getFile(const std::string&);
		static std::string getFileName(const std::string&);
		static std::string getFileExtension(const std::string&);
	private:
		FileSystem( ) = delete;
		~FileSystem( ) = delete;
		FileSystem& operator=(const FileSystem&) = delete;
		FileSystem(const FileSystem&) = delete;
};

#endif

