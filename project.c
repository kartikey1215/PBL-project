#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 1000

long getFileSize(FILE *fp)
{
    long size;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    return size;
}

typedef struct {
    int ascii;
    int frequency;
} CharFreq;

typedef struct HuffmanNode {
    int ascii;
    int frequency;
    struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct {
    HuffmanNode* nodes[256];
    int size;
} MinHeap;

int compare(const void *a, const void *b) {
    CharFreq *c1 = (CharFreq *)a;
    CharFreq *c2 = (CharFreq *)b;
    return c1->frequency - c2->frequency;
}

HuffmanNode* createNode(int ascii, int frequency) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->ascii = ascii;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

void swap(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, int i) {
    int smallest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < heap->size && heap->nodes[left]->frequency < heap->nodes[smallest]->frequency)
        smallest = left;
    if (right < heap->size && heap->nodes[right]->frequency < heap->nodes[smallest]->frequency)
        smallest = right;

    if (smallest != i) {
        swap(&heap->nodes[i], &heap->nodes[smallest]);
        minHeapify(heap, smallest);
    }
}

HuffmanNode* extractMin(MinHeap* heap) {
    HuffmanNode* min = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    minHeapify(heap, 0);
    return min;
}

void insertHeap(MinHeap* heap, HuffmanNode* node) {
    int i = heap->size++;
    heap->nodes[i] = node;

    while (i && heap->nodes[i]->frequency < heap->nodes[(i - 1) / 2]->frequency) {
        swap(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

HuffmanNode* buildHuffmanTree(CharFreq freqArray[], int count) {
    MinHeap heap = { .size = 0 };

    for (int i = 0; i < count; i++) {
        insertHeap(&heap, createNode(freqArray[i].ascii, freqArray[i].frequency));
    }

    while (heap.size > 1) {
        HuffmanNode* left = extractMin(&heap);
        HuffmanNode* right = extractMin(&heap);
        HuffmanNode* merged = createNode(-1, left->frequency + right->frequency);
        merged->left = left;
        merged->right = right;
        insertHeap(&heap, merged);
    }

    return extractMin(&heap);
}

void printHuffmanCodes(HuffmanNode* root, char* code, int top) {
    if (root->left) {
        code[top] = '0';
        printHuffmanCodes(root->left, code, top + 1);
    }

    if (root->right) {
        code[top] = '1';
        printHuffmanCodes(root->right, code, top + 1);
    }

    if (!root->left && !root->right) {
        code[top] = '\0';
        if (root->ascii == '\n')
            printf(" \\n  : %s\n", code);
        else if (root->ascii == '\t')
            printf(" \\t  : %s\n", code);
        else if (root->ascii == ' ')
            printf(" ' '  : %s\n", code);
        else if (root->ascii < 32 || root->ascii == 127)
            printf("NonP : %s\n", code);
        else
            printf("  %c   : %s\n", root->ascii, code);
    }
}

void calculateCodeLengths(HuffmanNode* root, int depth, int codeLengths[256]) {
    if (root->left)
        calculateCodeLengths(root->left, depth + 1, codeLengths);
    if (root->right)
        calculateCodeLengths(root->right, depth + 1, codeLengths);
    if (!root->left && !root->right)
        codeLengths[root->ascii] = depth;
}

long calculateCompressedSize(CharFreq freqArray[], int count, HuffmanNode* root) {
    int codeLengths[256] = {0};
    calculateCodeLengths(root, 0, codeLengths);

    long totalBits = 0;
    for (int i = 0; i < count; i++) {
        int ascii = freqArray[i].ascii;
        int freq = freqArray[i].frequency;
        totalBits += freq * codeLengths[ascii];
    }

    return (totalBits + 7) / 8;
}

int countAndPrintFrequencies(FILE *fp, CharFreq freqArray[])
{
    int freq[256] = {0};
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        freq[ch]++;
    }
    rewind(fp);

    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            freqArray[count].ascii = i;
            freqArray[count].frequency = freq[i];
            count++;
        }
    }

    qsort(freqArray, count, sizeof(CharFreq), compare);

    printf("\nCharacter Frequency Table:\n");
    printf("-----------------------------------\n");
    printf("Char | ASCII | Frequency\n");
    printf("-----------------------------------\n");

    for (int i = 0; i < count; i++) {
        int i_ascii = freqArray[i].ascii;
        if (i_ascii == '\n')
            printf(" \\n   |  %3d  | %d\n", i_ascii, freqArray[i].frequency);
        else if (i_ascii == '\t')
            printf(" \\t  |  %3d  | %d\n", i_ascii, freqArray[i].frequency);
        else if (i_ascii == ' ')
            printf(" ' '  |  %3d  | %d\n", i_ascii, freqArray[i].frequency);
        else if (i_ascii < 32 || i_ascii == 127)
            printf("NonP |  %3d  | %d\n", i_ascii, freqArray[i].frequency);
        else
            printf("  %c   |  %3d  | %d\n", i_ascii, i_ascii, freqArray[i].frequency);
    }

    return count;
}

int main()
{
    FILE *ptr;
    char filepath[256], str[max];
    CharFreq freqArray[256];
    printf("Enter the file path: ");
    fgets(filepath, sizeof(filepath), stdin);
    filepath[strcspn(filepath, "\n")] = '\0';
    ptr = fopen(filepath, "r");
    if (ptr == NULL)
    {
        printf("Either wrong path entered or file does not exist.\n");
    }
    else
    {
        int count = countAndPrintFrequencies(ptr, freqArray);

        rewind(ptr);
        printf("\nThe content of the file is:\n");
        while (fgets(str, max, ptr) != NULL)
        {
            printf("%s", str);
        }

        long file_size = getFileSize(ptr);
        printf("\nOriginal File Size: %ld bytes\n", file_size);

        HuffmanNode* root = buildHuffmanTree(freqArray, count);
        char code[256];
        printf("\nHuffman Codes for each character:\n");
        printf("----------------------------------\n");
        printHuffmanCodes(root, code, 0);

        long compressedSize = calculateCompressedSize(freqArray, count, root);
        printf("\nEstimated Compressed Size: %ld bytes\n", compressedSize);

        fclose(ptr);
    }
}
