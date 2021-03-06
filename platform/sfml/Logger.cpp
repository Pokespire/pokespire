#include "Logger.h"
#include "TINYSTL/vector.h"
#include <algorithm>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#ifdef IMGUI
#include "imgui.h"
#endif


using namespace WalrusRPG;

namespace
{
    tinystl::vector<char *> log_list;
    bool show_log;
    bool show_debug = true;
    bool show_warn = true;
    bool show_error = true;
    char log_buffer[1024];
    int offset = 0;

    void prepare_buffer()
    {
        memset(log_buffer, 0, sizeof(log_buffer));
        offset = 0;
    }


    void append(char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        offset += vsnprintf(log_buffer + offset, 1024 - offset, fmt, args);
        va_end(args);
    }

    void print_timestamp_and_tag(const char *type)
    {
        time_t now = time(0);
        offset += strftime(log_buffer, 256, "%H:%M:%S ", localtime(&now));
        append((char *) "%-7s : ", type);
    }

    void append_to_log_console()
    {
        char *new_buf = new char[offset];
        strncpy(new_buf, log_buffer, offset);
        if (log_list.size() > 128)
        {
            char *pop = log_list[0];
            log_list.erase(log_list.begin(), log_list.begin() + 1);
            delete pop;
        }
        log_list.push_back(new_buf);
    }
}

// NOTE : I really wish there would be a better way to handle these stupid va_lists. So
// much redundant code...

void Logger::init()
{
}

void Logger::deinit()
{}

void Logger::log(const char *fmt, ...)
{
    prepare_buffer();
    print_timestamp_and_tag("[LOG]");
    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(log_buffer + offset, 1024, fmt, args) + offset;
    puts(log_buffer);
    va_end(args);
    append_to_log_console();
}

void Logger::debug(const char *fmt, ...)
{
    prepare_buffer();
    print_timestamp_and_tag("[DEBUG]");
    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(log_buffer + offset, 1024, fmt, args) + offset;
    puts(log_buffer);
    va_end(args);
    append_to_log_console();
}

void Logger::warn(const char *fmt, ...)
{
    prepare_buffer();
    print_timestamp_and_tag("[WARN]");
    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(log_buffer + offset, 1024, fmt, args) + offset;
    puts(log_buffer);
    va_end(args);
    append_to_log_console();
}

void Logger::error(const char *fmt, ...)
{
    prepare_buffer();
    print_timestamp_and_tag("[ERROR]");
    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(log_buffer + offset, 1024, fmt, args) + offset;
    puts(log_buffer);
    va_end(args);
    append_to_log_console();
}


#ifdef IMGUI
void Logger::debug_render()
{
    if (!ImGui::Begin("Logger"))
        return;
    ImGui::Checkbox("Log", &show_log);
    ImGui::SameLine();
    ImGui::Checkbox("Debug", &show_debug);
    ImGui::SameLine();
    ImGui::Checkbox("Warn", &show_warn);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &show_error);
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false,
                      ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    for (unsigned i = 0; i < log_list.size(); i++)
    {
        const char *item = log_list[i];
        ImVec4 col = ImColor(255, 255, 255); // A better implementation may store a type
                                             // per-item. For the sample let's just parse
                                             // the text.
        if (strstr(item, "[LOG]"))
        {
            if (!show_log)
                continue;
            col = ImColor(128, 128, 128);
        }
        if (strstr(item, "[DEBUG]"))
        {
            if (!show_debug)
                continue;
            col = ImColor(168, 168, 224);
        }
        if (strstr(item, "[WARN]"))
        {
            if (!show_warn)
                continue;
            col = ImColor(224, 224, 64);
        }
        if (strstr(item, "[ERROR]"))
        {
            if (!show_error)
                continue;
            col = ImColor(224, 64, 64);
        }
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::TextUnformatted(item);
        ImGui::PopStyleColor();
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::End();
}
#endif