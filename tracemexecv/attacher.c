#include <stdio.h>

#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/wait.h>

#ifdef __x86_64__
#define SYSCALL_NUMBER  (8 * ORIG_RAX)
#define SYSCALL_RETCODE (8 * RAX)
#else
#define SYSCALL_NUMBER  (4 * ORIG_EAX)
#define SYSCALL_RETCODE (4 * EAX)
#endif

int main(int argc, char **argv) {
  pid_t child;
  int status;
  long syscall_number, syscall_retcode;

  child = fork();

  if(child == 0) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execv(argv[1], argv + 1);
  }
  else {
    do {
      pid_t cid = wait(&status);
      syscall_number = ptrace(PTRACE_PEEKUSER, cid, SYSCALL_NUMBER, NULL);
      syscall_retcode = ptrace(PTRACE_PEEKUSER, cid, SYSCALL_RETCODE, NULL);
      printf("The child %i made a system call %ld %ld\n",
             cid, syscall_number, syscall_retcode);
      ptrace(PTRACE_SYSCALL, cid, NULL, NULL);
    } while(!WIFEXITED(status));
  }

  return 0;
}
