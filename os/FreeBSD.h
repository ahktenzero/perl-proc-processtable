#include <sys/types.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __FreeBSD_version >= 600000
/* in FreeBSD 6.x the format of /proc/$pid/status changed */
#define PROCFS_FREEBSD_6
#else
#undef  PROCFS_FREEBSD_6
#endif

struct procstat {
  char comm[MAXCOMLEN+1];
  int pid;
  int ppid;
  int pgid;
  int sid;
#ifdef PROCFS_FREEBSD_6
  char ttydev[SPECNAMELEN];
#else
  int tdev_maj;
  int tdev_min;
#endif
  char flags[256]; /* XXX */
  int start;
  int start_mic;
  int utime;
  int utime_mic;
  int stime;
  int stime_mic;
  char wchan[256]; /* XXX */
  int euid;
  int ruid;
  int rgid;
  int egid;
  char groups[256]; /* XXX */
};

/* We need to pass in a cap for ignore, lower for store on object */
/* We can just lc these! */
static char Defaultformat[] = "iiiiiissssssiisssiSI";

/* Mapping of field to type */
static char* Fields[] = {
  "uid",
#define F_UID 0 

  "gid",
#define F_GID 1

  "pid",
#define F_PID 2

  "ppid",
#define F_PPID 3

  "pgrp",
#define F_PGRP 4

  "sess",
#define F_SESS 5

  "flags",
#define F_FLAGS 6

  "utime",
#define F_UTIME 7

  "stime",
#define F_STIME 8

  "time",
#define F_TIME 9

  "wchan",
#define F_WCHAN 10

  "start",
#define F_START 11

  "euid",
#define F_EUID 12

  "egid",
#define F_EGID 13

  "fname",
#define F_FNAME 14

  "state",
#define F_STATE 15

  "ttydev",
#define F_TTYDEV 16

  "ttynum",
#define F_TTYNUM 17

  "cmndline",
#define F_CMNDLINE 18

  "priority"
#define F_PRIORITY 19
  
#define F_LASTFIELD 19
};

