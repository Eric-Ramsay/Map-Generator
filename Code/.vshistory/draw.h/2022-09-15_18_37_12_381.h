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
	if (WINDOW_DRAW) {
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
	if (WINDOW_DRAW) {
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
		return sf::Color(255, 0, 0);
	}
	else if (text == "green") {
		return sf::Color(0, 255, 0);
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


void DrawTile(std::vector<std::vector<Tile>>& map, int x, int y, int dX, int dY) {
	int sX = sourceX(map[y][x].type);
	int sY = 0;
	float offset = 0;

	if (zoom >= 1) {
		//Tile randomization to mix things up
		int useRand = randC(x, y) * 8;
		if (useRand > 4) {
			useRand = 0;
		}
		if (map[y][x].elev == HILL || map[y][x].elev == MOUNTAIN || map[y][x].forest != NONE || map[y][x].building.type > -1) {
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
		if (map[y][x].forest != NONE) {
			sY = 336;
			sX = forestX(map[y][x].forest);
			int temp = 0;
			if ((x % 100) == timer / 4 || (tH.y == y && tH.x == x)) {
				temp = -2;
			}
			drawSprite(sX, sY, 16, 32, dX, dY + temp + offset - (16));
		}
		if (map[y][x].building.type > -1) {
			sY = 80;
			sX = 16 * map[y][x].building.type;

			drawSprite(sX, sY, 16, 32, dX, dY - (16));
		}
		if (tH.y == y && tH.x == x) {
			drawSprite(416, 16, 16, 16, dX, dY + offset, 1, sf::Color::Yellow);
			if (building > -1) {
				sY = 80;
				sX = 16 * building;

				drawSprite(sX, sY, 16, 32, dX, dY + offset - (16), 1, sf::Color(255, 255, 255, 150));
			}
		}
	}
	else {
		sY = 17;
		int a = 255;
		if (map[y][x].elev > DEEP) {
			a = 150;
			a -= 25 * (map[y][x].elev - DEEP);
			if (map[y][x].forest != NONE) {
			//	a = 255;
			}
		}
		sf::Color color(a, a, a);
		drawSprite(sX, sY, 2, 2, dX, dY, 1, color);
	}
}

void DrawMap() {
	if (zoom >= 1) {
		int x_max = ceil(texture.getSize().x / 16.0) + 4;
		int y_max = ceil(texture.getSize().y / 16.0) + 4;

		for (int i = -ceil(y_max / 2.0); i <= y_max / 2; i++) {
			for (int j = -ceil(x_max / 2.0); j <= x_max / 2; j++) {
				int y = safeC(i + yView);
				int x = safeC(j + xView);
				DrawTile(map, x, y, xShift + j * 16 + texture.getSize().x / 2.0, yShift + i * 16 + texture.getSize().y / 2.0);
			}
		}
	}
	else {
		int x_max = ceil(texture.getSize().x / 2.0) + 4;
		int y_max = ceil(texture.getSize().y / 2.0) + 4;

		for (int i = -ceil(y_max / 2.0); i <= y_max / 2; i++) {
			for (int j = -ceil(x_max / 2.0); j <= x_max / 2; j++) {
				int y = safeC(i + yView);
				int x = safeC(j + xView);
				DrawTile(map, x, y, xShift + j * 2 + texture.getSize().x / 2.0, yShift + i * 2 + texture.getSize().y / 2.0);
			}
		}
	}
}

void DrawResources() {
	int sX, dX, dY, sW, sH;
	int scale = 4;
	int sY = 592;
	
	//Population
	sX = 176; sW = 47; sH = 23;
	dX = scale;	dY = scale;
	drawSprite(sX, sY, sW, sH, dX, dY, scale);
	dY += (sH + 1) * scale;
	drawSprite(sX, sY, sW, sH, dX, dY, scale);

	//Power, Wood, Stone, Gold
	sX = 224; sW = 41; sH = 23;
	dY = scale + 2 * (sH+1) * scale;
	dX = scale;
	for (int i = 0; i < 4; i++) {
		drawSprite(sX, sY, sW, sH, dX, dY, scale);
		dY += (sH + 1) * scale;
	}

	//Draw Jobs
	sX = 304; sW = 41; sH = 39;
	dX = scale + 48 * scale;
	dY = scale;
	drawSprite(sX, sY, sW, sH, dX, dY, scale);
	for (int i = 0; i < 3; i++) {
		Print();
	}

	//Draw Food/Water
	sX = 224; sW = 41; sH = 23;
	dY = scale;
	dX = 91 * scale;
	for (int i = 0; i < 2; i++) {
		drawSprite(sX, sY, sW, sH, dX, dY, scale);
		dX += (sW + 1) * scale;
	}

}

void DrawRightBar() {
	float rX = W - 320;
	float rY = 0;
	float rW = 20 * 16;
	float rH = H;

	drawSprite(528, 16, 16, 16, W - 320, 0, 20, H / 16.0);
	//Right Bar Stuff -----
	static int selected = 0;
	//Draw all buildings
	int sX = 0;
	int sY = 80;
	float x_pos, y_pos;
	for (int i = 0; i < buildings.size(); i++) {
		int scale = 3;
		x_pos = rX + 7 + (10 + 24 * scale) * (i % 4);
		y_pos = rY + 10 + (10 + 40 * scale) * (i / 4);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (mX >= x_pos && mY >= y_pos) {
				if (mX <= x_pos + 16 * scale && mY <= y_pos + 32 * scale) {
					selected = i;
					building = -1;
				}
			}
		}
		if (i != selected) {
			drawSprite(112, 160, 18, 32, rX + 7 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
		}
		else {
			drawSprite(130, 160, 18, 32, rX + 7 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
		}
		drawSprite(sX + 16 * i, sY, 16, 32, rX + 10 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
	}

	Print(buildings[selected].name, rX + rW / 2, rY + rH - 350, 2, true);
	//Draw Cost of Current Building:
	Print("*BLUE*Workers: *BLUE*" + to_str(buildings[selected].costs[3]), rX + rW / 4, rY + rH - 260, 2, true);
	Print("*ORANGE*Spec.: *ORANGE*" + to_str(buildings[selected].costs[4]), rX + 3 * rW / 4, rY + rH - 260, 2, true);
	Print("*BROWN*Wood: *BROWN*" + to_str(buildings[selected].costs[0]), rX + rW / 4, rY + rH - 190, 2, true);
	Print("*GREY*Stone: *GREY*" + to_str(buildings[selected].costs[1]), rX + rW / 4, rY + rH - 120, 2, true);
	Print("*GOLD*Gold: *GOLD*" + to_str(buildings[selected].costs[2]), rX + 3 * rW / 4, rY + rH - 190, 2, true);
	Print("*GREEN*Time: *GREEN*" + to_str(buildings[selected].time), rX + 3 * rW / 4, rY + rH - 120, 2, true);

	Print("Buy", rX + rW / 2, rY + rH - 50, 3, true);
	x_pos = rX + rW / 2;
	y_pos = rY + rH - 50;
	x_pos -= 40;
	y_pos -= 20;
	if (inGame) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (mX >= x_pos && mY >= y_pos) {
				if (mX <= x_pos + 80 && mY <= y_pos + 40) {
					building = selected;
				}
			}
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::B))) {
			building = selected;
		}
	}

	//Buy Button
}

void DrawUI() {
	DrawResources();
	DrawRightBar();

}