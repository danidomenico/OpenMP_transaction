/**
 * \file cowichan_openmp_wong/winnow.cpp
 * \brief OpenMP winnow implementation (transactional memory).
 * \see CowichanOpenMP::winnow
 */

#include "../cowichan_openmp/cowichan_openmp.hpp"
#include "sort.hpp"

namespace cowichan_openmp {

/**
 * Count the number of set cells in each thread bucket.
 * \param mask boolean mask.
 * \param nr number of rows in mask.
 * \param nc number of columns in mask.
 */
index_t mask_count(BoolMatrix mask, index_t nr, index_t nc);

}

/*****************************************************************************/

void CowichanOpenMP::winnow(IntMatrix matrix, BoolMatrix mask, PointVector points) {

	index_t r, c;
	index_t len; // number of points
	index_t stride; // selection stride
	index_t i;

	// count set cell
	len = mask_count (mask, nr, nc);

	if(len < n) {
		not_enough_points();
	}

	WeightedPointVector weightedPoints = NULL;
	try {
		weightedPoints = NEW_VECTOR_SZ(WeightedPoint, len);
	}
	catch (...) {out_of_memory();}
  
	// fill temporary vector
	i = 0; //Proteje a variável 'i' na transação. 
	       //A ordem de acesso ao 'weightedPoints' não importa, pois ele será ordenado na sequência*/
	#pragma omp parallel for schedule(static) transaction
	for (r = 0; r < nr; r++) {
		#pragma omp parallel for schedule(static)
		for (c = 0; c < nc; c++) {
			if (MATRIX_RECT(mask, r, c)) {
				weightedPoints[i++] = WeightedPoint((real)c, (real)r, MATRIX_RECT(matrix, r, c));
			}
		}
	}

#ifdef SORT_TIME
	INT64 start, end;
	start = get_ticks ();
#endif

	// sort
#if defined(LIN32) || defined(LIN64)  
	#pragma omp parallel //Sempre vai cair aqui, pois o compilador suporta OpenMP tasks
	{
	#pragma omp single
	{
		quick_sort(weightedPoints, len);
	}
	}
#else
	histogram_sort(weightedPoints, len); //Versão sem tasks, não alterada
#endif

#ifdef SORT_TIME
  end = get_ticks ();
#endif

	// copy over points
	stride = len / n;

	#pragma omp parallel for schedule(static)
	for (i = n - 1; i >= 0; i--) {
#ifdef WINNOW_OUTPUT
		std::cout << weightedPoints[len - 1 - (n - 1 - i) * stride].weight << "\n";
#endif
		points[i] = weightedPoints[len - 1 - (n - 1 - i) * stride].point;
	}
  
#ifdef SORT_TIME
	std::cout << "winnow sort: ";
	print_elapsed_time(start, end);
	std::cout << std::endl;
#endif

	delete [] weightedPoints;
}

/*****************************************************************************/

namespace cowichan_openmp {

index_t mask_count(BoolMatrix mask, index_t nr, index_t nc) {
	index_t r, c;
	index_t sum = 0;

	#pragma omp parallel for schedule(static) transaction //Reduction não funcionaria com laço encadeado
	for (r = 0; r < nr; r++) {
		#pragma omp parallel for schedule(static)
		for (c = 0; c < nc; c++) {
			if (MATRIX_RECT_NC(mask, r, c, nc)) {
				sum++;
			}
		}
	}

	return sum;
}

}

