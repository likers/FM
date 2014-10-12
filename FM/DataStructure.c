//
//  DataStructure.c
//  FM
//
//  Created by Jinhuan Li on 10/7/14.
//  Copyright (c) 2014 Jinhuan Li. All rights reserved.
//

#include "DataStructure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Nets *net_new(void){
    struct net *newlist;
    newlist = malloc(sizeof(struct net));
    if (newlist == NULL)
    {
        return NULL;
    }
    
    newlist->next = NULL;
    newlist->prev = NULL;
    newlist->head = NULL;
    newlist->nname = 0;
    
    return (newlist);
}

Cells *cell_new(void){
    struct cell *newlist;
    newlist = malloc(sizeof(struct cell));
    if (newlist == NULL)
    {
        return NULL;
    }
    
    newlist->head = NULL;
    newlist->next = NULL;
    newlist->prev = NULL;
    
    newlist->cname = 0;
    newlist->area = 0;
    
    return (newlist);
}


void InitCellVector (CellVector *vector ) {
    vector->used = 0;
    vector->capacity = VectorInitialSize;
    vector->NetList = malloc(vector->capacity * sizeof(Nets));
}

void InitNetVector (NetVector *vector ) {
    vector->used = 0;
    vector->capacity = VectorInitialSize;
    vector->CellList = malloc(vector->capacity * sizeof(Cells));
}

/*void InitAreaVector (AreaVector *vector ) {
    vector->used = 0;
    vector->capacity = VectorInitialSize;
    vector->area = malloc(vector->capacity * sizeof(long));
}*/



void InsertCellVector (CellVector *vector, long CellNumber, long NetNumber) {
    Nets *net;
    Nets *NetHead;
    
    if (CellNumber > vector->used) {
        vector->used = CellNumber;
    }
    
    if (vector->used >= vector->capacity) {
        vector->capacity = vector->used*2;
        vector->NetList = realloc(vector->NetList, vector->capacity * sizeof(Nets));
    }
    
    NetHead = &vector->NetList[CellNumber];
    net = net_new();
    net->nname = NetNumber;
    
    if (NetHead->next == NULL) {
        vector->used++;
    }
    while (NetHead->next != NULL) {
        NetHead = NetHead->next;
    }
    NetHead->next = net;
}

void InsertNetVector (NetVector *vector, long CellNumber, long NetNumber, NetVector *areavector) {
    
    Cells *cell;
    Cells *CellHead;

    if (vector->used + 30 >= vector->capacity) {
        vector->capacity = vector->used *1024;
        vector->CellList = realloc(vector->CellList, vector->capacity * sizeof(Cells));
    }
    
    CellHead = &vector->CellList[NetNumber];
    cell = cell_new();
    cell->cname = CellNumber;
    //determined this is an axxx cell, has area.
    if (CellNumber % 2 != 0) {
        //printf("CellNumber: %ld; Area: %ld\n", (CellNumber-1)/2, areavector->area[(CellNumber-1)/2]);
        //printf("NetNumber: %ld\n", NetNumber);
        cell->area = areavector->CellList[(CellNumber-1)/2].area;
    }
    
    //if cellhead->next == NULL, means a new net, then used++
    if (CellHead->next == NULL) {
        vector->used++;
    }
    while (CellHead->next != NULL) {
        CellHead = CellHead->next;
    }
    CellHead->next = cell;
}

void InsertAreaVector (NetVector *areavector, long area){
    
    if (areavector->used >= areavector->capacity) {
        areavector->capacity = areavector->used*1024;
        areavector->CellList = realloc(areavector->CellList, areavector->capacity * sizeof(Cells));
    }
    areavector->CellList[areavector->used].area = area;
    areavector->CellList[areavector->used].cname = areavector->used;
    areavector->used++;
}


