#include <stdio.h>
#include <stdlib.h>

typedef struct Token
{
    int offset;
}Token;

typedef struct Header
{
    int nbfile;
    Token* tok;
}Header;

void get_header(Header* head, FILE* in);
void dump_file(Header* header, FILE* in);

int main(int argc, char* argv[])
{
    Header head;
    FILE* in = fopen(argv[1], "rb");
    
    if(!in)
    {
        printf("Impossible d'ouvrir le fichier: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    get_header(&head, in);
    dump_file(&head, in);
    
    free(head.tok);
    
    return 0;
}

void get_header(Header* head, FILE* in)
{
    fread(&head->nbfile, sizeof(char), 4, in);
    
    printf("File: %X\n", head->nbfile);
    
    head->tok = calloc(head->nbfile, sizeof(Token));
    
    for(int i = 1; i <= head->nbfile; i++)
    {
        fseek(in, i*0x10, SEEK_SET);
        fread(&head->tok[i-1].offset, sizeof(char), 4, in);
        //printf("File[%d]: %X\n", i, head->tok[i].offset);
    }
}

void dump_file(Header* head, FILE* in)
{
    unsigned char a = 1;
    FILE* out = fopen("out.txt", "wb+");
    
    if(!out)
    {
        printf("Impossible de créer le fichier : out.txt\n");
        free(head->tok);
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < head->nbfile; i++)
    {
        fseek(in, head->tok[i].offset, SEEK_SET);
        
        while(a != 0)
        {
            fread(&a, sizeof(char), 1, in);
            
            if(a == 0)
            {
                break;
            }
            
            fwrite(&a, sizeof(char), 1, out);
        }
        
        fprintf(out, "\n");
        a = 1;
    }
}
        
        
        
        
        
        
