import math
from world import (
    World,
    ObjectListEntry,
    Link,
    OBJECT_ID_STRAIGHT_ROAD,
    OBJECT_ID_LEFTCURVE_ROAD,
    OBJECT_ID_RIGHTCURVE_ROAD,
    OBJECT_ID_TJUNCTION,
    OBJECT_ID_CROSSROADS,
    OBJECT_ID_ROUNDABOUT,
    OBJECT_ID_LAMP_POST,
    OBJECT_ID_LAMP,
    OBJECT_ID_HOUSE,
    OBJECT_ID_POLICE_CAR,
    OBJECT_ID_ROAD_SIGN,
    OBJECT_ID_SMALL_STRAIGHT_ROAD,
    OBJECT_ID_LEFTCORNER_ROAD,
    OBJECT_ID_RIGHTCORNER_ROAD,
    OBJECT_ID_LEFT_TJUNCTION,
    OBJECT_ID_RIGHT_TJUNCTION,
    OBJECT_ID_ZEBRA,
    OBJECT_ID_ISLAND,
    OBJECT_ID_HOUSE_END,
    OBJECT_ID_PETROL_STATION,
    OBJECT_ID_TRAFFIC_LIGHT,
    OBJECT_ID_SLOPE,
    OBJECT_ID_SLOPE_CORNER,
    OBJECT_ID_CENTRAL_RESERVATION,
)

# Constants for instruction types
LEFT_HAND_CURVE = 1
RIGHT_HAND_CURVE = 2
STRAIGHT = 3
T_JUNCTION = 4
ROUND_ABOUT = 5
CROSSROAD = 6
SMALL_STRAIGHT = 7
LEFT_CORNER = 8
RIGHT_CORNER = 9
LEFT_T_JUNCTION = 10
RIGHT_T_JUNCTION = 11
ZEBRA = 12

DIALOGBAR_MODE_ROADS = 1
DIALOGBAR_MODE_OBJECTS = 2


