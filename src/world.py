import os
import math

# Object ID Constants
OBJECT_ID_STRAIGHT_ROAD = 0
OBJECT_ID_LEFTCURVE_ROAD = 1
OBJECT_ID_RIGHTCURVE_ROAD = 2
OBJECT_ID_TJUNCTION = 3
OBJECT_ID_CROSSROADS = 4
OBJECT_ID_ROUNDABOUT = 5
OBJECT_ID_LAMP_POST = 6
OBJECT_ID_LAMP = 7
OBJECT_ID_HOUSE = 8
OBJECT_ID_POLICE_CAR = 9
OBJECT_ID_ROAD_SIGN = 10
OBJECT_ID_SMALL_STRAIGHT_ROAD = 11
OBJECT_ID_LEFTCORNER_ROAD = 12
OBJECT_ID_RIGHTCORNER_ROAD = 13
OBJECT_ID_LEFT_TJUNCTION = 14
OBJECT_ID_RIGHT_TJUNCTION = 15
OBJECT_ID_ZEBRA = 16
OBJECT_ID_ISLAND = 17
OBJECT_ID_HOUSE_END = 18
OBJECT_ID_PETROL_STATION = 19
OBJECT_ID_TRAFFIC_LIGHT = 20
OBJECT_ID_SLOPE = 21
OBJECT_ID_SLOPE_CORNER = 22
OBJECT_ID_CENTRAL_RESERVATION = 23


class Vert:
    def __init__(self, x=0.0, y=0.0, z=0.0):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

    def __repr__(self):
        return f"Vert({self.x}, {self.y}, {self.z})"


class ObjectListEntry:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
        self.rot_angle = 0.0
        self.type = 0
        self.light = -1
        self.inactive = 0
        self.monsterid = 0
        self.monstertexture = 0
        self.monstername = ""
        self.name = ""
        self.rcolour = 0.0
        self.gcolour = 0.0
        self.bcolour = 0.0
        self.dirx = 0.0
        self.diry = -1.0
        self.dirz = 0.0
        self.ltype = 0
        self.ability = 0
        self.ctext = ""


class ObjectDataEntry:
    def __init__(self):
        self.v = []  # list of Vert
        self.connection = [Vert() for _ in range(4)]  # 4 connection points
        self.tex = []  # texture index per polygon
        self.num_vert = []  # number of vertices per polygon (3 or 4)


class Link:
    def __init__(self, x=0.0, z=0.0, angle=0.0, active=True, inactive=0, objectid=0):
        self.last_x = float(x)
        self.last_z = float(z)
        self.last_angle = float(angle)
        self.active_link_flag = bool(active)
        self.inactive = int(inactive)
        self.objectid = int(objectid)


class World:
    def __init__(self):
        self.oblist = []  # list of ObjectListEntry
        self.obdata = {}  # dict mapping int object_id -> ObjectDataEntry
        self.num_vert_per_object = {}
        self.num_polys_per_object = {}
        self.object_names = {}  # dict mapping name -> id
        self.id_to_name = {}  # dict mapping id -> name
        self.texture_names = []  # list of texture name strings
        self.model_names = []  # list of model dicts {"name": str, "num": int}
        self.cell = {}  # (cx, cz) -> list of object indices
        self.cell_length = {}


