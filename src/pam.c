// A simple tool allowing users to set or delete their Samba password.
// Copyright (C) <2018>  Michael R. Shannon <mrshannon.aerospace@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <security/pam_appl.h>

#include "pam.h"


static int pam_conversation(
        int num_msg, const struct pam_message **msg,
        struct pam_response **resp, void *appdata_ptr)
{
    // check number of messages
    if (num_msg <= 0 || num_msg > PAM_MAX_NUM_MSG)
    {
        return PAM_CONV_ERR;
    }

    // allocate response buffer
    struct pam_response *responses = NULL;
    if ((responses = calloc((size_t) num_msg, sizeof *responses)) == NULL)
    {
        return PAM_BUF_ERR;
    }

    // loop through each response
    for (int i = 0; i < num_msg; ++i)
    {
        responses[i].resp = NULL;

        // resp_retcode is unused and must be 0
        responses[i].resp_retcode = 0;

        switch (msg[i]->msg_style)
        {
            case PAM_PROMPT_ECHO_OFF:  // get password
                if (appdata_ptr == NULL)
                {
                    responses[i].resp = strdup(getpass(msg[i]->msg));
                }
                else
                {
                    responses[i].resp = strdup(appdata_ptr);
                }
                if (responses[i].resp == NULL)
                {
                    goto fail;
                }
                break;

            case PAM_PROMPT_ECHO_ON:  // no username support
                goto fail;

            case PAM_ERROR_MSG:  // display error
                fputs(msg[i]->msg, stderr);
                // ensure a newline is printed
                if (strlen(msg[i]->msg) > 0 &&
                    msg[i]->msg[strlen(msg[i]->msg) - 1] != '\n')
                {
                    fputc('\n', stderr);
                }
                break;

            case PAM_TEXT_INFO:  // display info
                fputs(msg[i]->msg, stdout);
                // ensure a newline is printed
                if (strlen(msg[i]->msg) > 0 &&
                    msg[i]->msg[strlen(msg[i]->msg) - 1] != '\n')
                {
                    fputc('\n', stdout);
                }
                break;

            default:  // impossible to reach
                goto fail;
        }

    }

    // return responses
    *resp = responses;
    return PAM_SUCCESS;

fail:
    // cleanup messages
    for (int i = 0; i < num_msg; ++i)
    {
        if (responses[i].resp != NULL)
        {
            // clear response buffer
            memset(responses[i].resp, 0, strlen(responses[i].resp));
            // free response buffer
            free(responses[i].resp);
        }
    }
    // cleanup responses buffer
    memset(responses, 0, num_msg * sizeof *responses);
    *resp = NULL;
    return PAM_CONV_ERR;
}


int authenticate(const char *username, const char *password)
{
    int authenticated = -1;
    pam_handle_t *pamh = NULL;
    int pam_error = 0;

    // start PAM conversation
    static struct pam_conv conv = {
            &pam_conversation,
            NULL
    };
    conv.appdata_ptr = (void *)password;
    pam_error = pam_start("login", username, &conv, &pamh);
    if (pam_error != PAM_SUCCESS)
    {
        return -1;
    }

    // authenticate user
    pam_error = pam_authenticate(pamh, 0);
    authenticated = (pam_error == PAM_SUCCESS) ? 0 : -1;

    // close PAM conversation
    if (pam_end(pamh, pam_error) != PAM_SUCCESS)
    {
        pamh = NULL;
        return -1;
    }

    // return result
    return authenticated;
}