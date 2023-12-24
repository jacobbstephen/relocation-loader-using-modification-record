#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reloc(){
    FILE* obj = fopen("object.txt","r");
    FILE* memfile = fopen("memoryfile.txt","w+");

    int PROGADDR,start_addr;
    char rec[80],addr[6],loc[7],code[3],mod[4];
    mod[6] = addr[6] = code[2] = '\0';

    printf("Enter program address from OS: ");
    scanf("%x", &PROGADDR);

    fgets(rec,sizeof(rec),obj);
    strncpy(addr, rec+9, 6);
    start_addr = (int) strtol(addr, NULL, 16);

    fgets(rec,sizeof(rec),obj);
    while(rec[0] == 'T'){
        
        strncpy(addr, rec+2, 6);
        sprintf(addr,"%06X", (int) strtol(addr, NULL, 16) - start_addr + PROGADDR);
        
        for(int i = 12; rec[i] != '\n'; i += 7){
            fprintf(memfile,"[%s]: %c%c\n",addr,rec[i],rec[i + 1]);
            sprintf(addr,"%06X", (int) strtol(addr, NULL, 16) + 1);
            fprintf(memfile,"[%s]: %c%c\n",addr,rec[i + 2],rec[i + 3]);
            sprintf(addr,"%06X", (int) strtol(addr, NULL, 16) + 1);
            fprintf(memfile,"[%s]: %c%c\n",addr,rec[i + 4],rec[i +  5]);
            sprintf(addr,"%06X", (int) strtol(addr, NULL, 16) + 1);
            if(rec[i + 6] == '\n') break;
            
        }
        fgets(rec,sizeof(rec),obj);
    }
    rewind(memfile); 
    while(rec[0] == 'M'){
        strncpy(addr, rec+2, 6);
        sprintf(addr,"%06X", (int) strtol(addr, NULL, 16) - start_addr + PROGADDR);

        fscanf(memfile, "[%6s]: %s\n", loc, code);
        while (strcmp(addr, loc))
            fscanf(memfile, "[%6s]: %s\n", loc, code);
        strcpy(mod, code);
        fscanf(memfile, "[%6s]: %s\n", loc, code);
        strcat(mod, code);
        
        sprintf(mod, "%04X", (int)strtol(mod, NULL, 16) - start_addr + PROGADDR);
        fseek(memfile,-28,SEEK_CUR);
        fprintf(memfile,"[%s]: %c%c\n",addr,mod[0],mod[1]);
        sprintf(addr,"%06X", (int) strtol(addr, NULL, 16) + 1);
        fprintf(memfile,"[%s]: %c%c\n",addr,mod[2],mod[3]);
        sprintf(addr,"%06X", (int) strtol(addr, NULL, 16) + 1);
        fseek(memfile, 0, SEEK_CUR);
        fgets(rec,sizeof(rec),memfile);
    }

}
int main(){
    reloc();
    printf("Done\n");
    return(0);
}