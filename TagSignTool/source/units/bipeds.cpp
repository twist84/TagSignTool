#include "units/bipeds.hpp"

#include "units/biped_definitions.hpp"

void modify_biped_instance_callback(cache_file_tag_instance* instance)
{
	if (!instance)
		return;

	if (instance->tag_group != BIPED_TAG)
		return;

	_biped_definition* biped = instance->cast_to<_biped_definition>();

	// "edge drop" fix
	biped->physics.ground_physics.scale_ground_adhesion_velocity = 30.0f / 60; // 0.5f
}

