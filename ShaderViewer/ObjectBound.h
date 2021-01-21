#ifndef _OBJECT_BOUND_H_
#define _OBJECT_BOUND_H_

#include "Script.h"
#include "Object.h"

class ObjectBound : public Script {
public:
	ObjectBound(Object &object);

	void Update(Time tick);

private:
	Object* mObject;
};


#endif // !1
