#include "levelgridactor.h"

#include "game/game.h"
#include "game/scene/scene.h"

#include "utils/random.h"

#include <iostream>

static const int TOP = 1;
static const int LEFT = 2;
static const int BOTTOM = 4;
static const int RIGHT = 8;

static int getType(const Grid<bool>& grid, int x, int y) {
	int result = 0;
	if (y == grid.getHeight() - 1 || !grid[x][y + 1]) result |= TOP;
	if (x > 0 && !grid[x - 1][y]) result |= LEFT;
	if (y > 0 && !grid[x][y - 1]) result |= BOTTOM;
	if (x < grid.getWidth() - 1 && !grid[x + 1][y]) result |= RIGHT;

	return result;
}

static SpriteDrawObject generateSpriteDrawObject(const Grid<bool>& grid, int x, int y) {

	int type = ::getType(grid, x, y);	

	int vcount = 4;
	if (type & TOP) vcount += 4;
	if (type & LEFT) vcount += 4;
	if (type & BOTTOM) vcount += 4;
	if (type & RIGHT) vcount += 4;

	VertexData data(vcount);

	Sprite s = Game::getInstance().getResources().getSprite("forest.s0");

	data.setVertex(0, x * 48, y * 48);
	data.setVertex(1, x * 48 + 48, y * 48);
	data.setVertex(2, x * 48 + 48, y * 48 + 48);
	data.setVertex(3, x * 48, y * 48 + 48);

	data.setRectangleTextureCoordinates(*s.getDiffuse());



	return SpriteDrawObject(s, data);
}

static PhysicsShape generatePhysicsShape(const Grid<bool>& grid, int x, int y) {
	PhysicsShape result(PhysicsShape::SOLID, 24, 24);

	result.setPosition(x * 48 + 24, y * 48 + 24);
	result.setGravityFactor(0);

	if (y == grid.getHeight() - 1 || !grid[x][y + 1]) {
		result.setCanHangOn(true);
	} 

	return result;
}


LevelGridActor::LevelGridActor(const Grid<bool>& grid, const std::string& sprite_key) {
	sprite_key_ = sprite_key;

	Grid<bool> back_grid = generateBackTileGrid(grid);

	for (int x = 0; x < grid.getWidth(); ++x) {
		for (int y = 0; y < grid.getHeight(); ++y) {
			if (grid[x][y]) {
				generateTileDrawObject(grid, x, y);
				generateTilePhysicsShape(grid, x, y);
			}
			if (back_grid[x][y] && !grid[x][y]) {
				generateBackTileDrawObject(back_grid, x, y);
			}
		}
	}
}

/*void LevelGridActor::generateTileDrawObject(const Grid<bool>& grid, int x, int y) {
	int type = ::getType(grid, x, y);	
	int vcount = 4;
	if (type & TOP) vcount += 4;
	if (type & LEFT) vcount += 4;
	if (type & BOTTOM) vcount += 4;
	if (type & RIGHT) vcount += 4;

	VertexData data(vcount, vcount * 1.5);

	Sprite s = Game::getInstance().getResources().getSprite("forest.s0");
	Sprite t = Game::getInstance().getResources().getSprite("forest.t0");
	Sprite l = Game::getInstance().getResources().getSprite("forest.l0");
	Sprite r = Game::getInstance().getResources().getSprite("forest.r0");
	Sprite b = Game::getInstance().getResources().getSprite("forest.b0");


	// Solid
	data.setRectangleVertices(24 + x * 48, 24 + y * 48, s.getPixelWidth(), s.getPixelHeight());
	data.setRectangleIndices();
	data.setRectangleTextureCoordinates(*s.getDiffuse());

	int v_offset = 4;
	int i_offset = 6;

	if (type & LEFT) {
		data.setRectangleVertices(4 + x * 48, 24 + y * 48, l.getPixelWidth(), l.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*l.getDiffuse(), v_offset);
		v_offset += 4;
		i_offset += 6;
	}

	if (type & RIGHT) {
		data.setRectangleVertices(44 + x * 48, 24 + y * 48, r.getPixelWidth(), r.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*r.getDiffuse(), v_offset);
		v_offset += 4;
		i_offset += 6;
	}

	if (type & BOTTOM) {
		data.setRectangleVertices(24 + x * 48, 4 + y * 48, b.getPixelWidth(), b.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*b.getDiffuse(), v_offset);	
		v_offset += 4;
		i_offset += 6;
	}

	if (type & TOP) {
		data.setRectangleVertices(24 + x * 48, 44 + y * 48, t.getPixelWidth(), t.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*t.getDiffuse(), v_offset);
		v_offset += 4;
		i_offset += 6;
	}

	draw_objects_.push_back(SpriteDrawObject(s, data));
	draw_objects_[draw_objects_.size() - 1].setPosition();
}*/

