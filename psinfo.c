#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  struct psinfo p;
  int show_all = 1;
  int specific_pid = -1;

  // Si se pasa un PID como argumento, mostrar solo ese proceso
  if(argc == 2) {
    specific_pid = atoi(argv[1]);
    show_all = 0;
  }

  printf(1, "\n================================================\n");
  printf(1, "          INFORMACIÓN DE PROCESOS XV6           \n");
  printf(1, "================================================\n");

  // Mostrar información de un proceso específico
  if(!show_all && specific_pid > 0) {
    if(getprocinfo(specific_pid, &p) == 0) {
      printf(1, "PID:             %d\n", p.pid);
      printf(1, "Nombre:          %s\n", p.name);
      printf(1, "Estado:          %s\n", p.state);
      printf(1, "PID Padre:       %d\n", p.ppid);
      printf(1, "Memoria:         %d bytes\n", p.sz);
    } else {
      printf(1, "Proceso %d no encontrado\n", specific_pid);
    }
  } else {
    // Mostrar tabla con todos los procesos
    printf(1, "PID  ESTADO     NOMBRE        MEMORIA  PADRE\n");
    printf(1, "---  ---------  ------------  -------  -----\n");

    int count = 0;

    for(int pid = 1; pid <= 100; pid++) {
      if(getprocinfo(pid, &p) == 0) {
        // Sin formato %-9s, solo valores simples
        printf(1, "%d    %s    %s    %d    %d\n",
               p.pid, p.state, p.name, p.sz, p.ppid);
        count++;
      }
    }

    printf(1, "\nTotal de procesos: %d\n", count);
  }

  printf(1, "================================================\n\n");

  exit();
}

