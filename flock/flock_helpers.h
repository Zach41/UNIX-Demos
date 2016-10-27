#ifndef FLOCK_HELPERS_H
#define FLOCK_HELPERS_H

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

int lock_test(int fd, int type, off_t offset, int whence, off_t len);

/* set & get lock */
#define read_lock(fd, offset, whence, len) \
    lock_reg(fd, F_SETLK, F_RDLCK, (offset), (whence), (len))

#define readw_lock(fd, offset, whence, len) \
    lock_reg(fd, F_SETLKW, F_RDLCK, (offset), (whence), (len))

#define write_lock(fd, offset, whence, len) \
    lock_reg(fd, F_SETLK, F_WRLCK, (offset), (whence), (len))

#define writew_lock(fd, offset, whence, len)			\
    lock_reg(fd, F_SETLKW, F_WRLCK, (offset), (whence), (len))

#define un_lock(fd, offset, whence, len) \
    lock_reg(fd, F_SETLK, F_UNLCK, (offset), (whence), (len))

/* test lock*/
#define is_read_lockable(fd, offset, whence, len) \
    lock_test(fd, F_RDLCK, (offset), (whence), (len))

#define is_write_lockable(fd, offset, whence, len) \
    lock_test(fd, F_WRLCK, (offset), (whence), (len))

#endif /* FLOCK_HELPERS_H */
