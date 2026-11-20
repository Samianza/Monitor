#include <stdio.h>
#include <windows.h> //para usar a api do windows

#define MAX_TECLAS 90 // o total de teclas que seram monitoradas

typedef struct {
    int vk;
    int estava_pressionada;
} EstadoTecla;

EstadoTecla teclas[MAX_TECLAS]; // vetor para armazenar todas as teclas monitoradas
int total_teclas = 0 // variavel usada na função AdicionarTecla


// essa função torna humanamente legível o código vk das teclas
void VKParaString(int vk, char *buffer) {
    switch (vk) {
        // primeira parte contendo as teclas especiais
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
        default
            // numeros e letras
            if (vk >= '0' && vk <= '9') {
                buffer[0] = (char)vk; buffer[1] = '\0';
            } else if (vk >= 'A' && vk <= 'Z') {
                // validação usada para diferenciar maiúsculas de minúsculas
                // primeiro uso das funções da api win32
                int shift = (GetAsyncKeyState(VK_SHIFT) & 0X8000) ? 1 : 0; 
                int caps = (GetKeyState(VK_CAPITAL) & 0x0001) ? 1 : 0;
                buffer[0] = (shift ^ caps) ? (char)vk : (char)(vk + 32);
                buffer[1] = '\0';
            } else {
                // registra o código vk se não existir tradução
                // caso adicionar tecla que ainda não possui tradução
                sprintf(buffer, "VK_%d",vk);
            }
            break;
    }
}

// essa função adiciona as teclas monitoradas no vetor de structs
void AdicionarTecla(int vk) {
    // condição para evitar o estouro do limite do vetor teclas
    if (total_teclas >= MAX_TECLAS) return;
    teclas[total_teclas].vk = vk; // registra o codigo vk
    teclas[total_teclas].estava_pressionada = 0; // marca como não pressionada
    total_teclas++; // contador para atender limite de 90 teclas

}

void InicializarTeclas()
void VerificarTeclas(FILE *arquivo)
