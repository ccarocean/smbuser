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


#ifndef SMBUSER_SMB_H
#define SMBUSER_SMB_H


/** Add user to SMB and set passowrd.
 *
 * Will also set password of existing SMB user.
 *
 * @param username Username of the new or existing user.
 * @param password New password for the user.
 * @return 0 on success, -1 on failure
 */
int smbuseradd(const char *username, const char *password);


/** Delete SMB user.
 *
 * @param username Username of the user to remove from the SMB database.
 * @return 0 on success, -1 on failure
 */
int smbuserdel(const char *username);


#endif //SMBUSER_SMB_H
