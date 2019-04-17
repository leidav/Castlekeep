#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "memory/linear_allocator.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include "file.h"

namespace castlekeep
{
namespace loader
{
enum class Type { INTEGER, STRING };
struct Value {
	Type type;
	union {
		int number;
		const char *string;
	};
};

class DataLoader
{
public:
	DataLoader(const memory::Arena &arena, File &file);

	~DataLoader();

	int begin(const char *type);

	int readSectionInfo(const char **name, int &rows, int &columns);

	int readRow(Value row[], int columns);

	int skipRows(int rows);

private:
	int readLine();

	int skipWhitespace(int pos);

	int parseInteger(int &val, int pos);

	int parseName(char *val, int size, int pos);

	File &m_file;
	char *m_line_buffer;
	char *m_section_name;
	char *m_format;
	memory::LinearAllocator m_allocator;
};
}  // namespace loader
}  // namespace castlekeep
#endif