def check_object_id(world, name):

    if not name:
        return -1
    clean_name = name.strip()
    if clean_name in world.object_names:
        return world.object_names[clean_name]

    # Predefined fallbacks
    mapping = {
        "straight_road": OBJECT_ID_STRAIGHT_ROAD,
        "left_curve_road": OBJECT_ID_LEFTCURVE_ROAD,
        "right_curve_road": OBJECT_ID_RIGHTCURVE_ROAD,
        "t_junction": OBJECT_ID_TJUNCTION,
        "crossroads": OBJECT_ID_CROSSROADS,
        "mainroad_crossroads": OBJECT_ID_CROSSROADS,
        "lamp_post": OBJECT_ID_LAMP_POST,
        "road_sign": OBJECT_ID_ROAD_SIGN,
        "house_semileft": OBJECT_ID_HOUSE,
        "house_semiright": OBJECT_ID_HOUSE_END,
        "left_corner": OBJECT_ID_LEFTCORNER_ROAD,
        "right_corner": OBJECT_ID_RIGHTCORNER_ROAD,
        "left_t_junction": OBJECT_ID_LEFT_TJUNCTION,
        "right_t_junction": OBJECT_ID_RIGHT_TJUNCTION,
        "zebra": OBJECT_ID_ZEBRA,
        "island": OBJECT_ID_ISLAND,
        "small_straight_road": OBJECT_ID_SMALL_STRAIGHT_ROAD,
        "petrol_station": OBJECT_ID_PETROL_STATION,
        "traffic_light": OBJECT_ID_TRAFFIC_LIGHT,
        "slope": OBJECT_ID_SLOPE,
        "slope_corner": OBJECT_ID_SLOPE_CORNER,
    }
    return mapping.get(clean_name, -1)


def load_rr_textures(filename, world):

    if not os.path.exists(filename):
        return False

    try:
        with open(filename, "r") as f:
            lines = f.read().split()

        i = 0
        while i < len(lines):
            token = lines[i]
            if token == "Alias":
                if i + 2 < len(lines):
                    alias_num = lines[i + 1]
                    alias_name = lines[i + 2]
                    world.texture_names.append(alias_name)
                i += 3
            elif token == "END_FILE":
                break
            else:
                i += 1
        return True
    except Exception as e:
        print(f"Error loading textures: {e}")
        return False


def load_models(filename, world):

    if not os.path.exists(filename):
        return False

    try:
        with open(filename, "r") as f:
            lines = f.read().split()

        i = 0
        while i < len(lines):
            token = lines[i]
            if token in ("PLAYER", "YOURGUN"):

                # Search for player name towards end of block or line
                # PLAYER <num> <type> model_id <id> <filename> tex <tex> pos <x y z> angle <a> scale <s> ... name <name>
                model_id = 0
                for j in range(i, min(i + 30, len(lines))):
                    if lines[j] == "model_id" and j + 2 < len(lines):
                        try:
                            model_id = int(lines[j + 1])
                        except ValueError:
                            pass
                    if lines[j] == "name" and j + 1 < len(lines):
                        mname = lines[j + 1]
                        world.model_names.append({"name": mname, "num": model_id})
                        break
            elif token == "END_FILE":
                break
            i += 1
        return True
    except Exception as e:
        print(f"Error loading models: {e}")
        return False


def create_default_object_data(world):

    defaults = {
        OBJECT_ID_STRAIGHT_ROAD: ("straight_road", 0, 0, 120, [(0, 0, 120)]),
        OBJECT_ID_LEFTCURVE_ROAD: ("left_curve_road", -20, 0, 40, [(-20, 0, 40)]),
        OBJECT_ID_RIGHTCURVE_ROAD: ("right_curve_road", 20, 0, 40, [(20, 0, 40)]),
        OBJECT_ID_SMALL_STRAIGHT_ROAD: ("small_straight_road", 0, 0, 40, [(0, 0, 40)]),
        OBJECT_ID_LEFTCORNER_ROAD: ("left_corner", -20, 0, 20, [(-20, 0, 20)]),
        OBJECT_ID_RIGHTCORNER_ROAD: ("right_corner", 20, 0, 20, [(20, 0, 20)]),
        OBJECT_ID_TJUNCTION: ("t_junction", 0, 0, 40, [(0, 0, 40), (40, 0, 20)]),
        OBJECT_ID_LEFT_TJUNCTION: ("left_t_junction", 0, 0, 40, [(0, 0, 40), (-40, 0, 20)]),
        OBJECT_ID_RIGHT_TJUNCTION: ("right_t_junction", 0, 0, 40, [(0, 0, 40), (40, 0, 20)]),
        OBJECT_ID_CROSSROADS: ("crossroads", 0, 0, 40, [(0, 0, 40), (-40, 0, 20), (40, 0, 20)]),
        OBJECT_ID_ZEBRA: ("zebra", 0, 0, 40, [(0, 0, 40)]),
    }

    for obj_id, (name, dx, dy, dz, conns) in defaults.items():
        if obj_id not in world.obdata:
            entry = ObjectDataEntry()
            # Simple rectangle quad for 2D map wireframe representation
            entry.v = [
                Vert(-20, 0, 0),
                Vert(20, 0, 0),
                Vert(20, 0, dz if dz != 0 else 40),
                Vert(-20, 0, dz if dz != 0 else 40),
            ]
            entry.num_vert = [4]
            entry.tex = [1]
            for idx, c in enumerate(conns):
                if idx < 4:
                    entry.connection[idx] = Vert(c[0], c[1], c[2])
            world.obdata[obj_id] = entry
            world.object_names[name] = obj_id
            world.id_to_name[obj_id] = name
            world.num_vert_per_object[obj_id] = 4
            world.num_polys_per_object[obj_id] = 1


