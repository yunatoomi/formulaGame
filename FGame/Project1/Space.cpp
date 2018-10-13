#include "Space.h"

mt19937 engine;
uniform_real_distribution<> dist;

PlanetParams planets[4];

PlanetParams::PlanetParams(int t, Color c, Vector2f s, Vector2f pos) {
	type = t;
	color = c;
	scale = s;
	position = pos;
	radius = 260 * s.x;
}

PlanetParams getPlanetParams(int id) {
	return planets[id];
}

PlanetParams::PlanetParams() {

}

int getIdSector(string sector) {
	string letters = sector.substr(0, 2);
	int numb = 0;
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < alphabet.length(); i++) {
		if (letters[0] == alphabet[i]) {
			numb += i * alphabet.length();
		}
		if (letters[1] == alphabet[i]) {
			numb += i;
		}
	}
	numb *= 500;
	string numbers = sector.substr(2, 3);
	numb += stoi(numbers);
	return numb;
}

int getIdSector(int planetId) {
	return floor(planetId / 4);
}

int arraySize(char* arr) {
	return sizeof(arr) / sizeof(*arr);
}

string getStringSector(int sectorId) {
	string numberPart = to_string(sectorId % 1000);
	while (numberPart.length() < 3) {
		numberPart = "0" + numberPart;
	}
	int stringPart = sectorId - sectorId % 1000;
	stringPart = stringPart / 500;
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int secondLetter = stringPart % arraySize(alphabet);
	stringPart -= stringPart % arraySize(alphabet);
	int firstLetter = stringPart / arraySize(alphabet);
	stringstream stream;
	stream << alphabet[firstLetter];
	stream << alphabet[secondLetter];
	stream << numberPart;
	return stream.str();
}

float range(float min, float max) {
	return min + dist(engine) * (max - min);
}

Vector2f loadSpaceSector(String sector) {
	dist.reset();
	engine = mt19937(getIdSector(sector));
	for (int i = 0; i < 4; i++) {
		int type = dist(engine) * 11;
		Color randColor = Color(range(80, 180), range(80, 180), range(80, 180));
		float scale = 0.5 + dist(engine) * 1.5;
		Vector2f pos(range(500, 1500), range(500, 1500));
		if (i > 0) {
			pos += planets[i - 1].position;
		}
		PlanetParams p(type, randColor, Vector2f(scale, scale), pos);
		planets[i] = p;
	}
	reset();
	setScale(0.5f + dist(engine) * 1.5f);
	setPosition(Vector2f(range(0, 6000), range(0, 6000)));
	return Vector2f(0, 0);
}

void spawnerUpdate(Vector2f playerPos, float deltaTime) {
	spawn(playerPos);
	for (int i = 0; i < getEnemyCount(); i++) {
		updateEnemy(i, deltaTime, playerPos);
		if (getEnemy(i).isDied)
			eraseEnemy(i);
	}
}

bool isDied(Vector2f pos) {
	for (int i = 0; i < getEnemyCount(); i++) {
		if (isEnemyCollided(i, pos))
			return true;
	}
	return false;
}

void drawSpawner(RenderWindow& window, Camera cam, Sprite spawnerSprite) {
	spawnerSprite.setPosition(getPosition() - cam.getCameraPos());
	spawnerSprite.setScale(Vector2f(getScale(), getScale()));
	window.draw(spawnerSprite);
	for (int i = 0; i < getEnemyCount(); i++) {
		Enemy e = getEnemy(i);
		e.draw(window, cam);
	}
}

Vector2f loadSpaceSector(Planet currentPlanet) {

	loadSpaceSector(getStringSector(getIdSector(currentPlanet.getId())));
	return planets[currentPlanet.getId()%4].position;
}

bool* getSectorBorders(string sector) {
	int sectorId = getIdSector(sector);
	bool* ret = new bool[4];
	if (sectorId % 500 == 0) {
		ret[0] = true;
	}
	else {
		ret[0] = false;
	}
	if (sectorId % 500 == 499) {
		ret[1] = true;
	}
	else {
		ret[1] = false;
	}
	if (floor(sectorId / 500) == 0) {
		ret[2] = true;
	}
	else {
		ret[2] = false;
	}
	if (floor(sectorId / 500) == 499) {
		ret[3] = true;
	}
	else {
		ret[3] = false;
	}
	return ret;
}