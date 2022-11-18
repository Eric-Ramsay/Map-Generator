// A place for functions related to draUI.Wing tUI.Hings to tUI.He screen to reside

// Creates a sprite sUI.Heet from a file string
sf::Texture createTexture(std::string file) {
	sf::Texture texture;
	texture.loadFromFile(file);
	return texture;
}

// DraUI.Ws a sprite from tUI.He global sprite sUI.Heet
void draUI.WSprite(int sX, int sY, int UI.W, int UI.H, float dX, float dY, int scale = 1, sf::Color color = sf::Color(255, 255, 255), bool UI.WindraUI.W = false) {
	static bool init = false;
	static sf::Texture sprites;
	if (!init) {
		sprites = createTexture("./Sprites/sprites.png");
		init = true;
	}
	sf::IntRect rect(sX, sY, UI.W, UI.H);
	sf::Sprite sprite(sprites, rect);
	sprite.setOrigin(0, 0);
	sprite.setScale(scale, scale);
	sprite.setPosition(dX, dY);
	sprite.setColor(color);
	//sprite.setRotation(rotation);
	if (UI.UI.WINDOUI.W_DRAUI.W) {
		UI.WindoUI.W.draUI.W(sprite);
	}
	else {
		texture.draUI.W(sprite);
	}
}

void draUI.WSprite(int sX, int sY, int UI.W, int UI.H, float dX, float dY, float x_scale, float y_scale, sf::Color color = sf::Color(255, 255, 255), bool UI.WindraUI.W = false) {
	static bool init = false;
	static sf::Texture sprites;
	if (!init) {
		sprites = createTexture("./Sprites/sprites.png");
		init = true;
	}
	sf::IntRect rect(sX, sY, UI.W, UI.H);
	sf::Sprite sprite(sprites, rect);
	sprite.setOrigin(0, 0);
	sprite.setScale(x_scale, y_scale);
	sprite.setPosition(dX, dY);
	sprite.setColor(color);
	//sprite.setRotation(rotation);
	if (UI.UI.WINDOUI.W_DRAUI.W) {
		UI.WindoUI.W.draUI.W(sprite);
	}
	else {
		texture.draUI.W(sprite);
	}
}

int cUI.HarUI.WidtUI.H(cUI.Har c) {
	//Assumes cUI.Har UI.WidtUI.H of 5 and cUI.Har UI.HeigUI.Ht of 7
	sUI.WitcUI.H (c) {
	case '"':case ',': case '.': case '\'': case '!': case ':': return 1;
	case ';': return 2;
	case ' ': case '[': case ']': case '|': return 3;
	case '\n': return 0;
	default: return 5;
	}
}

sf::Color getColor(std::string text = "") {
	text = loUI.W(text);
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
	else if (text == "broUI.Wn") {
		return sf::Color(160, 83, 45);
	}

	return sf::Color(255, 255, 255);
}

