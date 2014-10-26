//
//  DataStructure.h
//  FM
//
//  Created by Jinhuan Li on 10/7/14.
//  Copyright (c) 2014 Jinhuan Li. All rights reserved.
//

#ifndef __FM__DataStructure__
#define __FM__DataStructure__

#include <stdio.h>

#endif /* defined(__FM__DataStructure__) */

#include <stdlib.h>
#include <string.h>

typedef struct cell {
    long cname;
    struct cell *next;
    long area;
    
    //0 means on the left; 1 means on the right
    int partition;
}Cells;

typedef struct net {
    long nname;
    struct net *next;
}Nets;

//store the total number of nets and cells
typedef struct arraysize {
    long TotalNets;
    long TotalPCells;
    long TotalACells;
}ArraySize;

// Each slot of the CellVector is a cell, stores the nets where this cell belongs
typedef struct cellvector {
    long capacity;
    Nets *NetList;
}CellVector;

// Each slot of the NetVector is a net, stores the cells it contains
typedef struct netvector{
    long capacity;
    Cells *CellList;
}NetVector;




//initial a new net/cell structure
Nets *net_new(void);
Cells *cell_new(void);
ArraySize *arraysize_new (void);

//initial a new net/cell vector
int InitCellVector (CellVector *vector, long size);
int InitNetVector (NetVector *vector, long size);

void InsertCellVector (CellVector *vector, long CellNumber, long NetNumber);
void InsertNetVector (NetVector *vector, long CellNumber, long NetNumber, NetVector *freecell);

void InsertFreeACell (NetVector *freeacell, long area, long cname);
void InsertFreePCell (NetVector *freepcell);

int GetArraySize(const char NetFilename[], ArraySize *ArraySize1);

int ReadNetFile(const char NetFilename[],
                CellVector *ACellVector, CellVector *PCellVector,
                NetVector *NetVector1, NetVector *freeacell);
int ReadAreaFile(const char AreFilename[], NetVector *freeacell, NetVector *freepcell);

void Output(const char filename[],
            CellVector *ACellVector, CellVector *PCellVector, NetVector *NetVector1);







