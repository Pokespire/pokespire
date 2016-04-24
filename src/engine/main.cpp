#include "StateMachine.h"
#include "Timing.h"
#include "Logger.h"
#include "render/Text.h"
#include "Graphics.h"
#include "Quirks.h"
#include "Status.h"
#include "map/Map.h"
#include "map/StateMap.h"
#include "piaf/Archive.h"
#include "utility/misc.h"

using namespace WalrusRPG;
using WalrusRPG::PIAF::Archive;
using WalrusRPG::Graphics::Texture;
using namespace WalrusRPG::Graphics;


int main(int argc, char *argv[])
{
    UNUSED(argc);
    Graphics::init();
    Logger::init();
    Logger::log("WalrusRPG Init");
    Logger::debug("This is a debug line");
    Logger::warn("Warning color!");
    Logger::error("/!\\This shouldn't have happened!");
    Status::init();
    Timing::init();
    Quirks::init(argv[0]);

    Text::init();
    Archive arc("data/wip_data.wrf");
    Texture tex(arc.get("ov.png"));
    WalrusRPG::PIAF::File f1 = arc.get("l1.bin");
    WalrusRPG::PIAF::File f2 = arc.get("l2.bin");

    const uint8_t *l1 = f1.get();
    const uint8_t *l2 = f2.get();

    // TODO better map reading.
    uint16_t *dungeonTest = new uint16_t[f1.file_size / 2 + 1];
    uint16_t *dungeonTest2 = new uint16_t[f1.file_size / 2 + 1];

    for (unsigned i = 0; i < f1.file_size / 2; i++)
    {
        dungeonTest[i] = read_big_endian_value<uint16_t>(&l1[i * 2]);
        dungeonTest2[i] = read_big_endian_value<uint16_t>(&l2[i * 2]);
    }

    Map map(20, 20, dungeonTest, dungeonTest2, tex);
    tinystl::vector<Frame> stripe21;
    tinystl::vector<Frame> stripe22;
    stripe21.push_back({21, 23});
    stripe21.push_back({22, 31});
    stripe22.push_back({22, 37});
    stripe22.push_back({21, 41});
    map.anim.add_animation(21, {stripe21, true, 0});
    map.anim.add_animation(22, {stripe22, true, 0});

    StateMachine::init();
    StateMachine::push(new States::StateMap(0, 0, map));
    StateMachine::run();

    Logger::log("WalrusRPG Deinit");
    StateMachine::deinit();
    Text::deinit();

    Quirks::deinit();
    Timing::deinit();
    Status::deinit();
    delete[] dungeonTest;
    delete[] dungeonTest2;
    Logger::log("WalrusRPG Exit");
    Graphics::deinit();

    return 0;
}
