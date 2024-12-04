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
    FileStruct file = ReadFile("input_01");

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
    FileStruct file = ReadFile("input_01");

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

void
Day02_1() {
    FileStruct file = ReadFile("input_02");

    int count = 0;
    char* rowPtr;
    char *row = strtok_r(file.data, "\n", &rowPtr);
    while (row != NULL) {
        char* wordPtr;
        char *word = strtok_r(row, " ", &wordPtr);
        int direction = 0;
        int lastNumber = -1;
        ++count;
        while (word != NULL) {
            if (lastNumber == -1) {
                sscanf(word, "%d", &lastNumber);
            } else if (direction == 0) {
                int nextNumber;
                sscanf(word, "%d", &nextNumber);
                direction = nextNumber - lastNumber;
                if (abs(direction) < 1 || abs(direction) > 3) {
                    --count;
                    break;
                }
                lastNumber = nextNumber;
            } else {
                int nextNumber;
                sscanf(word, "%d", &nextNumber);
                int diff = nextNumber - lastNumber;
                if ((diff * direction) <= 0 || abs(diff) < 1 || abs(diff) > 3) {
                    --count;
                    break;
                }
                lastNumber = nextNumber;
            }
            word = strtok_r(NULL, " ", &wordPtr);
        }
        row = strtok_r(NULL, "\n", &rowPtr);
    }
    printf("Day02-1 Result: %d\n", count);

    free(file.data);
}

void
Day02_2() {
    FileStruct file = ReadFile("input_02");

    int inputs[1000][16] = {0};
    char *row, *word, *rowPtr, *wordPtr;
    row = strtok_r(file.data, "\n", &rowPtr);
    for (int i = 0; i < 1000; ++i) {
        word = strtok_r(row, " ", &wordPtr);
        for (int ii = 0; ii < 16; ++ii) {
            sscanf(word, "%d", &inputs[i][ii]);
            word = strtok_r(NULL, " ", &wordPtr);
            if (word == NULL) break;
        }
        row = strtok_r(NULL, "\n", &rowPtr);
    }

    for (int i = 0; i < 1000; ++i) {
        for (int ii = 0; ii < 16; ++ii) {
            if (inputs[i][ii] == 0) break;
            printf("%d ", inputs[i][ii]);
        }
        printf("\n");
    }

    int safeCount = 0;
    for (int i = 0; i < 1000; ++i) {
        int isSafe = 0;
        for (int ignore = 0; ignore < 16; ++ignore) {
            if (isSafe) break;
            int line[16];
            for (int ii = 0; ii < 16; ++ii) line[ii] = inputs[i][ii];
            for (int ii = ignore; ii < 15; ++ii) line[ii] = line[ii+1];

            int direction = line[1] - line[0];
            for (int ii = 0; ii < 15; ++ii) {
                if (line[ii+1] != 0) {
                    int diff = line[ii+1] - line[ii];
                    if ((diff * direction) < 0 || abs(diff) < 1 || abs(diff) > 3) {
                        break;
                    }
                } else {
                    isSafe = 1;
                    break;
                }
            }
        }
        safeCount += isSafe;
    }
    printf("Day02-1 Result: %d\n", safeCount);

    free(file.data);
}

void
Day03_1() {
    FileStruct file = ReadFile("input_03");
    char* progress = file.data;

    int sum = 0;
    int doMul = 1;
    while (*progress != '\0') {
        int num1 = 0, num2 = 0;
        char c;
        if (sscanf(progress, "mul(%d,%d%c", &num1, &num2, &c) == 3 && c == ')') {
            sum += num1 * num2;
        }
        ++progress;
    }
    printf("Day03_1 Result: %d\n", sum);

    free(file.data);
}

void
Day03_2() {
    FileStruct file = ReadFile("input_03");
    char* progress = file.data;

    int sum = 0;
    int doMul = 1;
    while (*progress != '\0') {
        if (*progress == 'd' && *(progress + 1) == 'o') {
            if (*(progress + 2) == '(' && *(progress + 3) == ')') doMul = 1;
            if (*(progress + 2) == 'n' && *(progress + 3) == '\'' && *(progress + 4) == 't' && *(progress + 5) == '(' && *(progress + 6) == ')') doMul = 0;
        }
        int num1 = 0, num2 = 0;
        char c;
        if (sscanf(progress, "mul(%d,%d%c", &num1, &num2, &c) == 3 && c == ')' && doMul == 1) {
            sum += num1 * num2;
        }
        ++progress;
    }
    printf("Day03_2 Result: %d\n", sum);

    free(file.data);
}

