#ifndef LAS_CSR_H_
#define LAS_CSR_H_
#include "lasSys.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
namespace las
{
  class Sparsity;
  /**
   * A utility class to describe the sparse structure of a serial
   *  compressed sparse row (CSR) format matrix,
   *  can be used on in conjunction with any linear storage container
   *  to index a sparse matrix.
   */
  class CSR
  {
  private:
    int nr;
    int nc;
    int nnz;
    std::vector<int> rws;
    std::vector<int> cls;
    CSR();
  public:
    /**
     * @param r The number of rows in the sparse matrix
     * @param c The number of cols in the sparse matrix
     * @param nnz The number of nonzeros in the sparse matrix
     * @param rs An array of length r+1 containing the row offsets into cs
     * @param cs An array of length nnz containing the column ids for each nonzero in the matrix
     * @note See the lasCSRBuilder.h file for a basic interface to build these array from the
     *       set of (row,col) values of all nonzeros in a matrix.
     * @note The rs and cs arrays should use 1-indexing for fortran interoperability, if the
     *       first rs offset is zero, it is assumed all values in rs and cs are 0-indexed and
     *       they are converted to use 1-indexing (in a debug build this generates a warning).
     */
    CSR(int r, int c, int nnz, int * rs, int * cs);
    CSR(int r, int c, int nnz, std::vector<int> const & rs, std::vector<int> const & cs);
    int getNumRows() const { return nr; }
    int getNumCols() const { return nc; }
    int getNumNonzero() const { return nnz; }
    // return the index into the values array
    // if the location is not stored then return -1
    // note rw and cl start at zero
    int operator()(int rw, int cl) const
    {
      assert(rw < nr && rw>=0);
      assert(cl < nc && cl>=0);
      // the row is empty
      if(rws[rw+1]-rws[rw] == 0)
        return -1;
      // this approach finds the correct index in log(n) time where
      // n is the number of elements on the row
      typedef std::vector<int>::const_iterator vit_t;
      vit_t bgn = cls.begin()+rws[rw]-1;
      vit_t end = cls.begin()+rws[rw+1]-1;
      std::pair<vit_t, vit_t> bounds = equal_range(bgn, end, cl+1);
      if(bounds.first == bounds.second)
        return -1;
      return bounds.first-cls.begin();
    }
    int * getRows() { return &rws[0]; }
    int * getCols() { return &cls[0]; }
    int getMaxEntPerRow();
    int getMaxEntPerCol();
  };
  /*
   * Construct a csr sparse matrix structure from csr arrays
   * \param rws number of rows int the csr matrix
   * \param cls number of columns in the csr matrix
   * \param nnz number of nonzeroes in the csr matrix
   * \param row_arr array where first entry is 1 and all successive entries
   *        hold row_arry[i-1]+number of entries on i-1 row. (one indexed)
   * \param col_arr column index of each matrix entry (should be len nnz)
   */ 
  Sparsity * csrFromArray(int rws, int cls, int nnz, int * row_arr, int * col_arr);
  /**
   * Construct a CSR sparse matrix structure from a full matrix buffer
   * @param mat Pointer to array of rws x cls scalars containing the full matrix
   * @param rws Number of rows in the full matrix
   * @param cls Number of cls in the full matrix
   */
  Sparsity * csrFromFull(scalar * mat, int rws, int cls);
  /**
   *  Produce a full version of the sparse matrix, this is typically
   *   only used for debugging purposes.
   * @param csr A csr object describing the structure of the sprs_mat
   * @param sprs_mat A buffer of length csr->getNumNonzeros() containing the matrix values
   * @param fll_mat A preallocated buffer of length csr->getNumEqs()^2 which will contain
   *                all nonzero and zero values of the matrix;
   */
  void constructFullMatrix(Sparsity * csr,scalar * sprs_mat,scalar * fll_mat);
}
#include "lasCSR_impl.h"
#endif
