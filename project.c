#include <stdio.h>
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
int countAndPrintFrequencies(FILE *fp)
{
    int freq[256] = {0};
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        freq[ch]++;
    }

    rewind(fp);
    printf("\nCharacter Frequency Table:\n");
    printf("--------------------------\n");
    printf("Char | ASCII | Frequency\n");
    printf("--------------------------\n");

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            if (i == '\n')
                printf(" \\n  |  %3d  | %d\n", i, freq[i]);
            else if (i == '\t')
                printf(" \\t  |  %3d  | %d\n", i, freq[i]);
            else if (i == ' ')
                printf(" ' '  |  %3d  | %d\n", i, freq[i]);
            else if (i < 32 || i == 127)
                printf("NonP |  %3d  | %d\n", i, freq[i]);
            else
                printf("  %c   |  %3d  | %d\n", i, i, freq[i]);
        }
    }
    return 0;
}

void main()
{
    FILE *ptr;
    char filepath[256],str[max];
    printf("Enter the file path: ");
    fgets(filepath, sizeof(filepath), stdin);
    filepath[strcspn(filepath, "\n")] = '\0';
    ptr=fopen(filepath,"r");
    if(ptr==NULL)
    {
        printf("either wrong path entered or file does not exist");
    }
    else
    {
        countAndPrintFrequencies(ptr);
        rewind(ptr);
        printf("the content of the file is \n");
        while (fgets(str, max, ptr) != NULL)
        {
            printf("%s", str);
        }
        long file_size = getFileSize(ptr);
        printf("The size of the file is: %ld bytes\n", file_size);

        fclose(ptr);
    }
}
