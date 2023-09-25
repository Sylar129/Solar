#pragma once

#include <vector>

#include "Core/Base/Layer.h"

namespace Solar {

class LayerStack {
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> overlay);
    void PopLayer(Ref<Layer> layer);
    void PopOverlay(Ref<Layer> overlay);

    auto begin() { return m_Layers.begin(); }
    auto begin() const
    {
        return m_Layers.begin();
    }

    auto end() { return m_Layers.end(); }
    auto end() const { return m_Layers.end(); }

    auto rbegin() { return m_Layers.rbegin(); }
    auto rbegin() const
    {
        return m_Layers.rbegin();
    }

    auto rend() { return m_Layers.rend(); }
    auto rend() const
    {
        return m_Layers.rend();
    }

private:
    std::vector<Ref<Layer>> m_Layers;
    unsigned int m_LayerInsertIndex;
};

} // namespace Solar
