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
#define VectorInitialSize 100;

typedef struct cell {
    long cname;
    struct cell *next, *prev, *head;
    long area;
}Cells;

typedef struct net {
    long nname;
    struct net *next, *prev, *head;
}Nets;

// Each slot of the CellVector is a cell, stores the nets where this cell belongs
typedef struct {
    long used;
    long capacity;
    Nets *NetList;
}CellVector;

// Each slot of the NetVector is a net, stores the cells it contains
typedef struct {
    long used;
    long capacity;
    Cells *CellList;
}NetVector;

typedef struct {
    long used;
    long capacity;
    long *area;
}AreaVector;


//initial a new net/cell structure
Nets *net_new(void);
Cells *cell_new(void);

//initial a new net/cell vector
void InitCellVector (CellVector *vector );
void InitNetVector (NetVector *vector );
void InitAreaVector (AreaVector *vector );

void InsertCellVector (CellVector *vector, long CellNumber, long NetNumber);
void InsertNetVector (NetVector *vector, long CellNumber, long NetNumber, AreaVector *arevector);
void InsertAreaVector (AreaVector *areavector, long area);

int ReadNetFile(const char NetFilename[],
                CellVector *ACellVector, CellVector *PCellVector,
                NetVector *NetVector1, AreaVector *areavector);
int ReadAreaFile(const char AreFilename[], AreaVector *areavector);

void Output(const char filename[],
            CellVector *ACellVector, CellVector *PCellVector, NetVector *NetVector1);







