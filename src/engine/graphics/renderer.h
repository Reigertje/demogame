#ifndef RENDERER_H
#define RENDERER_H

class GraphicsWorld;

class Renderer {
friend class GraphicsWorld;

protected:

	virtual void render(GraphicsWorld& graphics_world) = 0;

};

#endif