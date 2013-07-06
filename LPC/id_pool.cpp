#include "id_pool.h"

int IDPool::Get() {
	CSGuard l(&lock_);
	std::size_t i = 0;
	for (; i < ids_.size(); ++i) {
		if (ids_[i] != -1) {
			int n = 0;
			while (((ids_[i] >> n) & 1) == 1) n++;
			ids_[i] |= 1 << n;
			return n + i * 32;
		}
	}
	
	// 现有的位都在使用中
	int ret = ids_.size() * 32;
	ids_.push_back(1);
	return ret;
}

void IDPool::Put( int id ) {
	CSGuard l(&lock_);
	ids_.at(id >> 5) &= ~(1 << (id & 31));
}

void IDPool::Occupy( int id ) {
	CSGuard l(&lock_);
	std::size_t index = id >> 5;
	if (index >= ids_.size())
		ids_.resize(index + 1);
	ids_[index] |= (1 << (id & 31));
}


