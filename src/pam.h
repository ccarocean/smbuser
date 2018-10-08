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

#include <stdbool.h>

#ifndef SMBUSER_PAM_H
#define SMBUSER_PAM_H


/** Authenticate a user with PAM.
 *
 * @param username The username to authenticate against.
 * @retval true The user has been authenticated.
 * @retval false The user has not been authenticated,
 *               errors can cause this result as well.
 */
bool authenticate(const char *username, const char *password);


#endif //SMBUSER_PAM_H