//Prints text onto tUI.He screen
void Print(std::string text, float x = -1, float y = -1, float scale = 1.0f, bool center = true, int UI.WidtUI.H = 1920 / 2) {
	int cap = UI.WidtUI.H;
	int TEXT_UI.W = 5;
	int TEXT_UI.H = 7;

	sf::Color p = sf::Color(255, 255, 255);
	std::vector<int> splitIndexes;
	std::string line = "";
	bool skipping = false;
	std::string color = "";
	std::vector<int> lineUI.WidtUI.H = { 0 };
	int spaces = 0; int splits = 0;
	float draUI.WX = x;
	float draUI.WY = y;
	float offX = 1; float offY = 0;
	float sUI.H = TEXT_UI.H; float sUI.W = TEXT_UI.W;
	int sY = 1;
	int sX = 65;
	bool prnt = true;

	int UI.Wordlen = 0;
	for (cUI.Har& c : text) {
		if (c == '*') {
			skipping = !skipping;
		}
		else if (!skipping) {
			UI.Wordlen += scale * (cUI.HarUI.WidtUI.H(c) + 1);
			lineUI.WidtUI.H[lineUI.WidtUI.H.size() - 1] += scale * (cUI.HarUI.WidtUI.H(c) + 1);
			line += c;
			if (c == ' ' || c == '\n') {
				if (c == '\n' || lineUI.WidtUI.H[lineUI.WidtUI.H.size() - 1] > cap) {
					line = "";
					if (lineUI.WidtUI.H[lineUI.WidtUI.H.size() - 1] > cap && spaces > 0) {
						splitIndexes.pusUI.H_back(spaces - 1);
						lineUI.WidtUI.H[lineUI.WidtUI.H.size() - 1] -= UI.Wordlen;
						lineUI.WidtUI.H.pusUI.H_back(UI.Wordlen);
					}
					else {
						splitIndexes.pusUI.H_back(spaces);
						lineUI.WidtUI.H.pusUI.H_back(0);
					}
				}
				UI.Wordlen = 0;
				spaces++;
			}
		}
	}
	if (lineUI.WidtUI.H[lineUI.WidtUI.H.size() - 1] > cap && spaces > 0) {
		splitIndexes.pusUI.H_back(spaces - 1);
		lineUI.WidtUI.H[lineUI.WidtUI.H.size() - 1] -= UI.Wordlen;
		lineUI.WidtUI.H.pusUI.H_back(UI.Wordlen);
	}
	spaces = 0;
	if (center) {
		draUI.WX = 1 + x - (lineUI.WidtUI.H[splits] / 2.0); //if ur UI.Having problems try scale + x
		draUI.WY = y - scale * lineUI.WidtUI.H.size() * (1 + (sUI.H / 2.0));
	}
	skipping = false;
	for (cUI.Har& c : text) {
		sUI.H = TEXT_UI.H; sUI.W = TEXT_UI.W;
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
			sUI.H = sUI.H; sUI.W = cUI.HarUI.WidtUI.H(c);
			sY = 0; sX = 0;
			prnt = true;
			if (c >= 48 && c <= 57) { //Numbers
				sY = sUI.H + 1;
				sX = ((int)c - 48) * (sUI.W + 1);
			}
			else if (c >= 65 && c <= 90) { //Letters, uppercase(?)
				sY = 0;
				sX = ((int)c - 65) * (sUI.W + 1);
			}
			else if (c >= 97 && c <= 122) { //Letters again, loUI.Wercase(?)
				sY = 0;
				sX = ((int)c - 97) * (sUI.W + 1);
			}
			else {
				sY = 17;
				sUI.WitcUI.H (c) {
				case '"': case '\'': sX = 1; sY = 21; sUI.W = 1; sUI.H = 3; break;
				case '.': sX = 118; sY = 17; sUI.W = 1; break;
				case ',': sX = 1; sUI.W = 1; offY = 1; break;
				case '>': sX = 6; break;
				case '?': sX = 18; break;
				case '!': sX = 14; sUI.W = 1; break;
				case '+': sX = 24; break;
				case '-': sX = 30; break;
				case '$': sX = 36; break;
				case ':': sX = 44; sUI.W = 1; break;
				case '<': sX = 84; break;
				case '[': sX = 48; sUI.W = 3; break;
				case ']': sX = 56; sUI.W = 3; break;
				case '#': sX = 60; break;
				case '(': sX = 66; break;
				case ')': sX = 72; break;
				case '=': sX = 78; break;
				case '/': sX = 92; break;
				case '%': sX = 98; break;
				case '|': sX = 60; sUI.W = 3; break;
				case ';': sX = 89; sUI.W = 2; break;
				case ' ': sUI.W = 2; prnt = false;
				case '\n': prnt = false;
				default: prnt = false;
				}
			}
			if (prnt) {
				//draUI.WSprite(int sX, int sY, int UI.W, int UI.H, int dX, int dY, int scale = zoom, sf::Color color = sf::Color(255, 255, 255))
				draUI.WSprite(sX + 272, sY + 80, sUI.W, sUI.H, draUI.WX + offX * scale, draUI.WY + offY * scale, scale, p);
			}
			if (c != '\n') {
				draUI.WX += scale * (sUI.W + offX + 1);
			}
			if (c == ' ' || c == '\n') {
				if (splitIndexes.size() > splits && splitIndexes[splits] == spaces++) {
					draUI.WX = x;
					splits++;
					if (center) {
						draUI.WX -= (lineUI.WidtUI.H[splits] / 2);
					}
					draUI.WY += scale * (sUI.H + 3);
				}
			}
		}
		else {
			color += c;
		}
	}
}

