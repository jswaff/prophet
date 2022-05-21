#include <prophet/commandline.h>
#include <prophet/eval.h>
#include <prophet/error_codes.h>
#include <prophet/util/output.h>

#include <stdlib.h>
#include <string.h>

/* structure mapping a property to a variable */
struct eval_weight_table_entry
{
    const char *property_name;
    int32_t* val_ptr;
};

/* a table of property->variable pointer mappings */
struct eval_weight_table_entry eval_weight_table[] = {
    {"KNIGHT_VAL", &knight_val}
};

/**
 * \brief Load eval properties
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int commandline_load_properties(const char* props_file)
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(props_file, "r");
    if (fp == NULL)
    {
        return P4_ERROR_CMDLINE_PROPERTIES_FILE_OPEN_FAILURE;
    }

    int nprops = sizeof(eval_weight_table) / sizeof(struct eval_weight_table_entry);
    out(stdout, "nprops: %d\n", nprops);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        out(stdout, "%s", line);
        if (strchr(line, '=')) 
        {
            char* key = strtok(line, "=");
            char* val = strtok(NULL, "=");
            out(stdout, "key: %s val:%s\n", key, val);

            /* map the key */
            for (int i=0; i<nprops; i++)
            {
                struct eval_weight_table_entry te = eval_weight_table[i];
                if (!strncmp(key, te.property_name, strlen(key)))
                {
                    out(stdout, "**** mapping KNIGHT_VAL to %s\n", val);
                    int32_t* valptr = te.val_ptr;
                    *valptr = atoi(val);
                }
            }

        }
    }

    fclose(fp);

    if (line)
    {
        free(line);
    }

    return 0;
}