def load_object_data(filename, world):

    load_rr_textures("textures.dat", world)
    load_models("modellist.dat", world)

    if not os.path.exists(filename):
        create_default_object_data(world)
        return True

    try:
        with open(filename, "r") as f:
            content = f.read()

        tokens = content.split()
        idx = 0
        current_obj_id = -1
        vert_count = 0
        poly_count = 0
        conn_cnt = 0
        texture = 1

        while idx < len(tokens):
            tok = tokens[idx]

            if tok == "OBJECT":
                if idx + 2 < len(tokens):
                    raw_id = tokens[idx + 1]
                    raw_name = tokens[idx + 2]
                    idx += 2

                    try:
                        current_obj_id = int(raw_id)
                    except ValueError:
                        current_obj_id = check_object_id(world, raw_name)

                    if current_obj_id not in world.obdata:
                        world.obdata[current_obj_id] = ObjectDataEntry()

                    world.object_names[raw_name] = current_obj_id
                    world.id_to_name[current_obj_id] = raw_name
                    vert_count = 0
                    poly_count = 0
                    conn_cnt = 0

            elif tok == "TEXTURE":
                if idx + 1 < len(tokens):
                    tex_name = tokens[idx + 1]
                    idx += 1
                    texture = 1
                    for t_i, t_n in enumerate(world.texture_names):
                        if t_n == tex_name:
                            texture = t_i + 1
                            break

            elif tok in ("QUAD", "QUADTEX"):
                is_tex = (tok == "QUADTEX")
                num_v = 4
                if current_obj_id >= 0:
                    obj_entry = world.obdata[current_obj_id]
                    for _ in range(4):
                        vx = float(tokens[idx + 1])
                        vy = float(tokens[idx + 2])
                        vz = float(tokens[idx + 3])
                        idx += 3
                        if is_tex:
                            idx += 2  # skip u, v
                        obj_entry.v.append(Vert(vx, vy, vz))
                        vert_count += 1
                    obj_entry.num_vert.append(4)
                    obj_entry.tex.append(texture)
                    poly_count += 1

            elif tok in ("TRI", "TRITEX"):
                is_tex = (tok == "TRITEX")
                if current_obj_id >= 0:
                    obj_entry = world.obdata[current_obj_id]
                    for _ in range(3):
                        vx = float(tokens[idx + 1])
                        vy = float(tokens[idx + 2])
                        vz = float(tokens[idx + 3])
                        idx += 3
                        if is_tex:
                            idx += 2  # skip u, v
                        obj_entry.v.append(Vert(vx, vy, vz))
                        vert_count += 1
                    obj_entry.num_vert.append(3)
                    obj_entry.tex.append(texture)
                    poly_count += 1

            elif tok == "CONNECTION":
                if current_obj_id >= 0 and conn_cnt < 4:
                    cx = float(tokens[idx + 1])
                    cy = float(tokens[idx + 2])
                    cz = float(tokens[idx + 3])
                    idx += 3
                    world.obdata[current_obj_id].connection[conn_cnt] = Vert(cx, cy, cz)
                    conn_cnt += 1
                else:
                    idx += 3

            elif tok == "END_FILE":
                if current_obj_id >= 0:
                    world.num_vert_per_object[current_obj_id] = vert_count
                    world.num_polys_per_object[current_obj_id] = poly_count
                break

            idx += 1

        create_default_object_data(world)
        return True
    except Exception as e:
        print(f"Error reading {filename}: {e}")
        create_default_object_data(world)
        return False


