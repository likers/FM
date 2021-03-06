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
#include "DataStructure.h"

int main(int argc, const char * argv[]) {
    char filename[15],filename2[15];
    CellVector ACellVector, PCellVector;
    NetVector NetVector1;
    NetVector AreaVector1;
    ArraySize *ArraySize1;

    //strcpy(filename, argv[1]);
    //strcpy(filename2, argv[2]);
    //use scanf for debug:
    printf(":");
    scanf("%s",filename);
    scanf("%s",filename2);
    
    ArraySize1 = arraysize_new();
    GetArraySize(filename, ArraySize1);
    
    InitCellVector(&PCellVector, &ArraySize1->TotalPCells);
    InitCellVector(&ACellVector, &ArraySize1->TotalACells);
    InitNetVector(&NetVector1, &ArraySize1->TotalNets);
    InitNetVector(&AreaVector1, &ArraySize1->TotalACells);
    //if ((argv[2] != NULL) && (argv[1] != NULL)){
        ReadAreaFile(filename2, &AreaVector1);
        ReadNetFile(filename, &ACellVector, &PCellVector, &NetVector1, &AreaVector1);
        Output(filename, &ACellVector, &PCellVector, &NetVector1);
    
        free(ACellVector.NetList);
        free(PCellVector.NetList);
        free(NetVector1.CellList);
        free(AreaVector1.CellList);
        free(ArraySize1);
        return 0;
    /*}else {
        printf("Error1!: The .net file is necessary, please indicate the .net file like this: \n./team18.out ibm01.net ibm01.are");
        return 0;
    }*/
    
}