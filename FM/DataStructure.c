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
    
    newlist->next = NULL;
    newlist->cname = 0;
    newlist->area = 0;
    
    return (newlist);
}

ArraySize *arraysize_new(void){
    struct arraysize *newarraysize;
    newarraysize = malloc(sizeof(struct arraysize));
    if (newarraysize == NULL)
    {
        return NULL;
    }
    newarraysize->TotalACells = 0;
    newarraysize->TotalNets = 0;
    newarraysize->TotalPCells = 0;
    return (newarraysize);
}


int InitCellVector (CellVector *vector, long size) {
    vector->capacity = size;
    vector->NetList = calloc(vector->capacity, sizeof(Nets));
    if (vector->NetList == NULL)
    {
        return -1;
    } else return 0;
    
}

int InitNetVector (NetVector *vector, long size) {
    vector->capacity = size;
    vector->CellList = calloc(vector->capacity, sizeof(Cells));
    if (vector->CellList == NULL)
    {
        return -1;
    } else return 0;
}

void InsertCellVector (CellVector *vector, long CellNumber, long NetNumber) {
    Nets *net;
    Nets *NetHead;
    
    
    NetHead = &vector->NetList[CellNumber];
    net = net_new();
    net->nname = NetNumber;
    
    while (NetHead->next) {
        NetHead = NetHead->next;
    }
    NetHead->next = net;
    
}


void InsertNetVector (NetVector *vector, long CellNumber, long NetNumber, NetVector *areavector) {
    
    Cells *cell;
    Cells *CellHead;
    
    CellHead = &vector->CellList[NetNumber];
    cell = cell_new();
    cell->cname = CellNumber;
    //determined this is an axxx cell, has area.
    if (CellNumber % 2 != 0) {
        cell->area = areavector->CellList[(CellNumber-1)/2].area;
    }
    
    while (CellHead->next != NULL) {
        CellHead = CellHead->next;
    }
    CellHead->next = cell;
}

void InsertAreaVector (NetVector *areavector, long area, long cellnumber){
    Cells *CellHead;
    CellHead = &areavector->CellList[cellnumber];
    CellHead->area = area;
    CellHead->cname = cellnumber;
    //printf("cname:%ld\n",CellHead->cname);
    CellHead->next = NULL;
    
}

int GetArraySize(const char NetFilename[], ArraySize *ArraySize1){
    
    FILE *fp;
    char StrLine[15];
    int i;
    long Total = 0;
    fp = fopen(NetFilename, "r");
    if (!fp) {
        printf("Error2!: Can't find the file: %s. Please try again with correct file name\n",NetFilename);
        return -1;
    }
    
    for (i = 0; i < 5; i++) {
        fgets(StrLine, 25, fp);
        if (i == 2) {
            ArraySize1->TotalNets = atol(&StrLine[0]);
        }
        if (i == 3) {
            Total = atol(&StrLine[0]);
        }
        if (i == 4) {
            ArraySize1->TotalACells = atol(&StrLine[0])+1;
        }
    }
    ArraySize1->TotalPCells = Total - ArraySize1->TotalACells;
    fclose(fp);
    return 0;
}


int ReadAreaFile(const char AreFilename[], NetVector *Areavector1){
    FILE *fp;
    char StrLine[25];
    int i;
    long j = 0;
    long area = 0;
    
    if((fp = fopen(AreFilename,"r")) == NULL){
        printf("Error2!: Can't find the file: %s. Please try again with correct file name\n",AreFilename);
        return -1;
    }
    
    while (fgets(StrLine, 25, fp) != NULL) {
        i = 0;
        
        //only need to read aXXX cells, all pXXX cells' area == 0
        if (StrLine[0] == 'a') {
            while (StrLine[i] != ' ') {
                i++;
            }
            area = atol(&StrLine[i+1]);
            //printf("%ld\n",area);
            InsertAreaVector(Areavector1, area, j);
        } else break;
        j++;
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
    while (fgets(StrLine,25,fp) != NULL){
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
    for (i = 1; i <= NetVector1->capacity; i++) {
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
