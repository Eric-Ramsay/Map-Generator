// A place for functions related to drawing things to the screen to reside

// Creates a sprite sheet from a file string
sf::Texture createTexture(std::string file) {
	sf::Texture texture;
	texture.loadFromFile(file);
	return texture;
}

// Draws a sprite from the global sprite sheet
void drawSprite(int sX, int sY, int w, int h, float dX, float dY, int scale = 1, sf::Color color = sf::Color(255, 255, 255), bool windraw = false) {
	static bool init = false;
	static sf::Texture sprites;
	if (!init) {
		sprites = createTexture("./Sprites/sprites.png");
		init = true;
	}
	sf::IntRect rect(sX, sY, w, h);
	sf::Sprite sprite(sprites, rect);
	sprite.setOrigin(0, 0);
	sprite.setScale(scale, scale);
	sprite.setPosition(dX, dY);
	sprite.setColor(color);
	//sprite.setRotation(rotation);
	if (UI.WINDOW_DRAW) {
		window.draw(sprite);
	}
	else {
		texture.draw(sprite);
	}
}

void drawSprite(int sX, int sY, int w, int h, float dX, float dY, float x_scale, float y_scale, sf::Color color = sf::Color(255, 255, 255), bool windraw = false) {
	static bool init = false;
	static sf::Texture sprites;
	if (!init) {
		sprites = createTexture("./Sprites/sprites.png");
		init = true;
	}
	sf::IntRect rect(sX, sY, w, h);
	sf::Sprite sprite(sprites, rect);
	sprite.setOrigin(0, 0);
	sprite.setScale(x_scale, y_scale);
	sprite.setPosition(dX, dY);
	sprite.setColor(color);
	//sprite.setRotation(rotation);
	if (UI.WINDOW_DRAW) {
		window.draw(sprite);
	}
	else {
		texture.draw(sprite);
	}
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
	text = low(text);
	if (text == "gold") {
		return sf::Color(255, 223, 0);
	}
	else if (text == "red") {
		return sf::Color(220, 60, 80);
	}
	else if (text == "green") {
		return sf::Color(76, 182, 90);
	}
	else if (text == "blue") {
		return sf::Color(0, 0, 255);
	}
	else if (text == "orange") {
		return sf::Color(255, 165, 0);
	}
	else if (text == "grey") {
		return sf::Color(200, 200, 200);
	}
	else if (text == "brown") {
		return sf::Color(160, 83, 45);
	}

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
				case '.': sX = 118; sY = 17; sW = 1; break;
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
				//drawSprite(int sX, int sY, int UI.W, int UI.H, int dX, int dY, int scale = zoom, sf::Color color = sf::Color(255, 255, 255))
				drawSprite(sX + 272, sY + 80, sW, sH, drawX + offX * scale, drawY + offY * scale, scale, p);
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
	drawSprite(448, 16, 1, 1, x, y, size, color);
}

void DrawElevation(int x, int y, int dX, int dY, int e = -1) {
	int elev = e;
	if (elev == -1) {
		elev = map[y][x].elev;
	}
	if (elev == LOW) {
		return;
	}
	DrawElevation(x, y, dX, dY, elev - 1);
	int sX = 0;
	int sY = 368;

	sY += (16 * (elev - FLAT));

	bool u = map[safeC(y - 1)][x].elev >= elev; // up * 8
	bool d = map[safeC(y + 1)][x].elev >= elev; // down * 4
	bool l = map[y][safeC(x - 1)].elev >= elev; // left * 2
	bool r = map[y][safeC(x + 1)].elev >= elev; // right * 1
	sX = 16 * ((u * 8) + (d * 4) + (l * 2) + r);
	
	//Draw the appropriate hill/elevation tile
	drawSprite(sX, sY, 16, 16, dX, dY);

	//Check if the corners need to be filled in
	bool dr = d && r && map[safeC(y + 1)][safeC(x + 1)].elev < elev;
	bool dl = d && l && map[safeC(y + 1)][safeC(x - 1)].elev < elev;

	if (dr) {
		drawSprite(272, sY, 16, 16, dX, dY);
	}
	if (dl) {
		drawSprite(256, sY, 16, 16, dX, dY);
	}
}


void DrawForest() {

}

void DrawUnit() {

}


