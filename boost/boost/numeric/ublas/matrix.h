//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_MATRIX_H
#define NUMERICS_MATRIX_H

#include "config.h"
#include "storage.h"
#include "vector.h"
#include "matrix_et.h"
#include "matrix_pr.h"

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numerics {

    // This functor computes the address translation 
    // matrix [i] [j] -> storage [i * size2 + j]
    struct row_major {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef row_major_tag orientation_category;

        // Indexing
        static
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            return i * size2 + j;
        }
        static
        NUMERICS_INLINE
        difference_type distance1 (difference_type k, size_type size1, size_type size2) {
            return size2 != 0 ? k / size2 : 0;
        }
        static
        NUMERICS_INLINE
        difference_type distance2 (difference_type k, size_type size1, size_type size2) {
            return k;
        }
        static
        NUMERICS_INLINE
        size_type index1 (difference_type k, size_type size1, size_type size2) {
            return size2 != 0 ? k / size2 : 0;
        }
        static
        NUMERICS_INLINE
        size_type index2 (difference_type k, size_type size1, size_type size2) {
            return size2 != 0 ? k % size2 : 0;
        }
        static
        NUMERICS_INLINE
        bool fast1 () {
            return false;
        }
        static
        NUMERICS_INLINE
        size_type one1 (size_type size1, size_type size2) {
            return size2;
        }
        static
        NUMERICS_INLINE
        bool fast2 () {
            return true;
        }
        static
        NUMERICS_INLINE
        size_type one2 (size_type size1, size_type size2) {
            return 1;
        }

        static
        NUMERICS_INLINE
        size_type lower_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i >= j, bad_index ());
            // sigma_i (i + 1) = (i + 1) * i / 2
            // i = 0 1 2 3, sigma = 0 1 3 6
            return ((i + 1) * i) / 2 + j; 
        }
        static 
        NUMERICS_INLINE
        size_type upper_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i <= j, bad_index ());
            // sigma_i (size - i) = size * i - i * (i - 1) / 2
            // i = 0 1 2 3, sigma = 0 4 7 9
            return (i * (2 * std::max (size1, size2) - i + 1)) / 2 + j - i; 
        }

        static 
        NUMERICS_INLINE
        size_type element1 (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            return i;
        }
        static 
        NUMERICS_INLINE
        size_type element2 (size_type i, size_type size1, size_type j, size_type size2) {
            check (j <= size2, bad_index ());
            return j;
        }
        static 
        NUMERICS_INLINE
        size_type index1 (size_type index1, size_type index2) {
            return index1;
        }
        static
        NUMERICS_INLINE
        size_type index2 (size_type index1, size_type index2) {
            return index2;
        }
        static 
        NUMERICS_INLINE
        size_type size1 (size_type size1, size_type size2) {
            return size1;
        }
        static
        NUMERICS_INLINE
        size_type size2 (size_type size1, size_type size2) {
            return size2;
        }

        // Iterating
        template<class I>
        static
        NUMERICS_INLINE
        void increment1 (I &it, size_type size1, size_type size2) {
            it += size2;
        }
        template<class I>
        static
        NUMERICS_INLINE
        void decrement1 (I &it, size_type size1, size_type size2) {
            it -= size2;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void increment2 (I &it, size_type size1, size_type size2) {
            ++ it;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void decrement2 (I &it, size_type size1, size_type size2) {
            -- it;
        }
    };

    // This functor computes the address translation 
    // matrix [i] [j] -> storage [i + j * size1]
    struct column_major {
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef column_major_tag orientation_category;

        // Indexing
        static 
        NUMERICS_INLINE
        size_type element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            return i + j * size1;
        }
        static 
        NUMERICS_INLINE
        difference_type distance1 (difference_type k, size_type size1, size_type size2) {
            return k;
        }
        static
        NUMERICS_INLINE
        difference_type distance2 (difference_type k, size_type size1, size_type size2) {
            return size1 != 0 ? k / size1 : 0;
        }
        static
        NUMERICS_INLINE
        size_type index1 (difference_type k, size_type size1, size_type size2) {
            return size1 != 0 ? k % size1 : 0;
        }
        static
        NUMERICS_INLINE
        size_type index2 (difference_type k, size_type size1, size_type size2) {
            return size1 != 0 ? k / size1 : 0;
        }
        static
        NUMERICS_INLINE
        bool fast1 () {
            return true;
        }
        static
        NUMERICS_INLINE
        size_type one1 (size_type size1, size_type size2) {
            return 1;
        }
        static
        NUMERICS_INLINE
        bool fast2 () {
            return false;
        }
        static
        NUMERICS_INLINE
        size_type one2 (size_type size1, size_type size2) {
            return size1;
        }

        static 
        NUMERICS_INLINE
        size_type lower_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i >= j, bad_index ());
            // sigma_j (size - j) = size * j - j * (j - 1) / 2
            // j = 0 1 2 3, sigma = 0 4 7 9
            return i - j + (j * (2 * std::max (size1, size2) - j + 1)) / 2; 
        }
        static 
        NUMERICS_INLINE
        size_type upper_element (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            check (j <= size2, bad_index ());
            check (i <= j, bad_index ());
            // sigma_j (j + 1) = (j + 1) * j / 2
            // j = 0 1 2 3, sigma = 0 1 3 6
            return i + ((j + 1) * j) / 2; 
        }

        static 
        NUMERICS_INLINE
        size_type element1 (size_type i, size_type size1, size_type j, size_type size2) {
            check (j <= size2, bad_index ());
            return j;
        }
        static 
        NUMERICS_INLINE
        size_type element2 (size_type i, size_type size1, size_type j, size_type size2) {
            check (i <= size1, bad_index ());
            return i;
        }
        static 
        NUMERICS_INLINE
        size_type index1 (size_type index1, size_type index2) {
            return index2;
        }
        static 
        NUMERICS_INLINE
        size_type index2 (size_type index1, size_type index2) {
            return index1;
        }
        static
        NUMERICS_INLINE
        size_type size1 (size_type size1, size_type size2) {
            return size2;
        }
        static
        NUMERICS_INLINE
        size_type size2 (size_type size1, size_type size2) {
            return size1;
        }

        // Iterating
        template<class I>
        static 
        NUMERICS_INLINE
        void increment1 (I &it, size_type size1, size_type size2) {
            ++ it;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void decrement1 (I &it, size_type size1, size_type size2) {
            -- it;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void increment2 (I &it, size_type size1, size_type size2) {
            it += size1;
        }
        template<class I>
        static 
        NUMERICS_INLINE
        void decrement2 (I &it, size_type size1, size_type size2) {
            it -= size1;
        }
    };

    struct sparse_proxy_row_major_tag {};
    struct sparse_proxy_column_major_tag {};
    struct sparse_row_major_tag: public sparse_proxy_row_major_tag {};
    struct sparse_column_major_tag: public sparse_proxy_column_major_tag {};

    template<class S, class O>
    struct storage_orientation_traits {
        typedef S storage_orientation_category;
    };

    template<>
    struct storage_orientation_traits<sparse_tag, row_major_tag> {
        typedef sparse_row_major_tag storage_orientation_category;
    };
    template<>
    struct storage_orientation_traits<sparse_tag, column_major_tag> {
        typedef sparse_column_major_tag storage_orientation_category;
    };
    template<> 
    struct storage_orientation_traits<sparse_proxy_tag, row_major_tag> {
        typedef sparse_proxy_row_major_tag storage_orientation_category;
    };
    template<> 
    struct storage_orientation_traits<sparse_proxy_tag, column_major_tag> {
        typedef sparse_proxy_column_major_tag storage_orientation_category;
    };

    template<class LSO, class A> 
    struct matrix_assign_scalar_traits {
        typedef LSO dispatch_category;
    };

    // matrix assignment_operation scalar
    template<class F>
    struct matrix_assign_scalar {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        // Iterating case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (M &m, const T &t) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (m.size2 ());
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                check ((*it1).end () - it2 == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, t), ++ it2;
#else
                DD (size2, 4, r, (functor_type () (*it2, t), ++ it2));
#endif
                ++ it1;
            }
#else
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (m.size2 ());
                typename M::iterator2 it2 (it1.begin ());
                check (it1.end () - it2 == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, t), ++ it2;
#else
                DD (size2, 4, r, (functor_type () (*it2, t), ++ it2));
#endif
                ++ it1;
            }