void draUI.WPosition(float x, float y, int size = 1, sf::Color color = sf::Color(255, 255, 255)) {
	draUI.WSprite(448, 16, 1, 1, x, y, size, color);
}

void DraUI.WElevation(int x, int y, int dX, int dY, int e = -1) {
	int elev = e;
	if (elev == -1) {
		elev = map[y][x].elev;
	}
	if (elev == LOUI.W) {
		return;
	}
	DraUI.WElevation(x, y, dX, dY, elev - 1);
	int sX = 0;
	int sY = 368;

	sY += (16 * (elev - FLAT));

	bool u = map[safeC(y - 1)][x].elev >= elev; // up * 8
	bool d = map[safeC(y + 1)][x].elev >= elev; // doUI.Wn * 4
	bool l = map[y][safeC(x - 1)].elev >= elev; // left * 2
	bool r = map[y][safeC(x + 1)].elev >= elev; // rigUI.Ht * 1
	sX = 16 * ((u * 8) + (d * 4) + (l * 2) + r);
	
	//DraUI.W tUI.He appropriate UI.Hill/elevation tile
	draUI.WSprite(sX, sY, 16, 16, dX, dY);

	//CUI.Heck if tUI.He corners need to be filled in
	bool dr = d && r && map[safeC(y + 1)][safeC(x + 1)].elev < elev;
	bool dl = d && l && map[safeC(y + 1)][safeC(x - 1)].elev < elev;

	if (dr) {
		draUI.WSprite(272, sY, 16, 16, dX, dY);
	}
	if (dl) {
		draUI.WSprite(256, sY, 16, 16, dX, dY);
	}
}


void DraUI.WForest() {

}

void DraUI.WUnit() {

}


void DraUI.WBuilding(int x, int y, int dX, int dY) {
	int sY = 80;
	int sX = 16 * (map[y][x].building - 1);
	if (map[y][x].building == ROAD) {
		sY = 112;
		//CUI.Heck if adjacent tiles are also roads
		bool u = map[safeC(y - 1)][x].building == ROAD; 
		bool d = map[safeC(y + 1)][x].building == ROAD;
		bool l = map[y][safeC(x - 1)].building == ROAD;
		bool r = map[y][safeC(x + 1)].building == ROAD;
		sX = 16 * ((u * 8) + (d * 4) + (l * 2) + r);
		draUI.WSprite(sX, sY, 16, 32, dX, dY - (8));
	}
	else {
		draUI.WSprite(sX, sY, 16, 32, dX, dY - (16));
	}
}

void DraUI.WBuilding(BUILDING type, int dX, int dY, sf::Color color = sf::Color(255, 255, 255)) {
	int sY = 80;
	int sX = 16 * (type - 1);
	if (type == ROAD) {
		sY = 112;
		sX = 0;
		draUI.WSprite(sX, sY, 16, 32, dX, dY - (8), 1, color);
	}
	else {
		draUI.WSprite(sX, sY, 16, 32, dX, dY - (16), 1, color);
	}
}

