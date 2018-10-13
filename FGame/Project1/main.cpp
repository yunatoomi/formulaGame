#include <SFML/Graphics.hpp>
#include "Space.h"
#include <Windows.h>
#include "iostream"
#include "string"
#include <iomanip>
#include "Player.h"
#include "SupMath.h"
#include "Camera.h"
#include "random"
#include "Tree.h"
#include "Bullet.h"
#include "Planet.h"
#include "vector"
#include "PlanetTextures.h"
#include "Enemy.h"
#include "EnemySpawner.h"

using namespace sf;
using namespace std;

const string name = "Formula game";

bool isWorking = true;

const int blockSize = 10;

bool playingGame = true;

Camera camera(Vector2i(900, 700), Vector2f(0, 0));
Camera spaceCamera(Vector2i(900, 700), Vector2f(0, 0));

Player player(Vector2f((900) / 2, (700) / 2), Vector2f(500, 500), Vector2f((900)/2, (700)/2), 300, 15);

RectangleShape block;

vector<Sprite> treeSprites;

Sprite pl1;
Sprite pl2;
Sprite pl3;
Sprite pl4;
Sprite pl5;
Sprite pl6;
Sprite pl7;
Sprite pl8;
Sprite pl9;
Sprite pl10;
Sprite pl11;
Sprite pl12;

Sprite spawnerSprite;

Planet currentPlanet(4005);

bool isOnPlanet = true;
int timeToExitPlanet = 0;
bool activatedExiting = true;
int currentSector = 0;
int respawn = 0;

void setCameraPos() {
	Vector2f cameraPos = camera.getCameraPos();
	Vector2i cameraSize = camera.getCameraSize();
	cameraPos = player.getPosition() - player.getOffset();
	cameraPos.y = clamp(-currentPlanet.getGroundPadding()*blockSize, mapHeight*blockSize - cameraSize.y, cameraPos.y);
	cameraPos.x = clamp(0, mapWidth*blockSize - cameraSize.x, cameraPos.x);
	camera.setCameraPos(cameraPos);
}