#endif
        }
        // Indexing case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (M &m, const T &t) {
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            difference_type size2 (m.size2 ());
            for (difference_type i = 0; i < size1; ++ i) {
#ifndef NUMERICS_USE_DUFF_DEVICE
                for (difference_type j = 0; j < size2; ++ j)
                    functor_type () (m (i, j), t);
#else
                difference_type j (0);
                DD (size2, 4, r, (functor_type () (m (i, j), t), ++ j));
#endif
            }
        }

        // Dense (proxy) case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, dense_proxy_tag) {
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            difference_type size1 (m.size1 ());
            difference_type size2 (m.size2 ());
            if (size1 >= NUMERICS_ITERATOR_THRESHOLD &&
                size2 >= NUMERICS_ITERATOR_THRESHOLD)
                iterating_assign (m, t);
            else
                indexing_assign (m, t);
        }
        // Packed (proxy) case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, packed_proxy_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            difference_type size1 (m.end1 () - it1);
            while (-- size1 >= 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                difference_type size2 ((*it1).end () - it2);
                while (-- size2 >= 0)
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
#else
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            difference_type size1 (m.end1 () - it1);
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                difference_type size2 (it1.end () - it2);
                while (-- size2 >= 0)
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
#endif
        }
        // Sparse (proxy) row major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, sparse_proxy_row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end)
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
#else
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end)
                    functor_type () (*it2, t), ++ it2;
                ++ it1;
            }
#endif
        }
        // Sparse (proxy) column major case
        template<class M, class T>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const T &t, sparse_proxy_column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            while (it2 != it2_end) {
                typename matrix_column<M>::iterator1 it1 ((*it2).begin ());
                typename matrix_column<M>::iterator1 it1_end ((*it2).end ());
                while (it1 != it1_end)
                    functor_type () (*it1, t), ++ it1;
                ++ it2;
            }
#else
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end)
                    functor_type () (*it1, t), ++ it1;
                ++ it2;
            }
#endif
        }

        // Dispatcher
        template<class M, class T>
        NUMERICS_INLINE
        void operator () (M &m, const T &t) {
            typedef typename storage_orientation_traits<NUMERICS_TYPENAME M::storage_category,
                                                        NUMERICS_TYPENAME M::orientation_category>::storage_orientation_category storage_orientation_category;
            typedef typename matrix_assign_scalar_traits<storage_orientation_category,
                                                         assign_category>::dispatch_category dispatch_category;
            operator () (m, t, dispatch_category ());
        }
    };

    template<class LSO, class A, class RO, class RI1, class RI2>
    struct matrix_assign_traits {
        typedef LSO dispatch_category;
    };

    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, unknown_orientation_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, unknown_orientation_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, row_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, row_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, column_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, column_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_column_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };

    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, unknown_orientation_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, unknown_orientation_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, row_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, row_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, column_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, column_major_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };

    template<>
    struct matrix_assign_traits<packed_tag, assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_tag, computed_assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_tag, assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_tag, computed_assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_tag, assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_column_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_tag, computed_assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };

    template<>
    struct matrix_assign_traits<packed_proxy_tag, assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_proxy_tag, computed_assign_tag, unknown_orientation_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_proxy_tag, assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_proxy_tag, computed_assign_tag, row_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_proxy_tag, assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };
    template<>
    struct matrix_assign_traits<packed_proxy_tag, computed_assign_tag, column_major_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };

    // matrix assignment_operation matrix_expression
    template<class F>
    struct matrix_assign {
        typedef F functor_type;
        typedef typename F::assign_category assign_category;

        // Iterating case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void iterating_assign (M &m, const matrix_expression<E> &e) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            typename E::const_iterator1 it1e (e ().begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            check (e ().end1 () - it1e == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (common (m.size2 (), e ().size2 ()));
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                check ((*it1).end () - it2 == size2, bad_size ());
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                check ((*it1e).end () - it2e == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
#else
                DD (size2, 2, r, (functor_type () (*it2, *it2e), ++ it2, ++ it2e));
#endif
                ++ it1, ++ it1e;
            }
#else
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            typename M::iterator1 it1 (m.begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            check (m.end1 () - it1 == size1, bad_size ());
#endif
            typename E::const_iterator1 it1e (e ().begin1 ());
#ifdef NUMERICS_BOUNDS_CHECK_EX
            check (e ().end1 () - it1e == size1, bad_size ());
#endif
            while (-- size1 >= 0) {
                difference_type size2 (common (m.size2 (), e ().size2 ()));
                typename M::iterator2 it2 (it1.begin ());
                check (it1.end () - it2 == size2, bad_size ());
                typename E::const_iterator2 it2e (it1e.begin ());
                check (it1e.end () - it2e == size2, bad_size ());
#ifndef NUMERICS_USE_DUFF_DEVICE
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
#else
                DD (size2, 2, r, (functor_type () (*it2, *it2e), ++ it2, ++ it2e));
#endif
                ++ it1, ++ it1e;
            }
#endif
        }
        // Indexing case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void indexing_assign (M &m, const matrix_expression<E> &e) {
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            difference_type size2 (common (m.size2 (), e ().size2 ())); 
            for (difference_type i = 0; i < size1; ++ i) {
#ifndef NUMERICS_USE_DUFF_DEVICE
                for (difference_type j = 0; j < size2; ++ j) 
                    functor_type () (m (i, j), e () (i, j)); 
#else
                difference_type j (0);
                DD (size2, 2, r, (functor_type () (m (i, j), e () (i, j)), ++ j));
#endif
            }
        }

        // Dense (proxy) case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, dense_proxy_tag) {
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            difference_type size1 (common (m.size1 (), e ().size1 ()));
            difference_type size2 (common (m.size2 (), e ().size2 ()));
            if (size1 >= NUMERICS_ITERATOR_THRESHOLD &&
                size2 >= NUMERICS_ITERATOR_THRESHOLD)
                iterating_assign (m, e);
            else
                indexing_assign (m, e);
        }
        // Packed (proxy) case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, packed_proxy_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::size_type size_type;
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
            while (it1 != it1_end && it1e != it1e_end && it1.index () < it1e.index ()) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
            while (it1 != it1_end && it1e != it1e_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                if (it2e != it2e_end && it2e.index2 () < it2.index2 ()) 
                    it2e += std::min (it2.index2 () - it2e.index2 () <= size_type (it2e_end - it2e));
                while (it2 != it2_end && it2e != it2e_end && it2.index () < it2e.index ()) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                while (it2 != it2_end && it2e != it2e_end) {
                    functor_type () (*it2, *it2e);
                    ++ it2, ++ it2e;
                }
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1, ++ it1e;
            }
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            {
                // Need the const member dispatched.
                const M &cm = m;
                typename E::const_iterator1 it1e (e ().begin1 ());
                typename E::const_iterator1 it1e_end (e ().end1 ());
                while (it1e != it1e_end) {
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2e != it2e_end) {
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
                    ++ it1e;
                }
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::size_type size_type;
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            if (it1e != it1e_end && it1e.index1 () < it1.index1 ())
                it1e += std::min (it1.index1 () - it1e.index1 (), size_type (it1e_end - it1e));
            while (it1 != it1_end && it1e != it1e_end && it1.index1 () < it1e.index1 ()) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
            while (it1 != it1_end && it1e != it1e_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                if (it2e != it2e_end && it2e.index2 () < it2.index2 ()) 
                    it2e += std::min (it2.index2 () - it2e.index2 (), size_type (it2e_end - it2e));
                while (it2 != it2_end && it2e != it2e_end && it2.index2 () < it2e.index2 ()) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                while (it2 != it2_end && it2e != it2e_end) {
                    functor_type () (*it2, *it2e);
                    ++ it2, ++ it2e;
                }
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1, ++ it1e;
            }
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            {
                // Need the const member dispatched.
                const M &cm = m;
                typename E::const_iterator1 it1e (e ().begin1 ());
                typename E::const_iterator1 it1e_end (e ().end1 ());
                while (it1e != it1e_end) {
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
                    ++ it1e;
                }
            }
#endif
#endif
        }
        // Sparse row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            m.clear ();
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1e != it1e_end) {
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end)
                    m.insert (it1e.index (), it2e.index (), *it2e), ++ it2e;
                ++ it1e;
            }
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            m.clear ();
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1e != it1e_end) {
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end)
                    m.insert (it2e.index1 (), it2e.index2 (), *it2e), ++ it2e;
                ++ it1e;
            }
