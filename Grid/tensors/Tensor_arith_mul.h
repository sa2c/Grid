/*************************************************************************************

    Grid physics library, www.github.com/paboyle/Grid 

    Source file: ./lib/tensors/Tensor_arith_mul.h

    Copyright (C) 2015

Author: Peter Boyle <paboyle@ph.ed.ac.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    See the full license in the file "LICENSE" in the top level distribution directory
*************************************************************************************/
/*  END LEGAL */
#ifndef GRID_MATH_ARITH_MUL_H
#define GRID_MATH_ARITH_MUL_H

NAMESPACE_BEGIN(Grid);

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// MUL         ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
    
template<class rtype,class vtype,class mtype>
accelerator_inline void mult(iScalar<rtype> * __restrict__ ret,const iScalar<mtype> * __restrict__ lhs,const iScalar<vtype> * __restrict__ rhs){
  mult(&ret->_internal,&lhs->_internal,&rhs->_internal);
}

template<class rrtype, class ltype, class rtype, int Ncol, int Nrow = Ncol>
accelerator_inline void mult(iMatrix<rrtype, Ncol, Nrow> * __restrict__ ret, const iMatrix<ltype, Ncol, Nrow> * __restrict__ lhs, const iMatrix<rtype, Ncol, Nrow> * __restrict__ rhs){
  // TODO: Eventually remove
  static_assert(Nrow == Ncol, "this mult implementation requires square matrices");
  for(int c1 = 0; c1 < Nrow; c1++){
    for(int c2 = 0; c2 < Ncol; c2++){
      mult(&ret->_internal[c1][c2],&lhs->_internal[c1][0],&rhs->_internal[0][c2]);
    }
  }
  for(int c1 = 0; c1 < Ncol; c1++){
    for(int c3 = 1; c3 < Ncol; c3++){
      for(int c2 = 0; c2 < Nrow; c2++){
	mac(&ret->_internal[c1][c2],&lhs->_internal[c1][c3],&rhs->_internal[c3][c2]);
      }
    }
  }
  return;
}

template<class rrtype, class ltype, class rtype, int Ncol, int Nrow = Ncol>
accelerator_inline void mult(iMatrix<rrtype, Ncol, Nrow> * __restrict__ ret, const iMatrix<ltype, Ncol, Nrow> * __restrict__ lhs, const iScalar<rtype> * __restrict__ rhs){
  for(int c2 = 0; c2 < Ncol; c2++){
    for(int c1 = 0; c1 < Nrow; c1++){
      mult(&ret->_internal[c1][c2],&lhs->_internal[c1][c2],&rhs->_internal);
    }}
  return;
}

template<class rrtype, class ltype, class rtype, int Ncol, int Nrow = Ncol>
accelerator_inline void mult(iMatrix<rrtype, Ncol, Nrow> * __restrict__ ret, const iScalar<ltype>   * __restrict__ lhs, const iMatrix<rtype, Ncol, Nrow> * __restrict__ rhs){
  for(int c2 = 0; c2 < Ncol; c2++){
    for(int c1 = 0; c1 < Nrow; c1++){
      mult(&ret->_internal[c1][c2],&lhs->_internal,&rhs->_internal[c1][c2]);
    }}
  return;
}
// Matrix left multiplies vector
template<class rtype, class vtype, class mtype, int Ncol, int Nrow = Ncol>
accelerator_inline void mult(iVector<rtype, Ncol> * __restrict__ ret, const iMatrix<mtype, Ncol, Nrow> * __restrict__ lhs, const iVector<vtype, Ncol> * __restrict__ rhs)
{
  // TODO: Eventually remove
  static_assert(Nrow == Ncol, "this mult implementation requires square matrices");
  for(int c1 = 0; c1 < Nrow; c1++){
    mult(&ret->_internal[c1],&lhs->_internal[c1][0],&rhs->_internal[0]);
    for(int c2 = 1; c2 < Ncol; c2++){
      mac(&ret->_internal[c1],&lhs->_internal[c1][c2],&rhs->_internal[c2]);
    }
  }
  return;
}
template<class rtype,class vtype,class mtype,int N>
accelerator_inline void mult(iVector<rtype,N> * __restrict__ ret,
			const iScalar<mtype>   * __restrict__ lhs,
			const iVector<vtype,N> * __restrict__ rhs){
  for(int c1=0;c1<N;c1++){
    mult(&ret->_internal[c1],&lhs->_internal,&rhs->_internal[c1]);
  }
}
template<class rtype,class vtype,class mtype,int N>
accelerator_inline void mult(iVector<rtype,N> * __restrict__ ret,
			const iVector<vtype,N> * __restrict__ rhs,
			const iScalar<mtype> * __restrict__ lhs){
  for(int c1=0;c1<N;c1++){
    mult(&ret->_internal[c1],&rhs->_internal[c1],&lhs->_internal);
  }                 
}
    


