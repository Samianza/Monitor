#include <stdio.h>
#include <windows.h> //para usar a api do windows

#define MAX_TECLAS 90 

typedef struct {
    int vk;
    int estava_pressionada;
} EstadoTecla;

void VKParaString(int vk, char *buffer) {
    switch (vk) {
        case VK_SPACE:      strcpy(buffer, "ESPACO");    break;
        case VK_RETURN:     strcpy(buffer, "ENTER");     break;
        case VK_BACK:       strcpy(buffer, "BACKSPACE"); break;
        case VK_TAB:        strcpy(buffer, "TAB");       break;
        case VK_ESCAPE:     strcpy(buffer, "ESC");       break;
        case VK_SHIFT:      strcpy(buffer, "SHIFT");     break;
        case VK_CONTROL:    strcpy(buffer, "CTRL");      break;
        case VK_MENU:       strcpy(buffer, "ALT");       break;
        case VK_CAPITAL:    strcpy(buffer, "CAPS LOCK"); break;
        case VK_LEFT:       strcpy(buffer, "SETA ESQ");  break;
        case VK_UP:         strcpy(buffer, "SETA CIMA"); break;
        case VK_RIGHT:      strcpy(buffer, "SETA DIR");  break;
        case VK_DOWN:       strcpy(buffer, "SETA BAIXO");break;
        case VK_DELETE:     strcpy(buffer, "DELETE");    break;
        case VK_F1:  case VK_F2:  case VK_F3:  case VK_F4:
        case VK_F5:  case VK_F6:  case VK_F7:  case VK_F8:
        case VK_F9:  case VK_F10: case VK_F11: case VK_F12:
            sprintf(buffer, "F%d", vk - VK_F1 + 1);
            break;
}


void AdicionarTecla(int vk)
void InicializarTeclas()
void VerificarTeclas(FILE *arquivo)
