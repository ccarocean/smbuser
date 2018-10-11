smbuser
=======

Allows user's to manage their own SMB accounts (assuming they have a valid UNIX
login on the machine).

The only requirements is a POSIX system that is using PAM for login
authentication.


Install
-------

Before building you will need the following:

* C compiler, such as GCC or Clang.
* `libpam`, specifically the development version if your distribution separates
  out development files.

Now, to build `smbuser simply run:

```
$ mkdir build && cd build
$ cmake .. && make
# make install
```

This will install `smbuseradd` and `smbuserdel` into `/usr/local/bin`.

The minimum SMB password length and the location of the `smbpasswd` command can
be changed by editing the `src/config.h` file.


Usage
-----

`smbuser` consists of two separate commands:

* `smbuseradd` - adds a UNIX user to the SMB database, can also change password
* `smbuserdel` - remove a user from the SMB database

Both of these require the UNIX password of the user to be modified.

For example, to add the UNIX user `nick` to the SMB database (or to change
`nick`'s password if is already in the SMB database):

```
$ smbuseradd nick
UNIX Password for nick: ********
New SMB password: ********
Re-enter SMB password: ********
```

Even when run as root `smbuseradd` (and `smbuserdel`) require the user's UNIX
password be entered before any modification of the SMB database is performed. 
If you need to modify a user as root use the `smbpasswd` command that came with
Samba.

To remove `nick` from the SMB database:

```
$ smbuserdel nick
UNIX Password for nick: ********
```

