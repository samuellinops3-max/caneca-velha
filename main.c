#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <io/pad.h>
#include <rsx/rsx.h>
#include <sysutil/sysutil.h>

// Configurações básicas de vídeo do PS3
gcmContextData *context;
void *host_addr = NULL;
u32 local_mem_address;

#define HOST_SIZE (1024*1024)

// Função obrigatória para fechar o jogo corretamente se o usuário apertar o botão PS
static void sysutil_callback(u64 status, u64 param, void *userdata) {
    if (status == SYSUTIL_EXIT_GAME) {
        exit(0);
    }
}

// Inicializa a placa de vídeo (RSX) do PS3
void init_screen() {
    host_addr = memalign(1024*1024, HOST_SIZE);
    assert(host_addr != NULL);

    context = rsxInit(0x10000, HOST_SIZE, host_addr);
    assert(context != NULL);

    videoState state;
    assert(videoGetState(0, 0, &state) == 0);
    assert(videoConfigure(0, &state.configuration, NULL, 0) == 0);
    assert(videoStateFlip(0) == 0);

    rsxSetupContextPointer(context);
}

int main(int argc, const char* argv[]) {
    // Registra o botão Home/PS
    sysRegisterCallback(SYSUTIL_EVENT_SLOT0, sysutil_callback, NULL);

    // Inicializa vídeo e controle
    init_screen();
    ioPadInit(7);

    padInfo pad_info;
    padData pad_data;

    printf("Cuphead Meio Velho inicializado com sucesso!\n");

    // Loop principal do jogo
    while (1) {
        sysUtilCheckCallback();

        // Lê as entradas do controle 1
        ioPadGetInfo(&pad_info);
        if (pad_info.status[0]) {
            ioPadGetData(0, &pad_data);
            
            // Se apertar START, fecha o jogo
            if (pad_data.BTN_START) {
                break;
            }
        }

        // Aqui vai a lógica de renderização e movimentação do Cuphead futuramente
        // Por enquanto, apenas segura o console atualizado para evitar congelamento
        usleep(16666); // Roda próximo a 60 FPS
    }

    // Limpeza ao sair do jogo
    ioPadEnd();
    return 0;
}
