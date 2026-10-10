import os
import sys

sys.path.insert(0, os.path.abspath("src"))

from world import (
    World,
    load_object_data,
    load_map_in_editor,
    save_map,
    init_world_map,
    OBJECT_ID_STRAIGHT_ROAD,
    OBJECT_ID_LEFTCURVE_ROAD,
    OBJECT_ID_HOUSE,
)

from editor_logic import (
    EditorState,
    STRAIGHT,
    LEFT_HAND_CURVE,
    RIGHT_HAND_CURVE,
    T_JUNCTION,
    CROSSROAD,
)


def test_object_data_parsing():
    world = World()
    res = load_object_data("objects.dat", world)
    assert res is True
    assert len(world.obdata) > 0
    assert "straight_road" in world.object_names


def test_road_section_building_and_export():
    world = World()
    load_object_data("objects.dat", world)

    editor = EditorState(world)

    # Build road layout
    editor.draw_road_section(STRAIGHT)
    editor.draw_road_section(LEFT_HAND_CURVE)
    editor.draw_road_section(RIGHT_HAND_CURVE)
    editor.draw_road_section(T_JUNCTION)
    editor.draw_road_section(CROSSROAD)

    assert len(world.oblist) == 5

    # Test saving map
    out_map = "test_level.map"
    saved = save_map(out_map, world, editor.links_list)
    assert saved is True
    assert os.path.exists(out_map)
    assert os.path.exists("level1.cmp")

    # Test re-loading map
    world2 = World()
    editor2 = EditorState(world2)
    loaded = load_map_in_editor(out_map, world2, editor2.links_list)
    assert loaded is True
    assert len(world2.oblist) == 5

    # Cleanup
    if os.path.exists(out_map):
        os.remove(out_map)
    if os.path.exists("level1.cmp"):
        os.remove("level1.cmp")


def test_undo_and_delete_operations():
    world = World()
    load_object_data("objects.dat", world)
    editor = EditorState(world)

    editor.draw_road_section(STRAIGHT)
    editor.draw_road_section(LEFT_HAND_CURVE)
    assert len(world.oblist) == 2

    # Undo
    editor.undo_last_link()
    assert len(world.oblist) == 1

    # Place object
    editor.place_object_with_mouse(100.0, 100.0)
    assert len(world.oblist) == 2

    # Delete active object
    editor.delete_current_object()
    assert world.oblist[-1].inactive == 1


if __name__ == "__main__":
    test_object_data_parsing()
    test_road_section_building_and_export()
    test_undo_and_delete_operations()
    print("ALL INTEGRATION TESTS PASSED!")
