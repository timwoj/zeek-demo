
#ifndef BRO_PLUGIN_DEMO_ALLCAPS
#define BRO_PLUGIN_DEMO_ALLCAPS

#include <plugin/Plugin.h>

namespace plugin {
namespace Demo_AllCaps {

class Plugin : public ::plugin::Plugin
{
protected:
	// Overridden from plugin::Plugin.
	plugin::Configuration Configure() override;
};

extern Plugin plugin;

}
}

#endif
