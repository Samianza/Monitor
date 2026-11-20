#include <stdio.h>
#include <windows.h>

#define MAX_TECLAS 90

typedef struct {
    int vk;
    int estava_pressionada;
} EstadoTecla;