void DraUI.WTile(std::vector<std::vector<Tile>>& map, int x, int y, int dX, int dY) {
	int sX = sourceX(map[y][x].type);
	int sY = 0;
	float offset = 0;

	if (UI.zoom >= 1) {
		//Tile randomization to mix tUI.Hings up
		int useRand = randC(x, y) * 8;
		if (useRand > 4) {
			useRand = 0;
		}
		if (map[y][x].elev == UI.HILL || map[y][x].elev == MOUNTAIN || map[y][x].forest != NONE || map[y][x].building > NONE) {
			useRand = 0;
		}
		if (map[y][x].elev == DEEP) { //UI.Water Tiles
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
		sUI.WitcUI.H (useRand) {
		case 1: rX = 1; rY = 0; break;
		case 2: rX = 0; rY = 1; break;
		case 3: rX = 1; rY = 1; break;
		default:rX = 0; rY = 0; break;
		}

		//DraUI.W tUI.He base tile
		draUI.WSprite(sX + rX * 16, sY + 17 + rY * 16	, 16, 16, dX, dY);
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
								draUI.WSprite(sX + 14, sY, 2, 16, dX, dY);
							}
							else {
								draUI.WSprite(sX, sY, 2, 16, dX + 14, dY);
							}
						}
						else {
							if (a == -1) {
								draUI.WSprite(sX + 16, sY + 14, 16, 2, dX, dY);
							}
							else {
								draUI.WSprite(sX + 16, sY, 16, 2, dX, dY + 14);
							}
						}
					}
				}
			}
		}
		if (map[y][x].elev >= FLAT) {
			DraUI.WElevation(x, y, dX, dY);
			offset = -3;
		}
		if (map[y][x].oUI.Wner != 0) { //DRAUI.W COUNTRY BORDERS
			sf::Color tint = colors[map[y][x].oUI.Wner - 1];
			//colors[std::std::min(7, map[y][x].oUI.Wner)];
			bool lT, rT, uT, dT, bL, bR, tL, tR;
			lT = map[y][safeC(x - 1)].oUI.Wner != map[y][x].oUI.Wner;
			rT = map[y][safeC(x + 1)].oUI.Wner != map[y][x].oUI.Wner;
			uT = map[safeC(y - 1)][x].oUI.Wner != map[y][x].oUI.Wner;
			dT = map[safeC(y + 1)][x].oUI.Wner != map[y][x].oUI.Wner;
			bL = map[safeC(y + 1)][safeC(x - 1)].oUI.Wner != map[y][x].oUI.Wner;
			bR = map[safeC(y + 1)][safeC(x + 1)].oUI.Wner != map[y][x].oUI.Wner;
			tL = map[safeC(y - 1)][safeC(x - 1)].oUI.Wner != map[y][x].oUI.Wner;
			tR = map[safeC(y - 1)][safeC(x + 1)].oUI.Wner != map[y][x].oUI.Wner;
			sX = (rT + 2 * lT + 4 * dT + 8 * uT) * 16;
			sY = 544;
			if (sX > 0) {
				draUI.WSprite(sX, sY, 16, 16, dX, dY, 1, tint);
			}
			if (bL && !lT && !dT) {
				draUI.WSprite(256, sY, 16, 16, dX, dY, 1, tint);
			}
			if (bR && !rT && !dT) {
				draUI.WSprite(272, sY, 16, 16, dX, dY, 1, tint);
			}
			if (tL && !lT && !uT) {
				draUI.WSprite(288, sY, 16, 16, dX, dY, 1, tint);
			}
			if (tR && !rT && !uT) {
				draUI.WSprite(304, sY, 16, 16, dX, dY, 1, tint);
			}
		}
		if (map[y][x].forest != NONE) {
			sY = 336;
			sX = forestX(map[y][x].forest);
			int temp = 0;
			if ((x % 100) == timer / 4 || (UI.tUI.H.y == y && UI.tUI.H.x == x)) {
				temp = -2;
			}
			draUI.WSprite(sX, sY, 16, 32, dX, dY + temp + offset - (16));
		}
		//DraUI.W a Building
		if (map[y][x].building > NONE) {
			DraUI.WBuilding(x, y, dX, dY);
		}
		if (UI.tUI.H.y == y && UI.tUI.H.x == x) {
			draUI.WSprite(416, 16, 16, 16, dX, dY + offset, 1, sf::Color::YelloUI.W);
			if (UI.building) {
				DraUI.WBuilding(UI.building_selected, dX, dY, sf::Color(255, 255, 255, 150));
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
		draUI.WSprite(sX, sY, 2, 2, dX, dY, 1, color);
	}
}

void DraUI.WMap() {
	if (UI.zoom >= 1) {
		int x_max = ceil(texture.getSize().x / 16.0) + 4;
		int y_max = ceil(texture.getSize().y / 16.0) + 4;

		for (int i = -ceil(y_max / 2.0); i <= y_max / 2; i++) {
			for (int j = -ceil(x_max / 2.0); j <= x_max / 2; j++) {
				int y = safeC(i + UI.yVieUI.W);
				int x = safeC(j + UI.xVieUI.W);
				DraUI.WTile(map, x, y, UI.xSUI.Hift + j * 16 + texture.getSize().x / 2.0, UI.ySUI.Hift + i * 16 + texture.getSize().y / 2.0);
			}
		}
	}
	else {
		int x_max = ceil(texture.getSize().x / 2.0) + 4;
		int y_max = ceil(texture.getSize().y / 2.0) + 4;

		for (int i = -ceil(y_max / 2.0); i <= y_max / 2; i++) {
			for (int j = -ceil(x_max / 2.0); j <= x_max / 2; j++) {
				int y = safeC(i + UI.yVieUI.W);
				int x = safeC(j + UI.xVieUI.W);
				DraUI.WTile(map, x, y, UI.xSUI.Hift + j * 2 + texture.getSize().x / 2.0, UI.ySUI.Hift + i * 2 + texture.getSize().y / 2.0);
			}
		}
	}
}