template<class rtype, class vtype, class mtype, int Ncol, int Nrow = Ncol> accelerator_inline
iVector<rtype, Ncol> operator * (const iMatrix<mtype, Ncol, Nrow>& lhs, const iVector<vtype, Ncol>& rhs)
{
  iVector<rtype, Ncol> ret;
  mult(&ret,&lhs,&rhs);
  return ret;
}

template<class rtype,class vtype,class mtype,int N> accelerator_inline
iVector<rtype,N> operator * (const iScalar<mtype>& lhs,const iVector<vtype,N>& rhs)
{
  iVector<rtype,N> ret;
  mult(&ret,&lhs,&rhs);
  return ret;
}

template<class rtype,class vtype,class mtype,int N> accelerator_inline
iVector<rtype,N> operator * (const iVector<mtype,N>& lhs,const iScalar<vtype>& rhs)
{
  iVector<rtype,N> ret;
  mult(&ret,&lhs,&rhs);
  return ret;
}

//////////////////////////////////////////////////////////////////
// Divide by scalar
//////////////////////////////////////////////////////////////////
template<class rtype,class vtype> accelerator_inline
iScalar<rtype> operator / (const iScalar<rtype>& lhs,const iScalar<vtype>& rhs)
{
  iScalar<rtype> ret;
  ret._internal = lhs._internal/rhs._internal;
  return ret;
}
template<class rtype,class vtype,int N> accelerator_inline
iVector<rtype,N> operator / (const iVector<rtype,N>& lhs,const iScalar<vtype>& rhs)
{
  iVector<rtype,N> ret;
  for(int i=0;i<N;i++){
    ret._internal[i] = lhs._internal[i]/rhs._internal;
  }
  return ret;
}
template<class rtype, class vtype, int Ncol, int Nrow = Ncol> accelerator_inline
iMatrix<rtype, Ncol, Nrow> operator / (const iMatrix<rtype, Ncol, Nrow>& lhs, const iScalar<vtype>& rhs)
{
  iMatrix<rtype, Ncol, Nrow> ret;
  for(int i = 0; i < Nrow; i++){
    for(int j = 0; j < Ncol; j++){
      ret._internal[i][j] = lhs._internal[i][j]/rhs._internal;
    }}
  return ret;
}
    