void DrawBuilding(int x, int y, int dX, int dY) {
	int sY = 80;
	int sX = 16 * (map[y][x].building - 1);
	if (map[y][x].building == ROAD) {
		sY = 112;
		//Check if adjacent tiles are also roads
		bool u = map[safeC(y - 1)][x].building == ROAD; 
		bool d = map[safeC(y + 1)][x].building == ROAD;
		bool l = map[y][safeC(x - 1)].building == ROAD;
		bool r = map[y][safeC(x + 1)].building == ROAD;
		sX = 16 * ((u * 8) + (d * 4) + (l * 2) + r);
		drawSprite(sX, sY, 16, 32, dX, dY);
	}
	else {
		drawSprite(sX, sY, 16, 32, dX, dY - (16));
	}
}

void DrawBuilding(BUILDING type, int dX, int dY, sf::Color color = sf::Color(255, 255, 255)) {
	int sY = 80;
	int sX = 16 * (type - 1);
	if (type == ROAD) {
		sY = 112;
		sX = 0;
		drawSprite(sX, sY, 16, 32, dX, dY, 1, color);
	}
	else {
		drawSprite(sX, sY, 16, 32, dX, dY - (16), 1, color);
	}
}

void DrawTile(std::vector<std::vector<Tile>>& map, int x, int y, int dX, int dY) {
	int sX = sourceX(map[y][x].type);
	int sY = 0;
	float offset = 0;

	if (UI.zoom >= 1) {
		//Tile randomization to mix things up
		int useRand = randC(x, y) * 8;
		if (useRand > 4) {
			useRand = 0;
		}
		if (map[y][x].elev == HILL || map[y][x].elev == MOUNTAIN || map[y][x].forest != BARE || map[y][x].building > -1) {
			useRand = 0;
		}
		if (map[y][x].elev == DEEP) { //Water Tiles
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
		drawSprite(sX + rX * 16, sY + 17 + rY * 16	, 16, 16, dX, dY);
		TERRAIN adjType;
		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if ((a == 0) != (b == 0)) {
					int side = abs(b);
					adjType = map[safeC(y + a)][safeC(x + b)].type;
					if (typePrecedent(adjType) > typePrecedent(map[y][x].type)) {
						sX = sourceX(adjType);
						sY = 0;
						if (map[y][x].elev == DEEP) {
							sY += 50;
						}
						if (side > 0) {
							if (b == -1) {
								drawSprite(sX + 14, sY, 2, 16, dX, dY);
							}
							else {
								drawSprite(sX, sY, 2, 16, dX + 14, dY);
							}
						}
						else {
							if (a == -1) {
								drawSprite(sX + 16, sY + 14, 16, 2, dX, dY);
							}
							else {
								drawSprite(sX + 16, sY, 16, 2, dX, dY + 14);
							}
						}
					}
				}
			}
		}
		if (map[y][x].elev >= FLAT) {
			DrawElevation(x, y, dX, dY);
			offset = -3;
		}
		if (map[y][x].owner != 0) { //DRAW COUNTRY BORDERS
			sf::Color tint = colors[map[y][x].owner - 1];
			//colors[std::std::min(7, map[y][x].owner)];
			bool lT, rT, uT, dT, bL, bR, tL, tR;
			lT = map[y][safeC(x - 1)].owner != map[y][x].owner;
			rT = map[y][safeC(x + 1)].owner != map[y][x].owner;
			uT = map[safeC(y - 1)][x].owner != map[y][x].owner;
			dT = map[safeC(y + 1)][x].owner != map[y][x].owner;
			bL = map[safeC(y + 1)][safeC(x - 1)].owner != map[y][x].owner;
			bR = map[safeC(y + 1)][safeC(x + 1)].owner != map[y][x].owner;
			tL = map[safeC(y - 1)][safeC(x - 1)].owner != map[y][x].owner;
			tR = map[safeC(y - 1)][safeC(x + 1)].owner != map[y][x].owner;
			sX = (rT + 2 * lT + 4 * dT + 8 * uT) * 16;
			sY = 544;
			if (sX > 0) {
				drawSprite(sX, sY, 16, 16, dX, dY, 1, tint);
			}
			if (bL && !lT && !dT) {
				drawSprite(256, sY, 16, 16, dX, dY, 1, tint);
			}
			if (bR && !rT && !dT) {
				drawSprite(272, sY, 16, 16, dX, dY, 1, tint);
			}
			if (tL && !lT && !uT) {
				drawSprite(288, sY, 16, 16, dX, dY, 1, tint);
			}
			if (tR && !rT && !uT) {
				drawSprite(304, sY, 16, 16, dX, dY, 1, tint);
			}
		}
		if (map[y][x].forest != BARE) {
			sY = 336;
			sX = forestX(map[y][x].forest);
			int temp = 0;
			if ((x % 100) == timer / 4 || (UI.tH.y == y && UI.tH.x == x)) {
				temp = -2;
			}
			drawSprite(sX, sY, 16, 32, dX, dY + temp + offset - (16));
		}
		//Draw a Building
		if (map[y][x].building > -1) {
			DrawBuilding(x, y, dX, dY);
		}
		if (UI.tH.y == y && UI.tH.x == x) {
			drawSprite(416, 16, 16, 16, dX, dY + offset, 1, sf::Color::Yellow);
			if (UI.building) {
				DrawBuilding(UI.bSelected, dX, dY, sf::Color(255, 255, 255, 150));
			}
		}
	}
	else {
		sY = 17;
		int a = 255;
		if (map[y][x].elev > DEEP) {
			a = 150;
			a -= 25 * (map[y][x].elev - DEEP);
			if (map[y][x].forest != BARE) {
			//	a = 255;
			}
		}
		sf::Color color(a, a, a);
		drawSprite(sX, sY, 2, 2, dX, dY, 1, color);
	}
}

