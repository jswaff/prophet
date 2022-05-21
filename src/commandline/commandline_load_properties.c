#include <prophet/commandline.h>
#include <prophet/error_codes.h>
#include <prophet/util/output.h>

#include <stdlib.h>
#include <string.h>

/**
 * \brief Load eval properties
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int commandline_load_properties(const char* props_file)
{
	FILE* fp;
	char* line = NULL;
	char* p = NULL;
	char* key = NULL;
	char* val = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(props_file, "r");
	if (fp == NULL)
	{
		return 1;
	}

	while ((read = getline(&line, &len, fp)) != -1)
	{
		out(stdout, "%s", line);
		p = strchr(line, '=');
		if (p)
		{
			key = strtok(line, "=");
			val = strtok(NULL, "=");
			out(stdout, "key: %s val:%s\n", key, val);
		}
	}

	fclose(fp);

	if (line)
	{
		free(line);
	}

	return 0;
}
