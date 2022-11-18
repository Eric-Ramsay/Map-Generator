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

void drawSprite(int sX, int sY, int w, int h, float dX, float dY, float x_scale, float y_scale, sf::Color color = sf::Color(255, 255, 255)) {
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
				//272, 128 is the font's position on the sprite sheet
				drawSprite(sX + 272, sY + 128, sW, sH, drawX + offX * scale, drawY + offY * scale, scale, p);
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

void DrawMap() {
	int width = ceil(float(ScreenWidth()) / float(zoom));
	int height = ceil(float(ScreenHeight()) / float(zoom));
	if (dXView > 0) { width += 2; }
	if (dYView > 0) { height += 2; }
	for (int i = -1 - ceil(height / 2); i <= 1 + ceil(height / 2); i++) {
		for (int j = -1 - ceil(width / 2); j <= ceil(width / 2); j++) {
			int y = safeC(i + yView);
			int x = safeC(j + xView);
			DrawTile(map, x, y, (ScreenWidth() / 2 + ((j * zoom) - dXView)), (ScreenHeight() / 2 + ((i * zoom) - dYView)), zoom);
			//int temp = latTemp(y-5);
			//Print(to_str(temp), (ScreenWidth() / 2 + ((j * zoom) - dXView)) + zoom/2, (ScreenHeight() / 2 + ((i * zoom) - dYView)) + zoom/2, true, zoom / 16.0);
		}
	}
}

//building type, draw area, map area (for animation seed), 
void DrawBuilding(int type, int dX, int dY, float scale, int HP = 99, C c = C(15, 15), int owner = 0, olc::Pixel pix = olc::WHITE) {
	float z = scale;
	int index = bIndex(type);

	int sX = buildings[index].sX;
	int sY = buildings[index].sY;


	if (index == 0) { //Farm
		sY += 32 * (type - 1);
	}
	/*else if (index == 2) { //bridge
		if (type == 12) {
			sY += 32;
		}
	}*/
	if (buildings[index].anim_num > 1) {
		sY += 32 * (((int)(randC(c.x, c.y) * 5) + timer / buildings[index].anim_speed) % buildings[index].anim_num);
	}
	DrawPartialDecal(olc::vf2d(dX, dY), building_sprites, olc::vf2d(sX, sY), olc::vf2d(16, 32), olc::vf2d(z, z), pix);
	if (buildings[index].top_color) {
		DrawPartialDecal(olc::vf2d(dX, dY), building_sprites, olc::vf2d(sX, 0), olc::vf2d(16, 16), olc::vf2d(z, z), colors[owner]);
	}
	if (buildings[index].bot_color) {
		DrawPartialDecal(olc::vf2d(dX, dY + 16 * scale), building_sprites, olc::vf2d(sX, 0), olc::vf2d(16, 16), olc::vf2d(z, z), colors[owner]);
	}
	if (HP < getBuildingHP(type, owner) / 2.0) { //draw fire sprite
		sX = 48;
		sY = 48;
		sY += 32 * (((int)(randC(c.x, c.y) * 5) + timer / 4) % 4);
		DrawPartialDecal(olc::vf2d(dX, dY), building_sprites, olc::vf2d(sX, sY), olc::vf2d(16, 32), olc::vf2d(z, z));
	}
}

void DrawDrag() { //Draw unit that is being dragged
	float z = (float)zoom / 16.0;
	int x = drag.x;
	int y = drag.y;
	int dX = GetMouseX() - 7 * z;
	int dY = GetMouseY();
	olc::Pixel ap = colors[map[y][x].unit.owner];
	olc::Pixel pix = olc::RED;
	pix.a = 125;
	int sX, sY;
	if (dragging_unit) {
		if (map[y][x].unit.type == NO_UNIT) {
			dragging_unit = false;
			return;
		}
		dY -= 7 * z;
		sX = units[map[y][x].unit.type - 1].sX;
		sY = units[map[y][x].unit.type - 1].sY;
		//Draw a unit being dragged
		DrawPartialDecal(olc::vf2d(dX, dY - z), armySprites, olc::vf2d(sX, sY), olc::vf2d(16, 17), olc::vf2d(z, z));
		//  x  y sX  sY  w   h  scale
		DrawPartialDecal(olc::vf2d(dX, dY), armySprites, olc::vf2d(sX, sY + 17), olc::vf2d(16, 14), olc::vf2d(z, z), ap); //Draw Color

		int hp = 12 * ((float)map[y][x].unit.HP / getMaxHP(map[y][x].unit.type, map[y][x].unit.owner));
		int mov = 12 * ((float)map[y][x].unit.MP / getMaxMP(map[y][x].unit.type, map[y][x].unit.owner));
		//			     x  y sX  sY  w   h  scale
		Draw(armySprites, dX + z, (dY - zoom) + 9 * z, 1, 9, 14, 5, z);
		DrawPartialDecal(olc::vf2d(dX + 2 * z, (dY - zoom) + 10 * z), flat, olc::vf2d(418, 0), olc::vf2d(1, 1), olc::vf2d(hp * z, z));
		DrawPartialDecal(olc::vf2d(dX + 2 * z, (dY - zoom) + 12 * z), flat, olc::vf2d(420, 0), olc::vf2d(1, 1), olc::vf2d(mov * z, z));
	}
	else if (buying_unit) {
		dY += 4 * z;
		ap.a = 125;
		if (canBuy(tH.x, tH.y, unit_selected)) {
			pix = olc::WHITE;
			pix.a = 200;
			ap.a = 200;
		}
		else {
			bool ape = canBuy(tH.x, tH.y, unit_selected);
		}
		//Draw a unit being purchased
		Draw(armySprites, dX, dY - zoom, units[unit_selected].sX, units[unit_selected].sY, 16, 17, z, pix);
		Draw(armySprites, dX, dY - (zoom - z), units[unit_selected].sX, 17 + units[unit_selected].sY, 16, 14, z, colors[p.turn]);
	}
	else if (buying_building) {
		sY = 16;
		ap.a = 125;
		x = tH.x;
		y = tH.y;
		if (canBuyBuilding(tH.x, tH.y, building_selected)) {
			pix = olc::WHITE;
			pix.a = 200;
			ap.a = 200;
		}
		DrawBuilding(buildings[building_selected].type, dX, dY - zoom, z, 99, C(15, 15), p.turn, pix);
	}
	if (attacking_unit) {
		if (map[tS.y][tS.x].unit.type > 0 && map[tS.y][tS.x].unit.owner == p.turn && map[tS.y][tS.x].unit.MP > 0) {
			int w = ceil(float(ScreenWidth()) / float(zoom));
			int h = ceil(float(ScreenHeight()) / float(zoom));
			int hX = GetMouseX();
			int hY = GetMouseY();
			bool sFound = false;
			x = safeC(xView + (-1 - ceil(w / 2)));
			y = safeC(yView + (-1 - ceil(h / 2)));
			if (dXView > 0) { w += 2; }
			if (dYView > 0) { h += 2; }
			for (int i = -1 - ceil(h / 2); i <= 1 + ceil(h / 2); i++) {
				for (int j = -1 - ceil(w / 2); j <= ceil(w / 2); j++) {
					x = safeC(j + xView);
					y = safeC(i + yView);
					if (y == tS.y && x == tS.x) {
						sX = (ScreenWidth() / 2 + ((j * zoom) - dXView));
						sY = (ScreenHeight() / 2 + ((i * zoom) - dYView));
						sFound = true;
					}
					if (sFound) {
						i = h;
						j = w;
					}
				}
			}
			if (sFound) {
				float angle = -atan2((sX - (hX - 8 * z)), (sY - (hY - 8 * z)));

				float dist_x = (sX - (hX - 8 * z)) * (sX - (hX - 8 * z));
				float dist_y = (sY - (hY - 8 * z)) * (sY - (hY - 8 * z));
				float dist = sqrt(dist_x + dist_y);
				if (dist < zoom) {
					dist = zoom;
				}
				if (dist > units[map[tS.y][tS.x].unit.type - 1].range * zoom) {
					dist = units[map[tS.y][tS.x].unit.type - 1].range * zoom;
				}

				DrawPartialRotatedDecal(olc::vf2d(sX + 8 * z, sY + 8 * z), icons, angle, olc::vf2d(6, 9), olc::vf2d(80, 80), olc::vf2d(12, 10), olc::vf2d(z, (dist / 9)));
			}
		}
	}
}

void DrawElevation(int x, int y, int dX, int dY, float z, int e = -1) {
	int elev = e;
	if (elev == -1) {
		elev = map[y][x].elev;
	}
	int sX = 0;
	int sY = 32;

	if (elev == MOUNTAIN) {
		//sY += 16;
		DrawElevation(x, y, dX, dY, z, elev - 1);
		sX = 0;
		sY = 80;
		if (map[safeC(y + 1)][x].elev == MOUNTAIN) {
			sX += 16;
			if (map[safeC(y - 1)][x].elev == MOUNTAIN) {
				sX += 32;
			}
		}
		Draw(terrain, dX, (dY - 16 * z), sX, sY, 16, 32, z);
		return;
	}


	bool u = map[safeC(y - 1)][x].elev >= elev; // up * 8
	bool d = map[safeC(y + 1)][x].elev >= elev; // down * 4
	bool l = map[y][safeC(x - 1)].elev >= elev; // left * 2
	bool r = map[y][safeC(x + 1)].elev >= elev; // right * 1
	if (!u && !d && !l && !r) {
		//DrawPartialDecal(olc::vf2d(dX, dY - zoom), terrain, olc::vf2d(sX, sY), olc::vf2d(16, 32), olc::vf2d(z, z));
		//return;
	}
	sX = 16 * ((u * 8) + (d * 4) + (l * 2) + r);

	DrawPartialDecal(olc::vf2d(dX, dY), terrain, olc::vf2d(sX, sY), olc::vf2d(16, 16), olc::vf2d(z, z));
	bool dr = d && r && map[safeC(y + 1)][safeC(x + 1)].elev < elev;
	bool dl = d && l && map[safeC(y + 1)][safeC(x - 1)].elev < elev;

	if (dr) {
		DrawPartialDecal(olc::vf2d(dX, dY), terrain, olc::vf2d(272, sY), olc::vf2d(16, 16), olc::vf2d(z, z));
	}
	if (dl) {
		DrawPartialDecal(olc::vf2d(dX, dY), terrain, olc::vf2d(256, sY), olc::vf2d(16, 16), olc::vf2d(z, z));
	}
}

void DrawTile(std::vector<std::vector<Tile>>& map, int x, int y, int dX, int dY, int zom = 16, bool borders = false, bool extra = true) {
	if (dY + zom < 0 || dX + zom < 0) {
		return;
	}
	bool drawDot = false;
	olc::Pixel dotColor = GREY;
	if (!buying_unit && !buying_building && map[tS.y][tS.x].unit.type > 0) {
		int MP = map[tS.y][tS.x].unit.MP;
		dotColor = colors[map[tS.y][tS.x].unit.owner];
		if (map[tS.y][tS.x].unit.owner != turn) {
			MP = getMaxMP(map[tS.y][tS.x].unit.type, map[tS.y][tS.x].unit.owner);
		}
		float mCost = moveCost(Spot(tS.x, tS.y), Spot(x, y), MP, map[tS.y][tS.x].unit);
		drawDot = mCost <= MP;

		if (mCost > MP - 1) {
			dotColor = GREY;
		}
		dotColor.a = 125;
	}

	float z = (float)zom / 16.0;
	if (zom >= 16) {
		int sX = sourceX(map[y][x].type);
		int sY = 0;
		int useRand = randC(x, y) * 4;
		if (map[y][x].elev == HILL || map[y][x].elev == MOUNTAIN || map[y][x].forest != NONE || map[y][x].building) {
			useRand = 0;
		}
		if (map[y][x].elev == WATER) { //Water Tiles
			useRand = timer / 50;
			if (map[y][x].type == 'a') {
				useRand++;
			}
			else if (map[y][x].type == 'r') {
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
		DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(sX + rX * 16, sY + 17 + rY * 16), olc::vf2d(16, 16), olc::vf2d(z, z));
		//Draw Tile Overlays/Borders
			//Adjacent Tile Terrain overlay
		TERRAIN adjType;
		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if ((a == 0) != (b == 0)) {
					int side = abs(b);
					adjType = map[safeC(y + a)][safeC(x + b)].type;
					if (typePrecedent(adjType) > typePrecedent(map[y][x].type)) {
						sX = sourceX(adjType);
						sY = 0;
						if (map[y][x].elev == WATER) {
							sY += 50;
						}
						float offset = z + (float)zom * (13.0 / 16.0);
						if ((offset - (int)offset) == .25) {
							offset += 1.0 / 8.0;
						}
						if (side > 0) {
							if ((offset - (int)offset) == .75) {
								offset += 2.0 / 8.0;
							}
							if (b == -1) {
								DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(sX + 14, sY), olc::vf2d(2, 16), olc::vf2d(z, z));
							}
							else {
								DrawPartialDecal(olc::vf2d(dX + offset, dY), flat, olc::vf2d(sX, sY), olc::vf2d(2, 16), olc::vf2d(z, z));
							}
						}
						else {
							if ((offset - (int)offset) == .75) {
								offset += a * 2 / 8.0;
							}
							if (a == -1) {
								DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(sX + 16, sY + 14), olc::vf2d(16, 2), olc::vf2d(z, z));
							}
							else {
								DrawPartialDecal(olc::vf2d(dX, dY + offset), flat, olc::vf2d(sX + 16, sY), olc::vf2d(16, 2), olc::vf2d(z, z));
							}
						}
					}
				}
			}
		}
		if (drawDot) { //Possible Unit Movement
			DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(448, 16), olc::vf2d(16, 16), olc::vf2d(z, z), dotColor);
		}
		/*if (borders && map[y][x].owner != 0) { //DRAW COUNTRY BORDERS
			olc::Pixel tint = colors[map[y][x].owner];
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
			DrawPartialDecal(olc::vf2d(dX, dY), terrain, olc::vf2d((rT + 2 * lT + 4 * dT + 8 * uT) * 16, 64), olc::vf2d(16, 16), olc::vf2d(z, z), tint);
			if (bL && !lT && !dT) {
				DrawPartialDecal(olc::vf2d(dX, dY + (11 * (z))), terrain, olc::vf2d(256, 64 + 11),
					olc::vf2d(3, 5), olc::vf2d(z, z), tint);
			}
			if (bR && !rT && !dT) {
				DrawPartialDecal(olc::vf2d(dX + (13 * (z)), dY + (11 * (z))), terrain, olc::vf2d(272 + 13, 64 + 11),
					olc::vf2d(3, 5), olc::vf2d(z, z), tint);
			}
			if (tL && !lT && !uT) {
				DrawPartialDecal(olc::vf2d(dX, dY), terrain, olc::vf2d(288, 64),
					olc::vf2d(3, 5), olc::vf2d(z, z), tint);
			}
			if (tR && !rT && !uT) {
				DrawPartialDecal(olc::vf2d(dX + (13 * (z)), dY), terrain, olc::vf2d(304 + 13, 64),
					olc::vf2d(3, 5), olc::vf2d(z, z), tint);
			}
		}*/
		if (map[y][x].elev >= HILL) {
			DrawElevation(x, y, dX, dY, z);
		}
		//Forest, Hills, Mountains
		if (map[y][x].elev >= HILL && map[safeC(y + 1)][x].elev < HILL) { //Hill
			dY -= 3 * z;
		}
		if (map[y][x].building > 0) {
			DrawBuilding(map[y][x].building, dX, dY - zom, z, map[y][x].HP, C(x, y), map[y][x].owner);
		}
		if (map[y][x].forest > FISH) {
			sY = 0;
			sX = forestX(map[y][x].forest);
			if ((x % 100) == timer / 4 || (tH.y == y && tH.x == x)) {
				sX += 64;
			}
			DrawPartialDecal(olc::vf2d(dX, dY - zom), terrain, olc::vf2d(sX, sY), olc::vf2d(16, 32), olc::vf2d(z, z));
		}
		else if (map[y][x].forest == FISH && map[y][x].harvest < harvest) {
			int step = ((int)(randC(x, y) * 8) + (timer / 8)) % 8;
			sX = 176 + 16 * (step % 4);
			sY = 16 * (int)(step / 4);
			if (sY > 16) {
				std::cout << sX << std::endl;
				std::cout << sY << std::endl;
			}
			DrawPartialDecal(olc::vf2d(dX, dY), terrain, olc::vf2d(sX, sY), olc::vf2d(16, 16), olc::vf2d(z, z));
		}
		if (map[y][x].unit.type > 0 && !(dragging_unit && drag.x == x && drag.y == y)) { //Unit on Tile
			olc::Pixel ap = colors[map[y][x].unit.owner];
			sX = units[map[y][x].unit.type - 1].sX;
			sY = units[map[y][x].unit.type - 1].sY;
			float off = 1;
			if (map[y][x].building == 7 || map[y][x].building == 12) { //Fort or Bridge
				off = 6;
			}
			if (map[y][x].building == 11) { //dock
				off = 5;
			}
			//Draw a unit
			DrawPartialDecal(olc::vf2d(dX, dY - ((off)*z)), armySprites, olc::vf2d(sX, sY), olc::vf2d(16, 17), olc::vf2d(z, z));
			//  x  y sX  sY  w   h  scale
			DrawPartialDecal(olc::vf2d(dX, dY - ((off - 1) * z)), armySprites, olc::vf2d(sX, sY + 17), olc::vf2d(16, 14), olc::vf2d(z, z), ap);

			int hp = 12 * ((float)map[y][x].unit.HP / getMaxHP(map[y][x].unit.type, map[y][x].unit.owner));
			int mov = 12 * ((float)map[y][x].unit.MP / getMaxMP(map[y][x].unit.type, map[y][x].unit.owner));
			//			     x  y sX  sY  w   h  scale
			Draw(armySprites, dX + z, (dY - zom) + (9 - off) * z, 1, 9, 14, 5, z);
			DrawPartialDecal(olc::vf2d(dX + 2 * z, (dY - zom) + (10 - off) * z), flat, olc::vf2d(418, 0), olc::vf2d(1, 1), olc::vf2d(hp * z, z));
			DrawPartialDecal(olc::vf2d(dX + 2 * z, (dY - zom) + (12 - off) * z), flat, olc::vf2d(420, 0), olc::vf2d(1, 1), olc::vf2d(mov * z, z));
		}
		//UI tile selection/tile hovered overlay
		if (tS.y == y && tS.x == x) {
			DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(416, 16), olc::vf2d(16, 16), olc::vf2d(z, z), UI_Blue1);
		}
		if (tH.y == y && tH.x == x) {
			olc::Pixel pix = Gold;
			if (attacking_unit && map[tS.y][tS.x].unit.MP > 0) {
				if (coord_dist(tH.x, tS.x) <= units[map[tS.y][tS.x].unit.type - 1].range && coord_dist(tH.y, tS.y) <= units[map[tS.y][tS.x].unit.type - 1].range) {
					pix = olc::RED;
				}
			}
			DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(416, 16), olc::vf2d(16, 16), olc::vf2d(z, z), pix);
		}
	}
	else {
		olc::Pixel p(255, 255, 255);
		switch (map[y][x].type) {
		case OCEAN: p = Water; break;
		case RIVER: p = River; break;
		case LAKE: p = Lake; break;
		case GRASS: p = Grass; break;
		case STEPPE: p = Steppe; break;
		case SAVANNA: p = Savanna; break;
		case COLD_DESERT: p = Cold_Desert; break;
		case DESERT: p = Desert; break;
		case TUNDRA: p = Tundra; break;
		case ICE: p = Ice; break;
		case BOG: p = Bog; break;
		case MEADOW: p = Meadow; break;
		case SHALLOW: p = Ocean; break;
		}
		if (map[y][x].forest != NONE) {
			p = Forest;
		}
		else if (map[y][x].elev == HILL) {
			p = Hill;
		}
		else if (map[y][x].elev == MOUNTAIN) {
			p = Mountain;
		}
		/*if (drawCurrentDirection) {
			if (map[y][x].elev == FLAT) {
				int intens = 255;// std::min(255, map[y][x].num / 35);
				switch (map[y][x].prevailing) {
				case -1: break;
				case 0: p = olc::Pixel(255, 165, 0, intens);  break; //d -> orange
				case 1: p = olc::Pixel(255, 0, 0, intens); break; //r -> red
				case 2: p = olc::Pixel(120, 255, 200, intens);  break; //u -> teal
				case 3: p = olc::Pixel(0, 255, 0, intens);  break; //l -> green
				}
				intens = map[y][x].forest_C;
				if (intens > 255) {
					intens = 255;
				}
				p = olc::Pixel(0, 255, 0, intens);
			}
		}
		//p = olc::Pixel(0, 0, 255, map[y][x].temp * 2.55);*/
		FillRect(olc::vf2d(dX, dY), olc::vf2d(zom, zom), p);
		if (map[y][x].owner != 0) {
			olc::Pixel p = colors[map[y][x].owner];
			p.a = 75;
			FillRect(olc::vf2d(dX, dY), olc::vf2d(zom, zom), p);
		}
	}
}

