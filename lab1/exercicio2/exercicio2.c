#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100
#define MAX_ID_LEN 50

typedef struct {
    char id[MAX_ID_LEN];
    int priority;
    time_t created_at;
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

void new_task(int priority, const char *id) {
    if (task_count >= MAX_TASKS) {
        printf("Limite de tarefas atingido.\n");
        return;
    }
    int i;
    for (i = 0; i < task_count; i++) {
        if (strcmp(tasks[i].id, id) == 0) {
            printf("TAREFA JÁ EXISTE\n");
            return;
        }
    }
    strcpy(tasks[task_count].id, id);
    tasks[task_count].priority = priority;
    tasks[task_count].created_at = time(NULL);
    task_count++;
}

int compare_tasks(const void *a, const void *b) {
    Task *t1 = (Task *)a;
    Task *t2 = (Task *)b;
    if (t1->priority != t2->priority)
        return t2->priority - t1->priority; // maior prioridade primeiro
    return (int)(t2->created_at - t1->created_at); // mais recente primeiro
}

void list_tasks(int min_priority) {
    Task filtered[MAX_TASKS];
    int count = 0;
    int i;
    for (i = 0; i < task_count; i++) {
        if (tasks[i].priority >= min_priority) {
            filtered[count++] = tasks[i];
        }
    }
    qsort(filtered, count, sizeof(Task), compare_tasks);
    
    for (i = 0; i < count; i++) {
        printf("%s (prioridade %d)\n", filtered[i].id, filtered[i].priority);
    }
}

void complete_task(const char *id) {
    for (int i = 0; i < task_count; i++) {
        if (strcmp(tasks[i].id, id) == 0) {
            for (int j = i; j < task_count - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            task_count--;
            return;
        }
    }
    printf("TAREFA INEXISTENTE\n");
}

int main() {
    char command[100];
    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "new") == 0) {
            int prio;
            char id[MAX_ID_LEN];
            scanf("%d %s", &prio, id);
            new_task(prio, id);
        } else if (strcmp(command, "list") == 0) {
            int prio;
            scanf("%d", &prio);
            list_tasks(prio);
        } else if (strcmp(command, "complete") == 0) {
            char id[MAX_ID_LEN];
            scanf("%s", id);
            complete_task(id);
        }
    }
    return 0;
}
