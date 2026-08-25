#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

class Camera;

class DrawableObject {
public:
    virtual void Draw(Camera& cam) = 0;
    virtual void ClearBuffers() = 0;

    Shader shader;
private:
};

#endif