#endif
        }
        // Sparse column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            m.clear ();
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2e != it2e_end) {
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end)
                    m.insert (it1e.index (), it2e.index (), *it1e), ++ it1e;
                ++ it2e;
            }
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            m.clear ();
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2e != it2e_end) {
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) 
                    m.insert (it1e.index1 (), it1e.index2 (), *it1e), ++ it1e;
                ++ it2e;
            }
#endif
        }
        // Sparse proxy row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_proxy_row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index () - it1e.index ();
                if (compare == 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index () - it2e.index ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            functor_type () (*it2, value_type ());
                            ++ it2;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::const_iterator1 it1e (e ().begin1 ());
            typename E::const_iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index1 () - it1e.index1 ();
                if (compare == 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index2 () - it2e.index2 ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            functor_type () (*it2, value_type ());
                            ++ it2;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    while (it2 != it2_end) {
                        functor_type () (*it2, value_type ());
                        ++ it2;
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#endif
        }
        // Sparse proxy column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e, sparse_proxy_column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index () - it2e.index ();
                if (compare == 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index () - it1e.index ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            functor_type () (*it1, value_type ());
                            ++ it1;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::const_iterator2 it2e (e ().begin2 ());
            typename E::const_iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index2 () - it2e.index2 ();
                if (compare == 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index1 () - it1e.index1 ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            functor_type () (*it1, value_type ());
                            ++ it1;
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    while (it1 != it1_end) {
                        functor_type () (*it1, value_type ());
                        ++ it1;
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end) {
                    functor_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#endif
        }

        // Dispatcher
        template<class M, class E>
        NUMERICS_INLINE
        void operator () (M &m, const matrix_expression<E> &e) {
            typedef typename storage_orientation_traits<NUMERICS_TYPENAME M::storage_category, 
                                                        NUMERICS_TYPENAME M::orientation_category>::storage_orientation_category storage_orientation_category;
            typedef typename matrix_assign_traits<storage_orientation_category, 
                                                  assign_category, 
                                                  NUMERICS_TYPENAME E::orientation_category, 
                                                  NUMERICS_TYPENAME E::const_iterator1::iterator_category,
                                                  NUMERICS_TYPENAME E::const_iterator2::iterator_category>::dispatch_category dispatch_category;
            operator () (m, e, dispatch_category ());
        }
    };

    template<class LSO, class RO, class RI1, class RI2> 
    struct matrix_swap_traits {
        typedef LSO dispatch_category;
    };

    template<>         
    struct matrix_swap_traits<dense_proxy_tag, unknown_orientation_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_swap_traits<dense_proxy_tag, row_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_swap_traits<dense_proxy_tag, column_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };

    template<>
    struct matrix_swap_traits<packed_proxy_tag, unknown_orientation_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_swap_traits<packed_proxy_tag, row_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_proxy_row_major_tag dispatch_category;
    };
    template<>
    struct matrix_swap_traits<packed_proxy_tag, column_major_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {
        typedef sparse_proxy_column_major_tag dispatch_category;
    };

    // matrix swap_operation matrix_expression
    template<class F>
    struct matrix_swap {
        typedef F functor_type;

        // Dense (proxy) case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, dense_proxy_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.size1 (), size_type (e ().end1 () - it1e)));
            while (-- size1 >= 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<E>::iterator it2e ((*it1e).begin ());
                difference_type size2 (common (m.size2 (), size_type ((*it1e).end () - it2e)));
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
#else
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.size1 (), size_type (e ().end1 () - it1e)));
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename E::iterator2 it2e (it1e.begin ());
                difference_type size2 (common (m.size2 (), size_type (it1e.end () - it2e)));
                while (-- size2 >= 0) 
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
#endif
        }
        // Packed (proxy) case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, packed_proxy_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.end1 () - it1, e ().end1 () - it1e));
            while (-- size1 >= 0) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<E>::iterator it2e ((*it1e).begin ());
                difference_type size2 (common ((*it1).end () - it2, (*it1e).end () - it2e));
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
#else
            typedef typename M::size_type size_type;
            typedef typename M::difference_type difference_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            difference_type size1 (common (m.end1 () - it1, e ().end1 () - it1e));
            while (-- size1 >= 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename E::iterator2 it2e (it1e.begin ());
                difference_type size2 (common (it1.end () - it2, it1e.end () - it2e));
                while (-- size2 >= 0)
                    functor_type () (*it2, *it2e), ++ it2, ++ it2e;
                ++ it1, ++ it1e;
            }
#endif
        }
        // Sparse (proxy) row major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, sparse_proxy_row_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            typename E::iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index () - it1e.index ();
                if (compare == 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    typename matrix_row<const E>::iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::iterator it2e_end ((*it1e).end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index () - it2e.index ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename matrix_row<M>::iterator it2 ((*it1).begin ());
                    typename matrix_row<M>::iterator it2_end ((*it1).end ());
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                    typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename matrix_row<M>::iterator it2 ((*it1).begin ());
                typename matrix_row<M>::iterator it2_end ((*it1).end ());
                if (it2 != it2_end) {
                    throw external_logic ();
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename matrix_row<const E>::const_iterator it2e ((*it1e).begin ());
                typename matrix_row<const E>::const_iterator it2e_end ((*it1e).end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator1 it1 (m.begin1 ());
            typename M::iterator1 it1_end (m.end1 ());
            typename E::iterator1 it1e (e ().begin1 ());
            typename E::iterator1 it1e_end (e ().end1 ());
            while (it1 != it1_end && it1e != it1e_end) {
                int compare = it1.index1 () - it1e.index1 ();
                if (compare == 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    typename E::iterator2 it2e (it1e.begin ());
                    typename E::iterator2 it2e_end (it1e.end ());
                    while (it2 != it2_end && it2e != it2e_end) {
                        int compare = it2.index2 () - it2e.index2 ();
                        if (compare == 0) {
                            functor_type () (*it2, *it2e);
                            ++ it2, ++ it2e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
#endif
                            ++ it2e;
                        }
                    }
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1, ++ it1e;
                } else if (compare < 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    if (it2 != it2_end) {
                        throw external_logic ();
                    }
                    ++ it1;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator2 it2e (it1e.begin ());
                    typename E::const_iterator2 it2e_end (it1e.end ());
                    while (it2e != it2e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                        ++ it2e;
                    }
#endif
                    ++ it1e;
                }
            }
            while (it1 != it1_end) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                if (it2 != it2_end) {
                    throw external_logic ();
                }
                ++ it1;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it1e != it1e_end) {
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it2e == cm (it2e.index1 (), it2e.index2 ()), bad_index ());
                    ++ it2e;
                }
                ++ it1e;
            }
#endif
#endif
        }
        // Sparse (proxy) column major case
        template<class M, class E>
        // This function seems to be big. So we do not let the compiler inline it.
        // NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e, sparse_proxy_column_major_tag) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            typename E::iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index () - it2e.index ();
                if (compare == 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    typename matrix_column<const E>::iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::iterator it1e_end ((*it2e).end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index () - it1e.index ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename matrix_column<M>::iterator it1 ((*it2).begin ());
                    typename matrix_column<M>::iterator it1_end ((*it2).end ());
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                    typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename matrix_column<M>::iterator it1 ((*it2).begin ());
                typename matrix_column<M>::iterator it1_end ((*it2).end ());
                if (it1 != it1_end) {
                    throw external_logic ();
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename matrix_column<const E>::const_iterator it1e ((*it2e).begin ());
                typename matrix_column<const E>::const_iterator it1e_end ((*it2e).end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index (), it2e.index ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#else
            check (m.size1 () == e ().size1 (), bad_size ());
            check (m.size2 () == e ().size2 (), bad_size ());
            typedef typename M::value_type value_type;
            typename M::iterator2 it2 (m.begin2 ());
            typename M::iterator2 it2_end (m.end2 ());
            typename E::iterator2 it2e (e ().begin2 ());
            typename E::iterator2 it2e_end (e ().end2 ());
            while (it2 != it2_end && it2e != it2e_end) {
                int compare = it2.index2 () - it2e.index2 ();
                if (compare == 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    typename E::iterator1 it1e (it2e.begin ());
                    typename E::iterator1 it1e_end (it2e.end ());
                    while (it1 != it1_end && it1e != it1e_end) {
                        int compare = it1.index1 () - it1e.index1 ();
                        if (compare == 0) {
                            functor_type () (*it1, *it1e);
                            ++ it1, ++ it1e;
                        } else if (compare < 0) {
                            throw external_logic ();
                        } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                            // Need the const member dispatched.
                            const M &cm = m;
                            // FIXME: we need a better floating point comparison...
                            check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
#endif
                            ++ it1e;
                        }
                    }
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2, ++ it2e;
                } else if (compare < 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    if (it1 != it1_end) {
                        throw external_logic ();
                    }
                    ++ it2;
                } else if (compare > 0) {
#ifdef NUMERICS_BOUNDS_CHECK_EX
                    typename E::const_iterator1 it1e (it2e.begin ());
                    typename E::const_iterator1 it1e_end (it2e.end ());
                    while (it1e != it1e_end) {
                        // Need the const member dispatched.
                        const M &cm = m;
                        // FIXME: we need a better floating point comparison...
                        check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                        ++ it1e;
                    }
#endif
                    ++ it2e;
                }
            }
            while (it2 != it2_end) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                if (it1 != it1_end) {
                    throw external_logic ();
                }
                ++ it2;
            }
#ifdef NUMERICS_BOUNDS_CHECK_EX
            while (it2e != it2e_end) {
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Need the const member dispatched.
                    const M &cm = m;
                    // FIXME: we need a better floating point comparison...
                    check (*it1e == cm (it1e.index1 (), it1e.index2 ()), bad_index ());
                    ++ it1e;
                }
                ++ it2e;
            }
#endif
#endif
        }

        // Dispatcher
        template<class M, class E>
        NUMERICS_INLINE
        void operator () (M &m, matrix_expression<E> &e) {
            typedef typename storage_orientation_traits<NUMERICS_TYPENAME M::storage_category,
                                                        NUMERICS_TYPENAME M::orientation_category>::storage_orientation_category storage_orientation_category;
            typedef typename matrix_swap_traits<storage_orientation_category, 
                                                NUMERICS_TYPENAME E::orientation_category, 
                                                NUMERICS_TYPENAME E::const_iterator1::iterator_category,
                                                NUMERICS_TYPENAME E::const_iterator2::iterator_category>::dispatch_category dispatch_category;
            operator () (m, e, dispatch_category ());
        }
    };

    template<class E> 
    class matrix_const_reference;
    template<class E> 
    class matrix_reference;

    // Array based matrix class 
    template<class T, class F, class A>
    class matrix: 
        public matrix_expression<matrix<T, F, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef F functor_type;
        typedef A array_type;
        typedef const A const_array_type;
        typedef const matrix<T, F, A> const_self_type;
        typedef matrix<T, F, A> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef matrix_range<self_type> matrix_range_type;