void setCameraSpacePos() {
	Vector2f cameraPos = camera.getCameraPos();
	cameraPos = player.getPosition() - player.getOffset();
	cameraPos.y = clamp(0, 6000-camera.getCameraSize().y, cameraPos.y);
	cameraPos.x = clamp(0, 6000-camera.getCameraSize().x, cameraPos.x);
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

void drawingModePlanet(RenderWindow& window, Camera camera) {
	window.clear(Color(255, 255, 255));
	currentPlanet.drawPlanet(window, camera);
	player.draw(window, camera);
}

double function1(int i, double step) {
	return tan((i*step)*3.14 / 180);
}

double function2(int i, double step) {
	return sin((i*step)*3.14);
}

double function3(int i, double step) {
	return cos((i*step)*3.14);
}

void checkIsOnPlanet() {
	if (player.getPosition().y <= 0 && activatedExiting) {
		player.freeze(1);
		activatedExiting = false;
		timeToExitPlanet = time(NULL) + 1;
	}
}

void drawPlanets(RenderWindow& window, Camera cam) {
	for (int i = 0; i < 4; i++) {
		PlanetParams p = getPlanetParams(i);
		Sprite s;
		switch (p.type) {
		case 0:
			s = pl1;
			break;
		case 1:
			s = pl2;
			break;
		case 2:
			s = pl3;
			break;
		case 3:
			s = pl4;
			break;
		case 4:
			s = pl5;
			break;
		case 5:
			s = pl6;
			break;
		case 6:
			s = pl7;
			break;
		case 7:
			s = pl8;
			break;
		case 8:
			s = pl9;
			break;
		case 9:
			s = pl10;
			break;
		case 10:
			s = pl11;
			break;
		case 11:
			s = pl12;
			break;
		default:
			s = pl1;
			break;
		}
		s.setPosition(p.position - camera.getCameraPos());
		s.setColor(p.color);
		s.setScale(p.scale);
		window.draw(s);
	}
}

void drawSpace(RenderWindow& window, Camera cam, float deltaTime) {
	window.clear(Color(25, 25, 25));
	drawPlanets(window, cam);
	if(playingGame)
		player.spaceDraw(window, cam, deltaTime, isDied);
	drawSpawner(window, cam, spawnerSprite);
}

bool isPlayerDied() {
	return isDied(player.getPosition());
}

double nextTimeToChange = 0;

float vectorDistance(Vector2f a, Vector2f b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void checkIsEnteringPlanet() {
	if (Keyboard::isKeyPressed(Keyboard::E)) {
		for (int i = 0; i < 4; i++) {
			if (vectorDistance(player.getPosition(), getPlanetParams(i).position) <= getPlanetParams(i).radius) {
				currentPlanet = Planet(currentPlanet.getId() - currentPlanet.getId() % 4 + i);
				isOnPlanet = true;
				player.setPosition(Vector2f((900) / 2, 10));
				player.setSpeed(Vector2f(0, player.getMaxSpeed().y / 2));
			}
		}
	}
}

float* getSpaceSectorSizes(bool* sectorBorders) {
	float* ret = new float[4];
	ret[0] = 0;
	ret[1] = 6000;
	ret[2] = 0;
	ret[3] = 6000;
	for (int i = 0; i < 4; i++) {
		if (!sectorBorders[i]) {
			ret[i] = -1;
		}
	}
	return ret;
}

void isExiting() {
	Vector2f playerPos = player.getPosition();
	Vector2f sp(0, 0);
	bool changed = false;
	if (playerPos.x <= 0) {
		currentSector --;
		playerPos.x = 5999;
		changed = true;
		sp.x = -player.getMaxSpeed().x;
	}
	if (playerPos.x >= 6000) {
		currentSector ++;
		playerPos.x = 1;
		changed = true;
		sp.x = player.getMaxSpeed().x;
	}
	if (playerPos.y <= 0) {
		currentSector -= 500;
		playerPos.y = 5999;
		sp.y = -player.getMaxSpeed().y;
		changed = true;
	}
	if (playerPos.y >= 6000) {
		currentSector += 500;
		playerPos.y = 1;
		sp.y = player.getMaxSpeed().y;
		changed = true;
	}
	if (changed) {
		player.setPosition(playerPos);
		player.setSpeed(sp);
		loadSpaceSector(getStringSector(currentSector));
	}
}

int main()
{
	/*std::mt19937 engine;
	engine = mt19937(100);
	std::uniform_real_distribution<> dist;
	for (int i = 0; i < 100; i++) {
		std::cout << dist(engine) << std::endl;
	}*/
	srand(time(NULL));
	currentPlanet = Planet(rand() * 99999);
	Clock clock;
	RenderWindow mWindow(VideoMode(camera.getCameraSize().x, camera.getCameraSize().y), name, Style::Close);
	block.setSize(Vector2f(blockSize, blockSize));
	block.setFillColor(Color(0, 0, 0));
	
	RectangleShape background(Vector2f(camera.getCameraSize().x, camera.getCameraSize().y));
	background.setFillColor(Color(255, 255, 255));

	player.setPlayerSpriteTexture(generatePlayerTexture());
	Texture playerTexture = generatePlayerTexture();

	player.setPlayerSpaceSpriteTexture(generateSpacePlayerTexture());
	Texture spacePlayerTexture = generateSpacePlayerTexture();

	pl1.setTexture(getPlanet1(function1, 1));
	Texture p1 = getPlanet1(function1, 0.1);
	pl1.setOrigin(Vector2f(p1.getSize().x/2, p1.getSize().y/2));
	pl2.setTexture(getPlanet1(function1, 1));
	Texture p2 = getPlanet1(function1, 0.2);
	pl2.setOrigin(Vector2f(p2.getSize().x / 2, p2.getSize().y / 2));
	pl3.setTexture(getPlanet1(function1, 1));
	Texture p3 = getPlanet1(function1, 0.3);
	pl3.setOrigin(Vector2f(p3.getSize().x / 2, p3.getSize().y / 2));
	pl4.setTexture(getPlanet1(function1, 1));
	Texture p4 = getPlanet1(function1, 0.4);
	pl4.setOrigin(Vector2f(p4.getSize().x / 2, p4.getSize().y / 2));
	pl5.setTexture(getPlanet1(function2, 1));
	Texture p5 = getPlanet1(function2, 1);
	pl5.setOrigin(Vector2f(p5.getSize().x / 2, p5.getSize().y / 2));
	pl6.setTexture(getPlanet1(function2, 1));
	Texture p6 = getPlanet1(function2, 2);
	pl6.setOrigin(Vector2f(p6.getSize().x / 2, p6.getSize().y / 2));
	pl7.setTexture(getPlanet1(function2, 1));
	Texture p7 = getPlanet1(function2, 3);
	pl7.setOrigin(Vector2f(p7.getSize().x / 2, p7.getSize().y / 2));
	pl8.setTexture(getPlanet1(function2, 1));
	Texture p8 = getPlanet1(function2, 4);
	pl8.setOrigin(Vector2f(p8.getSize().x / 2, p8.getSize().y / 2));
	pl9.setTexture(getPlanet1(function2, 1));
	Texture p9 = getPlanet1(function3, 1);
	pl9.setOrigin(Vector2f(p9.getSize().x/2, p9.getSize().y/2));
	pl10.setTexture(getPlanet1(function3, 1));
	Texture p10 = getPlanet1(function3, 2);
	pl10.setOrigin(Vector2f(p10.getSize().x / 2, p10.getSize().y / 2));
	pl11.setTexture(getPlanet1(function3, 1));
	Texture p11 = getPlanet1(function3, 3);
	pl11.setOrigin(Vector2f(p11.getSize().x / 2, p11.getSize().y / 2));
	pl12.setTexture(getPlanet1(function3, 1));
	Texture p12 = getPlanet1(function3, 4);
	pl12.setOrigin(Vector2f(p12.getSize().x / 2, p12.getSize().y / 2));

	setBulletTexture(getBulletTexture());
	Texture bulletTexture = getBulletTexture();

	setEnemyTexture(getEnemyTexture());
	Texture enemyTexture = getEnemyTexture();

	float minHeight = mapHeight*blockSize - ((currentPlanet.getGroundPadding() + currentPlanet.getGroundHeight())*blockSize + 180);
	Vector2f planetBarriers(0, mapWidth*blockSize);
	Vector2f spaceSectorBarriers(20000, 20000);

	spawnerSprite.setTexture(getSpawnTexture());
	Texture spawnerTexture = getSpawnTexture();
	while (mWindow.isOpen()) {
		Event event;
		
		while (mWindow.pollEvent(event)) {
			if (event.type == Event::Closed)
				mWindow.close();
			if (event.type == Event::Resized)
				mWindow.setSize(Vector2u(camera.getCameraSize().x, camera.getCameraSize().y));
		}

		float deltaTime = clock.getElapsedTime().asMicroseconds();
		deltaTime = deltaTime / 1000000;
		clock.restart();
		if (!playingGame && time(NULL) >= respawn) {
			playingGame = true;
			currentPlanet = Planet(rand() * 99999);
			isOnPlanet = true;
			player.setPosition(Vector2f((900) / 2, (700) / 2));
			player.setSpeed(Vector2f(0, 0));
		}
		if (!activatedExiting && time(NULL) >= timeToExitPlanet) {
			isOnPlanet = false;
			activatedExiting = true;
			Vector2f pos = loadSpaceSector(currentPlanet);
			player.setPosition(pos);
			player.setSpeed(Vector2f(0, 0));
			currentSector = floor(currentPlanet.getId()/4);
		}
		if (isOnPlanet) {
			player.planetUpdate(deltaTime, planetBarriers, minHeight);
			drawingModePlanet(mWindow, camera);
			checkIsOnPlanet();
			setCameraPos();
		}
		else {
			drawSpace(mWindow, camera, deltaTime);
			float *borders = getSpaceSectorSizes(getSectorBorders(getStringSector(currentSector)));
			if (playingGame) {
				player.spaceUpdate(deltaTime, Vector2f(borders[0], borders[2]), Vector2f(borders[1], borders[3]));
				if (isPlayerDied()) {
					playingGame = false;
					respawn = time(NULL) + 3;
				}
			}
			setCameraSpacePos();
			checkIsEnteringPlanet();
			spawnerUpdate(player.getPosition(), deltaTime);
			isExiting();
		}
		mWindow.display();
	}
	isWorking = false;
	return 0;
}