#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include "commandlinereader.h"

#define MAX_ARGS 7 // pathname + 5 args + NULL
#define BUFFER_SIZE 1024

// Estrutura para a lista ligada de processos
typedef struct process_node {
    pid_t pid;
    time_t start_time;
    time_t end_time;
    struct process_node *next;
} ProcessNode;

// Variáveis Globais Partilhadas
ProcessNode *process_list = NULL;
int numChildren = 0;
int term_signal = 0; // Sinal para a thread monitora terminar
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;

// Função auxiliar para adicionar à lista
void add_process(pid_t pid, time_t start) {
    pthread_mutex_lock(&list_lock);
    ProcessNode *new_node = malloc(sizeof(ProcessNode));
    new_node->pid = pid;
    new_node->start_time = start;
    new_node->end_time = 0;
    new_node->next = process_list;
    process_list = new_node;
    numChildren++;
    pthread_mutex_unlock(&list_lock);
}

// Função da Tarefa Monitora
void* monitor_task(void* arg) {
    while (1) {
        pthread_mutex_lock(&list_lock);
        
        // Verifica se deve terminar e se não há mais filhos
        if (term_signal && numChildren == 0) {
            pthread_mutex_unlock(&list_lock);
            break;
        }

        if (numChildren > 0) {
            pthread_mutex_unlock(&list_lock);
            
            int status;
            pid_t finished_pid = wait(&status);
            
            if (finished_pid > 0) {
                time_t now = time(NULL);
                pthread_mutex_lock(&list_lock);
                // Procurar o processo na lista e registar o fim
                ProcessNode *curr = process_list;
                while (curr != NULL) {
                    if (curr->pid == finished_pid) {
                        curr->end_time = now;
                        break;
                    }
                    curr = curr->next;
                }
                numChildren--;
                pthread_mutex_unlock(&list_lock);
            }
        } else {
            pthread_mutex_unlock(&list_lock);
            sleep(1); // Evita espera ativa intensa
        }
    }
    return NULL;
}

int main() {
    char *args[MAX_ARGS];
    char buffer[BUFFER_SIZE];
    pthread_t monitor_tid;

    if (pthread_create(&monitor_tid, NULL, monitor_task, NULL) != 0) {
        perror("Erro ao criar thread monitora");
        exit(1);
    }

    while (1) {
        int numArgs = readLineArguments(args, MAX_ARGS, buffer, BUFFER_SIZE);

        if (numArgs <= 0) continue;

        if (strcmp(args[0], "exit") == 0) {
            pthread_mutex_lock(&list_lock);
            term_signal = 1;
            pthread_mutex_unlock(&list_lock);
            break;
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Processo Filho
            execv(args[0], args);
            // Se chegar aqui, o exec falhou
            perror("Erro ao executar comando");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            // Processo Pai
            add_process(pid, time(NULL));
        } else {
            perror("Erro no fork");
        }
    }

    // Espera pela thread monitora
    pthread_join(monitor_tid, NULL);

    // Imprimir resultados finais
    printf("\n--- Relatório de Execução ---\n");
    ProcessNode *curr = process_list;
    while (curr != NULL) {
        printf("PID: %d | Duração: %ld segundos\n", 
                curr->pid, (curr->end_time - curr->start_time));
        ProcessNode *temp = curr;
        curr = curr->next;
        free(temp);
    }

    return 0;
}