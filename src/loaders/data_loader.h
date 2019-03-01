#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "memory/linear_allocator.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>

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
	DataLoader(const memory::Arena &arena);

	~DataLoader();

	int open(const char *file);

	int readSectionInfo(const char **name, int &rows, int &columns);

	int readRow(Value row[], int columns);

	int skipRows(int rows);

	void close();

private:
	int readLine();

	int parseInteger(int &val, int pos);

	int parseName(char *val, int size, int pos);

	std::FILE *m_file;
	char *m_line_buffer;
	char *m_section_name;
	char *m_format;
	memory::LinearAllocator m_allocator;
};
}  // namespace loader
#endif
