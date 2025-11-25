#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *mythread(void *fname)
{
    FILE *fd;
    char ch;
    char *fn;
    
    pthread_t mythid;
    mythid = pthread_self();
    
    fn = (char *)fname;
    
    if ((fd = fopen(fn, "r")) == NULL)
    {
        printf("Thread %lu: Cannot open file '%s'\n", mythid, fn);
        pthread_exit(NULL);
    }
    
    printf("=== Thread %lu reading file: %s ===\n", mythid, fn);
    
    while (!feof(fd))
    {
        fscanf(fd, "%c", &ch);
        printf("Thread %lu: Symbol '%c'\n", mythid, ch);
    }
    
    fclose(fd);
    printf("=== Thread %lu finished reading file: %s ===\n", mythid, fn);
    
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        printf("Please provide at least one filename\n");
        return 1;
    }
    
    int file_count = argc - 1;
    pthread_t *threads = malloc(file_count * sizeof(pthread_t));
    
    if (threads == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Main thread started, creating %d threads for %d files\n", 
           file_count, file_count);
    
    // Создаем потоки для каждого файла
    for (int i = 0; i < file_count; i++)
    {
        int result = pthread_create(&threads[i], NULL, mythread, (void *)argv[i + 1]);
        if (result != 0)
        {
            printf("Error creating thread for file '%s', return value = %d\n", 
                   argv[i + 1], result);
            free(threads);
            exit(-1);
        }
        printf("Created thread %lu for file: %s\n", threads[i], argv[i + 1]);
    }
    
    pthread_t main_thid = pthread_self();
    printf("Main thread ID: %lu\n", main_thid);
    
    // Ожидаем завершения всех потоков
    for (int i = 0; i < file_count; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Thread %lu has finished\n", threads[i]);
    }
    
    free(threads);
    printf("All threads completed. Main thread exiting.\n");
    
    return 0;
}
