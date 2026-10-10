import os
import sys
sys.path.insert(0, os.path.abspath("src"))

from world import (
    World,
    load_object_data,
    load_map_in_editor,
    save_map,
    OBJECT_ID_STRAIGHT_ROAD,
    OBJECT_ID_LEFTCURVE_ROAD,
    Link,
    ObjectListEntry
)

def test_world_loading_and_saving():
    world = World()
    links = []

    # 1. Test load_object_data
    success = load_object_data("objects.dat", world)
    assert success, "load_object_data failed"
    assert len(world.obdata) > 0, "No object data loaded"
    print(f"Loaded {len(world.obdata)} object definitions.")

    # 2. Populate world with test objects
    obj1 = ObjectListEntry()
    obj1.name = "straight_road"
    obj1.type = OBJECT_ID_STRAIGHT_ROAD
    obj1.x = 600.0
    obj1.y = 0.0
    obj1.z = 600.0
    obj1.rot_angle = 0.0
    world.oblist.append(obj1)

    obj2 = ObjectListEntry()
    obj2.name = "left_curve_road"
    obj2.type = OBJECT_ID_LEFTCURVE_ROAD
    obj2.x = 600.0
    obj2.y = 0.0
    obj2.z = 720.0
    obj2.rot_angle = 15.0
    world.oblist.append(obj2)

    links.append(Link(600.0, 600.0, 0.0, active=True, inactive=0, objectid=0))
    links.append(Link(600.0, 720.0, 15.0, active=True, inactive=0, objectid=1))

    # 3. Test saving map and compiled map
    test_map_file = "test_output.map"
    save_success = save_map(test_map_file, world, links)
    assert save_success, "save_map failed"
    assert os.path.exists(test_map_file), "test_output.map was not created"
    assert os.path.exists("level1.cmp"), "level1.cmp was not created"
    print("Saved map file successfully.")

    # 4. Test loading saved map
    world2 = World()
    links2 = []
    load_success = load_map_in_editor(test_map_file, world2, links2)
    assert load_success, "load_map_in_editor failed"
    assert len(world2.oblist) == 2, f"Expected 2 objects, got {len(world2.oblist)}"
    assert len(links2) == 2, f"Expected 2 links, got {len(links2)}"
    assert world2.oblist[0].name == "straight_road"
    assert world2.oblist[1].name == "left_curve_road"
    print("Loaded saved map file successfully!")

    # Cleanup
    if os.path.exists(test_map_file):
        os.remove(test_map_file)
    if os.path.exists("level1.cmp"):
        os.remove("level1.cmp")

if __name__ == "__main__":
    test_world_loading_and_saving()
    print("ALL WORLD TESTS PASSED!")
