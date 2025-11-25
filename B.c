
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    char **files;
    int count;
    double **arrays;
    double *result;
} ThreadData;

void* sum_arrays(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    
    for (int i = 0; i < data->count - 1; i++) {
        FILE *f1 = fopen(data->files[i], "r");
        FILE *f2 = fopen(data->files[i+1], "r");
        
        int size;
        fscanf(f1, "%d", &size);
        fscanf(f2, "%d", &size);
        
        data->arrays[i] = malloc(size * sizeof(double));
        data->result = malloc(size * sizeof(double));
        
        for (int j = 0; j < size; j++) {
            double a, b;
            fscanf(f1, "%lf", &a);
            fscanf(f2, "%lf", &b);
            data->arrays[i][j] = a + b;
            if (i == 0) data->result[j] = data->arrays[i][j];
            else data->result[j] += data->arrays[i][j];
        }
        
        fclose(f1);
        fclose(f2);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    
    ThreadData data = {argv + 1, argc - 1, NULL, NULL};
    data.arrays = malloc((argc-2) * sizeof(double*));
    
    pthread_t thread;
    pthread_create(&thread, NULL, sum_arrays, &data);
    pthread_join(thread, NULL);
    
    int size;
    FILE *f = fopen(argv[1], "r");
    fscanf(f, "%d", &size);
    fclose(f);
    
    for (int i = 0; i < size; i++)
        printf("%.1f ", data.result[i]);
    
    free(data.result);
    for (int i = 0; i < argc-2; i++) free(data.arrays[i]);
    free(data.arrays);
    
    return 0;
}
