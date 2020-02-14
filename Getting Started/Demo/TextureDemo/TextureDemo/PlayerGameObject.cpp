#include "PlayerGameObject.h"

#include "Window.h"

#define MAX_VEL_X 3
#define MIN_VEL_X -3
#define MAX_VEL_Y 3
#define MIN_VEL_Y -3
#define FRICTION 0.1f

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityTexture, entityNumElements) {}

void PlayerGameObject::changeAcceleration(glm::vec3 newAcceleration)
{
	setAcceleration(newAcceleration);
	speed = 1.0f;
}

// Update function for moving the player object around
void PlayerGameObject::update(double deltaTime) {

	// Slow down speed by friction
	if (speed > 0)
		speed -= FRICTION;
	else
		speed = 0;

	// Update velocity based on current acceleration
	velocity.x += acceleration.x;
	velocity.y += acceleration.y;
	velocity.x *= speed;
	velocity.y *= speed;

	// Cap player speed
	if (velocity.x > MAX_VEL_X) velocity.x = MAX_VEL_X;
	if (velocity.y > MAX_VEL_Y) velocity.y = MAX_VEL_Y;
	if (velocity.x < MIN_VEL_X) velocity.x = MIN_VEL_X;
	if (velocity.y < MIN_VEL_Y) velocity.y = MIN_VEL_Y;

	// Change angle for rendering helicopter tilt
	angle = -(30 * (velocity.x / MAX_VEL_X));

	// Update object position with Euler integration
	position += velocity * (float)deltaTime;
}

void PlayerGameObject::transform(Shader &shader) {
	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), (float)(angle - 90), glm::vec3(0, 0, 1));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(objectSize, objectSize, 1));

	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);
}

void PlayerGameObject::render(Shader &shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	transform(shader);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
