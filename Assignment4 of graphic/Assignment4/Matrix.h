//
//  Matrix.h
//  Assignment-1
//
//  Created by Yixin Yin on 15-2-10.
//
//

#ifndef __Assignment_1__Matrix__
#define __Assignment_1__Matrix__

#include <stdio.h>

#endif /* defined(__Assignment_1__Matrix__) */
//
//  Matrix.h
//  Assignment-1
//
//  Created by Yixin Yin on 15-2-9.
//
//


class Matrix{
public:
    int row = 0, column = 0;
    float val[4][4];
public:
    Matrix(int r, int c)
    {
        row = r;
        column = c;
        for(int i = 0; i < r; i++)
        {
            for(int j = 0; j < c; j++)
            {
                val[i][j] = 0;
            }
        }
    }
    void setMatrix(Matrix a)
    {
        for(int i = 0; i < a.row; i ++)
        {
            for(int j = 0; j < a.column; j++)
            {
                val[i][j] = a.val[i][j];
            }
        }
    }
    
    void setIdentity(int r, int c)
    {
        if(r == c)
        {
            for(int i = 0; i < r; i++)
            {
                for(int j = 0; j < c; j++)
                {
                    if(i == j)
                        val[i][j] = 1;
                    else
                        val[i][j] = 0;
                }
            }
        }
    }
    
    Matrix mlt(Matrix x)//4*4 and 4*1
    {
        Matrix mat(4,1);
        float temp[4][1];
        for(int i = 0 ; i < 4 ; i++)
        {
            for(int j = 0; j < 1; j++)
                temp[i][j] = 0;
        }
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 1; j++)
            {
                for(int k = 0; k < 4; k++)
                {
                    mat.val[i][j] = mat.val[i][j] + val[i][k]*x.val[k][j];
                }
            }
            
        }
        return mat;
    }
    Matrix mlt2(Matrix x)//4*4 and 4*4
    {
        Matrix mat(4,4);
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4;j++)
            {
                for(int k = 0 ; k < 4; k++)
                {
                    mat.val[i][j] = mat.val[i][j] + val[i][k]*x.val[k][j];

                }
            }
        }
        
        return mat;
        
    }
    
    
};
