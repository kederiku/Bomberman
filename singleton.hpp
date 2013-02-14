#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#include "interface.h"
#include <new>
#include <pthread.h>

template <typename T>
class	Singleton
{
	public:
		inline static T*	GetInstance(void)
		{
			if (__instance == 0)
			{
				pthread_mutex_lock(&(__mutex));
				if (__instance == 0)
					__instance = new (std::nothrow) T;
				pthread_mutex_unlock(&(__mutex));
			}
			return __instance;
		};
		static void		Kill(void)
		{
			delete __instance;
			__instance = 0;
		};
	protected:
		Singleton<T>(void)
		{
			pthread_mutex_init(&(this->__mutex), 0);
		}
		virtual ~Singleton<T>(void)
		{
			pthread_mutex_destroy(&(this->__mutex));
		}
	private:
		static	T*		__instance;
		static pthread_mutex_t	__mutex;
};

template <typename T>
T*	Singleton<T>::__instance = 0;
template <typename T>
pthread_mutex_t	Singleton<T>::__mutex;

#endif
