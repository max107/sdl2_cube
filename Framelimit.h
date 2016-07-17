//
// Created by max on 29.06.16.
//

#ifndef FASTCRAFT_FRAMELIMIT_H
#define FASTCRAFT_FRAMELIMIT_H

#include <SDL2/SDL.h>

class FpsManager {

public:
    FpsManager(float fps_max) {
        // The +0.00001 on _max_frame_ticks is to offset the losses from rounding Otherwise we get 61/60 fps
        _max_frame_ticks = (1000.0 / fps_max) + 0.00001;
        _frame_count = 0;
        _last_second_ticks = SDL_GetTicks();
    }

    Uint32 getTicks() {
        return SDL_GetTicks();
    }

    void sleep(Uint32 sleep) {
        SDL_Delay(sleep);
    }

    bool limit() {
        _frame_count++;
        _target_ticks = _last_second_ticks + Uint32(_frame_count * _max_frame_ticks);
        _current_ticks = getTicks();

        _average_ticks += _current_ticks - _last_frame_ticks;
        if (_current_ticks - _last_frame_ticks <= _min_ticks) {
            _min_ticks = _current_ticks - _last_frame_ticks;
        }
        if (_current_ticks - _last_frame_ticks >= _max_frame_ticks) {
            _max_ticks = _current_ticks - _last_frame_ticks;
        }

        if (_current_ticks < _target_ticks) {
            sleep(_target_ticks - _current_ticks);
            _current_ticks = getTicks();
        }

        _last_frame_ticks = _current_ticks;

        if (_current_ticks - _last_second_ticks >= 1000) {
            // Setup variables for read
            _fps = _frame_count;
            _frame_average = _average_ticks / _frame_count;
            _frame_min = _min_ticks;
            _frame_max = _max_ticks;

            _frame_count = 0;
            _min_ticks = 1000;
            _max_ticks = 0;
            _average_ticks = 0;
            _last_second_ticks = getTicks();
            return true;
        }

        return false;
    }

    Uint32 getFrameMin() {
        return _frame_min;
    }

    Uint32 getFrameMax() {
        return _frame_max;
    }

    double getFrameAverage() {
        return _frame_average;
    }

    int getFps() {
        return _fps;
    }

private:
    Uint32 _frame_min;
    Uint32 _frame_max;
    double _frame_average;
    int _fps;

    float _max_frame_ticks;
    Uint32 _last_second_ticks;
    int _frame_count;

    Uint32 _min_ticks;
    Uint32 _max_ticks;
    double _average_ticks;
    Uint32 _last_frame_ticks;

    Uint32 _current_ticks;
    Uint32 _target_ticks;
};

#endif //FASTCRAFT_FRAMELIMIT_H