void DrawBox(int x, int y, int w, int h, olc::Pixel color = olc::WHITE, int thickness = 1) {
	FillRectDecal(olc::vf2d(x, y), olc::vf2d(thickness, h), color);
	FillRectDecal(olc::vf2d(x + w, y), olc::vf2d(thickness, h + thickness), color);
	FillRectDecal(olc::vf2d(x, y), olc::vf2d(w, thickness), color);
	FillRectDecal(olc::vf2d(x, y + h), olc::vf2d(w, thickness), color);
}
//End UI and Drawing----------------------------------------------------------------------------------

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
	int sY = 128;
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
						UI.click = CLAIM;
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
				//I would use DrawBuilding but it needs map coordinates
				drawSprite(sX + 16 * (i - 1), sY, 16, 32, rX + 10 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
			}
			else {
				//Draw Roads
				drawSprite(0, 160, 16, 16, rX + 10 + (10 + 24 * scale) * (i % 4), rY + 10 + (8 * scale), scale);
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
						UI.click = BUILD;
					}
				}
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::B))) {
				UI.click = BUILD;
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
			float sIncome[WATER + 1] = {};
			float sMax[WATER + 1] = {};
			BuildingIncome(UI.tH.x, UI.tH.y, wIncome, tMax, sIncome, sMax);
			float sEff = 1.0;
			float wEff = 1.0;
			if (p.income[S_JOBS] > 0) {
				sEff = min(1, (int)p.res[SPECIALISTS] / (float)p.res[S_JOBS]);
			}
			if (p.income[W_JOBS] > 0) {
				wEff = min(1, (int)p.res[WORKERS] / (float)p.res[W_JOBS]);
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