def find_model_id(world, name):

    for m in world.model_names:
        if m["name"] == name:
            return m["num"]
    return 0


def find_model_name(world, model_id):

    for m in world.model_names:
        if m["num"] == model_id:
            return m["name"]
    return ""


def find_texture_id(world, name):

    for idx, t_n in enumerate(world.texture_names):
        if t_n == name:
            return idx + 1
    return 0


def find_texture_name(world, tex_id):

    if 1 <= tex_id <= len(world.texture_names):
        return world.texture_names[tex_id - 1]
    return "0"


def load_map_in_editor(filename, world, links_list):

    if not os.path.exists(filename):
        return False

    try:
        with open(filename, "r") as f:
            lines = [line.strip() for line in f if line.strip()]

        world.oblist.clear()
        links_list.clear()

        i = 0
        current_obj = None

        while i < len(lines):
            line = lines[i]
            parts = line.split()
            if not parts:
                i += 1
                continue

            tag = parts[0]

            if tag == "OBJECT":
                if len(parts) > 1:
                    pname = parts[1]
                    current_obj = ObjectListEntry()
                    current_obj.name = pname
                    current_obj.type = check_object_id(world, pname)
                    world.oblist.append(current_obj)

            elif tag == "CO_ORDINATES" and current_obj:
                if len(parts) >= 4:
                    current_obj.x = float(parts[1])
                    current_obj.y = float(parts[2])
                    current_obj.z = float(parts[3])

            elif tag == "ROT_ANGLE" and current_obj:
                if len(parts) >= 2:
                    current_obj.rot_angle = float(parts[1])

                if "!" in current_obj.name and len(parts) >= 3:
                    # e.g., ROT_ANGLE <angle> <model_name> <texture_name> <id> <ability>
                    model_str = parts[2]
                    current_obj.monsterid = find_model_id(world, model_str)
                    if len(parts) >= 4:
                        tex_str = parts[3]
                        current_obj.monstertexture = find_texture_id(world, tex_str)
                    if len(parts) >= 6:
                        try:
                            current_obj.ability = int(parts[5])
                        except ValueError:
                            pass

                elif "text" in current_obj.name and len(parts) >= 2:
                    try:
                        current_obj.ability = int(parts[1])
                    except ValueError:
                        pass
                    if len(parts) >= 3:
                        current_obj.ctext = " ".join(parts[2:])

                elif len(parts) >= 3:
                    try:
                        current_obj.ability = int(parts[2])
                    except ValueError:
                        pass

            elif tag == "LIGHT_ON_VERT" and current_obj:
                if len(parts) >= 2:
                    current_obj.light = int(parts[1])

            elif tag == "LIGHT_SOURCE" and current_obj:
                # LIGHT_SOURCE Spotlight POS x y z DIR dx dy dz COLOUR r g b
                if len(parts) >= 2:
                    l_type_str = parts[1]
                    if l_type_str == "Spotlight":
                        current_obj.ltype = 0
                    elif l_type_str == "point":
                        current_obj.ltype = 1
                    elif l_type_str == "directional":
                        current_obj.ltype = 2
                    elif l_type_str == "flicker":
                        current_obj.ltype = 3

                # Parse POS, DIR, COLOUR tokens if present in remaining line
                if "POS" in parts:
                    p_idx = parts.index("POS")
                    if p_idx + 3 < len(parts):
                        current_obj.x = float(parts[p_idx + 1])
                        current_obj.y = float(parts[p_idx + 2])
                        current_obj.z = float(parts[p_idx + 3])
                if "DIR" in parts:
                    d_idx = parts.index("DIR")
                    if d_idx + 3 < len(parts):
                        current_obj.dirx = float(parts[d_idx + 1])
                        current_obj.diry = float(parts[d_idx + 2])
                        current_obj.dirz = float(parts[d_idx + 3])
                if "COLOUR" in parts:
                    c_idx = parts.index("COLOUR")
                    if c_idx + 3 < len(parts):
                        current_obj.rcolour = float(parts[c_idx + 1])
                        current_obj.gcolour = float(parts[c_idx + 2])
                        current_obj.bcolour = float(parts[c_idx + 3])

            elif tag == "LINK":
                if len(parts) >= 5:
                    lx = float(parts[1])
                    lz = float(parts[2])
                    la = float(parts[3])
                    lobj = int(parts[4])
                    links_list.append(Link(lx, lz, la, active=True, inactive=0, objectid=lobj))

            elif tag == "END_FILE":
                break

            i += 1

        return True
    except Exception as e:
        print(f"Error loading map {filename}: {e}")
        return False


