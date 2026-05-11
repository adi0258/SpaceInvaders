#include <iostream>
using namespace std;

#include "bagel.h"
using namespace bagel;

bool test_create_entities()
{
	ent_type e1 = World::createEntity();
	if (e1.id != 0) return false;
	ent_type e2 = World::createEntity();
	if (e2.id != 1) return false;
	ent_type e3 = World::createEntity();
	if (e3.id != 2) return false;

	World::deleteEntity(e2);
	World::deleteEntity(e1);
	e2 = World::createEntity();
	if (e2.id != 0) return false;

	return true;
}

bool test_world() {
	if (!test_create_entities())
		return false;
	return true;
}


bool test_dynamic_bag() {
	DynamicBag<int, 5> bag;

	bag.push(10);
	bag.push(20);
	if (bag.size() != 2) return false;

	if (bag[0] != 10 || bag[1] != 20) return false;

	int popped = bag.pop();
	if (popped != 20 || bag.size() != 1) return false;

	return true;
}

struct HealthComponent {
	int hp;
};

template<> DynamicBag<HealthComponent, 100> SparseStorage<HealthComponent>::_comps{};

bool test_sparse_storage() {
	ent_type e = World::createEntity();

	SparseStorage<HealthComponent>::add(e, HealthComponent{100});

	HealthComponent& h = SparseStorage<HealthComponent>::get(e);
	if (h.hp != 100) return false;

	h.hp = 85;
	if (SparseStorage<HealthComponent>::get(e).hp != 85) return false;

	return true;
}

void run_tests()
{
	if (!test_world()) {
		cout << "test_world() failed" << endl;
	} else {
		cout << "test_world() passed" << endl;
	}

	if (!test_dynamic_bag()) {
		cout << "test_dynamic_bag() failed" << endl;
	} else {
		cout << "test_dynamic_bag() passed" << endl;
	}

	if (!test_sparse_storage()) {
		cout << "test_sparse_storage() failed" << endl;
	} else {
		cout << "test_sparse_storage() passed" << endl;
	}
}