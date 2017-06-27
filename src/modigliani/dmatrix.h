/**
 * @file dmatrix.h
 * @brief DMatrix class header and implementation
 *
 * Copyright (C) 1997 Ahmed Aldo Faisal
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODIGLIANI_MODIGLIANI_BASE_DMATRIX_H_
#define MODIGLIANI_MODIGLIANI_BASE_DMATRIX_H_

#include <cstdlib>
#include <string>

namespace modigliani {
  /**
   * @brief A straight-forward double matrix class
   *
   * @warning INDICES in Matrix START with (1,1) and end with (row,col) !
   * @warning DESTRUCTIVE methods used! Be carefull because currently methods and operators
   * might modify the (*this) object and some generate a new one
   */
  class DMatrix {
 public:
    /**
     * @brief Construct a matrix
     * @param r Number of rows
     * @param c Number of columns
     */
    DMatrix(int r, int c);
    
    /**
     * @brief Construct a vector
     * @param r Number of elements
     */
    explicit DMatrix(int r);

    /**
     * @brief Duplicate from existent DMatrix object
     * @param copy Original matrix
     */
    DMatrix(DMatrix & copy);
    
    /**
     * @brief Destructor
     */
    ~DMatrix() {
      delete elements;
    }

    /**
     * @brief Adds two matrices
     * @param right Matrix to add
     * @return Result
     */
    DMatrix operator+(DMatrix & right);

    /**
     * @brief Substracts two matrices
     * @param right Matrix to substract
     * @return Result
     */
    DMatrix operator-(DMatrix & right);

    /**
     * @brief Multiplies two matrices
     * @param right Matrix to multiply
     * @return Result
     */
    DMatrix operator*(DMatrix & right);

    /**
     * @brief Scales a matric with a scalar factor
     * @param factor Scale factor
     * @return Result
     */
    DMatrix operator*(double factor);

    /**
     * @brief Copies the content of one matrix into another
     * @param right Matrix to copy data from
     * @return Result
     */
    DMatrix & operator=(DMatrix &);

    /**
     * @brief Access matrix components
     * @param size_x Row
     * @param size_y Column
     * @return Element
     */
    double &operator() (int size_x, int size_y);

    /**
     * @brief Access vector components
     * @param size Index
     * @return Element
     */
    double &operator() (int size);

    /**
     * @brief Writes the matrix to stdout.
     */
    void print();

    /**
     * @brief Returns the sum of all elements in the matrix
     * @return The sum of elements
     */
    double sum();

    /**
     * @brief Returns the quadratic sum of all elements in the matrix
     * @return The quadratic sum of elements
     */
    double quadSum();

    /**
     * @brief Compute inverse with Gauss-elimination including
     * Pivotizing
     * @return Inverse matrix
     */
    DMatrix operator~();

    /**
     * @brief Prints the matrix
     * @param  is ostream to output into
     * @param  m The matrix
     * @return ostream
     */
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

  DMatrix(modigliani::DMatrix & copy) {
    row = copy.row;
    col = copy.col;
    elements = new double[row * col + 1];
    memcpy(elements, copy.elements, (row * col + 1) * sizeof(elements[0]));
  }

  modigliani::DMatrix modigliani::DMatrix::
    operator+(modigliani::DMatrix & right) {
    modigliani::DMatrix left(*this);
    for (int i = 1; i <= row * col; i++)
      left.elements[i] = left.elements[i] + right.elements[i];
    return (left);
  }

  modigliani::DMatrix modigliani::DMatrix::
    operator-(modigliani::DMatrix & right) {
    modigliani::DMatrix left(*this);
    for (int i = 1; i <= row * col; i++) {
      left.elements[i] = left.elements[i] - right.elements[i];
    }
    return (left);
  }

  modigliani::DMatrix modigliani::DMatrix::
    operator*(modigliani::DMatrix & right) {
    modigliani::DMatrix prod(row, right.col);
    for (int i = 1; i <= row; i++) {
      for (int j = 1; j <= right.col; j++) {
        prod(i, j) = 0;
        for (int k = 1; k <= col; k++)
          prod(i, j) = prod(i, j) + (*this) (i, k) * right(k, j);
      }
    }
    return (prod);
  }

  modigliani::DMatrix modigliani::DMatrix::operator*(double factor) {
    for (int i = 1; i <= row; i++) {
      for (int j = 1; j <= col; j++) {
        (*this) (i, j) = (*this) (i, j) * factor;
      }
    }
    return (*this);
  }

  modigliani::DMatrix &
    modigliani::DMatrix::operator=(modigliani::DMatrix & right) {
    if (this == &right) {
      return (*this);
    } else {
      delete elements;
      elements = new double[row * col + 1];
      memcpy(elements, right.elements, (row * col + 1) * sizeof(elements[0]));
      return (*this);
    }
  }

  double &modigliani::DMatrix::operator() (int r, int c) {
    return (elements[(r - 1) * col + c]);
  }

  double &modigliani::DMatrix::operator() (int r) {
    return (elements[r]);
  }

  void modigliani::DMatrix::print() {
    std::cout.setf(std::ios::fixed);
    std::cout.precision(4);
    std::cout.width(8);
    for (int i = 1; i <= row; i++) {
      for (int j = 1; j <= col; j++)
        std::cout << (*this) (i, j);
      std::cout << std::endl;
    }
  }

  double modigliani::DMatrix::sum() {
    double temp = 0.0;
    for (int i = 1; i <= row; i++)
      for (int j = 1; j <= col; j++)
        temp += (*this) (i, j);
    return (temp);
  }

  double modigliani::DMatrix::quadSum() {
    double temp = 0.0;
    for (int i = 1; i <= row; i++)
      for (int j = 1; j <= col; j++)
        temp += (*this) (i, j) * (*this) (i, j);
    return (temp);
  }

  modigliani::DMatrix modigliani::DMatrix::operator~() {
    int i, j, k, n;
    int pivrow, tarrow;
    double pivelt, tarelt;
    n = row;                    // Zahl d.Gleich.
    modigliani::DMatrix aug(n, 2 * col);
    modigliani::DMatrix inverse(n, col);
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
        modigliani::DMatrix dum(2 * col);
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

  std::istream & operator>>(std::istream & is, modigliani::DMatrix & m) {
    for (int i = 1; i <= m.row; i++)
      for (int j = 1; j <= m.col; j++)
        is >> m(i, j);
    return (is);
  }
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_BASE_DMATRIX_H_
