#include <lilv/lilv.h>
#include <iostream>

int main() {
    LilvWorld* world = lilv_world_new();
    lilv_world_load_all(world);
    
    const LilvPlugins* plugins = lilv_world_get_all_plugins(world);
    size_t plugin_count = lilv_plugins_size(plugins);

    std::cout << "Number of LV2 Plugins Found: " << plugin_count << std::endl;

    // List plugin URIs
    if (plugin_count > 0) {
        LILV_FOREACH(plugins, i, plugins) {
            const LilvPlugin* plugin = lilv_plugins_get(plugins, i);
            std::cout << "Plugin URI: " << lilv_node_as_string(lilv_plugin_get_uri(plugin)) << std::endl;
        }
    }

    lilv_world_free(world);
    return 0;
}
