#include "../csc321.h"
#include "MyScene.h"

MasterSubgraphs::MasterSubgraphs() : names(), trees() {
}

MasterSubgraphs::~MasterSubgraphs() {
	for (int i = 0; i < trees.size(); i++) {
		delete trees[i];
	}
}

void MasterSubgraphs::addTree(std::string name, Tree* tree) {
	names.push_back(name);
	trees.push_back(tree);
}

Tree* MasterSubgraphs::get(std::string name) {
	for (int i = 0; i < names.size(); i++) {
		if (names[i] == name) 
			return trees[i];
	}
	return 0;
}