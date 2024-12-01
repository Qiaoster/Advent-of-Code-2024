#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_PATH_MAX 128
typedef struct {
    int size;
    char path[FILE_PATH_MAX];
    char* data;
} FileStruct;
FileStruct emptyFileStruct;

int
StringLength(const char* string) {
    int l = 0;
    while (string[l] != '\0') ++l;
    return l;
}

void
SetMemory(char* buffer, const char memoryContent, const int length) {
    for (int i = 0; i < length; ++i) {
        buffer[i] = memoryContent;
    }
}

void
CopyMemory(char* destination, const char* origin, const int length) {
    for (int i = 0; i < length; ++i) {
        destination[i] = origin[i];
    }
}

FileStruct
ReadFile(const char* filePath) {
    int pathLength = StringLength(filePath);
    if (pathLength >= FILE_PATH_MAX - 1) {
        printf("WARNING: File %s path longer than max path value %i", filePath, FILE_PATH_MAX - 1);
        return emptyFileStruct;
    }
    /* 1. Open file */
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Couldn't open file %s\n", filePath);
        return emptyFileStruct;
    }

    /* 2. Allocate buffer to store data */
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    rewind(file);
    int dataBufferSize = (fileSize + 1); // NOTE: Maybe add a guard for extremely large files?
    char* dataBuffer = (char*)malloc(dataBufferSize * sizeof(char));
    if (dataBuffer == NULL) {
        printf("ERROR: Failed to allocate file buffer.\n    File Name: %s\n    Allocation Size: %i\n", filePath, dataBufferSize);
        return emptyFileStruct;
    }
    SetMemory(dataBuffer, 0, dataBufferSize * sizeof(char));

    /* 3. Read data into buffer */
    int readSize = fread(dataBuffer, sizeof(char), fileSize, file);
    if (readSize != fileSize) {
        printf("ERROR: Labeled file size inconsistent with actual read size.\n    File Name: %s\n    Labeled Size: %i\n    Actual Read Size: %i\n", filePath, fileSize, readSize);
        fclose(file);
        return emptyFileStruct;
    }

    /* 4. Succeed, cleanup and return */
    dataBuffer[fileSize] = '\0';
    fclose(file);

    FileStruct fileStruct;
    fileStruct.size = dataBufferSize;
    fileStruct.data = dataBuffer;
    SetMemory(fileStruct.path, 0, FILE_PATH_MAX);
    CopyMemory(fileStruct.path, filePath, StringLength(filePath));

    return fileStruct;
}

void
Swap(int* a, int* b) {
    int holdme = *a;
    *a = *b;
    *b = holdme;
}

int
Partition(int* array, int lIndex, int rIndex) {
    int lowCount = 0;
    for (int i = lIndex; i < rIndex; ++i) {
        if (array[i] < array[rIndex]) {
            Swap(&array[i], &array[lIndex + lowCount]);
            ++lowCount;
        }
    }
    int pivotIndex = lIndex + lowCount;
    Swap(&array[rIndex], &array[pivotIndex]);
    return pivotIndex;
}

void
QuickSort(int* array, int lIndex, int rIndex) {
    if (rIndex > lIndex) {
       int pivotIndex = Partition(array, lIndex, rIndex);

       QuickSort(array, lIndex, pivotIndex - 1);
       QuickSort(array, pivotIndex + 1, rIndex);
    }
}

void
Day01_1() {
    FileStruct file = ReadFile("input");

    int col1[1024], col2[1024];
    int count = 0;
    char *row = strtok(file.data, "\n");
    while (row != NULL) {
        sscanf(row, "%d %d", &col1[count], &col2[count]);
        ++count;
        row = strtok(NULL, "\n");
    }
    QuickSort(col1, 0, count - 1);
    QuickSort(col2, 0, count - 1);

    int sum = 0;
    for (int i = 0; i < count; ++i) {
        sum += abs(col2[i] - col1[i]);
    }

    printf("Day01-1 Result: %d\n", sum);
    free(file.data);
}

void
Day01_2() {
    FileStruct file = ReadFile("input");

    int col1[1024], col2[1024];
    int count = 0;
    char *row = strtok(file.data, "\n");
    while (row != NULL) {
        sscanf(row, "%d %d", &col1[count], &col2[count]);
        ++count;
        row = strtok(NULL, "\n");
    }
    QuickSort(col1, 0, count - 1);
    QuickSort(col2, 0, count - 1);

    int sum = 0;
    for (int i = 0; i < count; ++i) {
        int repetition = 0;
        for (int ii = 0; ii < count; ++ii) {
            if (col2[ii] > col1[i]) break;
            if (col2[ii] == col1[i]) ++repetition;
        }
        sum += col1[i] * repetition;
    }
    printf("Day02-2 Result: %d\n", sum);

    free(file.data);
}

int
main() {
    Day01_1();
    Day01_2();
}
