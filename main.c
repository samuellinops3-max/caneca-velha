#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <io/pad.h>
#include <rsx/rsx.h>
#include <sysutil/sysutil.h>

gcmContextData *context;
void *host_addr = NULL;
#define HOST_SIZE (1024*1024)

static void sysutil_callback(u64 status, u64 param, void *userdata) {
    if (status == SYSUTIL_EXIT_GAME) {
        exit(0);
    }
}

void init_screen() {
    host_addr = memalign(1024*1024, HOST_SIZE);
    context = rsxInit(0x10000, HOST_SIZE, host_addr);
    videoState state;
    videoGetState(0, 0, &state);
    videoConfigure(0, &state.configuration, NULL, 0);
    videoStateFlip(0);
    rsxSetupContextPointer(context);
}

int main(int argc, const char* argv[]) {
    sysRegisterCallback(SYSUTIL_EVENT_SLOT0, sysutil_callback, NULL);
    init_screen();
    ioPadInit(7);

    padInfo pad_info;
    padData pad_data;

    // Loop do Jogo no PS3
    while (1) {
        sysUtilCheckCallback();
        ioPadGetInfo(&pad_info);
        
        if (pad_info.status) {
            ioPadGetData(0, &pad_data);
            // Se apertar START no controle do PS3, fecha o jogo de volta para a XMB
            if (pad_data.BTN_START) {
                break;
            }
        }
        usleep(16666); 
    }

    ioPadEnd();
    return 0;
}