//////////////////////////////////////////////////////////////////
// Glue operators to mult routines. Must resolve return type cleverly from typeof(internal)
// since nesting matrix<scalar> x matrix<matrix>-> matrix<matrix>
// while         matrix<scalar> x matrix<scalar>-> matrix<scalar>
// so return type depends on argument types in nasty way.
//////////////////////////////////////////////////////////////////
// scal x scal = scal
// mat x  mat  = mat
// mat  x scal = mat
// scal x mat  = mat
// mat  x vec  = vec
// vec  x scal = vec
// scal x vec  = vec
//
// We can special case scalar_type ??
template<class l,class r>
accelerator_inline auto operator * (const iScalar<l>& lhs,const iScalar<r>& rhs) -> iScalar<decltype(lhs._internal * rhs._internal)>
{
  typedef iScalar<decltype(lhs._internal*rhs._internal)> ret_t;
  ret_t ret;
  mult(&ret,&lhs,&rhs);
  return ret;
}
template<class l, class r, int Ncol, int Nrow = Ncol> accelerator_inline
auto operator * (const iMatrix<l, Ncol, Nrow>& lhs, const iMatrix<r, Ncol, Nrow>& rhs) -> iMatrix<decltype(lhs._internal[0][0]*rhs._internal[0][0]), Ncol, Nrow>
{
  typedef decltype(lhs._internal[0][0]*rhs._internal[0][0]) ret_t;
  iMatrix<ret_t, Ncol, Nrow> ret;
  mult(&ret,&lhs,&rhs);
  return ret;
}
template<class l,class r, int Ncol, int Nrow = Ncol> accelerator_inline
auto operator * (const iMatrix<r, Ncol, Nrow>& lhs, const iScalar<l>& rhs) -> iMatrix<decltype(lhs._internal[0][0]*rhs._internal), Ncol, Nrow>
{
  typedef decltype(lhs._internal[0][0]*rhs._internal) ret_t;
        
  iMatrix<ret_t, Ncol, Nrow> ret;
  for(int c1 = 0; c1 < Nrow; c1++){
    for(int c2 = 0; c2 < Ncol; c2++){
      mult(&ret._internal[c1][c2],&lhs._internal[c1][c2],&rhs._internal);
    }}
  return ret;
}
template<class l, class r, int Ncol, int Nrow = Ncol> accelerator_inline
auto operator * (const iScalar<l>& lhs, const iMatrix<r, Ncol, Nrow>& rhs) -> iMatrix<decltype(lhs._internal*rhs._internal[0][0]), Ncol, Nrow>
{
  typedef decltype(lhs._internal*rhs._internal[0][0]) ret_t;
  iMatrix<ret_t, Ncol, Nrow> ret;
  for(int c1 = 0; c1 < Nrow; c1++){
    for(int c2 = 0; c2 < Ncol; c2++){
      mult(&ret._internal[c1][c2],&lhs._internal,&rhs._internal[c1][c2]);
    }}
  return ret;
}
template<class l, class r, int Ncol, int Nrow = Ncol> accelerator_inline
auto operator * (const iMatrix<l, Ncol, Nrow>& lhs, const iVector<r, Ncol>& rhs) -> iVector<decltype(lhs._internal[0][0]*rhs._internal[0]), Ncol>
{
  // TODO: Eventually remove
  static_assert(Nrow == Ncol, "this mult implementation requires square matrices");
  typedef decltype(lhs._internal[0][0]*rhs._internal[0]) ret_t;
  iVector<ret_t, Ncol> ret;
  for(int c1 = 0; c1 < Nrow; c1++){
    mult(&ret._internal[c1],&lhs._internal[c1][0],&rhs._internal[0]);
    for(int c2 = 1; c2 < Ncol; c2++){
      mac(&ret._internal[c1],&lhs._internal[c1][c2],&rhs._internal[c2]);
    }
  }
  return ret;
}
template<class l,class r,int N> accelerator_inline
auto operator * (const iScalar<l>& lhs,const iVector<r,N>& rhs) -> iVector<decltype(lhs._internal*rhs._internal[0]),N>
{
  typedef decltype(lhs._internal*rhs._internal[0]) ret_t;
  iVector<ret_t,N> ret;
  for(int c1=0;c1<N;c1++){
    mult(&ret._internal[c1],&lhs._internal,&rhs._internal[c1]);
  }
  return ret;
}
template<class l,class r,int N> accelerator_inline
auto operator * (const iVector<l,N>& lhs,const iScalar<r>& rhs) -> iVector<decltype(lhs._internal[0]*rhs._internal),N>
{
  typedef decltype(lhs._internal[0]*rhs._internal) ret_t;
  iVector<ret_t,N> ret;
  for(int c1=0;c1<N;c1++){
    mult(&ret._internal[c1],&lhs._internal[c1],&rhs._internal);
  }
  return ret;
}

NAMESPACE_END(Grid);


#endif
