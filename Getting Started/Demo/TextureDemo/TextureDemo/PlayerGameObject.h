#pragma once

#include "GameObject.h"

// Inherits from GameObject
class PlayerGameObject : public GameObject {
public:
	PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);

	// Overrides
	void update(double deltaTime);
	void transform(Shader &shader);
	void render(Shader &shader);
	void damage();
	
	// Member functions
	void changeAcceleration(glm::vec3 newAcceleration);

	// Static data members
	static double lastDamageTime;
};
