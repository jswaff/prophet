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
    {"PAWN_VAL", &pawn_val},
    {"KNIGHT_VAL", &knight_val},
    {"BISHOP_VAL", &bishop_val},
    {"ROOK_VAL", &rook_val},
    {"QUEEN_VAL", &queen_val},
    {"BISHOP_PAIR", &bishop_pair}, 

    {"KING_SAFETY_PAWN_ONE_AWAY", &king_safety_pawn_one_away},
    {"KING_SAFETY_WING_PAWN_ONE_AWAY", &king_safety_wing_pawn_one_away}, 
    {"KING_SAFETY_PAWN_TWO_AWAY", &king_safety_pawn_two_away}, 
    {"KING_SAFETY_WING_PAWN_TWO_AWAY", &king_safety_wing_pawn_two_away}, 
    {"KING_SAFETY_PAWN_FAR_AWAY", &king_safety_pawn_far_away}, 
    {"KING_SAFETY_WING_PAWN_FAR_AWAY", &king_safety_wing_pawn_far_away}, 
    {"KING_SAFETY_MIDDLE_OPEN_FILE", &king_safety_middle_open_file},

    {"PASSED_PAWN", &passed_pawn},
    {"ISOLATED_PAWN", &isolated_pawn},
    {"DOUBLED_PAWN", &doubled_pawn}

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

    while ((read = getline(&line, &len, fp)) != -1)
    {
        out(stdout, "%s", line);
        if (strchr(line, '=')) 
        {
            char* key = strtok(line, "=");
            char* val = strtok(NULL, "=");
            out(stdout, "key: %s val:%s\n", key, val);

            /* map the key to a variable and set the value */
            for (int i=0; i<nprops; i++)
            {
                struct eval_weight_table_entry te = eval_weight_table[i];
                if (!strncmp(key, te.property_name, strlen(key)))
                {
                    int32_t* valptr = te.val_ptr;
                    *valptr = atoi(val);
                    break;
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
