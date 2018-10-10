#include "Tree.h"
/*
class A {
public:
	A() {
		rend();
	}

	void rend() {
		RenderTexture r;
	}
};

A a = A();
*/
Branch::Branch(Vector2f sp, int t, float l, float a, int lvl) {
	startPoint = Vector2f(sp.x, sp.y);
	length = l;
	thickness = t;
	angle = a;
	level = lvl;
}

Branch::Branch() {

}

Branch Branch::createNewBranch(Branch cur, Tree* parent, float angleMultiplier, int level) {
	Vector2f sp(cur.startPoint.x, cur.startPoint.y);

	float randomAngle = ((*parent).getRand() * 15) - 5;

	float t = cur.thickness * 0.75;
	float l = cur.length * 0.67;
	float a = cur.angle + 30*angleMultiplier + randomAngle;
	sp.y -= (cos((cur.angle-180)*3.1415926 / 180) * cur.length);
	sp.x += (sin((cur.angle-180)*3.1415926 / 180) * cur.length);
	Branch gen(sp, t, l, a, level);
	if (level > 0) {
		int count = 100 * (*parent).getRand();
		if (count >= 25 && count < 90) {
			(*parent).addBranch(gen.createNewBranch(gen, parent, 1, level - 1));
			(*parent).addBranch(gen.createNewBranch(gen, parent, -1, level - 1));
		}
		if (count >= 90) {
			if (count % 2 == 0) {
				(*parent).addBranch(gen.createNewBranch(gen, parent, -1, level - 1));
			}
			else {
				(*parent).addBranch(gen.createNewBranch(gen, parent, 1, level - 1));
			}
		}
	}

	return gen;
}

void Tree::addBranch(Branch branch) {
	branches.push_back(branch);
}

Tree::Tree(Branch& rt, int sd, int maxLvl) {
	root = rt;
	pos = rt.startPoint;
	//root.startPoint = Vector2f((rt.length * maxLvl) / 2, rt.length * maxLvl);
	maxBranchLevels = maxLvl;
	engine = mt19937(sd);
	generate();
}

float Tree::getRand() {
	return dist(engine);
}

void Tree::generate() {
	addBranch(root);
	addBranch(root.createNewBranch(root, getThis(), 1, maxBranchLevels));
	addBranch(root.createNewBranch(root, getThis(), -1, maxBranchLevels));
	/*Texture treeTexture = rendTexture(*this);
	treeSprite = Sprite(treeTexture);
	treeSprite.setOrigin(Vector2f((root.length*root.level)/2, root.length * root.level));
*/}

Tree* Tree::getThis() {
	return this;
}

Branch Tree::getRoot() {
	return root;
}

void Tree::draw(RenderWindow& window, Camera camera) {
	//treeSprite.setPosition(pos - camera.getCameraPos());
	//Sprite s(texture);
	//s.setPosition(500, 500);
	//window.draw(treeSprite);
	//cout << treeSprite.getTexture()->getSize().x << "/" << treeSprite.getTexture()->getSize().y << endl;
	//window.draw(s);
	
	for (int i = 0; i < getBranches().size(); i++) {
		Branch br = getBranches()[i];
		RectangleShape shape;
		shape.setSize(Vector2f(br.thickness, br.length));
		shape.setOrigin(Vector2f(br.thickness / 2, 0));
		shape.rotate(br.angle);
		shape.setFillColor(Color(0, 0, 0));
		shape.setPosition(br.startPoint - camera.getCameraPos());
		window.draw(shape);
	}
}

Vector2f Tree::getPos() {
	return pos;
}

vector<Branch> Tree::getBranches() {
	return branches;
}
