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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include "config.h"
#include "common.h"
#include "pam.h"
#include "smb.h"


#define UNKNOWN_ERROR 1
#define INVALID_INPUT 2
#define AUTH_ERROR 3
#define SMB_PASSWORD_ERROR 4


void print_usage(FILE *stream)
{
    fputs("Usage: smbuseradd [options] [USERNAME]\n\n"
          "Adds user with USERNAME to SMB database and sets their SMB password.  If the\n"
          "user is already in the SMB database their password will be changed.  The user\n"
          "must be a valid UNIX user on the system and smbuseradd will ask for the user's\n"
          "UNIX password, even when run as root.\n\n"
          "Options:\n"
          "  -h, --help           print this message and exit\n\n", stream);
}


int main(int argc, char *argv[])
{
    int retval = 0;
    char buf[1024];
    char *username = NULL;
    char *unix_password = NULL;
    char *smb_password = NULL;

    if (argc != 2)
    {
        print_usage(stderr);
        return INVALID_INPUT;
    }

    if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0))
    {
        print_usage(stderr);
        return 0;
    }

    if (!alnum_only(argv[1]))
    {
        fputs("Invalid username.\n", stderr);
        return INVALID_INPUT;
    }

    // get username and password
    username = strdup(argv[1]);
    snprintf(buf, sizeof(buf), "UNIX Password for %s: ", username);
    unix_password = strdup(getpass(buf));

    // verify the user
    if (authenticate(username, unix_password) != 0)
    {
        fputs("Invalid username or password.\n", stderr);
        retval = AUTH_ERROR;
        goto final;
    }

    // get new smb password
    smb_password = strdup(getpass("New SMB password: "));
    if (strcmp(smb_password, getpass("Re-enter SMB password: ")) != 0)
    {
        fputs("Passwords do not match.\n", stderr);
        retval = SMB_PASSWORD_ERROR;
        goto final;
    }
    if (strlen(smb_password) < SMB_MIN_PASSWD_LENGTH)
    {
        fprintf(stderr, "SMB password must be at least %d characters.\n",
                SMB_MIN_PASSWD_LENGTH);
        retval = SMB_PASSWORD_ERROR;
        goto final;
    }
    if (strlen(smb_password) > 16)
    {
        fputs("SMB password cannot exceed 16 characters.\n", stderr);
        retval = SMB_PASSWORD_ERROR;
        goto final;
    }

    // copy effective user and group id to real user and group id, otherwise
    // smbpasswd will throw away permissions
    if (raise_to_effective() != 0)
    {
        return -1;
    }

    // add user and set smb password
    if (smbuseradd(username, smb_password) != 0)
    {
        fprintf(stderr, "Failed to set password for user %s.\n", username);
        retval = SMB_PASSWORD_ERROR;
        goto final;
    }

    // cleanup
final:
    if (username != NULL)
    {
        free(username);
    }
    if (unix_password != NULL)
    {
        free(unix_password);
    }
    if (smb_password != NULL)
    {
        free(smb_password);
    }
    return retval;
}
