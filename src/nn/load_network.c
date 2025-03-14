#include "nn_internal.h"

#include "prophet/error_codes.h"

#include <stdio.h>
#include <stdlib.h>


int load_neural_network(neural_network_t* nn, const char* weights_file)
{
    FILE* wf;
    wf = fopen(weights_file, "r");
    if (wf == NULL) {
        return P4_ERROR_CMDLINE_NN_FILE_OPEN_FAILURE;
    }

    char buff[255];
    int val;

    /* load W0 */
    for (int row=0;row<NN_SIZE_L1;row++) {
        for (int col=0;col<768;col++) {
            if (!fgets(buff, 255, wf) || sscanf(buff, "%d", &val) != 1) {
                fclose(wf);
                return P4_ERROR_CMDLINE_NN_FILE_PARSE_FAILURE;
            }
            nn->W0[col * NN_SIZE_L1 + row] = (int8_t)val; /* transposition */
        }
    }

    /* load B0 */
    for (int i=0;i<NN_SIZE_L1;i++) {
        if (!fgets(buff, 255, wf) || sscanf(buff, "%d", &val) != 1) {
            fclose(wf);
            return P4_ERROR_CMDLINE_NN_FILE_PARSE_FAILURE;
        }
        nn->B0[i] = (int8_t)val;
    }

    /* load W1 */
    for (int i=0;i<NN_SIZE_L1 * 2 * NN_SIZE_L2;i++) {
        if (!fgets(buff, 255, wf) || sscanf(buff, "%d", &val) != 1) {
            fclose(wf);
            return P4_ERROR_CMDLINE_NN_FILE_PARSE_FAILURE;
        }
        nn->W1[i] = (int8_t)val;
    }

    /* load B1 */
    for (int i=0;i<NN_SIZE_L2;i++) {
        if (!fgets(buff, 255, wf) || sscanf(buff, "%d", &val) != 1) {
            fclose(wf);
            return P4_ERROR_CMDLINE_NN_FILE_PARSE_FAILURE;
        }
        nn->B1[i] = (int8_t)val;
    }
    

    fclose(wf);

    return 0;
}
