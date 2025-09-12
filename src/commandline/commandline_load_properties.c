#include "commandline.h"

#include "prophet/error_codes.h"

#include "eval/eval_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* structure mapping a property to a variable */
struct eval_weight_table_entry {
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

    {"PASSED_PAWN_MG", &passed_pawn_mg[0]},
    {"PASSED_PAWN_EG", &passed_pawn_eg[0]},
    {"ISOLATED_PAWN_MG", &isolated_pawn_mg},
    {"ISOLATED_PAWN_EG", &isolated_pawn_eg},
    {"DOUBLED_PAWN_MG", &doubled_pawn_mg},
    {"DOUBLED_PAWN_EG", &doubled_pawn_eg},

    {"KNIGHT_TROPISM_MG", &knight_tropism_mg},
    {"KNIGHT_TROPISM_EG", &knight_tropism_eg},
    {"KNIGHT_OUTPOST", &knight_outpost[0]},
    {"KNIGHT_SUPPORTED_OUTPOST", &knight_supported_outpost[0]},

    {"BISHOP_MOBILITY_MG", &bishop_mobility_mg[0]},
    {"BISHOP_MOBILITY_EG", &bishop_mobility_eg[0]},
    {"BISHOP_TRAPPED", &bishop_trapped},

    {"ROOK_MOBILITY_MG", &rook_mobility_mg[0]},
    {"ROOK_MOBILITY_EG", &rook_mobility_eg[0]},
    {"ROOK_OPEN_FILE_MG", &rook_open_file_mg},
    {"ROOK_OPEN_FILE_SUPPORTED_MG", &rook_open_file_supported_mg},
    {"ROOK_OPEN_FILE_EG", &rook_open_file_eg},
    {"ROOK_OPEN_FILE_SUPPORTED_EG", &rook_open_file_supported_eg},
    {"ROOK_HALF_OPEN_FILE_MG", &rook_half_open_file_mg},
    {"ROOK_HALF_OPEN_FILE_SUPPORTED_MG", &rook_half_open_file_supported_mg},
    {"ROOK_HALF_OPEN_FILE_EG", &rook_half_open_file_eg},
    {"ROOK_HALF_OPEN_FILE_SUPPORTED_EG", &rook_half_open_file_supported_eg},

    {"QUEEN_MOBILITY_MG", &queen_mobility_mg[0]},
    {"QUEEN_MOBILITY_EG", &queen_mobility_eg[0]},

    {"MAJOR_ON_7TH_MG", &major_on_7th_mg},
    {"MAJOR_ON_7TH_EG", &major_on_7th_eg},
    {"CONNECTED_MAJORS_ON_7TH_MG", &connected_majors_on_7th_mg},
    {"CONNECTED_MAJORS_ON_7TH_EG", &connected_majors_on_7th_eg},

    {"PAWN_PST_MG", &pawn_pst_mg[0]},
    {"PAWN_PST_EG", &pawn_pst_eg[0]},

    {"KNIGHT_PST_MG", &knight_pst_mg[0]},
    {"KNIGHT_PST_EG", &knight_pst_eg[0]},

    {"BISHOP_PST_MG", &bishop_pst_mg[0]},
    {"BISHOP_PST_EG", &bishop_pst_eg[0]},

    {"ROOK_PST_MG", &rook_pst_mg[0]},
    {"ROOK_PST_EG", &rook_pst_eg[0]},

    {"QUEEN_PST_MG", &queen_pst_mg[0]},
    {"QUEEN_PST_EG", &queen_pst_eg[0]},

    {"KING_PST_MG", &king_pst_mg[0]},
    {"KING_PST_EG", &king_pst_eg[0]}
};


int commandline_load_properties(const char *props_file)
{
    FILE *fp;
    char *line = NULL;
    char *line_buffer = NULL;
    char *val_buffer = NULL;
    size_t len = 0;
    int read;

    fp = fopen(props_file, "r");
    if (fp == NULL) {
        return ERROR_CMDLINE_PROPERTIES_FILE_OPEN_FAILURE;
    }

    int nprops = sizeof(eval_weight_table) / sizeof(struct eval_weight_table_entry);

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strchr(line, '=')) {
            char* key = strtok_r(line, "=", &line_buffer);
            char* val = strtok_r(NULL, "=", &line_buffer);

            /* map the key to a variable and set the value */
            for (int i=0; i<nprops; i++) {
                struct eval_weight_table_entry te = eval_weight_table[i];
                if (!strncmp(key, te.property_name, strlen(key))) {
                    int32_t* valptr = te.val_ptr;
                    char* v = strtok_r(val, ",", &val_buffer);
                    int i = 0;
                    while (v) {
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

    if (line) {
        free(line);
    }

    return 0;
}
