//
//  main.cpp
//  DataStruct_Prog_hw_1
//
//  Created by Michael Leong on 04/10/2022.
//

#include <iostream>
#define MAX_COL 1024
#define MAX_TERMS 1024
#define MAX_MATRIX 16

using namespace std;

typedef struct _term {
    int row;
    int col;
    int val;
} term;

typedef struct _Matrices {
    string name;
    term *matrix;
} Matrices;

Matrices matList[MAX_MATRIX];
static int topOfList = 0;

void initSparseMatrix(term a[], int size) {
    for(int i = 0; i <= size; i++) {
        a[i].row = 0;
        a[i].col = 0;
        a[i].val = 0;
    }
}

term *setSparseMatrix() {
    term *a;
    int noOfRow;
    int noOfCol;
    int noOfNonZeroTerms;
    
    cout << "Enter number of rows: ";
    cin >> noOfRow;
    
    cout << "Enter number of columns: ";
    cin >> noOfCol;
    
    cout << "Enter number of non zero terms: ";
    cin >> noOfNonZeroTerms;
    
    a = new term[noOfNonZeroTerms+1];
    //init matrix
    initSparseMatrix(a, noOfNonZeroTerms+1);
    
    a[0].row = noOfRow;
    a[0].col = noOfCol;
    a[0].val = noOfNonZeroTerms;
    
    cout << "Input data row-wise\n";
    for(int i = 1; i <= noOfNonZeroTerms; i++) {
        cout << "Enter term row(index starting from 1): ";
        cin >> a[i].row;
        cout << "Enter term col(index starting from 1): ";
        cin >> a[i].col;
        cout << "Enter term value: ";
        cin >> a[i].val;
    }
    
    return a;
}

//delete matrix (free memory)
void deleteSparseMatrix(term *a) {
    delete a;
}

void copyMat(term a[], term b[], int size) {
    //a copy to b
    for(int i = 0; i <= size; i++) {
        b[i].row = a[i].row;
        b[i].col = a[i].col;
        b[i].val = a[i].val;
    }
}

//print matrix
//time complexity o(number of non zero terms in matrix a)
void printMat(term a[]) {
    for(int i = 0; i < a[0].val+1; i++) {
        //just in case
        if(a[i].val == 0) {
            //if zero then don't need to print
            continue;
        }
        cout << "a[" << i << "]";
        cout << "row: " << a[i].row << " col: " << a[i].col << " val: " << a[i].val << '\n';
    }
}

//submatrix
void getSubMatrix(term a[], term result[]) {
    int rowStartId = 0;
    int rowEndId = 0;
    int colStartId = 0;
    int colEndId = 0;
    int noOfTerms = 1;
    cout << "Enter starting row index: ";
    cin >> rowStartId;
    cout << "Enter end row index: ";
    cin >> rowEndId;
    cout << "Enter starting col index: ";
    cin >> colStartId;
    cout << "Enter end col index: ";
    cin >> colEndId;
    
    for(int i = 1; i <= a[0].val; i++) {
        if(a[i].row >= rowStartId && a[i].row <= rowEndId && a[i].col >= colStartId && a[i].col <= colEndId) {
            result[noOfTerms].row = a[i].row;
            result[noOfTerms].col = a[i].col;
            result[noOfTerms].val = a[i].val;
            noOfTerms++;
        }
    }
    
    //set row, col, number of terms for submatrix b
    result[0].val = noOfTerms-1;
    result[0].row = rowEndId-rowStartId+1;
    result[0].col = colEndId-colStartId+1;
}

void eleWiseMult(term a[], term b[], term result[]) {
    if (a[0].row != b[0].row || a[0].col != b[0].col) {
        cout << "Matrices can't be element wise multiplied\n";
        return;
    }
    //else
    int resultIndex = 1;
    int aIndex = 1;
    int bIndex = 1;
    //set row and col
    result[0].row = a[0].row;
    result[0].col = a[0].col;
    result[0].val = 0;
    while(aIndex <= a[0].val && bIndex <= b[0].val) {
        if(a[aIndex].row > b[bIndex].row || (a[aIndex].row == b[bIndex].row && a[aIndex].col > b[bIndex].col)) {
            bIndex++;
        } else if(a[aIndex].row < b[bIndex].row || (a[aIndex].row == b[bIndex].row && a[aIndex].col < b[bIndex].col)) {
            bIndex++;
        } else {
            result[resultIndex].row = a[aIndex].row;
            result[resultIndex].col = a[aIndex].col;
            result[resultIndex].val = a[aIndex].val * b[bIndex].val;
            cout << "temp: " << a[aIndex].val * b[bIndex].val << '\n';
            resultIndex++;
            aIndex++;
            bIndex++;
        }
    }
    //set number of non-zero terms
    result[0].val = resultIndex-1;
    
}