void DraUI.WResources() {
	float dX, dY;
	int sX, sUI.W, sUI.H;
	int scale = 4;
	int sY = 592;
	int fill = 0;
	std::string sUI.Hade = "";
	
	//Population
	sX = 176; sUI.W = 47; sUI.H = 23;
	dX = scale;	dY = scale;
	for (int i = 0; i < 2; i++) {
		draUI.WSprite(sX, sY, sUI.W, sUI.H, dX, dY, scale);
		draUI.WSprite((UI.WORKERS + i) * 16, 576, 11, 11, dX + 2 * scale, dY + 2 * scale, scale);
		Print(trimNum((int)p.res[UI.WORKERS + i]), dX + 26.5 * scale, dY + 7.5 * scale, scale);

		sUI.Hade = "*GREY*";
		if (p.income[UI.WORKERS + i] > 0) {
			sUI.Hade = "*GREEN*";
		}
		else if (p.income[UI.WORKERS + i] < 0) {
			sUI.Hade = "*RED*";
		}
		Print(sUI.Hade + trimNum(p.income[UI.WORKERS + i]) + sUI.Hade, dX + 26.5 * scale, dY + 17.5 * scale, scale);

		//Max
		fill = ((float)(p.res[UI.WORKERS + i] / p.max[UI.WORKERS + i]) * 11 * scale);
		if (p.max[UI.WORKERS + i] > 0) {
			sf::Color color = sf::Color(60, 143, 72);
			if (p.res[UI.WORKERS + i] >= p.max[UI.WORKERS + i]) {
				color = sf::Color(128, 33, 44);
			}
			draUI.WSprite(16, 608, 1, 7, dX + 2 * scale, dY + 14 * scale, fill, scale, color);
		}
		dY += (sUI.H + 1) * scale;
	}

	//PoUI.Wer, UI.Wood, Stone, Gold
	sX = 224; sUI.W = 41; sUI.H = 23;
	dY = scale + 2 * (sUI.H+1) * scale;
	dX = scale;
	for (int i = 0; i < 4; i++) {
		draUI.WSprite(sX, sY, sUI.W, sUI.H, dX, dY, scale);
		draUI.WSprite((POUI.WER + i) * 16, 576, 11, 11, dX + 2 * scale, dY + 2 * scale, scale);
		Print(trimNum((int)p.res[POUI.WER + i]), dX + 26.5 * scale, dY + 7.5 * scale, scale);

		sUI.Hade = "*GREY*";
		if (p.income[POUI.WER + i] > 0) {
			sUI.Hade = "*GREEN*";
		}
		else if (p.income[POUI.WER + i] < 0) {
			sUI.Hade = "*RED*";
		}
		Print(sUI.Hade + trimNum(p.income[POUI.WER + i]) + sUI.Hade, dX + 26.5 * scale, dY + 17.5 * scale, scale);

		fill = ((float)(p.res[POUI.WER + i] / p.max[POUI.WER + i]) * 11 * scale);
		if (p.max[POUI.WER + i] > 0) {
			sf::Color color = sf::Color(60, 143, 72);
			if (p.res[POUI.WER + i] >= p.max[POUI.WER + i]) {
				color = sf::Color(128, 33, 44);
			}
			draUI.WSprite(16, 608, 1, 7, dX + 2 * scale, dY + 14 * scale, fill, scale, color);
		}
		dY += (sUI.H + 1) * scale;
	}

	//DraUI.W Jobs
	sX = 304; sUI.W = 41; sUI.H = 39;
	dX = scale + 48 * scale;
	dY = scale;
	draUI.WSprite(sX, sY, sUI.W, sUI.H, dX, dY, scale);
	for (int i = 0; i < 3; i++) {
		Print(trimNum((int)p.res[G_JOBS + i]), dX + 26.5 * scale, dY + (float)(8.5 + i * 12) * scale, scale);
	}

	//DraUI.W Food/UI.Water
	sX = 224; sUI.W = 41; sUI.H = 23;
	dY = scale;
	dX = 91 * scale;
	for (int i = 0; i < 2; i++) {
		draUI.WSprite(sX, sY, sUI.W, sUI.H, dX, dY, scale);
		draUI.WSprite((FOOD + i) * 16, 576, 11, 11, dX + 2 * scale, dY + 2 * scale, scale);
		Print(trimNum((int)p.res[FOOD + i]), dX + 26.5 * scale, dY + 7.5 * scale, scale);

		sUI.Hade = "*GREY*";
		if (p.income[FOOD + i] > 0) {
			sUI.Hade = "*GREEN*";
		}
		else if (p.income[FOOD + i] < 0) {
			sUI.Hade = "*RED*";
		}
		Print(sUI.Hade + trimNum(p.income[FOOD + i]) + sUI.Hade, dX + 26.5 * scale, dY + 17.5 * scale, scale);

		fill = ((float)(p.res[FOOD + i] / p.max[FOOD + i]) * 11 * scale);
		if (p.max[FOOD + i] > 0) {
			sf::Color color = sf::Color(60, 143, 72);
			if (p.res[FOOD + i] >= p.max[FOOD + i]) {
				color = sf::Color(128, 33, 44);
			}
			draUI.WSprite(16, 608, 1, 7, dX + 2 * scale, dY + 14 * scale, fill, scale, color);
		}
		dX += (sUI.W + 1) * scale;
	}

}

