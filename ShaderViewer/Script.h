#ifndef _SCRIPT_H_
#define _SCRIPT_H_

class Time;

class Script {
public:
	Script();

	virtual void Start();
	virtual void Update(Time tick);
	virtual void Finish();
};

#endif // !_SCRIPT_H_
