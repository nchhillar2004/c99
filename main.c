#include "c99.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

CompilerConfig config;

void config_init()
{
	config.save_temps = false;
	config.time = false;
	config.warnings = false;
	config.debug = false;
	config.mode = MODE_EXECUTABLE;
	config.output = "a.out";
	config.source_count = 0;
	config.source_capacity = 8;
	config.include_count = 0;
	config.include_capacity = 8;
	config.source_files = malloc(config.source_capacity * sizeof(char *));
	config.include_dirs = malloc(config.include_capacity * sizeof(char *));
}

char *compilation_mode_to_str(CompilationMode mode)
{
	switch (mode) {
		case MODE_PREPROCESS:
			return "preprocessing";
		case MODE_ASSEMBLY:
			return "assembly";
		case MODE_OBJECT:
			return "object";
		case MODE_EXECUTABLE:
			return "executable";
	}
	return "executable";
}

char *bool_to_str(bool b)
{
	return b ? "true" : "false";
}

void config_destroy()
{
	free(config.source_files);
	free(config.include_dirs);

	config.source_files = NULL;
	config.include_dirs = NULL;
	config.source_count = 0;
	config.source_capacity = 0;
	config.include_count = 0;
	config.include_capacity = 0;
}

void print_help()
{
	printf("Usage: c99 [options] file...\n");
	printf("Options:\n");
	printf("  --help            Display this information\n");
	printf("  --version         Display compiler version information.\n");
	printf("  -save-temps       Do not delete intermediate files.\n");
	printf("  -time             Time the execution of each subprocess.\n");
	printf("  -I <directory>    Add header search directory.\n");
	printf("  -E                Preprocess only; do not compile, assemble or link.\n");
	printf("  -S                Compile only; do not assemble or link.\n");
	printf("  -c                Compile and assemble, but do not link.\n");
	printf("  -o <file>         Place the output into <file>.\n");
	printf("  -Wall             Display all warnings.\n");
	printf("  -g                Generate debugging information.\n");
}

void print_version()
{
	printf("c99 %s 2026\n", COMPILER_VERSION);
	printf("This is a free and open-source software;\n");
}

char *str_lower(const char *str)
{
	if (!str) return NULL;

	size_t len = strlen(str);

	char *res = malloc(len + 1);

	if (!res) return NULL;

	for (size_t i = 0; str[i] != '\0'; i++)
		res[i] = (char)tolower((unsigned char)str[i]);

	res[len] = '\0';

	return res;
}

bool str_starts_with(const char *str, const char *prefix)
{
	if (!str || !prefix) return false;
	return strncmp(str, prefix, strlen(prefix)) == 0;
}

bool str_ends_with(const char *str, const char *suffix)
{
	if (!str || !suffix) return false;

	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);

	return str_len >= suffix_len && strcmp(str + str_len - suffix_len, suffix) == 0;
}

bool is_c99_source_file(const char *filepath)
{
	char *path = str_lower(filepath);
	if (!str_ends_with(path, ".c") && !str_ends_with(path, ".c99")) {
		free(path);
		return false;
	}
	free(path);
	FILE *file;
	if ((file = fopen(filepath, "r"))) {
		fclose(file);
		return true;
	}
	printf("c99: error: file '%s' does not exist.\n", filepath);
	return false;
}

void print_compiler_config()
{
	printf("Configuration:\n");
	printf("  Save temps: %s\n", bool_to_str(config.save_temps));
	printf("  Time:       %s\n", bool_to_str(config.time));
	printf("  Warnings:   %s\n", bool_to_str(config.warnings));
	printf("  Debug:      %s\n", bool_to_str(config.debug));
	printf("  Compilation Mode: %s\n", compilation_mode_to_str(config.mode));
	if (config.source_count == 1)
		printf("  Source file: %s;\n", config.source_files[0]);
	else if (config.source_count > 1) {
		printf("  Source files: ");
		for (size_t i = 0; i < config.source_count - 1; i++)
			printf("%s, ", config.source_files[i]);
		printf("%s;\n", config.source_files[config.source_count - 1]);
	}
	if (config.include_count == 1)
		printf("  Included dir: %s;\n", config.include_dirs[0]);
	else if (config.include_count > 1) {
		printf("  Included dirs: ");
		for (size_t i = 0; i < config.include_count - 1; i++)
			printf("%s, ", config.include_dirs[i]);
		printf("%s;\n", config.include_dirs[config.include_count - 1]);
	}
	printf("  Output file: %s\n", config.output);
}

int parse_cli_args(int argc, char *argv[])
{
	if (argc < 2) {
no_source_files:
		printf("c99: fatal error: no input file;\ncompilation terminated.\n");
		return -1;
	}
	for (int i = 1; i < argc; i++) {
		char *arg = argv[i];
		if (strcmp(arg, "--help") == 0) {
			print_help();
			return 0;
		}
		if (strcmp(arg, "--version") == 0) {
			print_version();
			return 0;
		}
		if (strcmp(arg, "-save-temps") == 0) {
			config.save_temps = true;
			continue;
		}
		if (strcmp(arg, "-time") == 0) {
			config.time = true;
			continue;
		}
		if (strcmp(arg, "-Wall") == 0) {
			config.warnings = true;
			continue;
		}
		if (strcmp(arg, "-g") == 0) {
			config.debug = true;
			continue;
		}
		if (strcmp(arg, "-I") == 0) {
			if (i + 1 >= argc || argv[i + 1][0] == '\0') {
				printf("error: missing argument: include directory path not found.\n");
				return -1;
			}
			if (config.include_capacity == config.include_count) {
				config.include_capacity *= 2;

				const char **tmp =
					realloc(config.include_dirs,
						config.include_capacity * sizeof(char *));
				if (!tmp) return -1;

				config.include_dirs = tmp;
			}
			config.include_dirs[config.include_count++] = argv[i + 1];
			i++;
			continue;
		}
		if (strcmp(arg, "-o") == 0) {
			if (i + 1 >= argc || argv[i + 1][0] == '\0') {
				printf("error: missing argument: output file name not found.\n");
				return -1;
			}
			config.output = argv[i + 1];
			i++;
			continue;
		}
		if (strcmp(arg, "-E") == 0) {
			config.mode = MODE_PREPROCESS;
			continue;
		} else if (strcmp(arg, "-S") == 0) {
			config.mode = MODE_ASSEMBLY;
			continue;
		} else if (strcmp(arg, "-c") == 0) {
			config.mode = MODE_OBJECT;
			continue;
		} else if (arg[0] == '-') {
			printf("c99: error: unrecognized command-line option '%s';\n", arg);
			return -1;
		} else if (is_c99_source_file(arg)) {
			if (config.source_capacity == config.source_count) {
				config.source_capacity *= 2;

				const char **tmp = realloc(config.source_files,
							   config.source_capacity * sizeof(char *));
				if (!tmp) return -1;

				config.source_files = tmp;
			}
			config.source_files[config.source_count++] = arg;
		}
	}

	if (config.debug) print_compiler_config();

	if (config.source_count == 0) goto no_source_files;

	return 0;
}

int main(int argc, char *argv[])
{
	config_init();

	if (parse_cli_args(argc, argv) == -1) {
		config_destroy();
		return 1;
	}

	// compiler_driver();

	config_destroy();
	return 0;
}
