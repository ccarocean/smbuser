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


#ifndef SMBUSER_PAM_H
#define SMBUSER_PAM_H


/** Authenticate a user with PAM.
 *
 * @param username The username to authenticate against.
 * @return 0 on success, -1 on failure
 */
int authenticate(const char *username, const char *password);


#endif //SMBUSER_PAM_H
