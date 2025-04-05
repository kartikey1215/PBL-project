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
        printf("the content of the file is \n");
        while (fgets(str, max, ptr) != NULL)
        {
            printf("%s", str);
        }
        long file_size = getFileSize(ptr);
        printf("The size of the file is: %ld bytes\n", file_size);
    }
    fclose(ptr);
}