//fast transpose
//time complexity: o(cols)+o(elements)
void fastTranspose(term a[], term b[]) {
    int nonZeroRow[MAX_COL] = {0}, startingPos[MAX_COL] = {0};
    b[0].col = a[0].row;
    b[0].row = a[0].col;
    b[0].val = a[0].val;
    for (int i = 1; i <= a[0].val; i++) {
        nonZeroRow[a[i].col]++;
    }
    startingPos[0] = 1;
    for (int i = 1; i <= a[0].col; i++) {
        startingPos[i] = startingPos[i-1] + nonZeroRow[i-1];
    }
    for (int i = 1; i <= a[0].val; i++) {
        int j = startingPos[a[i].col]++;
        b[j].row = a[i].col;
        b[j].col = a[i].row;
        b[j].val = a[i].val;
    }
}

void storeSum (term d[], int *totalD, int row, int column, int *sum) {
    if (*sum) {
        if (*totalD < MAX_TERMS) {
            d[++*totalD].row = row;
            d[*totalD].col = column;
            d[*totalD].val = *sum;
            *sum = 0;
        }
    }
}

int compareTerm(int a, int b) {
    if(a == b) {
        return 0;
    } else if(a > b) {
        return 1;
    } else if(a < b) {
        return -1;
    }
    
    return 0;
}

//sparse matrix multiplication
void sparseMatMult(term a[], term b[], term d[]) {
    term newB[MAX_TERMS];
    fastTranspose(b, newB);
    int column, row = a[1].row, sum = 0;
    int totalD = 0;
    int rowsA = a[0].row, colsB = b[0].col;
    int rowBegin = 1;
    
    a[a[0].val+1].row = rowsA;
    newB[b[0].val+1].col = colsB;
    newB[b[0].val+1].col = 0;
    
    for (int i = 1; i <= a[0].val; ) {
        column = newB[1].row;
        for (int j = 1; j <= b[0].val+1; ) {
            if (a[i].row != row) {
                storeSum(d, &totalD, row, column, &sum);
                i = rowBegin;
                for(; newB[j].row == column; j++);
                column = newB[j].row;
            } else if (newB[j].row != column) {
                storeSum(d, &totalD, row, column, &sum);
                i = rowBegin;
                column = newB[j].row;
            } else switch (compareTerm(a[i].col, newB[j].col)) {
                case -1:
                    i++;
                    break;
                case 0:
                    sum += (a[i++].val * newB[j++].val);
                    break;
                case 1:
                    j++;
            }
        }
        for (; a[i].row == row; i++);
        rowBegin = i;
        row = a[i].row;
    }
    d[0].row = rowsA;
    d[0].col = colsB;
    d[0].val = totalD;
}

void powMat(term a[], term result[], int power) {
    if(power < 2) {
        cout << "power must be more than 2\n";
        return;
    }
    
    copyMat(a, result, a[0].val);
    cout << "resultMat after copy\n";
    printMat(result);
    
    for(int i = 0; i < power-1; i++) {
        sparseMatMult(a, result, result);
    }
}

void printMatList() {
    cout << "----current matrices----\n";
    for(int i = 0; i < topOfList; i++) {
        cout << matList[i].name << '\n';
    }
    cout << "------------------------\n";
}

void createMatUI() {
    cout << "Enter matrix name: ";
    cin >> matList[topOfList].name;
    matList[topOfList].matrix = setSparseMatrix();
    topOfList++;
}

void printMatUI() {
    printMatList();
    string searchName;
    cout << "Enter matrix to print: ";
    cin >> searchName;
    bool found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            found = true;
            cout << "------matrix: " << matList[i].name << "------\n";
            printMat(matList[i].matrix);
            cout << "------------------\n";
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
    }
}

