#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  if(argc != 2) {
    printf(2, "Uso: strace <on|off>\n");
    exit();
  }
  
  if(strcmp(argv[1], "on") == 0) {
    if(trace(1) < 0) {
      printf(2, "Error: No se pudo activar\n");
      exit();
    }
    printf(1, "Syscall tracing: ON\n");
  }
  else if(strcmp(argv[1], "off") == 0) {
    if(trace(0) < 0) {
      printf(2, "Error: No se pudo desactivar\n");
      exit();
    }
    printf(1, "Syscall tracing: OFF\n");
  }
  else {
    printf(2, "Uso: strace <on|off>\n");
    exit();
  }
  
  exit();
}
