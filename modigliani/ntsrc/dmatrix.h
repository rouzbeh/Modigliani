/** dmatrix.h
 Header-File for a Double-Matrix-Class
 Released 14.7.1997 , afaisal@techfak.uni-bielefeld.de
 */
/*
 * Dmatrix - a straight-forward double matrix class
 * Version:  0.1
 * Copyright (C) 1997 Ahmed Aldo Faisal
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// $Id: dmatrix.h,v 1.3 2003/06/20 16:15:09 face Exp $
// $Log: dmatrix.h,v $
// Revision 1.3  2003/06/20 16:15:09  face
// *** empty log message ***
//
// Revision 1.2  2003/06/20 13:26:00  face
// *** empty log message ***
//
// Revision 1.1  2001/06/11 13:18:56  face
// *** empty log message ***
//
// Revision 1.1.1.1  1999/07/03 15:17:20  face
// Switching from RCS to CVS to improve
// the handling of the new src code subdirectory
// structure. aaf
//
//
// Revision 1.2  1999/06/06 23:51:28  face
// *** empty log message ***
//
// Revision 1.1  1999/04/20 13:47:34  face
// Initial revision
//
// Revision 1.1  1999/04/20 13:46:50  face
// Initial revision
//
// Revision 1.8  1999/01/07 23:52:56  face
// *** empty log message ***
//
// Revision 1.8  1998/12/25 21:28:17  face
//  new NetTrader version 0.2 set with this revision of this file
//
// Revision 1.7  1998/10/19 14:09:03  face
// *** empty log message ***
//
// Revision 1.6  1998/07/07 12:10:15  face
// *** empty log message ***
//
// Revision 1.5  1998/05/22 11:42:22  face
// added gnu pl
//

/**
  Genuine double matrix class
  IMPORTANT : INDICES in Matrix START with (1,1)
             and end with (row,col) !

 NOTE :
 DESTRUCTIVE methods used!
 Be carefull because currently methods and operators
 might modify the (*this) object and some generate a new
 one
 */
#ifndef _dmatrix_h_
#define _dmatrix_h_

#include <iostream>
#include <cstdlib>

//   needed for memcpy:
#include <cstring>
//   i know, it`s not very nice
//   actually memory.h would be
//   nicer, though I don`t know
//   if it is a standard (in UNIX)
//   the rest is the nicer compatibility stuff (cutted`n pasted)
#ifdef _MSC_VER
#include <memory.h>
#endif
#ifdef _BORLANDC_
#include <mset.h>
#endif



class DMatrix
{
private:
    int row,col;
    double *elements;

public:
    DMatrix(int,int);/** init Matrix */
    DMatrix(int);	   /** init Vector */
    DMatrix(DMatrix &); /** duplicate from existent DMatrix object */

    ~DMatrix() {
        delete elements;
    }

    DMatrix operator+(DMatrix &); //!< addition C = A+B
    DMatrix operator-(DMatrix &); //!< subtraction C = A-B
    DMatrix operator*(DMatrix &); //!< multiplication A = A*B
    DMatrix operator*(double); //!< Scaling, factor multiplication (from the RIGHT)
    DMatrix &operator=(DMatrix &);//!< equalizing :) A=B

    double &operator()(int,int); //!< Access matrix components
    double &operator()(int); //!< Access vector components

    void print();
    double sum();
    double quadSum();
    DMatrix operator~();
    friend std::istream &operator>>(std::istream &,DMatrix &);

};

DMatrix::DMatrix(int r,int c)
{
    elements = new double[r*c+1];
    memset(elements,' ',(r*c+1)*sizeof(double));
    row = r;
    col = c;
}

DMatrix::DMatrix(int r)
{
    elements = new double[r+1];
    memset(elements,' ',(r+1)*sizeof(double));
    row = r;
    col = 1;
}

DMatrix::DMatrix(DMatrix &copy)
{
    row = copy.row;
    col = copy.col;
    elements = new double[row*col+1];
    memcpy(elements,copy.elements,(row*col+1)*sizeof(double));
}

DMatrix DMatrix::operator+(DMatrix &right)
{
    DMatrix left(*this);
    for (int i = 1; i <= row*col; i++)
        left.elements[i] = left.elements[i] + right.elements[i];
    return (left);
}