void printSubMatUI() {
    printMatList();
    string searchName;
    cout << "Enter matrix to print submatrix: ";
    cin >> searchName;
    bool found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            found = true;
            cout << "------matrix: " << matList[i].name << "------\n";
            term result[matList[i].matrix[0].val];
            initSparseMatrix(result, matList[i].matrix[0].val);
            getSubMatrix(matList[i].matrix, result);
            printMat(result);
            cout << "------------------\n";
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
    }
}

void printTransposeUI() {
    printMatList();
    string searchName;
    cout << "Enter matrix to print transpose: ";
    cin >> searchName;
    bool found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            found = true;
            cout << "------matrix: " << matList[i].name << "------\n";
            term result[matList[i].matrix[0].val];
            initSparseMatrix(result, matList[i].matrix[0].val);
            fastTranspose(matList[i].matrix, result);
            printMat(result);
            cout << "------------------\n";
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
    }
}

void printEleMultUI() {
    printMatList();
    int searchIndex1 = -1;
    int searchIndex2 = -1;
    string searchName;
    cout << "Enter first matrix to be multiplied: ";
    cin >> searchName;
    bool found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            found = true;
            searchIndex1 = i;
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
        return;
    }
    
    printMatList();
    cout << "Enter second matrix to be multiplied: ";
    cin >> searchName;
    found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            found = true;
            searchIndex2 = i;
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
        return;
    }
    
    term result[matList[searchIndex1].matrix[0].val+matList[searchIndex2].matrix[0].val];
    initSparseMatrix(result, matList[searchIndex1].matrix[0].val+matList[searchIndex2].matrix[0].val);
    eleWiseMult(matList[searchIndex1].matrix, matList[searchIndex2].matrix, result);
    cout << "------result matrix------\n";
    printMat(result);
}

void printMultUI() {
    printMatList();
    int searchIndex1 = -1;
    int searchIndex2 = -1;
    string searchName;
    cout << "Enter first matrix to be multiplied: ";
    cin >> searchName;
    bool found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            found = true;
            searchIndex1 = i;
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
        return;
    }
    
    printMatList();
    cout << "Enter second matrix to be multiplied: ";
    cin >> searchName;
    found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            found = true;
            searchIndex2 = i;
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
        return;
    }
    
    term result[matList[searchIndex1].matrix[0].val+matList[searchIndex2].matrix[0].val];
    initSparseMatrix(result, matList[searchIndex1].matrix[0].val+matList[searchIndex2].matrix[0].val);
    sparseMatMult(matList[searchIndex1].matrix, matList[searchIndex2].matrix, result);
    cout << "------result matrix------\n";
    printMat(result);
}

void printMatPowUI() {
    printMatList();
    string searchName;
    int power = 0;
    cout << "Enter matrix to power: ";
    cin >> searchName;
    bool found = false;
    for(int i = 0; i < topOfList; i++) {
        if(matList[i].name == searchName) {
            cout << "Enter power: ";
            cin >> power;
            found = true;
            cout << "------matrix: " << matList[i].name << "------\n";
            term result[matList[i].matrix[0].val];
            initSparseMatrix(result, matList[i].matrix[0].val);
            powMat(matList[i].matrix, result, power);
            printMat(result);
            cout << "------------------\n";
        }
    }
    if(!found) {
        cout << "matrix not found please try again\n";
    }
}

int main(int argc, const char * argv[]) {
    int input = 0;
    
    while(input != -1) {
        printMatList();
        cout << "Enter 1 to create matrix\n";
        cout << "Enter 2 to print matrix\n";
        cout << "Enter 3 to get submatrix\n";
        cout << "Enter 4 to get transpose\n";
        cout << "Enter 5 to do element wise product\n";
        cout << "Enter 6 to do matrix product\n";
        cout << "Enter 7 to power a matrix\n";
        cout << "Enter -1 to exit\n";
        cout << "Input: ";
        cin >> input;
        
        if(input == 1) {
            createMatUI();
        } else if(input == 2) {
            printMatUI();
        } else if(input == 3) {
            printSubMatUI();
        } else if(input == 4) {
            printTransposeUI();
        } else if(input == 5) {
            printEleMultUI();
        } else if (input == 6) {
            printMultUI();
        } else if(input == 7) {
            printMatPowUI();
        }
    }
    
    cout << "exit\n";
    
    return 0;
}
