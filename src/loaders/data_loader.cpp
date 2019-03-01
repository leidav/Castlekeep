#include "data_loader.h"

#include <cstdio>

namespace loader
{
DataLoader::DataLoader(const memory::Arena &arena)
    : m_file(nullptr), m_allocator(arena)
{
	m_line_buffer = reinterpret_cast<char *>(m_allocator.allocate(1024));
	m_section_name = reinterpret_cast<char *>(m_allocator.allocate(256));
	m_format = reinterpret_cast<char *>(m_allocator.allocate(32));
}

DataLoader::~DataLoader()
{
	close();
}

int DataLoader::open(const char *file)
{
	m_file = std::fopen(file, "r");
	if (m_file == nullptr) {
		return -1;
	}
	return 0;
}

int DataLoader::readSectionInfo(const char **name, int &rows, int &columns)
{
	if (readLine() != 0) {
		return -1;
	}
	int pos = 0;
	if (m_line_buffer[pos] != '[') {
		return -1;
	}
	pos++;
	int len = parseName(m_section_name, 256, 1);
	if (len == 0) {
		return -1;
	}
	pos += len;
	if (m_line_buffer[pos] != ',') {
		return -1;
	}
	pos++;
	int row_count = 0;
	len = parseInteger(row_count, pos);
	if (len == 0) {
		return -1;
	}
	pos += len;
	if (m_line_buffer[pos] != ',') {
		return -1;
	}
	pos++;
	int column_count = 0;
	len = parseInteger(column_count, pos);
	if (len == 0) {
		return -1;
	}
	pos += len;
	if (m_line_buffer[pos] != ',') {
		return -1;
	}
	pos++;
	for (int i = 0; i < column_count; i++) {
		char c = m_line_buffer[pos];
		if (c == 0) {
			return -1;
		}
		if ((c != 'i') && (c != 's')) {
			return -1;
		}
		m_format[i] = c;
		pos++;
		c = m_line_buffer[pos];
		if (i < (column_count - 1)) {
			if (c != ',') {
				return -1;
			}
			pos++;
		}
	}
	if (m_line_buffer[pos] != ']') {
		return -1;
	}

	*name = m_section_name;
	rows = row_count;
	columns = column_count;
	return 0;
}

int DataLoader::readRow(Value row[], int columns)
{
	if (readLine() != 0) {
		return -1;
	}
	int pos = 0;
	for (int i = 0; i < columns; i++) {
		int len = 0;
		if (m_format[i] == 'i') {
			int val = 0;
			len = parseInteger(val, pos);
			if (len == 0) {
				return 0;
			}
			row[i].type = Type::INTEGER;
			row[i].number = val;
		} else {
		}
		pos += len;
		if (i < (columns - 1)) {
			if (m_line_buffer[pos] != ',') {
				return 0;
			}
			pos++;
		}
	}
	if ((m_line_buffer[pos] != '\n') && (m_line_buffer[pos] != 0)) {
		return 0;
	}
	return columns;
}

int DataLoader::skipRows(int rows)
{
	for (int i = 0; i < rows; i++) {
		if (readLine() != 0) {
			return -1;
		}
	}
	return 0;
}

void DataLoader::close()
{
	if (m_file != nullptr) {
		std::fclose(m_file);
	}
	m_file = nullptr;
}

int DataLoader::readLine()
{
	while (true) {
		if (std::fgets(m_line_buffer, 1024, m_file) == nullptr) {
			return -1;
		}
		if ((m_line_buffer[0] != '#') && (m_line_buffer[0] != '\n')) {
			break;
		}
	}
	return 0;
}

int DataLoader::parseInteger(int &val, int pos)
{
	char c;
	int len = 0;
	val = 0;
	while ((c = m_line_buffer[pos]) != 0) {
		if (!((c >= '0') && (c <= '9'))) {
			return len;
		}
		val *= 10;
		val += (c - '0');
		pos++;
		len++;
	}
	return len;
}

int DataLoader::parseName(char *str, int size, int pos)
{
	char c;
	int len = 0;
	while (((c = m_line_buffer[pos]) != 0) && len < size) {
		if (!(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')))) {
			break;
		}
		str[len] = c;
		pos++;
		len++;
	}
	str[len] = 0;
	return len;
}

}  // namespace loader