void LevelGridActor::generateTileDrawObject(const Grid<bool>& grid, int x, int y) {
	int type = ::getType(grid, x, y);	
	int vcount = 4;
	if (type & TOP) vcount += 4;
	if (type & LEFT) vcount += 4;
	if (type & BOTTOM) vcount += 4;
	if (type & RIGHT) vcount += 4;

	VertexData data(vcount, vcount * 1.5);

	Sprite s = Game::getInstance().getResources().getSprite("forest.s0");
	Sprite t = Game::getInstance().getResources().getSprite("forest.t0");
	Sprite l = Game::getInstance().getResources().getSprite("forest.l0");
	Sprite r = Game::getInstance().getResources().getSprite("forest.r0");
	Sprite b = Game::getInstance().getResources().getSprite("forest.b0");


	// Solid
	data.setRectangleVertices(24 + 0 * 48, 24 + 0 * 48, s.getPixelWidth(), s.getPixelHeight());
	data.setRectangleIndices();
	data.setRectangleTextureCoordinates(*s.getDiffuse());

	int v_offset = 4;
	int i_offset = 6;

	if (type & LEFT) {
		data.setRectangleVertices(4 + 0 * 48, 24 + 0 * 48, l.getPixelWidth(), l.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*l.getDiffuse(), v_offset);
		v_offset += 4;
		i_offset += 6;
	}

	if (type & RIGHT) {
		data.setRectangleVertices(44 + 0 * 48, 24 + 0 * 48, r.getPixelWidth(), r.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*r.getDiffuse(), v_offset);
		v_offset += 4;
		i_offset += 6;
	}

	if (type & BOTTOM) {
		data.setRectangleVertices(24 + 0 * 48, 4 + 0 * 48, b.getPixelWidth(), b.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*b.getDiffuse(), v_offset);	
		v_offset += 4;
		i_offset += 6;
	}

	if (type & TOP) {
		data.setRectangleVertices(24 + 0 * 48, 44 + 0 * 48, t.getPixelWidth(), t.getPixelHeight(), v_offset);
		data.setRectangleIndices(i_offset, v_offset);
		data.setRectangleTextureCoordinates(*t.getDiffuse(), v_offset);
		v_offset += 4;
		i_offset += 6;
	}

	draw_objects_.push_back(SpriteDrawObject(s, data));
	draw_objects_[draw_objects_.size() - 1].setPosition(x * 48, y * 48);
	draw_objects_[draw_objects_.size() - 1].setAllowFilterByPosition(true);
}

void LevelGridActor::generateBackTileDrawObject(const Grid<bool>& grid, int x, int y) {
	int type = ::getType(grid, x, y);	
	if (type == 15) return;

	Sprite b = Game::getInstance().getResources().getSprite("forest.a" + std::to_string(type));

	SpriteDrawObject res = SpriteDrawObject(b, 0 * 48 + 24, 0 * 48 + 24, b.getPixelWidth(), b.getPixelHeight());

	res.setDrawLayer(5.0);
	draw_objects_.push_back(res);
	draw_objects_[draw_objects_.size() - 1].setPosition(x * 48, y * 48);
	draw_objects_[draw_objects_.size() - 1].setAllowFilterByPosition(true);
}



