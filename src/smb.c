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

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"
#include "common.h"
#include "smb.h"


int smbuseradd(const char *username, const char *password)
{
    // prevent semicolon attack
    if (!alnum_only(username))
    {
        return -1;
    }

    // create pipe to talk to smbpasswd
    int pipefd[2];
    if (pipe(pipefd) != 0)
    {
        perror("pipe");
        return -1;
    }

    // call smbpasswd
    pid_t pid;
    if ((pid = fork()) == 0)
    {
        // close write end of pipe
        close(pipefd[1]);

        // attach stdin to pipe
        if (dup2(pipefd[0], 0) == -1)
        {
            perror("dup2");
            close(pipefd[0]);
            return -1;
        }

        // throw away output
        int devnull = open("/dev/null", O_WRONLY);
        dup2(devnull, 1);
        dup2(devnull, 2);

        // exec smbpasswd
        if (execl(SMBPASSWD_PATH, SMBPASSWD_PATH, "-a", username, NULL))
        {
            close(pipefd[0]);
            perror("execl");
            return -1;
        }
    }

    // close read end of pipe
    close(pipefd[0]);

    // write smb password to smbpasswd (2 times)
    FILE *stream = fdopen(pipefd[1], "w");
    fprintf(stream, "%s\n", password);
    fprintf(stream, "%s\n", password);
    fclose(stream); // also closes file descriptor

    // wait for smbpasswd to finish
    int status;
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return -1;
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        return -1;
    }

    return 0;
}
