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


#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"


bool alnum_only(const char *string)
{
    for (size_t i = 0; i < strlen(string); ++i)
    {
        if (!isalnum(string[i]))
        {
            return false;
        }
    }
    return true;
}


int raise_to_effective(void)
{
    if (setuid(geteuid()) == -1)
    {
        perror("setuid");
    }
    if (setgid(getegid()) == -1)
    {

    }
}
