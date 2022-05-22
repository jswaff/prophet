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
    {"KNIGHT_KAUFMAN_ADJ", &knight_kaufman_adj},
    {"ROOK_KAUFMAN_ADJ", &rook_kaufman_adj},


    {"KING_SAFETY_PAWN_ONE_AWAY", &king_safety_pawn_one_away},
    {"KING_SAFETY_WING_PAWN_ONE_AWAY", &king_safety_wing_pawn_one_away}, 
    {"KING_SAFETY_PAWN_TWO_AWAY", &king_safety_pawn_two_away}, 
    {"KING_SAFETY_WING_PAWN_TWO_AWAY", &king_safety_wing_pawn_two_away}, 
    {"KING_SAFETY_PAWN_FAR_AWAY", &king_safety_pawn_far_away}, 
    {"KING_SAFETY_WING_PAWN_FAR_AWAY", &king_safety_wing_pawn_far_away}, 
    {"KING_SAFETY_MIDDLE_OPEN_FILE", &king_safety_middle_open_file},

    {"PASSED_PAWN", &passed_pawn},
    {"ISOLATED_PAWN", &isolated_pawn},
    {"DOUBLED_PAWN", &doubled_pawn},

    {"KNIGHT_TROPISM", &knight_tropism},

    {"BISHOP_MOBILITY", &bishop_mobility},
    {"BISHOP_ENDGAME_MOBILITY", &bishop_endgame_mobility},

    {"ROOK_OPEN_FILE", &rook_open_file},
    {"ROOK_HALF_OPEN_FILE", &rook_half_open_file},

    {"QUEEN_MOBILITY", &queen_mobility},
    {"QUEEN_ENDGAME_MOBILITY", &queen_endgame_mobility},

    {"MAJOR_ON_7TH", &major_on_7th},
    {"CONNECTED_MAJORS_ON_7TH", &connected_majors_on_7th},

    {"PAWN_PST", &pawn_pst[0]},
    {"PAWN_ENDGAME_PST", &pawn_endgame_pst[0]},

    {"KNIGHT_PST", &knight_pst[0]},
    {"KNIGHT_ENDGAME_PST", &knight_endgame_pst[0]}
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
    char* line_buffer = NULL;
    char* val_buffer = NULL;
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
            char* key = strtok_r(line, "=", &line_buffer);
            char* val = strtok_r(NULL, "=", &line_buffer);
            out(stdout, "key: %s val:%s\n", key, val);

            /* map the key to a variable and set the value */
            for (int i=0; i<nprops; i++)
            {
                struct eval_weight_table_entry te = eval_weight_table[i];
                if (!strncmp(key, te.property_name, strlen(key)))
                {
                    int32_t* valptr = te.val_ptr;
                    char* v = strtok_r(val, ",", &val_buffer);
                    int i = 0;
                    while (v)
                    {
                        *(valptr+i) = atoi(v);
                        v = strtok_r(NULL, ",", &val_buffer);
                        ++i;
                    }
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
