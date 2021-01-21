#include "ObjectBound.h"

#include "Time.h"

ObjectBound::ObjectBound(Object &object) : mObject(&object) {

}

void ObjectBound::Update(Time tick) {
	mObject->transform.Translate(0.0f, sinf(tick.DeltaTime()), 0.0f);
}