// Made by Eric Schneider and Forrest Bourke for Elecanisms 2015

#include <p24FJ128GB206.h>
#include <math.h>
#include "common.h"
#include "control_tools.h"
#include "pin.h"

_FLIP_TRACKER FlipTracker;

void init_flip_tracking() {
    FlipTracker.rawPos = 0;
    FlipTracker.lastRawPos = 0;
    FlipTracker.lastLastRawPos = 0;
    FlipTracker.flipNumber = 0;
    FlipTracker.rawDiff = 0;
    FlipTracker.lastRawDiff = 0;
    FlipTracker.rawOffset = 0;
    FlipTracker.lastRawOffset = 0;
    FlipTracker.flipped = 0;
    pin_analogIn(&A[3]);
    FlipTracker.lastLastRawPos = pin_read(&A[3]) >> 6;
    FlipTracker.lastRawPos = pin_read(&A[3]) >> 6;
}

int get_flips() {
    return FlipTracker.flipNumber;
}

void track_flips(_TIMER *self) {
    // Get voltage output by MR sensor
    FlipTracker.rawPos = (int) (pin_read(&A[3]) >> 6);

    // Calculate differences between subsequent MR sensor readings
    FlipTracker.rawDiff = FlipTracker.rawPos - FlipTracker.lastRawPos;
    FlipTracker.lastRawDiff = FlipTracker.rawPos - FlipTracker.lastLastRawPos;
    FlipTracker.rawOffset = abs(FlipTracker.rawDiff);
    FlipTracker.lastRawOffset = abs(FlipTracker.lastRawDiff);

    // Update position record-keeping vairables
    FlipTracker.lastLastRawPos = FlipTracker.lastRawPos;
    FlipTracker.lastRawPos = FlipTracker.rawPos;

    // Keep track of flips over 180 degrees
    if((FlipTracker.lastRawOffset > FLIP_THRESHOLD) && (FlipTracker.flipped == 0)) { // enter this anytime the last offset is greater than the flip threshold AND it has not just flipped
        if (FlipTracker.lastRawDiff > 0) { FlipTracker.flipNumber--; }
        else { FlipTracker.flipNumber++; }
        FlipTracker.flipped = 1;
    }
    else { FlipTracker.flipped = 0; }
}
