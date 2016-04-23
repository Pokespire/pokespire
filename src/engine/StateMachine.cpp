#include "StateMachine.h"
#include "Logger.h"
#include "Timing.h"
#include "platform.h"
#include "Graphics.h"
#include "Status.h"
#include "render/Text.h"
#include "version.h"
#include "input/Input.h"

#if TARGET_SFML
#include <SFML/System.hpp>
#endif

using namespace WalrusRPG; /*::StateMachine*/
using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::Timing;
using namespace WalrusRPG::Logger;
using WalrusRPG::Input::Key;
using WalrusRPG::Input::KeyState;
using WalrusRPG::States::State;

namespace
{
#if TARGET_SFML
    float update_times[200] = {0};
    float render_times[200] = {0};
    bool show_logger = false;
    bool show_state_debug = false;
#endif

    /**
     * Debug function showing a button state.
     */
    void draw_button(signed x, signed y, KeyState state)
    {
        put_horizontal_line(x + 1, x + 5, y, Gray);
        put_horizontal_line(x + 1, x + 5, y + 6, Gray);
        put_vertical_line(x, y + 1, y + 5, Gray);
        put_vertical_line(x + 6, y + 1, y + 5, Gray);
        switch (state)
        {
            case KeyState::KS_RELEASED:
                put_rectangle({x + 1, y + 1, 5, 5}, White);
                break;
            case KeyState::KS_JUST_RELEASED:
                put_rectangle({x + 1, y + 1, 5, 5}, Magenta);
                break;
            case KeyState::KS_JUST_PRESSED:
                put_rectangle({x + 1, y + 1, 5, 5}, Cyan);
                break;
            case KeyState::KS_PRESSED:
                put_rectangle({x + 1, y + 1, 5, 5}, Black);
                break;
        }
    }
    /**
     * Draws WRPG's buttons states.
     */
    void draw_buttons()
    {
        draw_button(0, 24, key_get_state(Key::K_L));
        draw_button(56, 24, key_get_state(Key::K_R));

        draw_button(8, 32, key_get_state(Key::K_UP));
        draw_button(0, 40, key_get_state(Key::K_LEFT));
        draw_button(16, 40, key_get_state(Key::K_RIGHT));
        draw_button(8, 48, key_get_state(Key::K_DOWN));

        draw_button(28, 48, key_get_state(Key::K_SELECT));
        draw_button(36, 48, key_get_state(Key::K_START));

        draw_button(48, 44, key_get_state(Key::K_B));
        draw_button(56, 36, key_get_state(Key::K_A));
    }

    // State stack. Pointer because polymorphism.
    static tinystl::vector<WalrusRPG::States::State *> stack;

} /* namespace */

void StateMachine::init()
{
    log("State Machine init");
}

void StateMachine::deinit()
{
    log("State Machine deinit");
    stack.clear();
}

void StateMachine::push(State *state)
{
    stack.push_back(state);
}

void StateMachine::pop()
{
    // Mmmh, should StateMachine manage the state's destruction?...
    delete stack.back();
    stack.pop_back();
}

void StateMachine::run()
{
    const unsigned loop_time = TIMER_FREQ / 60;
    unsigned loop_next = loop_time;
    unsigned last_update = 0, update_stamp, update_time;
    unsigned last_frame = 0, frame_stamp, frame_time;

    // TODO : Better way to handle FPS while not breaking anything. There are some issues
    // if the update loop takes too much time.
    while (!stack.empty() && !Status::mustQuit())
    {
        Status::update();
        update_stamp = Timing::gettime();
        update_time = update_stamp - last_update;
        Input::key_poll();
        stack.back()->update(100 * update_time / TIMER_FREQ);
        last_update = update_stamp;
        if (Timing::gettime() < loop_next)
        {
            frame_stamp = Timing::gettime();
            frame_time = frame_stamp - last_frame;
            Graphics::frame_begin();
            // Update the current state
            stack.back()->render(100 * frame_time / TIMER_FREQ);
            last_frame = frame_stamp;

            // Text::print_format(0, 0, "WRPG build %s", git_version);
            if (frame_time != 0 && update_time != 0)
            {
                Text::print_format(0, 240 - 8, "%ufps, %uups", TIMER_FREQ / frame_time,
                                   TIMER_FREQ / update_time);
            }
// draw_buttons();

#if TARGET_SFML
            // Updating time graph
            float update_time = (frame_stamp - update_stamp) / 1000.;
            float render_time = (Timing::gettime() - frame_stamp) / 1000.;
            for (int i = 0; i < 200; ++i)
            {
                update_times[i] = update_times[i + 1];
                render_times[i] = render_times[i + 1];
            }
            update_times[199] = update_time;
            render_times[199] = render_time;
            // Drawing FPS and render graphs
            ImGui::BeginGroup();
            ImGui::Text("FPS: %.2f", TIMER_FREQ / (float) frame_time);
            ImGui::PlotLines("", update_times, 200, 0, "Update");
            ImGui::SameLine();
            ImGui::Text("%.2fms", update_time);
            ImGui::PlotLines("", render_times, 200, 0, "Render");
            ImGui::SameLine();
            ImGui::Text("%.2fms", render_time);
            ImGui::EndGroup();
            ImGui::Separator();
            // Drawing Window Toggles
            ImGui::Text("Toggles");
            ImGui::Indent();
            ImGui::BeginGroup();
            ImGui::Checkbox("Logger", &show_logger);
            ImGui::Checkbox("State", &show_state_debug);
            ImGui::Unindent();
            ImGui::EndGroup();
            if (show_logger)
                Logger::debug_render();
            if (show_state_debug)
                stack.back()->debug(100 * frame_time / TIMER_FREQ);
#endif

            Graphics::frame_end();
        }

#ifdef ACTIVE_WAIT
        while (Timing::gettime() < loop_next)
            ;
#endif
        loop_next += loop_time;
    }
}
