#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "iostream"
#include "string"
#include <iomanip>
#include "Player.h"
#include "SupMath.h"
#include "Camera.h"
#include "random"
#include "Tree.h"
#include "Planet.h"
#include "vector"
#include "future"

using namespace sf;
using namespace std;

const string name = "Formula game";

bool isWorking = true;

const int blockSize = 10;

Camera camera(Vector2i(900, 700), Vector2f(0, 0));

Player player(Vector2f((900) / 2, (700) / 2), Vector2f(500, 500), Vector2f((900)/2, (700)/2), 300, 15);

RectangleShape block;

vector<Sprite> treeSprites;

//Tree tree(Branch(Vector2f(300, 300), 8, 60, 180, 3), 13, 3);
//Tree tree2(Branch(Vector2f(500, 300), 8, 60, 180, 3), 22, 3);

Planet currentPlanet(0);

void setCameraPos() {
	Vector2f cameraPos = camera.getCameraPos();
	Vector2i cameraSize = camera.getCameraSize();
	cameraPos = player.getPosition() - player.getOffset();
	cameraPos.y = clamp(-currentPlanet.getGroundPadding()*blockSize, mapHeight*blockSize - cameraSize.y, cameraPos.y);
	cameraPos.x = clamp(0, mapWidth*blockSize - cameraSize.x, cameraPos.x);
	camera.setCameraPos(cameraPos);
}

Texture rendTexture(Tree& tree) {
	RenderTexture treeRenderTexture;
	if (!treeRenderTexture.create(tree.getRoot().length*tree.getRoot().level, tree.getRoot().length*tree.getRoot().level)) {
		cout << "failed to create tree texture" << endl;
	}
	treeRenderTexture.clear(Color(0, 0, 0, 0));
	
	for (int i = 0; i < tree.getBranches().size(); i++) {
		Branch br = tree.getBranches()[i];
		RectangleShape shape;
		shape.setSize(Vector2f(br.thickness, br.length));
		shape.setOrigin(Vector2f(br.thickness / 2, 0));
		shape.rotate(br.angle);
		shape.setFillColor(Color(0, 0, 0));
		shape.setPosition(br.startPoint);
		treeRenderTexture.draw(shape);
	}
	treeRenderTexture.display();
	return treeRenderTexture.getTexture();
}

Texture updatePlanetTrees() {
	Texture r;
	treeSprites.clear();
	for (int i = 0; i < currentPlanet.getTrees().size(); i++) {

	}
	return r;
}

int main()
{
	/*std::mt19937 engine;
	engine = mt19937(100);
	std::uniform_real_distribution<> dist;
	for (int i = 0; i < 100; i++) {
		std::cout << dist(engine) << std::endl;
	}*/
	Clock clock;
	RenderWindow mWindow(VideoMode(camera.getCameraSize().x, camera.getCameraSize().y), name, Style::Close);

	block.setSize(Vector2f(blockSize, blockSize));
	block.setFillColor(Color(0, 0, 0));
	
	RectangleShape background(Vector2f(camera.getCameraSize().x, camera.getCameraSize().y));
	background.setFillColor(Color(255, 255, 255));

	player.setPlayerSpriteTexture(generatePlayerTexture());
	Texture p = generatePlayerTexture();
	updatePlanetTrees();
	
	while (mWindow.isOpen()) {
		Event event;
		
		while (mWindow.pollEvent(event)) {
			if (event.type == Event::Closed)
				mWindow.close();
			if (event.type == Event::Resized)
				mWindow.setSize(Vector2u(camera.getCameraSize().x, camera.getCameraSize().y));
		}

		setCameraPos();
		float deltaTime = clock.getElapsedTime().asMicroseconds();
		deltaTime = deltaTime / 1000000;
		clock.restart();
		player.update(deltaTime);
		mWindow.draw(background);
		currentPlanet.drawPlanet(mWindow, camera);
		//drawTrees(mWindow, camera);
		player.draw(mWindow, camera);
	//	mWindow.draw(sadf);
		mWindow.display();
	}
	isWorking = false;
	return 0;
}