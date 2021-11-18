#include "ConcurrencyPCH.h"
#include "FuturesBasics.h"
#include "MemoryOrderBasics.h"
#include "SemaphoreLatchBarriers.h"
#include "JThreadsTokenBasics.h"
#include "ThreadLocksLocalStorage.h"
#include "TaskPromiseFuturesBase.h"
#include "ParalelAlgorithmsBasics.h"
#include "Coroutines/CoroutineExamples.hpp"
#include "Coroutines/CortnAsyncByteStreamParser.hpp"
#include "Coroutines/CortnAsyncByteStreamParserV2.hpp"
#include "Coroutines/CortnStartJobRequest.hpp"
#include "Coroutines/CortnThreadSync.hpp"
#include "Coroutines/CortnWithCustomAllocators.hpp"
#include "Coroutines/CortnWithMemoryPool.hpp"
#include "Coroutines/CortnPMRmemoryPool.hpp"
#include "Coroutines/CortnParserExceptions.hpp"
#include "AtomicsCpp20/AtomicCpp20Tests.hpp"
#include "SynchronizedStreamTests.hpp"

int main()
{
	//FutureBasics_Test();
	//Futures_WaitingTwoTasks();
	//SharedFutures_Example();
	//ThreadPromises_Example();
	//MutexLock_Basics();
	//ConditionVariable_Basics();
	//Atomics_Basics();
	//AtomicFlag_Basics();
	//SpinLock_Example();
	//AtomicFlag_ThreadSync();
	//AtomicTypes_Operations();
	//SharedPtr_AtomicOps();
	//AtomicRef_Example();
	//AcquireRelease_Example();
	//ReleaseSemantics_Example();
	//AcquireConsumeRelease_Example();
	//RelaxedSemantics_Example();
	//FencesAcquireRelease_Example();
	//FencesAtomicSignal_Example();

#if _HAS_CXX20
	//Semaphore_Example();
	//Latch_Example();
	//Barrier_Example();
	//StopToken_Example();
	//StopToken_ConditionVar();
	//JThread_Basics();
	//dotProductAsync();
	//SharedFuturePromise_Basics();
#endif
	//DeadLockSolution();
	//SharedLock_Example();
	//CallOnce_ThreadSafeInit();
	//Singleton_ThreadSafe();
	//ThreadLocalStorage_Basics();
	//ThreadLocalStorage_Basics2();
	//FireForgetFutures();
	
	//PackagedTask_Basics();
	//PackagedTask_Reuse();
	//PromiseFuture_Basics();
	//WaitFor_Basics();

	//SortParallel_Basics();
	//ForEachParallel_Basics();
	/* run this code on Release to see better results !!*/
	//TransformParallel_Basics();
	//AccumulateReduce_Basics(20'000'000);
	//LazyGenerator_Coroutine();
	//EagerFuture_Coroutine();
	ByteStreamParser_OLDWAY();
	AsyncByteStreamParser_Cortn();
	//AsyncByteStreamParserV2_Cortn();
	//InfinitiDataStream_Cornt();
	//Cortn_StartJobRequest();
	//CortnThreadSync_Example();
	//StreamParser_CustomNewDelete();
	StreamParser_MemoryPool();
	//StreamParser_PMRMemoryPool();
	//StreamParser_CortnExceptions();
	//AtomicRef();
	//AtomicSharedPointer();
	//AtomicFlag();
	//BinarySemaphore_Example();
	//LatchExample2();
	//BarrierExample2();
	//StopSource_Basics();
	//SynchronizedOutPut();
	//LazyFuture_Coroutine();
	//FutureOnThread_Coroutine();
	//GeneratorWithContainers_Cornt();
	//TransparentAwaiter_Cornt();
	//AutoResumeAwaiter_Cornt();
	//AwaiterThread_Cornt();
	//CoroutineBasics();
	//CoroutineBasicsYield();
	//CoroutineBasicsIterator();
	//CoroutineBasicsCoReturn();


	/* C++17 add to optimize the cached either for one thread sees one cache or multiple threads see the see same cache */
	
	/* "If you want to access two different (atomic) objects with the same thread use alignas with the size of constructive
		or smaller"
	*/
	/* "hardware_constructive_interference_size is the recommended maximum size of contiguous
		memory within which two objects are placed in the same L1 cache line"
	*/
	//std::cout <<"hardware_constructive_interference_size: " <<std::hardware_constructive_interference_size << '\n';

	/* "If you want to access two different (atomic) objects with different threads: 
		use alignas at least with the size of destructor or smaller ; sharing same cache is thread safe but expensive !!!"
	*/
	//std::cout <<"hardware_destructive_interference_size: " <<std::hardware_destructive_interference_size << '\n';


}