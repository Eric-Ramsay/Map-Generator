// A place for functions related to drawing things to the screen to reside

// Creates a sprite sheet from a file string
sf::Texture createTexture(std::string file) {
	sf::Texture texture;
	texture.loadFromFile(file);
	return texture;
}

// Draws a sprite from the global sprite sheet
void drawSprite(int sX, int sY, int w, int h, float dX, float dY, int scale = 1, sf::Color color = sf::Color(255, 255, 255)) {
	static bool init = false;
	static sf::Texture sprites;
	if (!init) {
		sprites = createTexture("./Sprites/sprites.png");
		init = true;
	}
	sf::IntRect rect(sX, sY, w, h);
	sf::Sprite sprite(sprites, rect);
	sprite.setOrigin(w / 2, h / 2);
	sprite.setScale(scale, scale);
	sprite.setPosition(dX, dY);
	sprite.setColor(color);
	//sprite.setRotation(rotation);
	window.draw(sprite);
}

int charWidth(char c) {
	//Assumes char width of 5 and char height of 7
	switch (c) {
	case '"':case ',': case '.': case '\'': case '!': case ':': return 1;
	case ';': return 2;
	case ' ': case '[': case ']': case '|': return 3;
	case '\n': return 0;
	default: return 5;
	}
}

sf::Color getColor(std::string text = "") {
	return sf::Color(255, 255, 255);
}

//Prints text onto the screen
void Print(std::string text, float x = -1, float y = -1, float scale = 1.0f, bool center = true, int width = 1920 / 2) {
	int cap = width;
	int TEXT_W = 5;
	int TEXT_H = 7;

	sf::Color p = sf::Color(255, 255, 255);
	std::vector<int> splitIndexes;
	std::string line = "";
	bool skipping = false;
	std::string color = "";
	std::vector<int> lineWidth = { 0 };
	int spaces = 0; int splits = 0;
	float drawX = x;
	float drawY = y;
	float offX = 1; float offY = 0;
	float sH = TEXT_H; float sW = TEXT_W;
	int sY = 1;
	int sX = 65;
	bool prnt = true;

	int wordlen = 0;
	for (char& c : text) {
		if (c == '*') {
			skipping = !skipping;
		}
		else if (!skipping) {
			wordlen += scale * (charWidth(c) + 1);
			lineWidth[lineWidth.size() - 1] += scale * (charWidth(c) + 1);
			line += c;
			if (c == ' ' || c == '\n') {
				if (c == '\n' || lineWidth[lineWidth.size() - 1] > cap) {
					line = "";
					if (lineWidth[lineWidth.size() - 1] > cap && spaces > 0) {
						splitIndexes.push_back(spaces - 1);
						lineWidth[lineWidth.size() - 1] -= wordlen;
						lineWidth.push_back(wordlen);
					}
					else {
						splitIndexes.push_back(spaces);
						lineWidth.push_back(0);
					}
				}
				wordlen = 0;
				spaces++;
			}
		}
	}
	if (lineWidth[lineWidth.size() - 1] > cap && spaces > 0) {
		splitIndexes.push_back(spaces - 1);
		lineWidth[lineWidth.size() - 1] -= wordlen;
		lineWidth.push_back(wordlen);
	}
	spaces = 0;
	if (center) {
		drawX = 1 + x - (lineWidth[splits] / 2.0); //if ur having problems try scale + x
		drawY = y - scale * lineWidth.size() * (1 + (sH / 2.0));
	}
	skipping = false;
	for (char& c : text) {
		sH = TEXT_H; sW = TEXT_W;
		if (c == '*') {
			skipping = !skipping;
			if (!skipping) {
				if (getColor(color) != p) {
					p = getColor(color);
				}
				else {
					p = sf::Color(255, 255, 255);
				}
				color = "";
			}
		}
		else if (!skipping) {
			offX = 0; offY = 0;
			sH = sH; sW = charWidth(c);
			sY = 0; sX = 0;
			prnt = true;
			if (c >= 48 && c <= 57) { //Numbers
				sY = sH + 1;
				sX = ((int)c - 48) * (sW + 1);
			}
			else if (c >= 65 && c <= 90) { //Letters, uppercase(?)
				sY = 0;
				sX = ((int)c - 65) * (sW + 1);
			}
			else if (c >= 97 && c <= 122) { //Letters again, lowercase(?)
				sY = 0;
				sX = ((int)c - 97) * (sW + 1);
			}
			else {
				sY = 17;
				switch (c) {
				case '"': case '\'': sX = 1; sY = 21; sW = 1; sH = 3; break;
				case '.': sX = 104; sY = 17; sW = 2; break;
				case ',': sX = 1; sW = 1; offY = 1; break;
				case '>': sX = 6; break;
				case '?': sX = 18; break;
				case '!': sX = 14; sW = 1; break;
				case '+': sX = 24; break;
				case '-': sX = 30; break;
				case '$': sX = 36; break;
				case ':': sX = 44; sW = 1; break;
				case '<': sX = 84; break;
				case '[': sX = 48; sW = 3; break;
				case ']': sX = 56; sW = 3; break;
				case '#': sX = 60; break;
				case '(': sX = 66; break;
				case ')': sX = 72; break;
				case '=': sX = 78; break;
				case '/': sX = 92; break;
				case '%': sX = 98; break;
				case '|': sX = 60; sW = 3; break;
				case ';': sX = 89; sW = 2; break;
				case ' ': sW = 2; prnt = false;
				case '\n': prnt = false;
				default: prnt = false;
				}
			}
			if (prnt) {
				//drawSprite(int sX, int sY, int w, int h, int dX, int dY, int scale = zoom, sf::Color color = sf::Color(255, 255, 255))
				drawSprite(sX + 320, sY, sW, sH, drawX + offX * scale, drawY + offY * scale, scale, p);
			}
			if (c != '\n') {
				drawX += scale * (sW + offX + 1);
			}
			if (c == ' ' || c == '\n') {
				if (splitIndexes.size() > splits && splitIndexes[splits] == spaces++) {
					drawX = x;
					splits++;
					if (center) {
						drawX -= (lineWidth[splits] / 2);
					}
					drawY += scale * (sH + 3);
				}
			}
		}
		else {
			color += c;
		}
	}
}

