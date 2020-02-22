#include "PlayerGameObject.h"

#define MAX_VEL_X 3
#define MIN_VEL_X -3
#define MAX_VEL_Y 3
#define MIN_VEL_Y -3
#define FRICTION 0.1f
#define DAMAGE_I_TIME 3

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player

*/

double PlayerGameObject::lastDamageTime = -DAMAGE_I_TIME;
int PlayerGameObject::numWeapons = 0;

PlayerGameObject::PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::vector<GLuint*> extraTextures)
	: GameObject(entityPos, entityTexture, entityNumElements), extraTextures(extraTextures) {
	maxHealth = 5;
	health = maxHealth;
	isFriendly = true;
	weapons = new std::vector<Weapon*>();

	// Setup weapons
	// { lifeSpan, weight, fireRate, speed, radius, name }
	Weapon machineGun = { -1.0f, 0.0f, 2.0f, 2.0f, 0.1f, "machineGun" };
	Weapon rockets = { 2.0f, 0.1f, 0.5f, 1.0f, 0.75f, "rockets" };
	//Weapon scudMissles	= { 2.0f, 1.0f, 0.75f, 0.5f, 0.5f, "scudMissles" };
	//Weapon laser		= { 3.0f, 0.0f, 0.1f, -1.0f, 0.15, "laser" };
	// Give the player these weapons
	giveWeapon(machineGun);
	giveWeapon(rockets);
	//giveWeapon(scudMissles);
	//giveWeapon(laser);
	equip(0);

	numWeapons = 2;
}

PlayerGameObject::~PlayerGameObject()
{
	for (int i = 0; i < numWeapons; i++)
		delete (*weapons)[i];
	delete weapons;
}

void PlayerGameObject::giveWeapon(Weapon& weap)
{
	Weapon* ptr = new Weapon(weap);
	weapons->push_back(ptr);
	numWeapons++;
}

void PlayerGameObject::changeAcceleration(glm::vec3 newAcceleration)
{
	setAcceleration(newAcceleration);
	speed = 1.0f;
}

void PlayerGameObject::equip(int index)
{
	if (index >= 0 && index < numWeapons) {
		equipped = (*weapons)[index];
	}
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

void PlayerGameObject::render(Shader &shader) {
	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), (float)(angle - 90), glm::vec3(0, 0, 1));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(objectSize, objectSize, 1));
	glm::mat4 damageOffset = glm::translate(glm::mat4(1.0f), glm::vec3(cos(glfwGetTime() * 50) / 50, 0, 0));
	
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	if (lastDamageTime + DAMAGE_I_TIME > glfwGetTime()) {
		transformationMatrix = damageOffset * transformationMatrix;
	}

	// ---------- DRAW WEAPON ----------
	if (numWeapons > 0) {
		int texIndex = 0;
		if (equipped->name == "machineGun") texIndex = 0;
		if (equipped->name == "rockets") texIndex = 1;

		// Weapon transformations


		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, *extraTextures[texIndex]);
		shader.setUniformMat4("transformationMatrix", transformationMatrix);
		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	}

	// ---------- DRAW PLAYER ----------
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void PlayerGameObject::damage() {
	if (lastDamageTime + DAMAGE_I_TIME < glfwGetTime()) {
		lastDamageTime = glfwGetTime();
		health -= 1;
		if (health <= 0) kill();
	}
}