int ReadAreaFile(const char AreFilename[], NetVector *Areavector1){
    FILE *fp;
    char StrLine[25];
    int i;
    long area;
    
    if((fp = fopen(AreFilename,"r")) == NULL){
        printf("Error2!: Can't find the file: %s. Please try again with correct file name\n",AreFilename);
        return -1;
    }
    
    area = 0;
    while (!feof(fp)) {
        i = 0;
        fgets(StrLine, 25, fp);
        if (StrLine[0] == 'a') {
            while (StrLine[i] != ' ') {
                i++;
            }
            area = atol(&StrLine[i+1]);
            //printf("%ld\n",area);
            InsertAreaVector(Areavector1, area);
        } else break;
    }
    fclose(fp);
    return 0;
}


int ReadNetFile(const char NetFilename[],
                CellVector *ACellVector, CellVector *PCellVector,
                NetVector *NetVector1, NetVector *AreaVector1){
    FILE *fp;
    int i;
    char StrLine[25];
    long cellnumber = 0;
    long netnumber = 0;
    
    if((fp = fopen(NetFilename,"r")) == NULL){
        printf("Error2!: Can't find the file: %s. Please try again with correct file name\n",NetFilename);
        return -1;
    }
    
    //skip the first five lines of fp1
    for (i = 0; i < 5; i++) {
        fgets(StrLine, 25, fp);
    }
    
    while (!feof(fp)){
        fgets(StrLine,25,fp);
        i = 0;
        
        //locate 's', get the net number tstrchr(). function works but a little bit complicated than this
        while (StrLine[i] != ' ') {
            i++;
        }
        if (StrLine[i+1] == 's') {
            netnumber++;
        }
        
        //use atol to convert cellnumber to long integer
        cellnumber = atol(&StrLine[1]);
        if (StrLine[0] == 'p') {
            InsertCellVector(PCellVector, cellnumber, netnumber);
            InsertNetVector(NetVector1, cellnumber*2, netnumber, AreaVector1);
        } else if (StrLine[0] == 'a'){
            InsertCellVector(ACellVector, cellnumber, netnumber);
            InsertNetVector(NetVector1, cellnumber*2+1, netnumber, AreaVector1);
        }
        
    }
    
    fclose(fp);
    return 0;
}

void Output(const char filename[],
            CellVector *ACellVector, CellVector *PCellVector, NetVector *NetVector1){
    FILE* fp;
    char txtfilename[25];
    char newname[15];
    int j;
    long i;
    Cells *CellHead;
    Nets *NetHead;
    
    //change "filename.net" to "filename.txt"
    for (j = 0; filename[j] != '.'; j++) {
        newname[j] = filename[j];
    }
    newname[j] = '.';
    newname[j+1] = 't';
    newname[j+2] = 'x';
    newname[j+3] = 't';
    newname[j+4] = '\0';
    
    //creat the standard filename team18_ibmxy.txt
    strcpy(txtfilename, "team18_");
    strcat(txtfilename, newname);
    if ((fp=fopen(txtfilename,"w"))==NULL)
    {
        printf("Error3!: cannot open file!");
        exit(0);
    }
    
    //write netvector into file:
    for (i = 1; i <= NetVector1->used; i++) {
        fprintf(fp, "net %ld: ",i);
        CellHead = &NetVector1->CellList[i];
        while (CellHead->next != NULL) {
            CellHead = CellHead->next;
            if (CellHead->cname % 2 == 0) {
                fprintf(fp, "p%ld ", CellHead->cname/2);
            }else{
                fprintf(fp, "a%ld ",(CellHead->cname-1)/2);
            }
        }
        
        fprintf(fp, "\n");
    }
    
    fprintf(fp, "\n\n");
    
    //write cellvector into file, p first
    for (i = 1; PCellVector->NetList[i].next != NULL; i++) {
        fprintf(fp, "p%ld: ",i);
        NetHead = &PCellVector->NetList[i];
        while (NetHead->next != NULL) {
            NetHead = NetHead->next;
            fprintf(fp, "net %ld ",NetHead->nname);
        }
        fprintf(fp, "\n");
    }
    
    for (i = 0; ACellVector->NetList[i].next != NULL; i++) {
        fprintf(fp, "a%ld: ",i);
        NetHead = &ACellVector->NetList[i];
        while (NetHead->next != NULL) {
            NetHead = NetHead->next;
            fprintf(fp, "net %ld ",NetHead->nname);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}