void DrawMap() {
	if (UI.zoom >= 1) {
		int x_max = ceil(texture.getSize().x / 16.0) + 4;
		int y_max = ceil(texture.getSize().y / 16.0) + 4;

		for (int i = -ceil(y_max / 2.0); i <= y_max / 2; i++) {
			for (int j = -ceil(x_max / 2.0); j <= x_max / 2; j++) {
				int y = safeC(i + UI.yView);
				int x = safeC(j + UI.xView);
				DrawTile(map, x, y, UI.xShift + j * 16 + texture.getSize().x / 2.0, UI.yShift + i * 16 + texture.getSize().y / 2.0);
			}
		}
	}
	else {
		int x_max = ceil(texture.getSize().x / 2.0) + 4;
		int y_max = ceil(texture.getSize().y / 2.0) + 4;

		for (int i = -ceil(y_max / 2.0); i <= y_max / 2; i++) {
			for (int j = -ceil(x_max / 2.0); j <= x_max / 2; j++) {
				int y = safeC(i + UI.yView);
				int x = safeC(j + UI.xView);
				DrawTile(map, x, y, UI.xShift + j * 2 + texture.getSize().x / 2.0, UI.yShift + i * 2 + texture.getSize().y / 2.0);
			}
		}
	}
}

void DrawResources() {
	float dX, dY;
	int sX, sW, sH;
	int scale = 4;
	int sY = 592;
	int fill = 0;
	std::string shade = "";
	
	//Population
	sX = 176; sW = 47; sH = 23;
	dX = scale;	dY = scale;
	for (int i = 0; i < 2; i++) {
		drawSprite(sX, sY, sW, sH, dX, dY, scale);
		drawSprite((WORKERS + i) * 16, 576, 11, 11, dX + 2 * scale, dY + 2 * scale, scale);
		Print(trimNum((int)p.res[WORKERS + i]), dX + 26.5 * scale, dY + 7.5 * scale, scale);

		shade = "*GREY*";
		if (p.income[WORKERS + i] > 0) {
			shade = "*GREEN*";
		}
		else if (p.income[WORKERS + i] < 0) {
			shade = "*RED*";
		}
		Print(shade + trimNum(p.income[WORKERS + i]) + shade, dX + 26.5 * scale, dY + 17.5 * scale, scale);

		//Max
		fill = ((float)(p.res[WORKERS + i] / p.max[WORKERS + i]) * 11 * scale);
		if (p.max[WORKERS + i] > 0) {
			sf::Color color = sf::Color(60, 143, 72);
			if (p.res[WORKERS + i] >= p.max[WORKERS + i]) {
				color = sf::Color(128, 33, 44);
			}
			drawSprite(16, 608, 1, 7, dX + 2 * scale, dY + 14 * scale, fill, scale, color);
		}
		dY += (sH + 1) * scale;
	}

	//Power, Wood, Stone, Gold
	sX = 224; sW = 41; sH = 23;
	dY = scale + 2 * (sH+1) * scale;
	dX = scale;
	for (int i = 0; i < 4; i++) {
		drawSprite(sX, sY, sW, sH, dX, dY, scale);
		drawSprite((POWER + i) * 16, 576, 11, 11, dX + 2 * scale, dY + 2 * scale, scale);
		Print(trimNum((int)p.res[POWER + i]), dX + 26.5 * scale, dY + 7.5 * scale, scale);

		shade = "*GREY*";
		if (p.income[POWER + i] > 0) {
			shade = "*GREEN*";
		}
		else if (p.income[POWER + i] < 0) {
			shade = "*RED*";
		}
		Print(shade + trimNum(p.income[POWER + i]) + shade, dX + 26.5 * scale, dY + 17.5 * scale, scale);

		fill = ((float)(p.res[POWER + i] / p.max[POWER + i]) * 11 * scale);
		if (p.max[POWER + i] > 0) {
			sf::Color color = sf::Color(60, 143, 72);
			if (p.res[POWER + i] >= p.max[POWER + i]) {
				color = sf::Color(128, 33, 44);
			}
			drawSprite(16, 608, 1, 7, dX + 2 * scale, dY + 14 * scale, fill, scale, color);
		}
		dY += (sH + 1) * scale;
	}

	//Draw Jobs
	sX = 304; sW = 41; sH = 39;
	dX = scale + 48 * scale;
	dY = scale;
	drawSprite(sX, sY, sW, sH, dX, dY, scale);
	for (int i = 0; i < 3; i++) {
		Print(trimNum((int)p.res[G_JOBS + i]), dX + 26.5 * scale, dY + (float)(8.5 + i * 12) * scale, scale);
	}

	//Draw Food/Water
	sX = 224; sW = 41; sH = 23;
	dY = scale;
	dX = 91 * scale;
	for (int i = 0; i < 2; i++) {
		drawSprite(sX, sY, sW, sH, dX, dY, scale);
		drawSprite((FOOD + i) * 16, 576, 11, 11, dX + 2 * scale, dY + 2 * scale, scale);
		Print(trimNum((int)p.res[FOOD + i]), dX + 26.5 * scale, dY + 7.5 * scale, scale);

		shade = "*GREY*";
		if (p.income[FOOD + i] > 0) {
			shade = "*GREEN*";
		}
		else if (p.income[FOOD + i] < 0) {
			shade = "*RED*";
		}
		Print(shade + trimNum(p.income[FOOD + i]) + shade, dX + 26.5 * scale, dY + 17.5 * scale, scale);

		fill = ((float)(p.res[FOOD + i] / p.max[FOOD + i]) * 11 * scale);
		if (p.max[FOOD + i] > 0) {
			sf::Color color = sf::Color(60, 143, 72);
			if (p.res[FOOD + i] >= p.max[FOOD + i]) {
				color = sf::Color(128, 33, 44);
			}
			drawSprite(16, 608, 1, 7, dX + 2 * scale, dY + 14 * scale, fill, scale, color);
		}
		dX += (sW + 1) * scale;
	}

}

