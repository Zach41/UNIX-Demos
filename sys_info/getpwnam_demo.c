#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/*
  in linux, passwd is defined as
  struct passwd {
      char *pw_name;
      char *pw_passwd;
      uid_t pw_uid;
      gid_t pw_gid;
      char *pw_gecos;
      char *pw_dir;
      char *pw_shell;
  }

 */
struct passwd *getpwdname_(const char *name) {
    struct passwd *ptr;

    setpwent();
    while ((ptr = getpwent()) != NULL) {
	if (strcmp(ptr -> pw_name, name) == 0)
	    break;	
    }
    endpwent();
    return ptr;
}

int main(void) {
    char *user = "zach41";

    struct passwd *ptr = getpwdname_(user);

    printf("name: %s\npasswd: %s\nuid: %d\ngid: %d\necos: %s\nhome: %s\nshell: %s\n",
	   ptr -> pw_name, ptr -> pw_passwd, ptr -> pw_uid, ptr -> pw_gid, ptr -> pw_gecos,
	   ptr -> pw_dir, ptr -> pw_shell);

    return 0;
}