DMatrix DMatrix::operator-(DMatrix &right)
{
    DMatrix left(*this);
    for (int i = 1; i <= row*col; i++)
    {
        left.elements[i] = left.elements[i] - right.elements[i];
    }
    return (left);
}

DMatrix DMatrix::operator*(DMatrix &right)
{
    DMatrix prod(row,right.col);
    for (int i = 1; i <= row; i++)
    {
        for (int j = 1; j <= right.col; j++)
        {
            prod(i,j) = 0;
            for (int k = 1; k <= col; k++)
                prod(i,j) = prod(i,j) + (*this)(i,k)*right(k,j);
        }
    }
    return (prod);
}


DMatrix DMatrix::operator*(double factor)
{
    for (int i = 1; i <= row; i++)
    {
        for (int j = 1; j <= col; j++)
        {
            (*this)(i,j) =  (*this)(i,j) * factor;
        }
    }
    return (*this);
}


DMatrix &DMatrix::operator=(DMatrix &right)
{
    if (this == &right) return (*this);
    else
    {
        delete elements;
        elements = new double[row*col+1];
        memcpy(elements,right.elements,(row*col+1)*sizeof(double));
        return (*this);
    }
}



double &DMatrix::operator()(int r,int c)
{
    return (elements[(r-1)*col+c]);
}



double &DMatrix::operator()(int r)
{
    return (elements[r]);
}



void DMatrix::print()
{
    std::cout.setf(std::ios::fixed);
    std::cout.precision(4);
    std::cout.width(8);
    for (int i = 1; i <= row; i++)
    {
        for (int j = 1; j <= col; j++)
            std::cout << (*this)(i,j);
        std::cout << std::endl;
    }
}



double DMatrix::sum()
{
    double temp = 0.0;
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            temp += (*this)(i,j);
    return (temp);
}

double DMatrix::quadSum()
{
    double temp = 0.0;
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            temp += (*this)(i,j)*(*this)(i,j);
    return (temp);
}


/** compute inverse with  Gauss-elimination including Pivotizing */
DMatrix DMatrix::operator~()
{
    int i, j, k, n;
    int pivrow, tarrow;
    double pivelt, tarelt;
    n = row; // Zahl d.Gleich.
    DMatrix aug(n,2*col);
    DMatrix inverse(n,col);
    for (i = 1; i <= n; i++)
        for (j = 1; j <= col; j++)
            aug(i,j) = (*this)(i,j);
    for (i = 1; i <= n; i++)
        for (j = n+1; j <= aug.col; j++)
            if (i == j-n) aug(i,j) = 1; // Einheitsmatrix
    for (pivrow = 1; pivrow <= n; pivrow++)
    {
        pivelt = aug(pivrow,pivrow);
        if (pivelt == 0)
        {
            k = pivrow + 1;	// Pivotsuche
            while (pivelt == 0 && k <= n)
            {
                pivelt = aug(k,pivrow);
                k++;
            }
            if (pivelt == 0)
            {
                std::cerr << "dmatrix : ERROR - Inversion attempted, but matrix is singular!";
                exit(1);
            }
        }
        else
        {
            k--;
            DMatrix dum(2*col);
            for (i = 1; i <= 2*col; i++)
                dum(i) = aug(pivrow,i);
            for (i = 1; i <= 2*col; i++)
                aug(pivrow,i) = aug(k,i);
            for (i = 1; i <= 2*col; i++)
                aug(k,i) = dum(i);
        }
    }

    for (j = 1; j <= 2*col; j++)
        aug(pivrow,j) = aug(pivrow,j)/pivelt;
    for (tarrow = 1; tarrow <= n; tarrow++)
        if (tarrow != pivrow)
        {
            tarelt = aug(tarrow,pivrow);
            for (j = 1; j<= 2*col; j++)
                aug(tarrow,j) = aug(tarrow,j) - aug(pivrow,j)*tarelt;
        }
    for (i = 1; i <= n; i++)
        for (j = n+1; j <= aug.col; j++)
            inverse(i,j-n) = aug(i,j);
    return (inverse);
}



std::istream &operator>>(std::istream &is,DMatrix &m)
{
    for (int i = 1; i <= m.row; i++)
        for (int j = 1; j <= m.col; j++)
            is >> m(i,j);
    return (is);
}



#endif  /* _dmatrix_h_ */

















