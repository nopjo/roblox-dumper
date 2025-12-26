#include "scanner.hpp"
#include "phases/atmosphere.hpp"
#include "phases/bloom_effect.hpp"
#include "phases/camera.hpp"
#include "phases/color_correction_effect.hpp"
#include "phases/data_model.hpp"
#include "phases/depth_of_field_effect.hpp"
#include "phases/gui_object.hpp"
#include "phases/highlight.hpp"
#include "phases/humanoid.hpp"
#include "phases/instance.hpp"
#include "phases/lighting.hpp"
#include "phases/mesh.hpp"
#include "phases/mouse_service.hpp"
#include "phases/part.hpp"
#include "phases/player.hpp"
#include "phases/proximity_prompt.hpp"
#include "phases/script.hpp"
#include "phases/sky.hpp"
#include "phases/special_mesh.hpp"
#include "phases/sun_rays_effect.hpp"
#include "phases/team.hpp"
#include "phases/terrain.hpp"
#include "phases/text_label.hpp"
#include "phases/tool.hpp"
#include "phases/ui_gradient.hpp"
#include "phases/value.hpp"
#include "phases/visual_engine.hpp"
#include "phases/workspace.hpp"

namespace scanner {

    bool run_all_phases() {
        Context ctx;
        PhaseRegistry registry;

        registry.register_phase("VisualEngine", phases::visual_engine);
        registry.register_phase("DataModel", phases::data_model);
        registry.register_phase("Instance", phases::instance);
        registry.register_phase("Workspace", phases::workspace);
        registry.register_phase("Player", phases::player);
        registry.register_phase("Team", phases::team);
        registry.register_phase("Part", phases::part);
        registry.register_phase("Humanoid", phases::humanoid);
        registry.register_phase("Mesh", phases::mesh);
        registry.register_phase("Value", phases::value);
        registry.register_phase("SpecialMesh", phases::special_mesh);
        registry.register_phase("Lighting", phases::lighting);
        registry.register_phase("Sky", phases::sky);
        registry.register_phase("Atmosphere", phases::atmosphere);
        registry.register_phase("BloomEffect", phases::bloom_effect);
        registry.register_phase("SunRaysEffect", phases::sun_rays_effect);
        registry.register_phase("ColorCorrectionEffect", phases::color_correction_effect);
        registry.register_phase("DepthOfFieldEffect", phases::depth_of_field_effect);
        registry.register_phase("MouseService", phases::mouse_service);
        registry.register_phase("UIGradient", phases::ui_gradient);
        registry.register_phase("GuiObject", phases::gui_object);
        registry.register_phase("TextLabel", phases::text_label);
        registry.register_phase("Tool", phases::tool);
        registry.register_phase("Highlight", phases::highlight);
        registry.register_phase("ProximityPrompt", phases::proximity_prompt);
        registry.register_phase("Script", phases::script);
        registry.register_phase("Terrain", phases::terrain);

        // this phase also gets view matrix for VisualEngine
        registry.register_phase("Camera", phases::camera);

        return registry.run_all(ctx);
    }

} // namespace scanner
