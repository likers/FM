//
//  FM.c
//  FM
//
//  Created by Jinhuan Li on 10/14/14.
//  Copyright (c) 2014 Jinhuan Li. All rights reserved.
//

#include "FM.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MergeSort(Cells *celllist, long size){
    if (size > 1) {
        
        //divide vector into two equal parts
        Cells *celllist1 = celllist;
        long celllist1_size = size/2;
        Cells *celllist2 = celllist + size/2;
        long celllist2_size = size - celllist1_size;
        
        //merge and sort
        MergeSort(celllist1, celllist1_size);
        MergeSort(celllist2, celllist2_size);
        
        //merge two arrays
        MergeVector(celllist1, celllist1_size, celllist2, celllist2_size);
        
    }
}

void MergeVector(Cells *celllist1, long celllist1_size,
                 Cells *celllist2, long celllist2_size){
    long i,j,k;
    i = j = k = 0;
    //call temp list to store merged result
    Cells *templist = malloc((celllist1_size + celllist2_size)*sizeof(Cells));
    
    //put smaller one into result list, then move pointer
    while (i < celllist1_size && j < celllist2_size) {
        if (celllist1[i].area <= celllist2[j].area) {
            templist[k].area = celllist1[i].area;
            templist[k].cname = celllist1[i].cname;
            k++;
            i++;
        } else {
            templist[k].area = celllist2[j].area;
            templist[k].cname = celllist2[j].cname;
            k++;
            j++;
        }
    }
    //if celllist1 has cells, put into result
    while (i < celllist1_size) {
        templist[k].area = celllist1[i].area;
        templist[k].cname = celllist1[i].cname;
        k++;
        i++;
    }
    //if celllist2 has cells, put into result
    while (j < celllist2_size) {
        templist[k].area = celllist2[j].area;
        templist[k].cname = celllist2[j].cname;
        k++;
        j++;
    }
    //copy result list into celllist1
    for (long m = 0; m < (celllist1_size + celllist2_size); ++m) {
        celllist1[m].area = templist[m].area;
        celllist1[m].cname = templist[m].cname;
    }
    free(templist);
}
