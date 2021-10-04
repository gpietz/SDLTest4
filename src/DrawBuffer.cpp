#include "DrawBuffer.h"
#include <tuple>
#include <algorithm>
#include <vector>
#include <cassert>

typedef std::tuple<uint32_t, DrawCommandVec> DrawCommandEntry;
typedef std::vector<DrawCommandEntry> DrawCommandEntries;
static DrawCommandEntries g_DrawCommands;

DrawBuffer::DrawBuffer() = default;

DrawBuffer::~DrawBuffer() = default;

void DrawBuffer::addCommand(DrawCommand* drawCommand) {
    assert(drawCommand != nullptr);
    if (drawCommand != nullptr) {
        m_drawCommands.push_back(DrawCommandPtr(drawCommand));
    }
}

void DrawBuffer::submit(uint32_t order) {
    if (m_drawCommands.empty()) {
        return;
    }

    DrawCommandVec batch = DrawCommandVec();
    while (!m_drawCommands.empty()) {
        const auto it = m_drawCommands.begin();
        batch.push_back(std::move(*it));
        m_drawCommands.erase(it);
    }

    g_DrawCommands.push_back(DrawCommandEntry(order, batch));
}

[[maybe_unused]] void DrawBuffer::clear() noexcept {
    m_drawCommands.clear();
}

[[maybe_unused]] uint32_t DrawBuffer::getSize() const noexcept {
    return m_drawCommands.size();
}

void DrawBuffer::render(const RenderContext* renderContext) noexcept {
    // sort drawbuffer by z-order
    std::sort(g_DrawCommands.begin(), g_DrawCommands.end(), [](const DrawCommandEntry& a, const DrawCommandEntry& b) {
        return std::get<0>(a) < std::get<0>(b);
    });

    // render draw commands
    for (auto entry : g_DrawCommands) {
        auto commands = std::get<1>(entry);
        for (auto & command : commands) {
            command->render(renderContext);
        }
    }

    // clear drawbuffer
    g_DrawCommands.clear();

    // update backbuffer
    SDL_RenderPresent(renderContext->m_renderer);
}
