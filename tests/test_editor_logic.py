import os
import sys
sys.path.insert(0, os.path.abspath("src"))

from world import World, load_object_data
from editor_logic import (
    EditorState,
    STRAIGHT,
    LEFT_HAND_CURVE,
    RIGHT_HAND_CURVE,
    T_JUNCTION,
    CROSSROAD
)

def test_editor_logic():
    world = World()
    load_object_data("objects.dat", world)

    editor = EditorState(world)
    assert len(editor.world.oblist) == 0
    assert len(editor.links_list) == 0

    # Test adding straight road
    editor.draw_road_section(STRAIGHT)
    assert len(editor.world.oblist) == 1
    assert editor.world.oblist[0].name == "straight_road"
    assert len(editor.links_list) == 1
    print("Straight road added successfully.")

    # Test adding left curve
    editor.draw_road_section(LEFT_HAND_CURVE)
    assert len(editor.world.oblist) == 2
    assert editor.world.oblist[1].name == "left_curve_road"
    assert editor.last_angle == 15.0
    print("Left curve added successfully.")

    # Test T-junction (creates multiple links)
    editor.draw_road_section(T_JUNCTION)
    assert len(editor.world.oblist) == 3
    assert editor.world.oblist[2].name == "t_junction"
    assert len(editor.links_list) == 4
    print("T-junction added successfully.")

    # Test undo
    editor.undo_last_link()
    assert len(editor.world.oblist) == 2
    print("Undo executed successfully.")

    # Test mouse placement of an object
    editor.current_object_id = 8
    editor.current_object_name = "house_semileft"
    editor.object_rot_angle = 90
    editor.place_object_with_mouse(103.5, 205.2)

    assert len(editor.world.oblist) == 3
    placed = editor.world.oblist[-1]
    assert placed.x == 100.0  # Snapped to grid 20
    assert placed.z == 200.0
    assert placed.rot_angle == 90.0
    assert placed.name == "house_semileft"
    print("Mouse placement executed successfully.")

    # Test set_link_with_mouse
    editor.set_link_with_mouse(100.0, 200.0)
    assert editor.last_x == 100.0
    assert editor.last_z == 200.0
    print("Link selection executed successfully.")

    # Test delete current object
    editor.delete_current_object()
    assert editor.world.oblist[-1].inactive == 1
    print("Object deletion executed successfully.")

if __name__ == "__main__":
    test_editor_logic()
    print("ALL EDITOR LOGIC TESTS PASSED!")
