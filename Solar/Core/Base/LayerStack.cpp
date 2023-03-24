#include "solpch.h"
#include "LayerStack.h"

namespace Solar {

LayerStack::LayerStack() : m_LayerInsertIndex(0) {}

LayerStack::~LayerStack()
{
    for (auto& layer : m_Layers) {
        layer->OnDetech();
    }
}

void LayerStack::PushLayer(Ref<Layer> layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Ref<Layer> overlay)
{
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Ref<Layer> layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
        layer->OnDetech();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PopOverlay(Ref<Layer> overlay)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (it != m_Layers.end()) {
        overlay->OnDetech();
        m_Layers.erase(it);
    }
}

} // namespace Solar
