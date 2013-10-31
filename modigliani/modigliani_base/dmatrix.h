/**
 * @file dmatrix.h
 * @brief DMatrix class header and implementation
 *
 * Copyright (C) 1997 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef MODIGLIANI_MODIGLIANI_BASE_DMATRIX_H_
#define MODIGLIANI_MODIGLIANI_BASE_DMATRIX_H_

#include <cstdlib>
#include <string>

namespace modigliani_base {
/**
 * @brief A straight-forward double matrix class
 *
 * @warning INDICES in Matrix START with (1,1) and end with (row,col) !
 * @warning DESTRUCTIVE methods used! Be carefull because currently methods and operators
 * might modify the (*this) object and some generate a new one
 */
  class DMatrix {
  public:
    /** init Matrix */
    DMatrix(int sizz_x, int size_y);

    /** init Vector */
    explicit DMatrix(int size);

    /** duplicate from existent DMatrix object */
     DMatrix(DMatrix & copy);

    ~DMatrix() {
      delete elements;
    }

    DMatrix operator+(DMatrix &);     // !< addition C = A+B

    DMatrix operator-(DMatrix &);       // !< subtraction C = A-B

    DMatrix operator*(DMatrix &);       // !< multiplication A = A*B

    DMatrix operator*(double factor);   // !< Scaling, factor
    // !multiplication

     DMatrix & operator=(DMatrix &);    // !< equalizing :) A=B

    double &operator() (int size_x, int size_y);        // !< Access matrix
    // !components

    double &operator() (int size);      // !< Access vector components

    void print();

    double sum();

    double quadSum();

    DMatrix operator~();

    friend std::istream & operator>>(std::istream &, DMatrix &);

  private:
    int row, col;
    double *elements;
  };

  DMatrix(int r, int c) {
    elements = new double[r * c + 1];
    memset(elements, ' ', (r * c + 1) * sizeof(elements[0]));
    row = r;
    col = c;
  }

  DMatrix(int r) {
    elements = new double[r + 1];
    memset(elements, ' ', (r + 1) * sizeof(elements[0]));
    row = r;
    col = 1;
  }

  DMatrix(modigliani_base::DMatrix & copy) {
    row = copy.row;
    col = copy.col;
    elements = new double[row * col + 1];
    memcpy(elements, copy.elements, (row * col + 1) * sizeof(elements[0]));
  }

  modigliani_base::DMatrix modigliani_base::DMatrix::
    operator+(modigliani_base::DMatrix & right) {
    modigliani_base::DMatrix left(*this);
    for (int i = 1; i <= row * col; i++)
      left.elements[i] = left.elements[i] + right.elements[i];
    return (left);
  }

  modigliani_base::DMatrix modigliani_base::DMatrix::
    operator-(modigliani_base::DMatrix & right) {
    modigliani_base::DMatrix left(*this);
    for (int i = 1; i <= row * col; i++) {
      left.elements[i] = left.elements[i] - right.elements[i];
    }
    return (left);
  }

  modigliani_base::DMatrix modigliani_base::DMatrix::
    operator*(modigliani_base::DMatrix & right) {
    modigliani_base::DMatrix prod(row, right.col);
    for (int i = 1; i <= row; i++) {
      for (int j = 1; j <= right.col; j++) {
        prod(i, j) = 0;
        for (int k = 1; k <= col; k++)
          prod(i, j) = prod(i, j) + (*this) (i, k) * right(k, j);
      }
    }
    return (prod);
  }

  modigliani_base::DMatrix modigliani_base::DMatrix::operator*(double factor) {
    for (int i = 1; i <= row; i++) {
      for (int j = 1; j <= col; j++) {
        (*this) (i, j) = (*this) (i, j) * factor;
      }
    }
    return (*this);
  }

  modigliani_base::DMatrix &
    modigliani_base::DMatrix::operator=(modigliani_base::DMatrix & right) {
    if (this == &right) {
      return (*this);
    } else {
      delete elements;
      elements = new double[row * col + 1];
      memcpy(elements, right.elements, (row * col + 1) * sizeof(elements[0]));
      return (*this);
    }
  }

  double &modigliani_base::DMatrix::operator() (int r, int c) {
    return (elements[(r - 1) * col + c]);
  }

  double &modigliani_base::DMatrix::operator() (int r) {
    return (elements[r]);
  }

  void modigliani_base::DMatrix::print() {
    std::cout.setf(std::ios::fixed);
    std::cout.precision(4);
    std::cout.width(8);
    for (int i = 1; i <= row; i++) {
      for (int j = 1; j <= col; j++)
        std::cout << (*this) (i, j);
      std::cout << std::endl;
    }
  }

  double modigliani_base::DMatrix::sum() {
    double temp = 0.0;
    for (int i = 1; i <= row; i++)
      for (int j = 1; j <= col; j++)
        temp += (*this) (i, j);
    return (temp);
  }

  double modigliani_base::DMatrix::quadSum() {
    double temp = 0.0;
    for (int i = 1; i <= row; i++)
      for (int j = 1; j <= col; j++)
        temp += (*this) (i, j) * (*this) (i, j);
    return (temp);
  }

/** compute inverse with  Gauss-elimination including Pivotizing */
  modigliani_base::DMatrix modigliani_base::DMatrix::operator~() {
    int i, j, k, n;
    int pivrow, tarrow;
    double pivelt, tarelt;
    n = row;                    // Zahl d.Gleich.
    modigliani_base::DMatrix aug(n, 2 * col);
    modigliani_base::DMatrix inverse(n, col);
    for (i = 1; i <= n; i++)
      for (j = 1; j <= col; j++)
        aug(i, j) = (*this) (i, j);
    for (i = 1; i <= n; i++)
      for (j = n + 1; j <= aug.col; j++)
        if (i == j - n)
          aug(i, j) = 1;        // Einheitsmatrix
    for (pivrow = 1; pivrow <= n; pivrow++) {
      pivelt = aug(pivrow, pivrow);
      if (pivelt == 0) {
        k = pivrow + 1;         // Pivotsuche
        while (pivelt == 0 && k <= n) {
          pivelt = aug(k, pivrow);
          k++;
        }
        if (pivelt == 0) {
          std::cerr
            << "dmatrix : ERROR - Inversion attempted, but matrix is singular!";
          exit(1);
        }
      } else {
        k--;
        modigliani_base::DMatrix dum(2 * col);
        for (i = 1; i <= 2 * col; i++)
          dum(i) = aug(pivrow, i);
        for (i = 1; i <= 2 * col; i++)
          aug(pivrow, i) = aug(k, i);
        for (i = 1; i <= 2 * col; i++)
          aug(k, i) = dum(i);
      }
    }

    for (j = 1; j <= 2 * col; j++)
      aug(pivrow, j) = aug(pivrow, j) / pivelt;
    for (tarrow = 1; tarrow <= n; tarrow++)
      if (tarrow != pivrow) {
        tarelt = aug(tarrow, pivrow);
        for (j = 1; j <= 2 * col; j++)
          aug(tarrow, j) = aug(tarrow, j) - aug(pivrow, j) * tarelt;
      }
    for (i = 1; i <= n; i++)
      for (j = n + 1; j <= aug.col; j++)
        inverse(i, j - n) = aug(i, j);
    return (inverse);
  }

  std::istream & operator>>(std::istream & is, modigliani_base::DMatrix & m) {
    for (int i = 1; i <= m.row; i++)
      for (int j = 1; j <= m.col; j++)
        is >> m(i, j);
    return (is);
  }
}  // namespace modigliani_base
#endif  // MODIGLIANI_MODIGLIANI_BASE_DMATRIX_H_
