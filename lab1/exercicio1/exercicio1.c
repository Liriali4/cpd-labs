#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

typedef struct{
 int pid;
 int start_time;
 int end_time;
}Process;

bool update_terminated_process( Process list[],int size, int pid, int end_time){
	int i;
for(i;i<size;i++){
    if(list[i].pid==pid){
       list[i].end_time=end_time;
       return true;
}	
}
return false;
}

int main(int argc, char *argv[]) {
     Process processes[2] = {
        {1, 10, -1},
        {2, 12, -1}
    };

    if (update_terminated_process(processes, 2, 2, 20)) {
        printf("Processo atualizado!\n");
    } else {
        printf("Processo não encontrado.\n");
    }
int i;
    for (i = 0; i < 2; i++) {
        printf("PID=%d, start=%d, end=%d\n", processes[i].pid, processes[i].start_time, processes[i].end_time);
    }
	return 0;
}