class EditorState:
    def __init__(self, world=None):
        self.world = world if world is not None else World()
        self.links_list = []
        self.current_link = 0
        self.last_link = 0
        self.last_x = 600.0
        self.last_z = 600.0
        self.last_angle = 0.0
        self.display_x_offset = 0
        self.display_y_offset = 0
        self.display_scale = 0.25
        self.object_rot_angle = 0
        self.current_object_id = OBJECT_ID_HOUSE
        self.current_object_name = "house_semileft"
        self.dialogbar_mode = DIALOGBAR_MODE_ROADS
        self.editor_mode = 0  # 0: Auto Road / Edit, 1: Object Placement
        self.draw_mode = 0
        self.jumpnewspot = False

        # Additional property settings
        self.ylocation = 0.0
        self.monsterid = 0
        self.monstertexture = 0
        self.modelname1 = ""
        self.rcolour = 0.0
        self.gcolour = 0.0
        self.bcolour = 0.0
        self.dirx = 0.0
        self.diry = -1.0
        self.dirz = 0.0
        self.ltype = 0
        self.ability = 0
        self.gctext = ""

        # Init default link
        self.init_link_list()

    def check_angle(self):
        while self.last_angle < 0:
            self.last_angle += 360.0
        while self.last_angle >= 360:
            self.last_angle -= 360.0

    def init_link_list(self):
        self.links_list.clear()
        self.current_link = 0
        self.last_link = 0
        self.last_x = 600.0
        self.last_z = 600.0
        self.last_angle = 0.0

    def add_link_to_list(self):
        oblist_idx = len(self.world.oblist) - 1 if self.world.oblist else 0
        new_link = Link(
            x=self.last_x,
            z=self.last_z,
            angle=self.last_angle,
            active=True,
            inactive=0,
            objectid=oblist_idx,
        )
        self.links_list.append(new_link)

        if not self.jumpnewspot:
            self.current_link = len(self.links_list) - 1
            self.last_link = len(self.links_list)

    def replace_link_in_list(self):
        self.add_link_to_list()
        if 0 <= self.current_link < len(self.links_list):
            self.links_list[self.current_link].active_link_flag = False

    def change_current_link(self, direction):
        if not self.links_list:
            return
        if direction == 1:
            self.current_link += 1
            if self.current_link >= len(self.links_list):
                self.current_link = 0
        else:
            self.current_link -= 1
            if self.current_link < 0:
                self.current_link = len(self.links_list) - 1

        lnk = self.links_list[self.current_link]
        self.last_x = lnk.last_x
        self.last_z = lnk.last_z
        self.last_angle = lnk.last_angle

    def set_link_with_mouse(self, x, z):
        if not self.links_list:
            return

        min_dist = 100000.0
        count = 0
        for i, lnk in enumerate(self.links_list):
            if lnk.inactive == 0:
                dx = abs(x - lnk.last_x)
                dz = abs(z - lnk.last_z)
                dist = math.sqrt(dx * dx + dz * dz)
                if dist < min_dist:
                    min_dist = dist
                    count = i

        self.current_link = count
        lnk = self.links_list[self.current_link]
        self.last_x = lnk.last_x
        self.last_z = lnk.last_z
        self.last_angle = lnk.last_angle

    def place_object_with_mouse(self, x, z):
        # Snap to 20 units grid
        dx = int(x / 20.0) * 20
        dz = int(z / 20.0) * 20

        obj = ObjectListEntry()
        obj.x = float(dx)
        obj.y = float(self.ylocation)
        obj.z = float(dz)
        obj.type = self.current_object_id
        obj.rot_angle = float(self.object_rot_angle)
        obj.light = -1
        obj.inactive = 0
        obj.monsterid = self.monsterid
        obj.monstertexture = self.monstertexture
        obj.monstername = self.modelname1
        obj.rcolour = self.rcolour
        obj.gcolour = self.gcolour
        obj.bcolour = self.bcolour
        obj.dirx = self.dirx
        obj.diry = self.diry
        obj.dirz = self.dirz
        obj.ltype = self.ltype
        obj.ability = self.ability
        obj.ctext = self.gctext
        obj.name = self.current_object_name

        self.world.oblist.append(obj)

        self.last_x = float(dx)
        self.last_z = float(dz)
        self.last_angle = float(self.object_rot_angle)

        self.add_link_to_list()

    def undo_last_link(self):
        if not self.world.oblist or not self.links_list:
            return

        oblist_len = len(self.world.oblist) - 1
        obtype = self.world.oblist[oblist_len].type

        num_links = 1
        if obtype in (OBJECT_ID_TJUNCTION, OBJECT_ID_LEFT_TJUNCTION, OBJECT_ID_RIGHT_TJUNCTION):
            num_links = 2
        elif obtype == OBJECT_ID_CROSSROADS:
            num_links = 3

        for _ in range(num_links):
            if self.links_list:
                self.links_list.pop()

        if self.world.oblist:
            self.world.oblist.pop()

        self.last_link = len(self.links_list)
        if self.last_link > 0:
            self.current_link = max(0, self.last_link - 1)
            lnk = self.links_list[self.current_link]
            self.last_x = lnk.last_x
            self.last_z = lnk.last_z
            self.last_angle = lnk.last_angle
        else:
            self.current_link = 0
            self.last_x = 600.0
            self.last_z = 600.0
            self.last_angle = 0.0

    def delete_current_object(self):
        if not self.links_list or not (0 <= self.current_link < len(self.links_list)):
            return

        target_obj_id = self.links_list[self.current_link].objectid

        # Mark object as inactive
        if 0 <= target_obj_id < len(self.world.oblist):
            self.world.oblist[target_obj_id].inactive = 1

        # Mark all associated links as inactive
        for lnk in self.links_list:
            if lnk.objectid == target_obj_id:
                lnk.inactive = 1

    def _get_connection(self, obj_id, conn_idx=0):
        if obj_id in self.world.obdata:
            conn = self.world.obdata[obj_id].connection
            if 0 <= conn_idx < len(conn):
                return conn[conn_idx].x, conn[conn_idx].z
        # Default connection fallback if obdata is missing
        if conn_idx == 0:
            return 0.0, 120.0
        return 0.0, 40.0

    def draw_road_section(self, instruction):
        if instruction == LEFT_HAND_CURVE:
            self.add_left_curve()
        elif instruction == RIGHT_HAND_CURVE:
            self.add_right_curve()
        elif instruction == STRAIGHT:
            self.add_straight()
        elif instruction == SMALL_STRAIGHT:
            self.add_small_straight()
        elif instruction == LEFT_CORNER:
            self.add_left_corner()
        elif instruction == RIGHT_CORNER:
            self.add_right_corner()
        elif instruction == T_JUNCTION:
            self.add_t_junction()
        elif instruction == LEFT_T_JUNCTION:
            self.add_left_t_junction()
        elif instruction == RIGHT_T_JUNCTION:
            self.add_right_t_junction()
        elif instruction == CROSSROAD:
            self.add_cross_road()
        elif instruction == ZEBRA:
            self.add_zebra()

    def add_left_curve(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        obj.type = OBJECT_ID_LEFTCURVE_ROAD
        obj.name = "left_curve_road"
        self.world.oblist.append(obj)

        rad_angle = math.radians(obj.rot_angle)
        x_conn, z_conn = self._get_connection(OBJECT_ID_LEFTCURVE_ROAD, 0)

        self.last_x += (x_conn * math.cos(rad_angle) - z_conn * math.sin(rad_angle))
        self.last_z += (x_conn * math.sin(rad_angle) + z_conn * math.cos(rad_angle))
        self.last_angle += 15.0
        self.check_angle()

        self.replace_link_in_list()

    def add_right_curve(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        obj.type = OBJECT_ID_RIGHTCURVE_ROAD
        obj.name = "right_curve_road"
        self.world.oblist.append(obj)

        rad_angle = math.radians(obj.rot_angle)
        x_conn, z_conn = self._get_connection(OBJECT_ID_RIGHTCURVE_ROAD, 0)

        self.last_x += (x_conn * math.cos(rad_angle) - z_conn * math.sin(rad_angle))
        self.last_z += (x_conn * math.sin(rad_angle) + z_conn * math.cos(rad_angle))
        self.last_angle -= 15.0
        self.check_angle()

        self.replace_link_in_list()

    def add_straight(self):
        obj = ObjectListEntry()
        if self.jumpnewspot:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = float(self.object_rot_angle)
            self.jumpnewspot = False
        elif 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        obj.type = OBJECT_ID_STRAIGHT_ROAD
        obj.name = "straight_road"
        self.world.oblist.append(obj)

        rad_angle = math.radians(obj.rot_angle)
        x_conn, z_conn = self._get_connection(OBJECT_ID_STRAIGHT_ROAD, 0)

        self.last_x += (x_conn * math.cos(rad_angle) - z_conn * math.sin(rad_angle))
        self.last_z += (x_conn * math.sin(rad_angle) + z_conn * math.cos(rad_angle))

        self.replace_link_in_list()

    def add_small_straight(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        obj.type = OBJECT_ID_SMALL_STRAIGHT_ROAD
        obj.name = "small_straight_road"
        self.world.oblist.append(obj)

        rad_angle = math.radians(obj.rot_angle)
        x_conn, z_conn = self._get_connection(OBJECT_ID_SMALL_STRAIGHT_ROAD, 0)

        self.last_x += (x_conn * math.cos(rad_angle) - z_conn * math.sin(rad_angle))
        self.last_z += (x_conn * math.sin(rad_angle) + z_conn * math.cos(rad_angle))

        self.replace_link_in_list()

    def add_left_corner(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        obj.type = OBJECT_ID_LEFTCORNER_ROAD
        obj.name = "left_corner"
        self.world.oblist.append(obj)

        self.last_angle = obj.rot_angle + 90.0
        self.check_angle()

        rad_angle = math.radians(obj.rot_angle)
        x_conn, z_conn = self._get_connection(OBJECT_ID_LEFTCORNER_ROAD, 0)

        self.last_x += (x_conn * math.cos(rad_angle) - z_conn * math.sin(rad_angle))
        self.last_z += (x_conn * math.sin(rad_angle) + z_conn * math.cos(rad_angle))

        self.replace_link_in_list()

    def add_right_corner(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        obj.type = OBJECT_ID_RIGHTCORNER_ROAD
        obj.name = "right_corner"
        self.world.oblist.append(obj)

        self.last_angle = obj.rot_angle - 90.0
        self.check_angle()

        rad_angle = math.radians(obj.rot_angle)
        x_conn, z_conn = self._get_connection(OBJECT_ID_RIGHTCORNER_ROAD, 0)

        self.last_x += (x_conn * math.cos(rad_angle) - z_conn * math.sin(rad_angle))
        self.last_z += (x_conn * math.sin(rad_angle) + z_conn * math.cos(rad_angle))

        self.replace_link_in_list()

    def add_t_junction(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        start_x = obj.x
        start_z = obj.z
        deg_angle = obj.rot_angle
        rad_angle = math.radians(deg_angle)

        obj.type = OBJECT_ID_TJUNCTION
        obj.name = "t_junction"
        self.world.oblist.append(obj)

        # Branch 1 (+270 deg)
        self.last_angle = deg_angle + 270.0
        self.check_angle()
        x_conn0, z_conn0 = self._get_connection(OBJECT_ID_TJUNCTION, 0)
        self.last_x = start_x + (x_conn0 * math.cos(rad_angle) - z_conn0 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn0 * math.sin(rad_angle) + z_conn0 * math.cos(rad_angle))
        self.replace_link_in_list()

        # Branch 2 (+90 deg)
        self.last_angle = deg_angle + 90.0
        self.check_angle()
        x_conn1, z_conn1 = self._get_connection(OBJECT_ID_TJUNCTION, 1)
        self.last_x = start_x + (x_conn1 * math.cos(rad_angle) - z_conn1 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn1 * math.sin(rad_angle) + z_conn1 * math.cos(rad_angle))
        self.add_link_to_list()

    def add_left_t_junction(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        start_x = obj.x
        start_z = obj.z
        deg_angle = obj.rot_angle
        rad_angle = math.radians(deg_angle)

        obj.type = OBJECT_ID_LEFT_TJUNCTION
        obj.name = "left_t_junction"
        self.world.oblist.append(obj)

        # Straight continuation
        self.last_angle = deg_angle
        self.check_angle()
        x_conn0, z_conn0 = self._get_connection(OBJECT_ID_LEFT_TJUNCTION, 0)
        self.last_x = start_x + (x_conn0 * math.cos(rad_angle) - z_conn0 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn0 * math.sin(rad_angle) + z_conn0 * math.cos(rad_angle))
        self.replace_link_in_list()

        # Left branch (+90 deg)
        self.last_angle = deg_angle + 90.0
        self.check_angle()
        x_conn1, z_conn1 = self._get_connection(OBJECT_ID_LEFT_TJUNCTION, 1)
        self.last_x = start_x + (x_conn1 * math.cos(rad_angle) - z_conn1 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn1 * math.sin(rad_angle) + z_conn1 * math.cos(rad_angle))
        self.add_link_to_list()

    def add_right_t_junction(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        start_x = obj.x
        start_z = obj.z
        deg_angle = obj.rot_angle
        rad_angle = math.radians(deg_angle)

        obj.type = OBJECT_ID_RIGHT_TJUNCTION
        obj.name = "right_t_junction"
        self.world.oblist.append(obj)

        # Straight continuation
        self.last_angle = deg_angle
        self.check_angle()
        x_conn0, z_conn0 = self._get_connection(OBJECT_ID_RIGHT_TJUNCTION, 0)
        self.last_x = start_x + (x_conn0 * math.cos(rad_angle) - z_conn0 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn0 * math.sin(rad_angle) + z_conn0 * math.cos(rad_angle))
        self.replace_link_in_list()

        # Right branch (+270 deg)
        self.last_angle = deg_angle + 270.0
        self.check_angle()
        x_conn1, z_conn1 = self._get_connection(OBJECT_ID_RIGHT_TJUNCTION, 1)
        self.last_x = start_x + (x_conn1 * math.cos(rad_angle) - z_conn1 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn1 * math.sin(rad_angle) + z_conn1 * math.cos(rad_angle))
        self.add_link_to_list()

    def add_cross_road(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        start_x = obj.x
        start_z = obj.z
        deg_angle = obj.rot_angle
        rad_angle = math.radians(deg_angle)

        obj.type = OBJECT_ID_CROSSROADS
        obj.name = "mainroad_crossroads"
        self.world.oblist.append(obj)

        # Straight continuation
        self.last_angle = deg_angle
        x_conn0, z_conn0 = self._get_connection(OBJECT_ID_CROSSROADS, 0)
        self.last_x = start_x + (x_conn0 * math.cos(rad_angle) - z_conn0 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn0 * math.sin(rad_angle) + z_conn0 * math.cos(rad_angle))
        self.replace_link_in_list()

        # Right branch (+270 deg)
        self.last_angle = deg_angle + 270.0
        self.check_angle()
        x_conn1, z_conn1 = self._get_connection(OBJECT_ID_CROSSROADS, 1)
        self.last_x = start_x + (x_conn1 * math.cos(rad_angle) - z_conn1 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn1 * math.sin(rad_angle) + z_conn1 * math.cos(rad_angle))
        self.add_link_to_list()

        # Left branch (+90 deg)
        self.last_angle = deg_angle + 90.0
        self.check_angle()
        x_conn2, z_conn2 = self._get_connection(OBJECT_ID_CROSSROADS, 2)
        self.last_x = start_x + (x_conn2 * math.cos(rad_angle) - z_conn2 * math.sin(rad_angle))
        self.last_z = start_z + (x_conn2 * math.sin(rad_angle) + z_conn2 * math.cos(rad_angle))
        self.add_link_to_list()

    def add_zebra(self):
        obj = ObjectListEntry()
        if 0 <= self.current_link < len(self.links_list):
            lnk = self.links_list[self.current_link]
            obj.x = lnk.last_x
            obj.y = self.ylocation
            obj.z = lnk.last_z
            obj.rot_angle = lnk.last_angle
        else:
            obj.x = self.last_x
            obj.y = self.ylocation
            obj.z = self.last_z
            obj.rot_angle = self.last_angle

        obj.type = OBJECT_ID_ZEBRA
        obj.name = "zebra"
        self.world.oblist.append(obj)

        rad_angle = math.radians(obj.rot_angle)
        x_conn, z_conn = self._get_connection(OBJECT_ID_ZEBRA, 0)

        self.last_x += (x_conn * math.cos(rad_angle) - z_conn * math.sin(rad_angle))
        self.last_z += (x_conn * math.sin(rad_angle) + z_conn * math.cos(rad_angle))

        self.replace_link_in_list()
