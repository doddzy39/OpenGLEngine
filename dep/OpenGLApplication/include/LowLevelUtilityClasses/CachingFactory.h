#pragma once

//Source: https://github.com/notoes/RefEngine

#include "VariadicHash.h"

#include <functional>
#include <map>

namespace reng {

/**
* A Factory that caches the created objects and will return the cached object on subsequent requests.
* The factory function is passed into the constructor. The factory function can take any number of parameters.
* The created object is cached with a hash of the parameters used to create it.
*
* Example usage:
*   int max(int a, int b) { return std::max(a, b);  }
*	CachingFactory<int, int, int> maxFactory( max );
*	int m = maxFactory.Get(1, 2);
*
*   will calculate max once, and from then on return the cached value.
*   Although this is most useful for loading reasources or assets and caching the result.
*/
template<typename TMake, typename...TArgs>
class CachingFactory
{
public:
	CachingFactory(std::function<TMake(TArgs...)> factoryFunction)
		: m_factoryFunction(factoryFunction)
	{
	}

	TMake Get(TArgs... args) 
	{
		// Get a hash value that represents all the params.
		int hashKey = vhash(args...);

		// Check to see if we already have created this object.
		if (m_cacheMap.find(hashKey) != m_cacheMap.end()) { return m_cacheMap[hashKey]; }

		// Create and return
		m_cacheMap[hashKey] = m_factoryFunction(args...);
		return m_cacheMap[hashKey];
	}

private:
	std::map<int, TMake> m_cacheMap; // A map of parameter hashes to created objects.
	std::function<TMake(TArgs...)> m_factoryFunction; // The function that will create the object.
};

}
