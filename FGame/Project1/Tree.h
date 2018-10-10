#ifndef TREE_H
#define TREE_H

#include <SFML/Graphics.hpp>
#include "vector"
#include "cmath"
#include "iostream"
#include "random"
#include "Camera.h"

using namespace std;
using namespace sf;

class Tree;

struct Branch {
public:
	int level;
	int thickness;
	float length;
	float angle;
	Vector2f startPoint;
	Branch(Vector2f sp, int t, float l, float a, int lvl);
	Branch();
	Branch createNewBranch(Branch cur, Tree* parent, float angleMultiplier, int level);
};

class Tree {
private:
	vector<Branch> branches;
	Branch root;
	Vector2f pos;
	int maxBranchLevels;
	mt19937 engine;
	uniform_real_distribution<> dist;
public:
	Tree(Branch& root, int sd, int maxLvl);
	void generate();
	void draw(RenderWindow& window, Camera camera);
	void addBranch(Branch branch);
	Tree* getThis();
	float getRand();
	Branch getRoot();
	Vector2f getPos();
	vector<Branch> getBranches();
};
#endif