#ifndef TELL_WAIT_H
#define TELL_WAIT_H

void _tell_wait();
void _tell_parent();
void _tell_child();
void _wait_parent();
void _wait_child();

#define TELL_WAIT()         _tell_wait()
#define TELL_PARENT(pid)    _tell_parent()
#define TELL_CHILD(pid)     _tell_child()
#define WAIT_PARENT()       _wait_parent()
#define WAIT_CHILD()        _wait_child()

#endif /* TELL_WAIT_H */
