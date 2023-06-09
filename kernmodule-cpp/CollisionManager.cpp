
#include <iostream>

#include "CollisionManager.h"

#include "GameManager.h"
#include "GlobalData.h"

CollisionManager::CollisionManager(GameManager* gameManager) : gameManager(gameManager) {
	
}

void CollisionManager::ups() {

	for (list<PhysicsObject*>::iterator it = gameManager->physicsObjectList.begin(); it != gameManager->physicsObjectList.end(); it++) {
		PhysicsObject* object1 = *it;

		for (list<PhysicsObject*>::iterator it2 = gameManager->physicsObjectList.begin(); it2 != gameManager->physicsObjectList.end(); it2++) {
			PhysicsObject* object2 = *it2;

			if (getDistance(object1, object2) < minCollisionDistance && object1 != object2 && !listContains(object1) && !listContains(object2)) {

				if (checkCollision(object1, object2)) {

					doCollision(object1, object2);
					calcedObjects.push_back(object1);
					calcedObjects.push_back(object2);
				}
			}
		}
	}

	calcedObjects.clear();
}

bool CollisionManager::checkCollision(PhysicsObject* object1, PhysicsObject* object2) {
	//Object2 topLeft is inside Object 1
	if (object2->topLeft.x > object1->topLeft.x && object2->topLeft.y > object1->topLeft.y && object2->topLeft.x < object1->bottomRight.x && object2->topLeft.y < object1->bottomRight.y) { 
		return true;
	}

	//Object2 topRight is inside Object 1
	if (object2->topRight.x > object1->topLeft.x && object2->topRight.y > object1->topLeft.y && object2->topRight.x < object1->bottomRight.x && object2->topRight.y < object1->bottomRight.y) {
		return true;
	}

	//Object2 bottomLeft is inside Object 1
	if (object2->bottomLeft.x > object1->topLeft.x && object2->bottomLeft.y > object1->topLeft.y && object2->bottomLeft.x < object1->bottomRight.x && object2->bottomLeft.y < object1->bottomRight.y) {
		return true;
	}

	//Object2 bottomRight is inside Object 1
	if (object2->bottomRight.x > object1->topLeft.x && object2->bottomRight.y > object1->topLeft.y && object2->bottomRight.x < object1->bottomRight.x && object2->bottomRight.y < object1->bottomRight.y) {
		return true;
	}

	return false;
}

void CollisionManager::doCollision(PhysicsObject* object1, PhysicsObject* object2) {

	PhysicsObject* currentObject;
	PhysicsObject* otherObject;

	//fastest Object
	if (abs(object1->velocity.x) + abs(object1->velocity.y) > abs(object2->velocity.x) + abs(object2->velocity.y)) {
		currentObject = object1;
		otherObject = object2;
	}
	else {
		currentObject = object2;
		otherObject = object1;
	}
	
	//Damage
	float currentNewton = (abs(currentObject->velocity.x) + abs(currentObject->velocity.y)) * currentObject->mass;
	float otherNewton = (abs(otherObject->velocity.x) + abs(otherObject->velocity.y)) * otherObject->mass;

	currentObject->doDamage((int)(otherNewton / newtonDamage));
	otherObject->doDamage((int)(currentNewton / newtonDamage));

	//Velocity
	float totalXVelocity = currentObject->velocity.x + otherObject->velocity.x;
	float totalYVelocity = currentObject->velocity.y + otherObject->velocity.y;

	currentObject->removeInstantForce(watenk::Vector2(totalXVelocity * currentObject->mass * (collisionForceTransfer / 100), totalYVelocity * currentObject->mass * (collisionForceTransfer / 100)));
	otherObject->addInstantForce(watenk::Vector2(totalXVelocity * otherObject->mass * (collisionForceTransfer / 100), totalYVelocity * otherObject->mass * (collisionForceTransfer / 100)));

	//Pos
	if (abs(currentObject->velocity.x) > abs(currentObject->velocity.y)) {
		if (currentObject->velocity.x > 0) {
			otherObject->pos.x = currentObject->pos.x + currentObject->colliderSize.x + otherObject->colliderSize.x + collisionExtraDistance;
		}
		else if (currentObject->velocity.x < 0) {
			otherObject->pos.x = currentObject->pos.x - currentObject->colliderSize.x - otherObject->colliderSize.x - collisionExtraDistance;
		}
	}
	else {
		if (currentObject->velocity.y > 0) {
			otherObject->pos.y = currentObject->pos.y + currentObject->colliderSize.y + otherObject->colliderSize.y + collisionExtraDistance;
		}
		else if (currentObject->velocity.y < 0) {
			otherObject->pos.y = currentObject->pos.y - currentObject->colliderSize.y - otherObject->colliderSize.y - collisionExtraDistance;
		}
	}
}

float CollisionManager::getDistance(PhysicsObject* object1, PhysicsObject* object2) {

	watenk::Vector2 difference = object1->pos - object2->pos;
	return abs(difference.x + difference.y);
}

bool CollisionManager::listContains(PhysicsObject* object) {
	for (list<PhysicsObject*>::iterator it = calcedObjects.begin(); it != calcedObjects.end(); it++) {
		if (object == *it) {
			return true;
		}
	}
	return false;
}