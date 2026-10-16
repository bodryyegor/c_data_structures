#include "deque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PATIENTS 10

typedef struct {
    char id[100];
    int ta;
    int ts;
    int is_priority;
} ParsedPatient;

int parse_patient(const char* line, ParsedPatient* patient) {
    const char* ptr = line;
    
    memset(patient, 0, sizeof(ParsedPatient));
    
    if (*ptr == '*') {
        patient->is_priority = 1;
        ptr++;
    } else {
        patient->is_priority = 0;
    }

    const char* slash1 = strchr(ptr, '/');
    if (!slash1) return -1;
    
    size_t id_len = slash1 - ptr;
    if (id_len >= sizeof(patient->id) || id_len == 0) return -1;
    
    strncpy(patient->id, ptr, id_len);
    patient->id[id_len] = '\0';
    ptr = slash1 + 1;

    const char* slash2 = strchr(ptr, '/');
    if (!slash2) return -1;
    
    size_t ta_len = slash2 - ptr;
    if (ta_len == 0) return -1;
    
    char ta_str[20];
    strncpy(ta_str, ptr, ta_len);
    ta_str[ta_len] = '\0';
    
    for (int i = 0; ta_str[i]; i++) {
        if (!isdigit(ta_str[i])) return -1;
    }
    
    patient->ta = atoi(ta_str);
    if (patient->ta < 0) return -1;
    ptr = slash2 + 1;

    size_t ts_len = strlen(ptr);
    if (ts_len == 0) return -1;
    
    char ts_str[20];
    strncpy(ts_str, ptr, sizeof(ts_str)-1);
    ts_str[sizeof(ts_str)-1] = '\0';
    
    for (int i = 0; ts_str[i]; i++) {
        if (!isdigit(ts_str[i]) && ts_str[i] != ' ') return -1;
    }
    
    patient->ts = atoi(ts_str);
    if (patient->ts <= 0) return -1;

    while (*ptr) {
        if (!isdigit(*ptr) && *ptr != ' ') return -1;
        ptr++;
    }

    return 0;
}

int compare_patients(const void* a, const void* b) {
    return ((ParsedPatient*)a)->ta - ((ParsedPatient*)b)->ta;
}

int main() {
    Deque* deque = deque_create(MAX_PATIENTS);
    if (!deque) {
        printf( "Ошибка при создании очереди\n");
        return 0;
    }

    ParsedPatient parsed_patients[MAX_PATIENTS];
    int patient_count = 0;

    printf("Введите пациентов (Ctrl+D для конца ввода):\n");
    while (1) {
        printf("<<< ");
        
        if (patient_count >= MAX_PATIENTS) {
            printf("Максимальное количество пациентов достигнуто\n");
            break;
        }

        char line[1024];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        ParsedPatient temp_patient;
        if (parse_patient(line, &temp_patient) != 0) {
            printf("Некорректный ввод. Формат: [*]id/ta/ts\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        parsed_patients[patient_count++] = temp_patient;
    }

    qsort(parsed_patients, patient_count, sizeof(ParsedPatient), 
          (int(*)(const void*, const void*))compare_patients);

    int current_time = 0;
    int next_patient = 0;

    while (1) {
        while (next_patient < patient_count && parsed_patients[next_patient].ta <= current_time) {
            if (parsed_patients[next_patient].is_priority) {
                add_head(deque, parsed_patients[next_patient].id, 
                        parsed_patients[next_patient].ta, 
                        parsed_patients[next_patient].ts);
            } else {
                add_tail(deque, parsed_patients[next_patient].id, 
                        parsed_patients[next_patient].ta, 
                        parsed_patients[next_patient].ts);
            }
            next_patient++;
        }

        deque_print(deque, current_time);

        if (deque_is_empty(deque) && next_patient >= patient_count) break;

        if (deque_is_empty(deque)) {
            current_time = parsed_patients[next_patient].ta;
            continue;
        }

        char id[100];
        int ta, ts;
        dell_head(deque, id, &ta, &ts);
        current_time += ts;
    }

    //deque_print(deque, current_time);

    if (deque_is_empty(deque)) {
        printf("Очередь пуста\n");
    }

    free_deque(deque);
    return 0;
}
