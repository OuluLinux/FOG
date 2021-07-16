#include "Console.h"


int Console::Get(char* line, int size) {
	if (!line || size <= 0)
		return 0;
	size_t i;
	for (i = 0; i < size - 1; ++i) {
		int ch = fgetc(stdin);
		if (ch == '\n' || ch == EOF) {
			break;
		}
		line[i] = ch;
	}
	line[i] = '\0';
	return i;
}

void Console::Put(const char* msg) {
	int len = strlen(msg);
	if (len)
		fwrite(msg, len, 1, stdout);
}
