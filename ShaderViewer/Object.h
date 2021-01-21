#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "TransformFixed.h"

class Engine;
class Time;

class Object {
public:
	Object(Engine &engine);

	virtual void Update(Time tick);

	TransformFixed transform;

protected:
	Engine* mEngine;
};

#endif // !_OBJECT_H_