def save_map(filename, world, links_list):

    try:
        with open(filename, "w") as f:
            count_inactive = 0

            for i, obj in enumerate(world.oblist):
                if obj.inactive != 0:
                    continue

                obj_name = obj.name if obj.name else "straight_road"
                f.write(f"OBJECT {obj_name}\n")
                f.write(f"CO_ORDINATES {obj.x:.6f} {obj.y:.6f} {obj.z:.6f}\n")

                if "!monster1" in obj_name or ("!" in obj_name and "text" not in obj_name):
                    model_str = find_model_name(world, obj.monsterid)
                    if not model_str:
                        model_str = "0"
                    tex_str = find_texture_name(world, obj.monstertexture)
                    if not tex_str:
                        tex_str = "0"
                    f.write(f"ROT_ANGLE {int(obj.rot_angle)} {model_str} {tex_str} {i} {obj.ability}\n")

                elif "text" in obj_name:
                    f.write(f"ROT_ANGLE {obj.ability} {obj.ctext}\n")

                else:
                    if "door" in obj_name:
                        f.write(f"ROT_ANGLE {int(obj.rot_angle)} {obj.ability}\n")
                    else:
                        f.write(f"ROT_ANGLE {int(obj.rot_angle)}\n")

                if obj_name == "lamp_post":
                    l_types = ["Spotlight", "point", "directional", "flicker"]
                    l_str = l_types[obj.ltype] if 0 <= obj.ltype < len(l_types) else "Spotlight"
                    f.write(
                        f"LIGHT_SOURCE {l_str} POS {obj.x:.6f} {obj.y:.6f} {obj.z:.6f} "
                        f"DIR {obj.dirx:.6f} {obj.diry:.6f} {obj.dirz:.6f} "
                        f"COLOUR {obj.rcolour:.6f} {obj.gcolour:.6f} {obj.bcolour:.6f}\n"
                    )

                if obj.light >= 0:
                    f.write(f"LIGHT_ON_VERT {obj.light}\n\n")

            last_obj = -999
            for link in links_list:
                if link.inactive == 0:
                    f.write(f"LINK {link.last_x:.6f} {link.last_z:.6f} {int(link.last_angle)} {link.objectid - count_inactive}\n")
                else:
                    if last_obj != link.objectid:
                        count_inactive += 1
                last_obj = link.objectid

            f.write("END_FILE\n")

        init_world_map("level1.cmp", world)
        return True
    except Exception as e:
        print(f"Error saving map {filename}: {e}")
        return False


def init_world_map(cmp_filename, world):

    try:
        grid = {}  # (cx, cz) -> list of object indices

        for i, obj in enumerate(world.oblist):
            if obj.inactive != 0:
                continue

            cx = int(obj.x / 256.0)
            cz = int(obj.z / 256.0)

            # Clamp grid coordinates to 0..199
            cx = max(0, min(199, cx))
            cz = max(0, min(199, cz))

            if (cx, cz) not in grid:
                grid[(cx, cz)] = []
            grid[(cx, cz)].append(i)

        with open(cmp_filename, "w") as f:
            for (cx, cz), obj_indices in grid.items():
                if obj_indices:
                    count = len(obj_indices)
                    indices_str = " ".join(str(idx) for idx in obj_indices)
                    f.write(f"{cx} {cz} {count} {indices_str}\n")
            f.write("END_FILE\n")

        return True
    except Exception as e:
        print(f"Error saving compiled map {cmp_filename}: {e}")
        return False