#endif
        typedef typename A::const_iterator const_iterator_type;
        typedef typename A::iterator iterator_type;
        typedef dense_tag storage_category;
        typedef unknown_orientation_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        matrix (): 
            size1_ (0), size2_ (0), data_ (0) {}
        NUMERICS_INLINE
        matrix (size_type size1, size_type size2):
            size1_ (size1), size2_ (size2), data_ (size1 * size2) {}
        NUMERICS_INLINE
        matrix (size_type size1, size_type size2, const array_type &data): 
            size1_ (size1), size2_ (size2), data_ (data) {}
        NUMERICS_INLINE
        matrix (const matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_), data_ (m.data_) {}
        template<class AE>
        NUMERICS_INLINE
        matrix (const matrix_expression<AE> &ae):
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()), data_ (ae ().size1 () * ae ().size2 ()) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
        }

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }
        NUMERICS_INLINE
        const_array_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
            data ().resize (size1 * size2);
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator () (size_type i, size_type j) const {
            return data () [functor_type::element (i, size1_, j, size2_)];
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) {
            return data () [functor_type::element (i, size1_, j, size2_)];
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        matrix &operator = (const matrix &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            data () = m.data ();
            return *this;
        }
        NUMERICS_INLINE
        matrix &assign_temporary (matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &operator = (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (ae));
#else
            // return assign (self_type (ae));
            self_type temporary (ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &reset (const matrix_expression<AE> &ae) { 
            self_type temporary (ae);
            resize (temporary.size1 (), temporary.size2 ());
            return assign_temporary (temporary);
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &assign (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix& operator += (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this + ae));
#else
            // return assign (self_type (*this + ae));
            self_type temporary (*this + ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        matrix& operator -= (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this - ae));
#else
            // return assign (self_type (*this - ae));
            self_type temporary (*this - ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        matrix& operator /= (const AT &at) {
            matrix_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            data ().swap (m.data ());
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (matrix &m1, matrix &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion and erasure
        // These functions should work with std::vector.
        // Thanks to Kresimir Fresl for spotting this.
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference t) {
            check (data () [functor_type::element (i, size1_, j, size2_)] == value_type (), bad_index ());
            // data ().insert (data ().begin () + functor_type::element (i, size1_, j, size2_), t);
            data () [functor_type::element (i, size1_, j, size2_)] = t;
        }
        NUMERICS_INLINE
        void erase (size_type i, size_type j) {
            // data ().erase (data ().begin () + functor_type::element (i, size1_, j, size2_));
            data () [functor_type::element (i, size1_, j, size2_)] = value_type ();
        }
        NUMERICS_INLINE
        void clear () {
            // data ().clear ();
            std::fill (data ().begin (), data ().end (), value_type ());
        }

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_iterator<self_type, dense_random_access_iterator_tag> iterator1;
        typedef matrix_column_iterator<self_type, dense_random_access_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<iterator1, typename matrix_row<self_type>, typename matrix_row<self_type> > reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
        typedef reverse_iterator<iterator2, typename matrix_column<self_type>, typename matrix_column<self_type> > reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<iterator1> reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator<iterator2> reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<self_type, dense_random_access_iterator_tag> iterator1;
        typedef indexed_iterator2<self_type, dense_random_access_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, dense_random_access_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, i, j);
#else
            return const_iterator1 (*this, data ().begin () + functor_type::element (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator1 (*this, i, j);
#else
            return iterator1 (*this, data ().begin () + functor_type::element (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, i, j);
#else
            return const_iterator2 (*this, data ().begin () + functor_type::element (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator2 (*this, i, j);
#else
            return iterator2 (*this, data ().begin () + functor_type::element (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_first1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_last1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_first2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_last2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }

        // Iterators simply are pointers.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
            typedef typename matrix::const_reference reference;
            typedef typename matrix::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<matrix> (), it_ () {}
            NUMERICS_INLINE
            const_iterator1 (const matrix &m, const const_iterator_type &it):
                container_const_reference<matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<matrix> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                functor_type::increment1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                functor_type::decrement1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it_ += n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it_ -= n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return functor_type::distance1 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            const_reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const matrix &m = (*this) ();
                return m.find_first2 (1, index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const matrix &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator1;
        };
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator1:
            public container_reference<matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator1, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
            typedef typename matrix::reference reference;
            typedef typename matrix::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<matrix> (), it_ () {}
            NUMERICS_INLINE
            iterator1 (matrix &m, const iterator_type &it):
                container_reference<matrix> (m), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                functor_type::increment1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                functor_type::decrement1 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it_ += n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it_ -= n * functor_type::one1 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return functor_type::distance1 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }
            
            NUMERICS_INLINE
            iterator2 begin () const {
                matrix &m = (*this) ();
                return m.find_first2 (1, index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                matrix &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin1 ().it_, m.size1 (), m.size2 ());
            }

            // Assignment 
            NUMERICS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator1;
        };
#endif

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
            typedef typename matrix::const_reference reference;
            typedef typename matrix::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<matrix> (), it_ () {}
            NUMERICS_INLINE
            const_iterator2 (const matrix &m, const const_iterator_type &it):
                container_const_reference<matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<matrix> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                functor_type::increment2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                functor_type::decrement2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it_ += n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it_ -= n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return functor_type::distance2 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            const_reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const matrix &m = (*this) ();
                return m.find_first1 (1, 0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const matrix &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator2;
        };
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2_);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator2:
            public container_reference<matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator2, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename matrix::difference_type difference_type;
            typedef typename matrix::value_type value_type;
            typedef typename matrix::reference reference;
            typedef typename matrix::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<matrix> (), it_ () {}
            NUMERICS_INLINE
            iterator2 (matrix &m, const iterator_type &it):
                container_reference<matrix> (m), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                functor_type::increment2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                functor_type::decrement2 (it_, (*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                it_ += n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                it_ -= n * functor_type::one2 ((*this) ().size1 (), (*this) ().size2 ());
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                return functor_type::distance2 (it_ - it.it_, (*this) ().size1 (), (*this) ().size2 ());
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                matrix &m = (*this) ();
                return m.find_first1 (1, 0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () const {
                matrix &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                matrix &m = (*this) ();
                return functor_type::index1 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }
            NUMERICS_INLINE
            size_type index2 () const {
                matrix &m = (*this) ();
                return functor_type::index2 (it_ - m.begin2 ().it_, m.size1 (), m.size2 ());
            }

            // Assignment 
            NUMERICS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator2;
        };
#endif

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find_last2 (0, 0, size2_);
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        NUMERICS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        size_type size1_;
        size_type size2_;
        array_type data_;
    };

    // Array based matrix class 
    template<class T, class F, class A>
    class vector_of_vector: 
        public matrix_expression<vector_of_vector<T, F, A> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef F functor_type;
        typedef A array_type;
        typedef const A const_array_type;
        typedef const vector_of_vector<T, F, A> const_self_type;
        typedef vector_of_vector<T, F, A> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef matrix_range<self_type> matrix_range_type;
#endif
        typedef typename A::const_iterator vector_const_iterator_type;
        typedef typename A::iterator vector_iterator_type;
        typedef typename A::value_type::const_iterator const_iterator_type;
        typedef typename A::value_type::iterator iterator_type;
        typedef dense_tag storage_category;
        typedef unknown_orientation_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        vector_of_vector (): 
            size1_ (0), size2_ (0), data_ (1) {}
        NUMERICS_INLINE
        vector_of_vector (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2), 
            data_ (functor_type::size1 (size1, size2) + 1) {
            for (size_type k = 0; k < functor_type::size1 (size1, size2); ++ k) 
                data_ [k].resize (functor_type::size2 (size1, size2));
        }
        NUMERICS_INLINE
        vector_of_vector (const vector_of_vector &m): 
            size1_ (m.size1_), size2_ (m.size2_), data_ (m.data_) {}
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector (const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()),
            data_ (functor_type::size1 (ae ().size1 (), ae ().size2 ()) + 1) { 
            for (size_type k = 0; k < functor_type::size1 (ae ().size1 (), ae ().size2 ()); ++ k) 
                data_ [k].resize (functor_type::size2 (ae ().size1 (), ae ().size2 ()));
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }
        NUMERICS_INLINE
        const_array_type &data () const {
            return data_;
        }
        NUMERICS_INLINE
        array_type &data () {
            return data_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
            data ().resize (functor_type::size1 (size1, size2) + 1);
            for (size_type k = 0; k < functor_type::size1 (size1, size2); ++ k) 
                data () [k].resize (functor_type::size2 (size1, size2));
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator () (size_type i, size_type j) const {
            return data () [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)]; 
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) {
            return data () [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)]; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        vector_of_vector &operator = (const vector_of_vector &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            data () = m.data ();
            return *this;
        }
        NUMERICS_INLINE
        vector_of_vector &assign_temporary (vector_of_vector &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &operator = (const matrix_expression<AE> &ae) { 
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (ae));
#else
            // return assign (self_type (ae));
            self_type temporary (ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &reset (const matrix_expression<AE> &ae) { 
            self_type temporary (ae);
            resize (temporary.size1 (), temporary.size2 ());
            return assign_temporary (temporary);
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector& operator += (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this + ae));
#else
            // return assign (self_type (*this + ae));
            self_type temporary (*this + ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector& operator -= (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this - ae));
#else
            // return assign (self_type (*this - ae));
            self_type temporary (*this - ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        vector_of_vector &minus_assign (const matrix_expression<AE> &ae) {
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector_of_vector& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        vector_of_vector& operator /= (const AT &at) {
            matrix_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (vector_of_vector &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            data ().swap (m.data ());
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (vector_of_vector &m1, vector_of_vector &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion and erasure
        // These functions should work with std::vector.
        // Thanks to Kresimir Fresl for spotting this.
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference t) {
            check (data () [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)] == value_type (), bad_index ());
            data () [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)] = t; 
        }
        NUMERICS_INLINE
        void erase (size_type i, size_type j) {
            data () [functor_type::element1 (i, size1_, j, size2_)] [functor_type::element2 (i, size1_, j, size2_)] = value_type (); 
        }
        NUMERICS_INLINE
        void clear () {
            for (size_type k = 0; k < functor_type::size1 (size1_, size2_); ++ k)
                // data () [k].clear ();
                std::fill (data () [k].begin (), data () [k].end (), value_type ());
        }

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_iterator<self_type, dense_random_access_iterator_tag> iterator1;
        typedef matrix_column_iterator<self_type, dense_random_access_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<iterator1, typename matrix_row<self_type>, typename matrix_row<self_type> > reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
        typedef reverse_iterator<iterator2, typename matrix_column<self_type>, typename matrix_column<self_type> > reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<iterator1> reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator<iterator2> reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<self_type, dense_random_access_iterator_tag> iterator1;
        typedef indexed_iterator2<self_type, dense_random_access_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, dense_random_access_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, i, j);
#else
            return const_iterator1 (*this, i, j, data () [functor_type::element1 (i, size1_, j, size2_)].begin ()  + functor_type::element2 (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator1 (*this, i, j);
#else
            return iterator1 (*this, i, j, data () [functor_type::element1 (i, size1_, j, size2_)].begin ()  + functor_type::element2 (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, i, j);
#else
            return const_iterator2 (*this, i, j, data () [functor_type::element1 (i, size1_, j, size2_)].begin ()  + functor_type::element2 (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator2 (*this, i, j);
#else
            return iterator2 (*this, i, j, data () [functor_type::element1 (i, size1_, j, size2_)].begin () + functor_type::element2 (i, size1_, j, size2_));
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_first1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_last1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_first2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_last2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }

        // Iterators simply are pointers.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<vector_of_vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
            typedef typename vector_of_vector::const_reference reference;
            typedef typename vector_of_vector::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            const_iterator1 (const vector_of_vector &m, size_type i, size_type j, const const_iterator_type &it):
                container_const_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<vector_of_vector> (it ()), i_ (it.i_), j_ (it.j_), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ i_;
                const vector_of_vector &m = (*this) ();
                if (functor_type::fast1 ())
                    ++ it_;
                else 
                    it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- i_;
                const vector_of_vector &m = (*this) ();
                if (functor_type::fast1 ())
                    -- it_;
                else
                    it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                i_ += n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                i_ -= n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                check (index2 () == it.index2 (), bad_index ());
                return index1 () - it.index1 ();
            }

            // Dereference
            NUMERICS_INLINE
            const_reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const vector_of_vector &m = (*this) ();
                return m.find_first2 (1, index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const vector_of_vector &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<vector_of_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            const_iterator_type it_;

            friend class iterator1;
        };
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator1:
            public container_reference<vector_of_vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator1, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
            typedef typename vector_of_vector::reference reference;
            typedef typename vector_of_vector::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            iterator1 (vector_of_vector &m, size_type i, size_type j, const iterator_type &it):
                container_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                ++ i_;
                vector_of_vector &m = (*this) ();
                if (functor_type::fast1 ())
                    ++ it_;
                else
                    it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                -- i_;
                vector_of_vector &m = (*this) ();
                if (functor_type::fast1 ())
                    -- it_;
                else
                    it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                i_ += n;
                vector_of_vector &m = (*this) ();
                it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                i_ -= n;
                vector_of_vector &m = (*this) ();
                it_ = m.find1 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                check (index2 () == it.index2 (), bad_index ());
                return index1 () - it.index1 ();
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }
            
            NUMERICS_INLINE
            iterator2 begin () const {
                vector_of_vector &m = (*this) ();
                return m.find_first2 (1, index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                vector_of_vector &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<vector_of_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            iterator_type it_;

            friend class const_iterator1;
        };
#endif

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<vector_of_vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
            typedef typename vector_of_vector::const_reference reference;
            typedef typename vector_of_vector::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            const_iterator2 (const vector_of_vector &m, size_type i, size_type j, const const_iterator_type &it):
                container_const_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<vector_of_vector> (it ()), i_ (it.i_), j_ (it.j_), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ j_;
                const vector_of_vector &m = (*this) ();
                if (functor_type::fast2 ())
                    ++ it_;
                else
                    it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- j_;
                const vector_of_vector &m = (*this) ();
                if (functor_type::fast2 ())
                    -- it_;
                else
                    it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                j_ += n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                j_ -= n;
                const vector_of_vector &m = (*this) ();
                it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                check (index1 () == it.index1 (), bad_index ());
                return index2 () - it.index2 ();
            }

            // Dereference
            NUMERICS_INLINE
            const_reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const vector_of_vector &m = (*this) ();
                return m.find_first1 (1, 0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const vector_of_vector &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<vector_of_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            const_iterator_type it_;

            friend class iterator2;
        };
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2_);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator2:
            public container_reference<vector_of_vector>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator2, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename vector_of_vector::difference_type difference_type;
            typedef typename vector_of_vector::value_type value_type;
            typedef typename vector_of_vector::reference reference;
            typedef typename vector_of_vector::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<vector_of_vector> (), i_ (), j_ (), it_ () {}
            NUMERICS_INLINE
            iterator2 (vector_of_vector &m, size_type i, size_type j, const iterator_type &it):
                container_reference<vector_of_vector> (m), i_ (i), j_ (j), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                ++ j_;
                vector_of_vector &m = (*this) ();
                if (functor_type::fast2 ())
                    ++ it_;
                else
                    it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                -- j_;
                vector_of_vector &m = (*this) ();
                if (functor_type::fast2 ())
                    -- it_;
                else
                    it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                j_ += n;
                vector_of_vector &m = (*this) ();
                it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                j_ -= n;
                vector_of_vector &m = (*this) ();
                it_ = m.find2 (1, i_, j_).it_;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                check (index1 () == it.index1 (), bad_index ());
                return index2 () - it.index2 ();
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                vector_of_vector &m = (*this) ();
                return m.find_first1 (1, 0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () const {
                vector_of_vector &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return i_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return j_;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<vector_of_vector>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            size_type i_;
            size_type j_;
            iterator_type it_;

            friend class const_iterator2;
        };
#endif

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find_last2 (0, 0, size2_);
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        NUMERICS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        size_type size1_;
        size_type size2_;
        array_type data_;
    };

    // Identity matrix class
    template<class T>
    class identity_matrix:
        public matrix_expression<identity_matrix<T> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const identity_matrix<T> const_self_type;
        typedef identity_matrix<T> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
#endif
        typedef size_type const_iterator_type;
        typedef packed_tag storage_category;
        typedef unknown_orientation_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        identity_matrix (): 
            size1_ (0), size2_ (0) {}
        NUMERICS_INLINE
        identity_matrix (size_type size): 
            size1_ (size), size2_ (size) {}
        NUMERICS_INLINE
        identity_matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2) {}
        NUMERICS_INLINE
        identity_matrix (const identity_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_) {}

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const {
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const {
            return size2_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size1_ = size;
            size2_ = size;
        }
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            return i == j; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        identity_matrix &operator = (const identity_matrix &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            return *this;
        }
        NUMERICS_INLINE
        identity_matrix &assign_temporary (identity_matrix &m) { 
            swap (m);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (identity_matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (identity_matrix &m1, identity_matrix &m2) {
            m1.swap (m2);
        }
#endif

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_const_iterator<self_type, packed_random_access_iterator_tag> iterator1;
        typedef matrix_column_const_iterator<self_type, packed_random_access_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, packed_random_access_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, packed_random_access_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator1<self_type, packed_random_access_iterator_tag> iterator1;
        typedef indexed_const_iterator2<self_type, packed_random_access_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, packed_random_access_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, packed_random_access_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class const_iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                i = std::max (i, j);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                i = std::min (i, j + 1);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            if (rank == 1) 
                j = std::max (j, i);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            if (rank == 1)
                j = std::min (j, i + 1);
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<identity_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename identity_matrix::difference_type difference_type;
            typedef typename identity_matrix::value_type value_type;
            typedef typename identity_matrix::value_type reference;
            typedef typename identity_matrix::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<identity_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const identity_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<identity_matrix> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return (*this) () (index1 (), index2 ());
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const identity_matrix &m = (*this) ();
                return m.find_first2 (1, index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const identity_matrix &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<identity_matrix>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_ ;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        typedef const_iterator1 iterator1;
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<identity_matrix>,
            public random_access_iterator_base<packed_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef packed_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename identity_matrix::difference_type difference_type;
            typedef typename identity_matrix::value_type value_type;
            typedef typename identity_matrix::value_type reference;
            typedef typename identity_matrix::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<identity_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const identity_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<identity_matrix> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return (*this) () (index1 (), index2 ());
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const identity_matrix &m = (*this) ();
                return m.find_first1 (1, 0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const identity_matrix &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<identity_matrix>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        typedef const_iterator2 iterator2;
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2_);
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
        size_type size1_;
        size_type size2_;
    };

    // Zero matrix class
    template<class T>
    class zero_matrix:
        public matrix_expression<zero_matrix<T> > {
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const zero_matrix<T> const_self_type;
        typedef zero_matrix<T> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
#endif
        typedef size_type const_iterator_type;
        typedef sparse_tag storage_category;
        typedef unknown_orientation_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        zero_matrix ():
            size1_ (0), size2_ (0) {}
        NUMERICS_INLINE
        zero_matrix (size_type size):
            size1_ (size), size2_ (size) {}
        NUMERICS_INLINE
        zero_matrix (size_type size1, size_type size2):
            size1_ (size1), size2_ (size2) {}
        NUMERICS_INLINE
        zero_matrix (const zero_matrix &m):
            size1_ (m.size1_), size2_ (m.size2_) {}

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const {
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const {
            return size2_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size) {
            size1_ = size;
            size2_ = size;
        }
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            return value_type ();
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        zero_matrix &operator = (const zero_matrix &m) {
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            return *this;
        }
        NUMERICS_INLINE
        zero_matrix &assign_temporary (zero_matrix &m) {
            swap (m);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (zero_matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (zero_matrix &m1, zero_matrix &m2) {
            m1.swap (m2);
        }
#endif

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_const_iterator<self_type, sparse_bidirectional_iterator_tag> iterator1;
        typedef matrix_column_const_iterator<self_type, sparse_bidirectional_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, sparse_bidirectional_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, sparse_bidirectional_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator1<self_type, sparse_bidirectional_iterator_tag> iterator1;
        typedef indexed_const_iterator2<self_type, sparse_bidirectional_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, sparse_bidirectional_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, sparse_bidirectional_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class const_iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            if (rank == 1)
                i = std::max (i, size_type (0));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            if (rank == 1)
                i = std::min (i, size_type (0));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            if (rank == 1)
                j = std::max (j, size_type (0));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            if (rank == 1)
                j = std::min (j, size_type (0));
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<zero_matrix>,
            public bidirectional_iterator_base<sparse_bidirectional_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename zero_matrix::difference_type difference_type;
            typedef typename zero_matrix::value_type value_type;
            typedef typename zero_matrix::value_type reference;
            typedef typename zero_matrix::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<zero_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const zero_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<zero_matrix> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return (*this) () (index1 (), index2 ());
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const zero_matrix &m = (*this) ();
                return m.find_first2 (1, index1 (), 0);
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const zero_matrix &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<zero_matrix>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_ ;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        typedef const_iterator1 iterator1;
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<zero_matrix>,
            public bidirectional_iterator_base<sparse_bidirectional_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef sparse_bidirectional_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename zero_matrix::difference_type difference_type;
            typedef typename zero_matrix::value_type value_type;
            typedef typename zero_matrix::value_type reference;
            typedef typename zero_matrix::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<zero_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const zero_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<zero_matrix> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return (*this) () (index1 (), index2 ());
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const zero_matrix &m = (*this) ();
                return m.find_first1 (1, 0, index2 ());
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const zero_matrix &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<zero_matrix>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        typedef const_iterator2 iterator2;
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2_);
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
        size_type size1_;
        size_type size2_;
    };

    // Scalar matrix class
    template<class T>
    class scalar_matrix: 
        public matrix_expression<scalar_matrix<T> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const scalar_matrix<T> const_self_type;
        typedef scalar_matrix<T> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
#endif
        typedef size_type const_iterator_type;
        typedef dense_tag storage_category;
        typedef unknown_orientation_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        scalar_matrix (): 
            size1_ (0), size2_ (0), value_ () {}
        NUMERICS_INLINE
        scalar_matrix (size_type size1, size_type size2, const value_type &value): 
            size1_ (size1), size2_ (size2), value_ (value_) {}
        NUMERICS_INLINE
        scalar_matrix (const scalar_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_), value_ (m.value_) {}

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            size1_ = size1;
            size2_ = size2;
        }

        // Element access
        NUMERICS_INLINE
        value_type operator () (size_type i, size_type j) const {
            return value_; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        scalar_matrix &operator = (const scalar_matrix &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            value_ = m.value_;
            return *this;
        }
        NUMERICS_INLINE
        scalar_matrix &assign_temporary (scalar_matrix &m) { 
            swap (m);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (scalar_matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            std::swap (value_, m.value_);
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (scalar_matrix &m1, scalar_matrix &m2) {
            m1.swap (m2);
        }
#endif

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_const_iterator<self_type, dense_random_access_iterator_tag> iterator1;
        typedef matrix_column_const_iterator<self_type, dense_random_access_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator1<self_type, dense_random_access_iterator_tag> iterator1;
        typedef indexed_const_iterator2<self_type, dense_random_access_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, dense_random_access_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class const_iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
            return const_iterator1 (*this, i, j);
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
#ifdef  NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
            return const_iterator2 (*this, i, j);
#endif
        }   
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }

        // Iterators simply are indices.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<scalar_matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename scalar_matrix::difference_type difference_type;
            typedef typename scalar_matrix::value_type value_type;
            typedef typename scalar_matrix::value_type reference;
            typedef typename scalar_matrix::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<scalar_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator1 (const scalar_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<scalar_matrix> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return it1_ - it.it1_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return (*this) () (index1 (), index2 ());
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const scalar_matrix &m = (*this) ();
                return m.find_first2 (1, index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const scalar_matrix &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<scalar_matrix>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_ ;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        typedef const_iterator1 iterator1;
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<scalar_matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename scalar_matrix::difference_type difference_type;
            typedef typename scalar_matrix::value_type value_type;
            typedef typename scalar_matrix::value_type reference;
            typedef typename scalar_matrix::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<scalar_matrix> (), it1_ (), it2_ () {}
            NUMERICS_INLINE
            const_iterator2 (const scalar_matrix &m, const const_iterator_type &it1, const const_iterator_type &it2):
                container_const_reference<scalar_matrix> (m), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it2_ - it.it2_;
            }

            // Dereference
            NUMERICS_INLINE
            value_type operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return (*this) () (index1 (), index2 ());
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const scalar_matrix &m = (*this) ();
                return m.find_first1 (1, 0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const scalar_matrix &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                return it1_;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                return it2_;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<scalar_matrix>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }

        private:
            const_iterator_type it1_;
            const_iterator_type it2_;
        };

        typedef const_iterator2 iterator2;
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2_);
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

    private:
        size_type size1_;
        size_type size2_;
        value_type value_;
    };

    // Array based matrix class 
    template<class T, std::size_t N, std::size_t M>
    class c_matrix: 
        public matrix_expression<c_matrix<T, N, M> > {
    public:      
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
        typedef const c_matrix<T, N, M> const_self_type;
        typedef c_matrix<T, N, M> self_type;
        typedef const matrix_const_reference<const_self_type> const_closure_type;
        typedef matrix_reference<self_type> closure_type;
#ifdef NUMERICS_DEPRECATED
        typedef const matrix_row<const_self_type> const_matrix_row_type;
        typedef matrix_row<self_type> matrix_row_type;
        typedef const matrix_column<const_self_type> const_matrix_column_type;
        typedef matrix_column<self_type> matrix_column_type;
        typedef const matrix_range<const_self_type> const_matrix_range_type;
        typedef matrix_range<self_type> matrix_range_type;
#endif
        typedef const T *const_iterator_type;
        typedef T *iterator_type;
        typedef dense_tag storage_category;
        typedef unknown_orientation_tag orientation_category;

        // Construction and destruction
        NUMERICS_INLINE
        c_matrix (): 
            size1_ (N), size2_ (M) /* , data_ () */ {
        }
        NUMERICS_INLINE
        c_matrix (size_type size1, size_type size2): 
            size1_ (size1), size2_ (size2) /* , data_ () */ {
            if (size1_ > N || size2_ > M) 
                throw std::bad_alloc ();
        }
        NUMERICS_INLINE
        c_matrix (const c_matrix &m): 
            size1_ (m.size1_), size2_ (m.size2_) /* , data_ () */ {
            if (size1_ > N || size2_ > M) 
                throw std::bad_alloc ();
            *this = m;
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix (const matrix_expression<AE> &ae): 
            size1_ (ae ().size1 ()), size2_ (ae ().size2 ()) /* , data_ () */ { 
            if (size1_ > N || size2_ > M) 
                throw std::bad_alloc ();
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
        }

        // Accessors
        NUMERICS_INLINE
        size_type size1 () const { 
            return size1_;
        }
        NUMERICS_INLINE
        size_type size2 () const { 
            return size2_;
        }

        // Resizing
        NUMERICS_INLINE
        void resize (size_type size1, size_type size2) {
            if (size1 > N || size2 > M) 
                throw std::bad_alloc ();
            // The content of the array is intentionally not copied.
            size1_ = size1;
            size2_ = size2;
        }

        // Element access
        NUMERICS_INLINE
        const_reference operator () (size_type i, size_type j) const {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            return data_ [i] [j]; 
        }
        NUMERICS_INLINE
        reference operator () (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            return data_ [i] [j]; 
        }

#ifdef NUMERICS_DEPRECATED
        NUMERICS_INLINE
        const_matrix_row_type operator [] (size_type i) const {
            return const_matrix_row_type (*this, i);
        }
        NUMERICS_INLINE
        matrix_row_type operator [] (size_type i) {
            return matrix_row_type (*this, i);
        }
#endif

        // Assignment
        NUMERICS_INLINE
        c_matrix &operator = (const c_matrix &m) { 
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            size1_ = m.size1_;
            size2_ = m.size2_;
            for (size_type i = 0; i < m.size1_; ++ i)
                std::copy (m.data_ [i], m.data_ [i] + m.size2_, data_ [i]);
            return *this;
        }
        NUMERICS_INLINE
        c_matrix &assign_temporary (c_matrix &m) { 
            swap (m);
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &operator = (const matrix_expression<AE> &ae) { 
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (ae));
#else
            // return assign (self_type (ae));
            self_type temporary (ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &reset (const matrix_expression<AE> &ae) { 
            self_type temporary (ae);
            resize (temporary.size1 (), temporary.size2 ());
            return assign_temporary (temporary);
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix& operator += (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this + ae));
#else
            // return assign (self_type (*this + ae));
            self_type temporary (*this + ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_plus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix& operator -= (const matrix_expression<AE> &ae) {
#ifdef NUMERICS_MUTABLE_TEMPORARY
            return assign_temporary (self_type (*this - ae));
#else
            // return assign (self_type (*this - ae));
            self_type temporary (*this - ae);
            return assign_temporary (temporary);
#endif
        }
        template<class AE>
        NUMERICS_INLINE
        c_matrix &minus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign<scalar_minus_assign<value_type, NUMERICS_TYPENAME AE::value_type> > () (*this, ae); 
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        c_matrix& operator *= (const AT &at) {
            matrix_assign_scalar<scalar_multiplies_assign<value_type, AT> > () (*this, at);
            return *this;
        }
        template<class AT>
        NUMERICS_INLINE
        c_matrix& operator /= (const AT &at) {
            matrix_assign_scalar<scalar_divides_assign<value_type, AT> > () (*this, at);
            return *this;
        }

        // Swapping
        NUMERICS_INLINE
        void swap (c_matrix &m) {
            check (this != &m, external_logic ());
            check (size1_ == m.size1_, bad_size ());
            check (size2_ == m.size2_, bad_size ());
            std::swap (size1_, m.size1_);
            std::swap (size2_, m.size2_);
            for (size_type i = 0; i < size1_; ++ i)
                std::swap_ranges (data_ [i], data_ [i] + size2_, m.data_ [i]);
        }
#ifdef NUMERICS_FRIEND_FUNCTION
        NUMERICS_INLINE
        friend void swap (c_matrix &m1, c_matrix &m2) {
            m1.swap (m2);
        }
#endif

        // Element insertion and erasure
        NUMERICS_INLINE
        void insert (size_type i, size_type j, const_reference t) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            check (data_ [i] [j] == value_type (), bad_index ());
            data_ [i] [j] = t;
        }
        NUMERICS_INLINE
        void erase (size_type i, size_type j) {
            check (i < size1_, bad_index ());
            check (j < size2_, bad_index ());
            data_ [i] [j] = value_type ();
        }
        NUMERICS_INLINE
        void clear () {
            for (size_type i = 0; i < size1_; ++ i)
                std::fill (data_ [i], data_ [i] + size2_, value_type ());
        }

#ifdef NUMERICS_USE_CANONICAL_ITERATOR
        typedef matrix_row_iterator<self_type, dense_random_access_iterator_tag> iterator1;
        typedef matrix_column_iterator<self_type, dense_random_access_iterator_tag> iterator2;
        typedef matrix_row_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef matrix_column_const_iterator<self_type, dense_random_access_iterator_tag> const_iterator2;
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator<const_iterator1, typename matrix_row<self_type>, typename matrix_row<const_self_type> > const_reverse_iterator1;
        typedef reverse_iterator<iterator1, typename matrix_row<self_type>, typename matrix_row<self_type> > reverse_iterator1;
        typedef reverse_iterator<const_iterator2, typename matrix_column<self_type>, typename matrix_column<const_self_type> > const_reverse_iterator2;
        typedef reverse_iterator<iterator2, typename matrix_column<self_type>, typename matrix_column<self_type> > reverse_iterator2;
#else
        typedef reverse_iterator<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator<iterator1> reverse_iterator1;
        typedef reverse_iterator<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator<iterator2> reverse_iterator2;
#endif
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<self_type, dense_random_access_iterator_tag> iterator1;
        typedef indexed_iterator2<self_type, dense_random_access_iterator_tag> iterator2;
        typedef indexed_const_iterator1<self_type, dense_random_access_iterator_tag> const_iterator1;
        typedef indexed_const_iterator2<self_type, dense_random_access_iterator_tag> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator1<const_iterator1, value_type, value_type> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2, value_type, value_type> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator1<iterator1> reverse_iterator1;
        typedef reverse_iterator2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator2<iterator2> reverse_iterator2;
#endif
#endif

        // Element lookup
        NUMERICS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, i, j);
#else
            return const_iterator1 (*this, &data_ [i] [j]);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator1 (*this, i);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator1 (*this, i, j);
#else
            return iterator1 (*this, &data_ [i] [j]);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return const_iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, i, j);
#else
            return const_iterator2 (*this, &data_ [i] [j]);
#endif
#endif
        }
        NUMERICS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
#ifdef NUMERICS_USE_CANONICAL_ITERATOR
            return iterator2 (*this, j);
#else
#ifdef NUMERICS_USE_INDEXED_ITERATOR
            return iterator2 (*this, i, j);
#else
            return iterator2 (*this, &data_ [i] [j]);
#endif
#endif
        }
        NUMERICS_INLINE
        const_iterator1 find_first1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_first1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator1 find_last1 (int rank, size_type i, size_type j) const {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator1 find_last1 (int rank, size_type i, size_type j) {
            return find1 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_first2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_first2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        const_iterator2 find_last2 (int rank, size_type i, size_type j) const {
            return find2 (rank, i, j);
        }
        NUMERICS_INLINE
        iterator2 find_last2 (int rank, size_type i, size_type j) {
            return find2 (rank, i, j);
        }

        // Iterators simply are pointers.

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator1:
            public container_const_reference<c_matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator1, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
            typedef typename c_matrix::const_reference reference;
            typedef typename c_matrix::const_pointer pointer;
#endif
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator1 ():
                container_const_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            const_iterator1 (const c_matrix &m, const const_iterator_type &it):
                container_const_reference<c_matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<c_matrix> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator1 &operator ++ () {
                it_ += M;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -- () {
                it_ -= M;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it_ += n * M;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it_ -= n * M;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                return (it_ - it.it_) / M;
            }

            // Dereference
            NUMERICS_INLINE
            const_reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator2 begin () const {
                const c_matrix &m = (*this) ();
                return m.find_first2 (1, index1 (), 0); 
            }
            NUMERICS_INLINE
            const_iterator2 end () const {
                const c_matrix &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) % M;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<c_matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator1;
        };
#endif

        NUMERICS_INLINE
        const_iterator1 begin1 () const {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator1 end1 () const {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator1:
            public container_reference<c_matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator1, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
            typedef typename c_matrix::reference reference;
            typedef typename c_matrix::pointer pointer;
#endif
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator1 ():
                container_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            iterator1 (c_matrix &m, const iterator_type &it):
                container_reference<c_matrix> (m), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator1 &operator ++ () {
                it_ += M;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -- () {
                it_ -= M;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator += (difference_type n) {
                it_ += n * M;
                return *this;
            }
            NUMERICS_INLINE
            iterator1 &operator -= (difference_type n) {
                it_ -= n * M;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator1 &it) const {
                return (it_ - it.it_) / M;
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }
            
            NUMERICS_INLINE
            iterator2 begin () const {
                c_matrix &m = (*this) ();
                return m.find_first2 (1, index1 (), 0);
            }
            NUMERICS_INLINE
            iterator2 end () const {
                c_matrix &m = (*this) ();
                return m.find_last2 (1, index1 (), m.size2 ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin1 ().it_) % M;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<c_matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator1 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator1;
        };
#endif

        NUMERICS_INLINE
        iterator1 begin1 () {
            return find_first1 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator1 end1 () {
            return find_last1 (0, size1_, 0);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class const_iterator2:
            public container_const_reference<c_matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               const_iterator2, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
            typedef typename c_matrix::const_reference reference;
            typedef typename c_matrix::const_pointer pointer;
#endif
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            const_iterator2 ():
                container_const_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            const_iterator2 (const c_matrix &m, const const_iterator_type &it):
                container_const_reference<c_matrix> (m), it_ (it) {}
            NUMERICS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<c_matrix> (it ()), it_ (it.it_) {}

            // Arithmetic
            NUMERICS_INLINE
            const_iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            const_reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            const_iterator1 begin () const {
                const c_matrix &m = (*this) ();
                return m.find_first1 (1, 0, index2 ()); 
            }
            NUMERICS_INLINE
            const_iterator1 end () const {
                const c_matrix &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) % M;
            }

            // Assignment 
            NUMERICS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<c_matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const const_iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            const_iterator_type it_;

            friend class iterator2;
        };
#endif

        NUMERICS_INLINE
        const_iterator2 begin2 () const {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        const_iterator2 end2 () const {
            return find_last2 (0, 0, size2_);
        }

#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR)
        class iterator2:
            public container_reference<c_matrix>,
            public random_access_iterator_base<dense_random_access_iterator_tag,
                                               iterator2, value_type> {
        public:
            typedef dense_random_access_iterator_tag iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename c_matrix::difference_type difference_type;
            typedef typename c_matrix::value_type value_type;
            typedef typename c_matrix::reference reference;
            typedef typename c_matrix::pointer pointer;
#endif
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            NUMERICS_INLINE
            iterator2 ():
                container_reference<c_matrix> (), it_ () {}
            NUMERICS_INLINE
            iterator2 (c_matrix &m, const iterator_type &it):
                container_reference<c_matrix> (m), it_ (it) {}

            // Arithmetic
            NUMERICS_INLINE
            iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            NUMERICS_INLINE
            iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            NUMERICS_INLINE
            difference_type operator - (const iterator2 &it) const {
                return it_ - it.it_;
            }

            // Dereference
            NUMERICS_INLINE
            reference operator * () const {
                check (index1 () < (*this) ().size1 (), bad_index ());
                check (index2 () < (*this) ().size2 (), bad_index ());
                return *it_;
            }

            NUMERICS_INLINE
            iterator1 begin () const {
                c_matrix &m = (*this) ();
                return m.find_first1 (1, 0, index2 ());
            }
            NUMERICS_INLINE
            iterator1 end () const {
                c_matrix &m = (*this) ();
                return m.find_last1 (1, m.size1 (), index2 ()); 
            }
            NUMERICS_INLINE
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            NUMERICS_INLINE
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
            }

            // Indices
            NUMERICS_INLINE
            size_type index1 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) / M;
            }
            NUMERICS_INLINE
            size_type index2 () const {
                const c_matrix &m = (*this) ();
                return (it_ - m.begin2 ().it_) % M;
            }

            // Assignment 
            NUMERICS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<c_matrix>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            NUMERICS_INLINE
            bool operator == (const iterator2 &it) const {
                check (&(*this) () == &it (), external_logic ());
                return it_ == it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator2;
        };
#endif

        NUMERICS_INLINE
        iterator2 begin2 () {
            return find_first2 (0, 0, 0);
        }
        NUMERICS_INLINE
        iterator2 end2 () {
            return find_last2 (0, 0, size2_);
        }

        // Reverse iterators

        NUMERICS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        NUMERICS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        NUMERICS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        NUMERICS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        NUMERICS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        size_type size1_;
        size_type size2_;
        value_type data_ [N] [M];
    };

}}

#endif




















