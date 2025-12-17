#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  // 1. Obtener tiempo del sistema (ticks)
  int ticks = uptime();
  
  // 2. Convertir ticks a tiempo
  int seconds = ticks / 100;   // 100 ticks = 1 segundo en xv6
  int minutes = seconds / 60;
  int hours   = minutes / 60;
  
  // 3. Obtener número de procesos
  int num_proc = getnproc();
  
  // 4. Obtener cambios de contexto
  int context_sw = getcontextsw();
  
  // 5. Mostrar información
  printf(1, "\n=========================================\n");
  printf(1, "        TIEMPO DE ACTIVIDAD XV6          \n");
  printf(1, "=========================================\n");
  printf(1, "Tiempo de actividad del sistema:\n");
  printf(1, "  • Ticks:     %d\n", ticks);
  printf(1, "  • Segundos:  %d\n", seconds);
  
  // Formato manual HH:MM:SS
  printf(1, "  • Tiempo:   %d:", hours);
  if((minutes % 60) < 10) printf(1, "0");
  printf(1, "%d:", minutes % 60);
  if((seconds % 60) < 10) printf(1, "0");
  printf(1, "%d\n", seconds % 60);
  
  printf(1, "-----------------------------------------\n");
  printf(1, "Información del sistema:\n");
  printf(1, "  • Procesos activos:     %d\n", num_proc);
  printf(1, "  • Cambios de contexto:  %d\n", context_sw);
  printf(1, "  • Máximo de procesos:   64\n");
  printf(1, "=========================================\n\n");
  
  exit();
}

