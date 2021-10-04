#pragma once

#include "DrawCommand.h"
#include "TextureManager.h"
#include <vector>

//class IDrawCommandProvider {
//    virtual DrawCommand getDrawCommand() = 0;
//};

typedef std::shared_ptr<DrawCommand> DrawCommandPtr;
typedef std::vector<DrawCommandPtr> DrawCommandVec;

class DrawBuffer {
public:
    DrawBuffer();
    ~DrawBuffer();

    void addCommand(DrawCommand* drawCommand);
    void submit(uint32_t order);

    [[maybe_unused]] void clear() noexcept;
    [[maybe_unused]] [[nodiscard]] unsigned int getSize() const noexcept;

    static void render(const RenderContext* renderContext) noexcept;

private:
    DrawCommandVec m_drawCommands;
};
