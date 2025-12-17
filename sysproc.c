#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
// Syscall para activar/desactivar tracing
extern int syscall_tracing;

int
sys_trace(void)
{
  int n;

  // Obtener argumento (0 o 1)
  if(argint(0, &n) < 0)
    return -1;

  // Validar que sea 0 o 1
  if(n != 0 && n != 1) {
    return -1;
  }
  // Activar/desactivar tracing
  syscall_tracing = n;
  return 0;
}

// Syscall para obtener número de procesos activos

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int
sys_getnproc(void)
{
  struct proc *p;
  int count = 0;

  acquire(&ptable.lock);

  // Contar procesos que no están UNUSED
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->state != UNUSED) {
      count++;
    }
  }
  release(&ptable.lock);
  return count;
}

// Syscall para obtener información de un proceso
int
sys_getprocinfo(void)
{
  int pid;
  struct psinfo *user_info;
  struct proc *p;
  int i;

  // Obtener parámetros
  if(argint(0, &pid) < 0 || 
     argptr(1, (char**)&user_info, sizeof(struct psinfo)) < 0) {
    return -1;
  }

  acquire(&ptable.lock);

  // Buscar proceso con el PID
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    if(p->pid == pid && p->state != UNUSED) {
      // 1. Copiar información básica
      user_info->pid = p->pid;
      user_info->ppid = (p->parent != 0) ? p->parent->pid : 1;
      user_info->sz = p->sz;

      const char* state_str;
      switch(p->state) {
        case UNUSED:    state_str = "UNUSED"; break;
        case EMBRYO:    state_str = "EMBRYO"; break;
        case SLEEPING:  state_str = "SLEEPING"; break;
        case RUNNABLE:  state_str = "RUNNABLE"; break;
        case RUNNING:   state_str = "RUNNING"; break;
        case ZOMBIE:    state_str = "ZOMBIE"; break;
        default:        state_str = "UNKNOWN"; break;
      }
      // Copiar estado carácter por carácter
      for(i = 0; i < 15 && state_str[i] != '\0'; i++) {
        user_info->state[i] = state_str[i];
      }
      user_info->state[i] = '\0';

      // 3. Nombre (copiar manualmente)
      for(i = 0; i < 15 && p->name[i] != '\0'; i++) {
        user_info->name[i] = p->name[i];
      }
      user_info->name[i] = '\0';
      release(&ptable.lock);
      return 0;  // Éxito
    }
  }

  release(&ptable.lock);
  return -1;  // No encontrado
}
// Syscall para obtener número de cambios de contexto
extern int context_switches;

int
sys_getcontextsw(void)
{
  return context_switches;
}
