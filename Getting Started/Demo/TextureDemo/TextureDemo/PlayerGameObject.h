#pragma once

#include "GameObject.h"

// Inherits from GameObject
class PlayerGameObject : public GameObject {
public:
	PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);

	// Overrides
	virtual void update(double deltaTime) override;
	virtual void render(Shader & shader) override;
};
