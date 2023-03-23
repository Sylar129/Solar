#pragma once

#include "Core/Base/Core.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"

namespace Solar {

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

    Entity GetSelectedEntity() const { return m_SelectionContext; }
    void SetSelectedEntity(Entity entity);
private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);
private:
    Ref<Scene> m_Context;
    Entity m_SelectionContext;
};

} // namespace Solar
