#pragma once

template<class T>
inline void parallel_shellSort(T* a, long nLength)
{
	unsigned long nThreads = getCountThreads(nLength, PARALLEL_SHELLSORT_MIN_PER_THREAD);
	unsigned long nBlockSize = getBlocksSize(nLength, nThreads);

	T* begin = a;
	if (nBlockSize < PARALLEL_SHELLSORT_CUTOFF)
	{
		shellSort(a, nLength);
		return;
	}

	std::vector<std::thread> threads(nThreads - 1);
	for(unsigned long i = 0; i < (nThreads - 1); ++i)
	{
		threads[i] = std::thread(&shellSort<T>, a + i*nBlockSize, nBlockSize - 1);
	}
	shellSort(a + (nThreads - 1)*nBlockSize, nLength - (nThreads - 1)*nBlockSize);

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	merge(begin, nBlockSize, nLength);
}
