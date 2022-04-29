#pragma once
#include <stdint.h>
#include <vector>
#include <algorithm>

#include <jsoncpp/json/json.h>

namespace maze {

const uint32_t WALL_COLOR = 0xffffffff;
const uint32_t LEAD_COLOR = 0xffff00ff;
const uint32_t MOVE_COLOR = 0xff00ffff;
const uint32_t MOVED_COLOR = 0xff00ffff;
const uint32_t ROAD_COLOR = 0x0;
const uint32_t OUT_COLOR = 0xff00ffff;

enum class MoveDir : int {
    MV_LEFT,
    MV_RIGHT,
    MV_UP,
    MV_DOWN,
};

enum class MoveState : int {
    MV_NO,
    MV_YES,
    MV_OUT,
};

class MazeView {
public:
    void Init(int width, int height) {
        _width = width;
        _height = height;
        _mat_color.resize(width * height);
        _mat_flag.resize(width * height);
        std::fill(_mat_color.begin(), _mat_color.end(), 0);
        std::fill(_mat_flag.begin(), _mat_flag.end(), 0);
    }

    bool Set(const std::vector<uint32_t>& mat, const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) {
        if (_mat_color.size() != mat.size()) {
            return false;
        }
        _mat_flag = mat;
        _start = start_pos;
        _end = end_pos;
        for (int i = 0; i < _mat_flag.size(); ++i) {
            if (_mat_flag[i]) {
                _mat_color[i] = WALL_COLOR;
            }
        }

        _mat_color[end_pos.second * _width + end_pos.first] = OUT_COLOR;
        _mat_color[start_pos.second * _width + start_pos.first] = LEAD_COLOR;
        return true;
    }

    MoveState Move(MoveDir dir) {
        auto pos = _lead;
        switch (dir)
        {
        case MoveDir::MV_LEFT:
            pos.first += 1;
            break;
        case MoveDir::MV_RIGHT:
            pos.first -= 1;
            break;
        case MoveDir::MV_UP:
            pos.second -= 1;
            break;
        case MoveDir::MV_DOWN:
            pos.second += 1;
            break;
        }
        if (pos.first < 0 || pos.first >= _width || pos.second < 0 || pos.second >= _height) {
            return MoveState::MV_NO;
        }
        if (_mat_flag[pos.second * _width + pos.first]) {
            return MoveState::MV_NO;
        }
        _mat_color[_lead.second * _width + _lead.first] = ROAD_COLOR;
        _mat_color[pos.second * _width + pos.first] = LEAD_COLOR;
        if (pos == _end) {
            return MoveState::MV_OUT;
        }
        _lead = pos;
        return MoveState::MV_YES;
    }

    const std::pair<int, int> GetWH() {
        return std::make_pair(_width, _height);
    }

    const std::vector<uint32_t>& GetMat() {
        return _mat_color;
    }

private:
    int _width = 0;
    int _height = 0;
    std::pair<int, int> _start;
    std::pair<int, int> _end;
    std::pair<int, int> _lead;
    std::vector<uint32_t> _mat_flag;
    std::vector<uint32_t> _mat_color;

};

};