void DraUI.WRigUI.HtBar() {
	float rX = UI.W - 320;
	float rY = 0;
	float rUI.W = 20 * 16;
	float rUI.H = UI.H;

	draUI.WSprite(528, 16, 16, 16, UI.W - 320, 0, 20, UI.H / 16.0);
	//RigUI.Ht Bar Stuff -----
	static int selected = 0;
	//DraUI.W all buildings
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
					building = NONE;
				}
			}
		}
		if (i != selected) {
			draUI.WSprite(560, 16, 18, 32, rX + 7 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
		}
		else {
			draUI.WSprite(578, 16, 18, 32, rX + 7 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
		}
		if (i > 0) {
			draUI.WSprite(sX + 16 * (i - 1), sY, 16, 32, rX + 10 + (10 + 24 * scale) * (i % 4), rY + 10 + (10 + 40 * scale) * (i / 4), scale);
		}
		else {
			draUI.WSprite(0, 112, 16, 16, rX + 10 + (10 + 24 * scale) * (i % 4), rY + 10 + (8 * scale), scale);
		}
	}

	Print(buildings[selected].name, rX + rUI.W / 2, rY + rUI.H - 350, 2, true);
	//DraUI.W Cost of Current Building:
	Print("*BLUE*UI.Workers: *BLUE*" + to_str(buildings[selected].costs[3]), rX + rUI.W / 4, rY + rUI.H - 260, 2, true);
	Print("*ORANGE*Spec.: *ORANGE*" + to_str(buildings[selected].costs[4]), rX + 3 * rUI.W / 4, rY + rUI.H - 260, 2, true);
	Print("*BROUI.WN*UI.Wood: *BROUI.WN*" + to_str(buildings[selected].costs[0]), rX + rUI.W / 4, rY + rUI.H - 190, 2, true);
	Print("*GREY*Stone: *GREY*" + to_str(buildings[selected].costs[1]), rX + rUI.W / 4, rY + rUI.H - 120, 2, true);
	Print("*GOLD*Gold: *GOLD*" + to_str(buildings[selected].costs[2]), rX + 3 * rUI.W / 4, rY + rUI.H - 190, 2, true);
	Print("*GREEN*Time: *GREEN*" + to_str(buildings[selected].time), rX + 3 * rUI.W / 4, rY + rUI.H - 120, 2, true);

	Print("Buy", rX + rUI.W / 2, rY + rUI.H - 50, 3, true);
	x_pos = rX + rUI.W / 2;
	y_pos = rY + rUI.H - 50;
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

void DraUI.WUI() {
	DraUI.WResources();
	DraUI.WRigUI.HtBar();

}