void drawPosition(float x, float y, int size = 1, sf::Color color = sf::Color(255, 255, 255)) {
	drawSprite(34, 0, 1, 1, x, y, size, color);
}


void DrawTile(std::vector<std::vector<Tile>>& map, int x, int y, int dX, int dY) {
	int sX = sourceX(map[y][x].type);
	int sY = 0;

	//Tile randomization to mix things up
	int useRand = randC(x, y) * 4;
	if (map[y][x].elev == HILL || map[y][x].elev == MOUNTAIN || map[y][x].forest != NONE || map[y][x].building) {
		useRand = 0;
	}
	if (map[y][x].elev == WATER) { //Water Tiles
		useRand = timer / 50;
		if (map[y][x].type == LAKE) {
			useRand++;
		}
		else if (map[y][x].type == RIVER) {
			useRand += 2;
		}
		useRand %= 4;
	}
	int rX = 0;
	int rY = 0;
	switch (useRand) {
	case 1: rX = 1; rY = 0; break;
	case 2: rX = 0; rY = 1; break;
	case 3: rX = 1; rY = 1; break;
	default:rX = 0; rY = 0; break;
	}

	//Draw the base tile
	drawSprite(sX + rX * 16, sY + 17 + rY * 16, 16, 16, dX, dY, z);
}

void DrawElevation() {

}

void DrawForest() {

}

void DrawUnit() {

}

void DrawBuilding() {

}

void DrawMap() {
	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++) {
			int y = safeC(i + yView);
			int x = safeC(j + xView);
			DrawTile(map, x, y, j * 8, i * 8, zoom);
		}
	}

	int width = ceil(float(view.getSize().x) / float(zoom));
	int height = ceil(float(view.getSize().y) / float(zoom));
	for (int i = -1 - ceil(height / 2); i <= 1 + ceil(height / 2); i++) {
		for (int j = -1 - ceil(width / 2); j <= ceil(width / 2); j++) {
			int y = safeC(i + yView);
			int x = safeC(j + xView);
			DrawTile(map, x, y, (view.getSize().x / 2 + ((j * zoom))), (view.getSize().y / 2 + ((i * zoom))), zoom);
			//int temp = latTemp(y-5);
			//Print(to_str(temp), (view.getSize().x / 2 + ((j * zoom) - dXView)) + zoom/2, (view.getSize().y / 2 + ((i * zoom) - dYView)) + zoom/2, true, zoom / 16.0);
		}
	}
}