//
//  main.c
//  FM
//
//  Created by Jinhuan Li on 10/7/14.
//  Copyright (c) 2014 Jinhuan Li. All rights reserved.
//

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "DataStructure.h"
#include "FM.h"

int main(int argc, const char * argv[]) {
    char filename[15],filename2[15];
    
    CellVector ACellVector, PCellVector, FreeNets;
    NetVector NetVector1, FreeACells, FreePCells;
    
    ArraySize ArraySize1;
    long i;
    //strcpy(filename, argv[1]);
    //strcpy(filename2, argv[2]);
    
    //use scanf for debug:
    printf(":");
    scanf("%s",filename);
    scanf("%s",filename2);
    
    ArraySize1 = *arraysize_new();
    GetArraySize(filename, &ArraySize1);
    
    //Vector initializations
    InitCellVector(&PCellVector, ArraySize1.TotalPCells);
    InitCellVector(&ACellVector, ArraySize1.TotalACells);
    InitCellVector(&FreeNets, ArraySize1.TotalNets);
    
    InitNetVector(&FreeACells, ArraySize1.TotalACells);
    InitNetVector(&FreePCells, ArraySize1.TotalPCells);
    InitNetVector(&NetVector1, ArraySize1.TotalNets);
    
    //if ((argv[2] != NULL) && (argv[1] != NULL)){
        ReadAreaFile(filename2, &FreeACells, &FreePCells);
    
        ReadNetFile(filename, &ACellVector, &PCellVector, &NetVector1, &FreeACells);
    
        Output(filename, &ACellVector, &PCellVector, &NetVector1);
        
        MergeSort(FreeACells.CellList, ArraySize1.TotalACells);
        for (i = 0; i < ArraySize1.TotalACells; ++i) {
            printf("a%ld: %ld\n",FreeACells.CellList[i].cname,FreeACells.CellList[i].area);
        }
    
        free(ACellVector.NetList);
        free(PCellVector.NetList);
        free(NetVector1.CellList);
        free(FreeACells.CellList);
        free(FreePCells.CellList);
        free(FreeNets.NetList);
        return 0;
    /*}else {
        printf("Error1!: The .net file is necessary, please indicate the .net file like this: \n./team18.out ibm01.net ibm01.are");
        return 0;
    }*/
    
}