static void spreadConnect(const Grid<bool>& grid, Grid<bool>& connect, int x, int y) {
	if (grid.isOutOfBound(x, y)) return;
	if (!grid[x][y] || connect[x][y]) return;
	connect[x][y] = true;
	spreadConnect(grid, connect, x - 1, y);
	spreadConnect(grid, connect, x + 1, y);
	spreadConnect(grid, connect, x, y - 1);
	spreadConnect(grid, connect, x, y + 1);
}


static void connect_dir(Grid<bool>& grid, Grid<bool>& connected, int x, int y, int dx, int dy, int offset) {
	if (grid.isOutOfBound(x, y) || connected[x][y]) return;
	grid[x][y] = true;
	//connected[x][y] = true;
	if (offset > 0 && utils::random()%8 == 0) {
		return;
	}

	connect_dir(grid, connected, x + dx, y + dy, dx, dy, offset);

	if (utils::random()%(4 + offset) == 0) connect_dir(grid, connected, x + dy, y + dx, dx, dy, offset + 1);
	if (utils::random()%(4 + offset) == 0) connect_dir(grid, connected, x - dy, y - dx, dx, dy, offset + 1);
}

static int getConnectDistance(Grid<bool>& grid, Grid<bool>& connected, int x, int y, int dx, int dy) {
	if (grid.isOutOfBound(x, y) && dy == 1) return 99999999;
	if (grid.isOutOfBound(x, y) || connected[x][y]) return 0;
	return 1 + getConnectDistance(grid, connected, x + dx, y + dy, dx, dy);
}

static void connect(Grid<bool>& grid, Grid<bool>& connect, int x, int y) {
	int l = getConnectDistance(grid, connect, x, y, -1, 0);
	int r = getConnectDistance(grid, connect, x, y, 1, 0);
	int u = getConnectDistance(grid, connect, x, y, 0, 1);
	int d = getConnectDistance(grid, connect, x, y, 0, -1);

	if (l < u && l < r && l < d) {
		connect_dir(grid, connect, x, y, -1, 0, 0);
	} else if (r < l && r < u && r < d) {
		connect_dir(grid, connect, x, y, 1, 0, 0);
	} else if (u < r && u < d && u < l ) {
		connect_dir(grid, connect, x, y, 0, 1, 0);
	} else {
		connect_dir(grid, connect, x, y, 0, -1, 0);
	}

}

Grid<bool> LevelGridActor::generateBackTileGrid(const Grid<bool>& grid) {

	Grid<bool> connected(grid.getWidth(), grid.getHeight(), false);

	Grid<bool> result = grid;

	for (int x = 0; x < grid.getWidth(); ++x) {
		for (int y = 0; y < grid.getHeight(); ++y) {
			if (result[x][y] && (x == 0 || y == 0 || x == grid.getWidth() - 1)) {
				spreadConnect(result, connected, x, y);
			}
		} 
	}

	for (int y = 0; y <grid.getHeight(); ++y) {
		for (int x = 0; x < grid.getWidth(); ++x) {
			if (result[x][y] && !connected[x][y]) {
				connect(result, connected, x, y);
				spreadConnect(result, connected, x, y);
			}	
		}
	}

	return result;
}

void LevelGridActor::generateTilePhysicsShape(const Grid<bool>& grid, int x, int y) {
	PhysicsShape result(PhysicsShape::SOLID, 24, 24);

	result.setPosition(x * 48 + 24 , y * 48 + 24);
	result.setGravityFactor(0);

	if (y == grid.getHeight() - 1 || !grid[x][y + 1]) {
		result.setCanHangOn(true);
	}

	shapes_.push_back(result);
}


void LevelGridActor::onAddToScene() {
	for (int i = 0; i < draw_objects_.size(); ++i) {
		getScene()->addDrawObject(&draw_objects_[i]);
	}
	for (int i = 0; i < shapes_.size(); ++i) {
		getScene()->addPhysicsShape(&shapes_[i]);
	}
}

void LevelGridActor::onRemoveFromScene() {
	for (int i = 0; i < draw_objects_.size(); ++i) {
		getScene()->removeDrawObject(&draw_objects_[i]);
	}
	for (int i = 0; i < shapes_.size(); ++i) {
		getScene()->removePhysicsShape(&shapes_[i]);
	}
}

void LevelGridActor::onStep(float dt) {

}