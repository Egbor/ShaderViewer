#include "Time.h"

#include <Windows.h>

Time::Time() 
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mPauseTime(0), mPrevTime(0), mCurrTime(0), mStopTime(0), mStopped(false) {
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0f / (double)countsPerSec;
}

float Time::DeltaTime() const {
	return (float)mDeltaTime;
}

float Time::TotalTime() const {
	if (mStopped) {
		return (float)(((mStopTime - mPauseTime) - mBaseTime) * mSecondsPerCount);
	} else {
		return (float)(((mCurrTime - mPauseTime) - mBaseTime) * mSecondsPerCount);
	}
}

void Time::Reset() {
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void Time::Stop() {
	if (!mStopped) {
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

void Time::Start() {
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped) {
		mPauseTime += (startTime - mStopTime);
		
		mPrevTime = mPauseTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void Time::Tick() {
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0) {
		mDeltaTime = 0.0;
	}
}