void
Day04_1() {
    FileStruct file = ReadFile("input_04");
    int count = 0;
    for (int i = 0; i < 140; ++i) {
        for (int ii = 0; ii < 140; ++ii) {
            int index = i * 141 + ii;
            if (file.data[index] == 'X') {
                /* right */
                if ((ii + 3) < 140 &&
                    file.data[index + 1] == 'M' &&
                    file.data[index + 2] == 'A' &&
                    file.data[index + 3] == 'S')
                    ++count;
                /* down */
                if ((i + 3) < 140 &&
                    file.data[(i+1)*141+ii] == 'M' &&
                    file.data[(i+2)*141+ii] == 'A' &&
                    file.data[(i+3)*141+ii] == 'S')
                    ++count;
                /* right-down */
                if ((ii + 3) < 140 && (i + 3) < 140 &&
                    file.data[(i+1)*141+ii+1] == 'M' &&
                    file.data[(i+2)*141+ii+2] == 'A' &&
                    file.data[(i+3)*141+ii+3] == 'S')
                    ++count;
                /* right-up */
                if ((ii + 3) < 140 && (i - 3) >= 0 &&
                    file.data[(i-1)*141+ii+1] == 'M' &&
                    file.data[(i-2)*141+ii+2] == 'A' &&
                    file.data[(i-3)*141+ii+3] == 'S')
                    ++count;
            } else if (file.data[index] == 'S') {
                /* right */
                if ((ii + 3) < 140 &&
                    file.data[index + 1] == 'A' &&
                    file.data[index + 2] == 'M' &&
                    file.data[index + 3] == 'X')
                    ++count;
                /* down */
                if ((i + 3) < 140 &&
                    file.data[(i+1)*141+ii] == 'A' &&
                    file.data[(i+2)*141+ii] == 'M' &&
                    file.data[(i+3)*141+ii] == 'X')
                    ++count;
                /* right-down */
                if ((ii + 3) < 140 && (i + 3) < 140 &&
                    file.data[(i+1)*141+ii+1] == 'A' &&
                    file.data[(i+2)*141+ii+2] == 'M' &&
                    file.data[(i+3)*141+ii+3] == 'X')
                    ++count;
                /* right-up */
                if ((ii + 3) < 140 && (i - 3) >= 0 &&
                    file.data[(i-1)*141+ii+1] == 'A' &&
                    file.data[(i-2)*141+ii+2] == 'M' &&
                    file.data[(i-3)*141+ii+3] == 'X')
                    ++count;

            }
        }
    }

    printf("Day04_1 Result: %d\n", count);
    free(file.data);
}

void
Day04_2() {
    FileStruct file = ReadFile("input_04");
    int count = 0;
    for (int i = 1; i < 139; ++i) {
        for (int ii = 1; ii < 139; ++ii) {
            int index = i * 141 + ii;
            if (file.data[index] == 'A' ) {
                if (file.data[(i-1)*141+ii-1] == 'M' && file.data[(i+1)*141+ii+1] == 'S' &&
                    file.data[(i-1)*141+ii+1] == 'M' && file.data[(i+1)*141+ii-1] == 'S') ++count;
                if (file.data[(i-1)*141+ii-1] == 'S' && file.data[(i+1)*141+ii+1] == 'M' &&
                    file.data[(i-1)*141+ii+1] == 'M' && file.data[(i+1)*141+ii-1] == 'S') ++count;
                if (file.data[(i-1)*141+ii-1] == 'M' && file.data[(i+1)*141+ii+1] == 'S' &&
                    file.data[(i-1)*141+ii+1] == 'S' && file.data[(i+1)*141+ii-1] == 'M') ++count;
                if (file.data[(i-1)*141+ii-1] == 'S' && file.data[(i+1)*141+ii+1] == 'M' &&
                    file.data[(i-1)*141+ii+1] == 'S' && file.data[(i+1)*141+ii-1] == 'M') ++count;
            }
        }
    }

    printf("Day04_2 Result: %d\n", count);
    free(file.data);
}

int
main() {
    /* Day01_1(); */
    /* Day01_2(); */
    /* Day02_1(); */
    /* Day02_2(); */
    /* Day03_1(); */
    /* Day03_2(); */
    Day04_1();
    Day04_2();
}
