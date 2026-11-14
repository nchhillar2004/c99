#ifndef c99_h
#define c99_h

#include <stdbool.h>
#include <stddef.h>

#define COMPILER_VERSION "v1.0.0"

typedef enum {
	MODE_PREPROCESS,
	MODE_ASSEMBLY,
	MODE_OBJECT,
	MODE_EXECUTABLE
} CompilationMode;

typedef struct {
	bool save_temps;
	bool time;
	bool warnings;
	bool debug;

	CompilationMode mode;

	const char *output;

	const char **source_files;
	size_t source_count;
	size_t source_capacity;

	const char **include_dirs;
	size_t include_count;
	size_t include_capacity;
} CompilerConfig;

#endif
