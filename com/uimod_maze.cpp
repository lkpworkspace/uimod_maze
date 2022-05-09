#include <cmath>
#include <memory>
#include <functional>
#include <unordered_map>

#include "SFEPanel.hpp"
#include "bpbase.pb.h"

#include "maze_view.hpp"

typedef ::google::protobuf::Message pb_msg_t;
typedef std::shared_ptr<pb_msg_t> pb_msg_ptr_t;

static std::unordered_map<std::string, std::function<pb_msg_ptr_t()>> g_msg_map = {
    // {"bpmath.String", [](){ return std::make_shared<bp::String>(); }}
};
static maze::MazeView g_maze;

static void update_pixels() {
    auto pos = g_maze.GetWH();
    auto data = g_maze.GetMat();
    Json::Value v;
    v["command"] = "draw";
    v["width"] = pos.first;
    v["height"] = pos.second;
    for (int i = 0; i < data.size(); ++i) {
        v["pixels"].append(data[i]);
    }
    sfe::SFEPanel::SendMessage("uimod_maze", "bitmap", v);
}

pb_msg_ptr_t create_msg(const std::string& msg_name) {
    if (g_msg_map.find(msg_name) == g_msg_map.end()) {
        return nullptr;
    }
    return g_msg_map[msg_name]();
}

pb_msg_ptr_t maze_init() {
    auto res = std::make_shared<bp::Bool>();
    res->set_var(true);

    g_maze.Init(4,4);
    g_maze.Set({
        1, 1, 1, 1,
        1, 0, 0, 0,
        0, 0, 1, 1,
        1, 1, 1, 1,
    }, std::make_pair(0, 2), std::make_pair(3, 1));

    update_pixels();
    return res;
}

pb_msg_ptr_t move_left() {
    auto res = std::make_shared<bp::Bool>();
    res->set_var(true);
    g_maze.Move(maze::MoveDir::MV_LEFT);
    update_pixels();
    return res;
}

pb_msg_ptr_t move_right() {
    auto res = std::make_shared<bp::Bool>();
    res->set_var(true);
    g_maze.Move(maze::MoveDir::MV_RIGHT);
    update_pixels();
    return res;
}

pb_msg_ptr_t move_up() {
    auto res = std::make_shared<bp::Bool>();
    res->set_var(true);
    g_maze.Move(maze::MoveDir::MV_UP);
    update_pixels();
    return res;
}

pb_msg_ptr_t move_down() {
    auto res = std::make_shared<bp::Bool>();
    res->set_var(true);
    g_maze.Move(maze::MoveDir::MV_DOWN);
    update_pixels();
    return res;
}