void DrawRightBar() {
	float rX = UI.W - 320;
	float rY = 0;
	float rW = 20 * 16;
	float rH = UI.H;
	int sX = 0;
	int sY = 80;
	float x_pos, y_pos;
	int scale = 3;

	drawSprite(528, 16, 16, 16, UI.W - 320, 0, 20, UI.H / 16.0);
	if (map[UI.tH.y][UI.tH.x].building == -1) {
		//Show the UI related to buying buildings + the cost of the currently selected building
		for (int i = 0; i < UI.buildings.size(); i++) {
			x_pos = rX + 7 + (10 + 24 * scale) * (i % 4);
			y_pos = rY + 10 + (10 + 40 * scale) * (i / 4);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (UI.mX >= x_pos && UI.mY >= y_pos) {
					if (UI.mX <= x_pos + 16 * scale && UI.mY <= y_pos + 32 * scale) {
						UI.bSelected = (BUILDING)(i);
						UI.building = false;
					}
				}
			}
			if (i != UI.bSelected) {
				drawSprite(560, 16, 18, 32, rX + 7 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
			}
			else {
				drawSprite(578, 16, 18, 32, rX + 7 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
			}
			if (i > 0) {
				drawSprite(sX + 16 * (i - 1), sY, 16, 32, rX + 10 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
			}
			else {
				drawSprite(0, 112, 16, 16, rX + 10 + (10 + 24 * scale) * (i % 4), rY + 10 + (8 * scale), scale);
			}
		}
		Print(UI.buildings[UI.bSelected].name, rX + rW / 2, rY + rH - 350, 2, true);
		//Draw Cost of Current Building:
		Print("*BLUE*Workers: *BLUE*" + to_str(UI.buildings[UI.bSelected].costs[3]), rX + rW / 4, rY + rH - 260, 2, true);
		Print("*ORANGE*Spec.: *ORANGE*" + to_str(UI.buildings[UI.bSelected].costs[4]), rX + 3 * rW / 4, rY + rH - 260, 2, true);
		Print("*BROWN*Wood: *BROWN*" + to_str(UI.buildings[UI.bSelected].costs[0]), rX + rW / 4, rY + rH - 190, 2, true);
		Print("*GREY*Stone: *GREY*" + to_str(UI.buildings[UI.bSelected].costs[1]), rX + rW / 4, rY + rH - 120, 2, true);
		Print("*GOLD*Gold: *GOLD*" + to_str(UI.buildings[UI.bSelected].costs[2]), rX + 3 * rW / 4, rY + rH - 190, 2, true);
		Print("*GREEN*Time: *GREEN*" + to_str(UI.buildings[UI.bSelected].time), rX + 3 * rW / 4, rY + rH - 120, 2, true);

		Print("Buy", rX + rW / 2, rY + rH - 50, 3, true);
		x_pos = rX + rW / 2;
		y_pos = rY + rH - 50;
		x_pos -= 40;
		y_pos -= 20;
		if (UI.inGame) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (UI.mX >= x_pos && UI.mY >= y_pos) {
					if (UI.mX <= x_pos + 80 && UI.mY <= y_pos + 40) {
						UI.building = true;
					}
				}
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::B))) {
				UI.building = true;
			}
		}
	}
	else {
		//Show the income/max that this building provides
		float tIncome[WATER + 1] = {};
		float tMax[WATER + 1] = {};
		
		if (UI.buildings[map[UI.tH.y][UI.tH.x].building].costs[3] == 0 && UI.buildings[map[UI.tH.y][UI.tH.x].building].costs[4] == 0) {
			//Building has no workers, ie wells or houses or roads
			BuildingIncome(UI.tH.x, UI.tH.y, tIncome, tMax, tIncome, tMax);
		}
		else {
			//If this building has workers
			float wIncome[WATER + 1] = {};
			float wMax[WATER + 1] = {};
			BuildingIncome(UI.tH.x, UI.tH.y, wIncome, tMax, sIncome, sMax);
			float sEff = 1.0;
			float wEff = 1.0;
			if (p.income[S_JOBS] > 0) {
				sEff = min(1, p.res[SPECIALISTS] / p.income[S_JOBS]);
			}
			if (p.income[W_JOBS] > 0) {
				wEff = min(1, p.res[WORKERS] / p.income[W_JOBS]);
			}
			//Combine resources and max, appropriately adjusted for worker efficiency
			for (int i = 0; i < WATER + 1; i++) {
				tIncome[i] = wIncome[i] * wEff + sIncome[i] * sEff;
				tMax[i] = wMax[i] * wEff + sMax[i] * sEff;
			}
			tIncome[W_JOBS] = UI.buildings[map[UI.tH.y][UI.tH.x].building].costs[3];
			tIncome[S_JOBS] = UI.buildings[map[UI.tH.y][UI.tH.x].building].costs[4];
		}

		//WORKERS, SPECIALISTS, W_WELLBEING, S_WELLBEING, G_JOBS, W_JOBS, S_JOBS, POWER, WOOD, STONE, GOLD, FOOD, WATER
		std::vector<std::string> words = {
			"Workers", "Specialists", "Worker Wellbeing", "Specialist Wellbeing", "Gatherer Jobs", "Worker Jobs", "Specialist Jobs", "Power", "Wood", "Stone", "Gold", "Food", "Water"
		};
		int dY = rY + 10;
		for (int a = 0; a < 2; a++) {
			for (int i = 0; i < WATER + 1; i++) {
				if (a == 0) {
					if (tIncome[i] > 0.01 || tIncome[i] < -0.01) {
						Print(words[i] + ": " + trimNum(tIncome[i]), rX + 10, dY, scale, false);
						dY += 25;
					}
				}
				else {
					if (tMax[i] > 0.01 || tMax[i] < -0.01) {
						Print("Max " + words[i] + ": " + trimNum(tMax[i]), rX + 10, dY, scale, false);
						dY += 25;
					}
				}
			}
		}
	}

	//Buy Button
}

void DrawUI() {
	DrawResources();
	DrawRightBar();

}