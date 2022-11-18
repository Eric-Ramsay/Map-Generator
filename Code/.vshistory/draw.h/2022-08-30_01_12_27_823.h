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

void DrawTile(std::vector<std::vector<Tile>>& map, int x, int y, int dX, int dY, int zom = 16, bool borders = false, bool extra = true) {
	if (dY + zom < 0 || dX + zom < 0) {
		return;
	}
	/*bool drawDot = false;
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
	}*/

	float z = (float)zom / 16.0;
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
	drawSprite(sX + rX * 16, sY + 17 + rY * 16, 16, 16, dX, dY, z);
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
							drawSprite(sX + 14, sY, 2, 16, dX, dY, flat, z);
						}
						else {
							drawSprite(sX, sY, 2, 16, dX + offset, dY, flat, z);
						}
					}
					else {
						if ((offset - (int)offset) == .75) {
							offset += a * 2 / 8.0;
						}
						if (a == -1) {
							drawSprite(sX + 16, sY + 14, 16, 2, dX, dY, flat, z);
						}
						else {
							drawSprite(sX + 16, sY, 16, 2, dX + offset, dY, flat, z);
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
			//DrawElevation(x, y, dX, dY, z);
		}
		//Forest, Hills, Mountains
		if (map[y][x].elev >= HILL && map[safeC(y + 1)][x].elev < HILL) { //Hill
			dY -= 3 * z;
		}
		if (map[y][x].building > 0) {
			//DrawBuilding(map[y][x].building, dX, dY - zom, z, map[y][x].HP, C(x, y), map[y][x].owner);
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
			//DrawPartialDecal(olc::vf2d(dX, dY), terrain, olc::vf2d(sX, sY), olc::vf2d(16, 16), olc::vf2d(z, z));
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
			//DrawPartialDecal(olc::vf2d(dX, dY - ((off)*z)), armySprites, olc::vf2d(sX, sY), olc::vf2d(16, 17), olc::vf2d(z, z));
			//  x  y sX  sY  w   h  scale
			//DrawPartialDecal(olc::vf2d(dX, dY - ((off - 1) * z)), armySprites, olc::vf2d(sX, sY + 17), olc::vf2d(16, 14), olc::vf2d(z, z), ap);

			int hp = 12 * ((float)map[y][x].unit.HP / getMaxHP(map[y][x].unit.type, map[y][x].unit.owner));
			int mov = 12 * ((float)map[y][x].unit.MP / getMaxMP(map[y][x].unit.type, map[y][x].unit.owner));
			//			     x  y sX  sY  w   h  scale
			//Draw(armySprites, dX + z, (dY - zom) + (9 - off) * z, 1, 9, 14, 5, z);
			//DrawPartialDecal(olc::vf2d(dX + 2 * z, (dY - zom) + (10 - off) * z), flat, olc::vf2d(418, 0), olc::vf2d(1, 1), olc::vf2d(hp * z, z));
			//DrawPartialDecal(olc::vf2d(dX + 2 * z, (dY - zom) + (12 - off) * z), flat, olc::vf2d(420, 0), olc::vf2d(1, 1), olc::vf2d(mov * z, z));
		}
		//UI tile selection/tile hovered overlay
		if (tS.y == y && tS.x == x) {
			//DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(416, 16), olc::vf2d(16, 16), olc::vf2d(z, z), UI_Blue1);
		}
		if (tH.y == y && tH.x == x) {
			olc::Pixel pix = Gold;
			if (attacking_unit && map[tS.y][tS.x].unit.MP > 0) {
				if (coord_dist(tH.x, tS.x) <= units[map[tS.y][tS.x].unit.type - 1].range && coord_dist(tH.y, tS.y) <= units[map[tS.y][tS.x].unit.type - 1].range) {
					pix = olc::RED;
				}
			}
			//DrawPartialDecal(olc::vf2d(dX, dY), flat, olc::vf2d(416, 16), olc::vf2d(16, 16), olc::vf2d(z, z), pix);
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