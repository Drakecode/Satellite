/*
 * Slow Motion Servo Library for Arduino
 *
 * Copyright Jean-Luc Béchennec 2015
 *
 * This software is distributed under the GNU Public Licence v2 (GPLv2)
 *
 * Please read the LICENCE file
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __SlowMotionServo_h__
#define __SlowMotionServo_h__

#include "Arduino.h"

class SlowMotionServo
{
	byte mPin;                  // connection pin
	byte mState : 3;              // state of the servo
	bool mDetachAtMin : 1;
	bool mDetachAtMax : 1;
	bool mReverted : 1;
	unsigned int mMinPulse;     // minimum position of the servo in microseconds
	unsigned int mMaxPulse;     // maximum position of the servo in microseconds
	unsigned long mStartTime;   // time when the movement begin
	float mInitialRelativeTime; // starting position-time of the servo
	float mTargetRelativeTime;  // target position-time of the servo
	float mCurrentRelativeTime; // current position-time of the servo
	float mTimeFactorUp;        // time factor of a millis for movement from min to max
	float mTimeFactorDown;      // time factor of a millis for movement from max to min
  
public:
  SlowMotionServo() {}
  SlowMotionServo(byte pin) {}
  void setPin(const byte pin) { }
  void setMinMax(unsigned int minPulse, unsigned int maxPulse) {}
	void setMin(unsigned int minPulse) { mMinPulse = minPulse; }
  void setMax(unsigned int maxPulse) { mMaxPulse = maxPulse; }
  void setReverted(const bool inReverted) {}
  void setMinToMaxSpeed(const float speed) { mTimeFactorUp = speed / 10000.0f; }
  void setMaxToMinSpeed(const float speed) { mTimeFactorDown = speed / 10000.0f; }
  void setSpeed(const float speed) { mTimeFactorUp = mTimeFactorDown = speed / 10000.0f; }
  void setInitialPosition(float position) {}
  void setDetachAtMin(bool detach) {}
  void setDetachAtMax(bool detach) {}
  void setDetach(bool detach) {}
  void goTo(float position) {}
  void goToMin() {}
  void goToMax() {}
  bool isStopped() { return true; }

	/*
	 * Live configuration functions.
	 */
	void setupMin(uint16_t minPulse) {}
	void setupMax(uint16_t maxPulse) {}
	void adjustMin(int16_t increment) {}
	void adjustMax(int16_t increment) {}
	void endSetup() {}
	
	byte pin() { return mPin; }
  bool detachAtMin() { return mDetachAtMin; }
  bool detachAtMax() { return mDetachAtMax; }
  unsigned int minimumPulse() { return mMinPulse; }
  unsigned int maximumPulse() { return mMaxPulse; }
  float minToMaxSpeed() { return 10000.0f * mTimeFactorUp; }
  float maxToMinSpeed() { return 10000.0f * mTimeFactorDown; }
  bool isReverted() { return mReverted; }

  virtual float slopeUp(float time) = 0;
  virtual float slopeDown(float time) = 0;

	static void update() {}
	static void setDelayUntilStop(unsigned int delayUntilStop) {}
};

class SMSLinear : public SlowMotionServo
{
public:
  virtual float slopeUp(float time)  { return 0; }
  virtual float slopeDown(float time) { return 0; }
  float slope(float time) { return 0; }
};

class SMSSmooth : public SlowMotionServo
{
public:
  virtual float slopeUp(float time) { return 0; }
  virtual float slopeDown(float time) { return 0; }
  float slope(float time) { return 0; }
};

class SMSSmoothBounce : public SlowMotionServo
{
public:
  virtual float slopeUp(float time) { return 0; }
  virtual float slopeDown(float time) { return 0; }
};

#endif
