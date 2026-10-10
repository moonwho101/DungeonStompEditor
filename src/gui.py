import math
import sys
import os

from PyQt5.QtCore import Qt, QPointF, QRectF, pyqtSignal
from PyQt5.QtGui import QPainter, QPen, QColor, QBrush, QFont, QIcon, QKeySequence
from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QGridLayout,
    QFormLayout,
    QSplitter,
    QTabWidget,
    QPushButton,
    QLabel,
    QLineEdit,
    QComboBox,
    QDoubleSpinBox,
    QSpinBox,
    QTextEdit,
    QGroupBox,
    QDockWidget,
    QAction,
    QToolBar,
    QStatusBar,
    QFileDialog,
    QMessageBox,
    QFrame,
    QScrollArea,
)

from world import (
    World,
    load_object_data,
    load_map_in_editor,
    save_map,
    init_world_map,
    find_model_id,
    find_texture_id,
    OBJECT_ID_STRAIGHT_ROAD,
    OBJECT_ID_LEFTCURVE_ROAD,
    OBJECT_ID_RIGHTCURVE_ROAD,
    OBJECT_ID_TJUNCTION,
    OBJECT_ID_CROSSROADS,
    OBJECT_ID_LAMP_POST,
    OBJECT_ID_HOUSE,
    OBJECT_ID_SMALL_STRAIGHT_ROAD,
    OBJECT_ID_LEFTCORNER_ROAD,
    OBJECT_ID_RIGHTCORNER_ROAD,
    OBJECT_ID_LEFT_TJUNCTION,
    OBJECT_ID_RIGHT_TJUNCTION,
    OBJECT_ID_ZEBRA,
)

from editor_logic import (
    EditorState,
    LEFT_HAND_CURVE,
    RIGHT_HAND_CURVE,
    STRAIGHT,
    T_JUNCTION,
    CROSSROAD,
    SMALL_STRAIGHT,
    LEFT_CORNER,
    RIGHT_CORNER,
    LEFT_T_JUNCTION,
    RIGHT_T_JUNCTION,
    ZEBRA,
    DIALOGBAR_MODE_ROADS,
    DIALOGBAR_MODE_OBJECTS,
)


class MapCanvas(QWidget):
    mouse_moved = pyqtSignal(float, float)
    object_selected = pyqtSignal(int)

    def __init__(self, editor_state, parent=None):
        super().__init__(parent)
        self.editor = editor_state
        self.setMouseTracking(True)
        self.setFocusPolicy(Qt.StrongFocus)
        self.is_panning = False
        self.last_mouse_pos = QPointF()

    def world_to_screen(self, wx, wz):
        s = self.editor.display_scale
        x_off = self.editor.display_x_offset
        z_off = self.editor.display_y_offset
        sx = s * (x_off + wx)
        sz = s * (z_off + wz)
        return sx, sz

    def screen_to_world(self, sx, sz):
        s = self.editor.display_scale
        x_off = self.editor.display_x_offset
        z_off = self.editor.display_y_offset
        if s == 0:
            s = 0.25
        wx = (sx / s) - x_off
        wz = (sz / s) - z_off
        return wx, wz

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing, True)

        # Clear background
        painter.fillRect(self.rect(), QColor(240, 240, 245))

        s = self.editor.display_scale
        x_off = self.editor.display_x_offset
        z_off = self.editor.display_y_offset
        w = self.width()
        h = self.height()

        start_wx, start_wz = self.screen_to_world(0, 0)
        end_wx, end_wz = self.screen_to_world(w, h)

        # Draw Grid
        grid_step = 20
        grid_start_x = int(math.floor(start_wx / grid_step)) * grid_step - 200
        grid_end_x = int(math.ceil(end_wx / grid_step)) * grid_step + 200
        grid_start_z = int(math.floor(start_wz / grid_step)) * grid_step - 200
        grid_end_z = int(math.ceil(end_wz / grid_step)) * grid_step + 200

        # Minor grid (20 units)
        if s * 20 > 3:
            painter.setPen(QPen(QColor(215, 215, 220), 1, Qt.DotLine))
            for z in range(grid_start_z, grid_end_z, grid_step):
                _, sz = self.world_to_screen(0, z)
                painter.drawLine(0, int(sz), w, int(sz))
            for x in range(grid_start_x, grid_end_x, grid_step):
                sx, _ = self.world_to_screen(x, 0)
                painter.drawLine(int(sx), 0, int(sx), h)

        # Major cell grid (260 units)
        cell_step = 260
        cell_start_x = int(math.floor(start_wx / cell_step)) * cell_step - 260
        cell_end_x = int(math.ceil(end_wx / cell_step)) * cell_step + 260
        cell_start_z = int(math.floor(start_wz / cell_step)) * cell_step - 260
        cell_end_z = int(math.ceil(end_wz / cell_step)) * cell_step + 260

        painter.setPen(QPen(QColor(230, 100, 100, 120), 1, Qt.DashLine))
        for z in range(cell_start_z, cell_end_z, cell_step):
            _, sz = self.world_to_screen(0, z)
            painter.drawLine(0, int(sz), w, int(sz))
        for x in range(cell_start_x, cell_end_x, cell_step):
            sx, _ = self.world_to_screen(x, 0)
            painter.drawLine(int(sx), 0, int(sx), h)

        # Draw Map Objects
        for idx, obj in enumerate(self.editor.world.oblist):
            if obj.inactive != 0:
                continue

            # Determine pen color
            if idx == self.editor.current_link:
                pen = QPen(QColor(0, 120, 255), 2, Qt.SolidLine)
            else:
                pen = QPen(QColor(40, 40, 50), 1, Qt.SolidLine)
            painter.setPen(pen)

            rad = math.radians(obj.rot_angle)
            cos_a = math.cos(rad)
            sin_a = math.sin(rad)

            ob_type = obj.type
            if ob_type in self.editor.world.obdata:
                data = self.editor.world.obdata[ob_type]
                v_list = data.v
                num_v_list = data.num_vert

                v_idx = 0
                for poly_v_count in num_v_list:
                    pts = []
                    for _ in range(poly_v_count):
                        if v_idx < len(v_list):
                            lx = v_list[v_idx].x
                            lz = v_list[v_idx].z
                            wx = obj.x + (lx * cos_a - lz * sin_a)
                            wz = obj.z + (lx * sin_a + lz * cos_a)
                            sx, sz = self.world_to_screen(wx, wz)
                            pts.append(QPointF(sx, sz))
                            v_idx += 1

                    if len(pts) >= 3:
                        for i_p in range(len(pts)):
                            p1 = pts[i_p]
                            p2 = pts[(i_p + 1) % len(pts)]
                            painter.drawLine(p1, p2)
            else:
                # Default box wireframe if no geometry data
                half = 15
                corners = [(-half, -half), (half, -half), (half, half), (-half, half)]
                pts = []
                for lx, lz in corners:
                    wx = obj.x + (lx * cos_a - lz * sin_a)
                    wz = obj.z + (lx * sin_a + lz * cos_a)
                    sx, sz = self.world_to_screen(wx, wz)
                    pts.append(QPointF(sx, sz))
                for i_p in range(4):
                    painter.drawLine(pts[i_p], pts[(i_p + 1) % 4])

        # Draw Crosshair at active link / location
        if 0 <= self.editor.current_link < len(self.editor.links_list):
            active_lnk = self.editor.links_list[self.editor.current_link]
            cx, cz = active_lnk.last_x, active_lnk.last_z
        else:
            cx, cz = self.editor.last_x, self.editor.last_z

        csx, csz = self.world_to_screen(cx, cz)
        painter.setPen(QPen(QColor(220, 20, 20), 2))
        cross_size = 12
        painter.drawLine(int(csx - cross_size), int(csz - cross_size), int(csx + cross_size), int(csz + cross_size))
        painter.drawLine(int(csx + cross_size), int(csz - cross_size), int(csx - cross_size), int(csz + cross_size))

    def mousePressEvent(self, event):
        wx, wz = self.screen_to_world(event.x(), event.y())

        if event.button() == Qt.MiddleButton or (event.button() == Qt.LeftButton and event.modifiers() & Qt.ControlModifier):
            self.is_panning = True
            self.last_mouse_pos = event.pos()
            self.setCursor(Qt.ClosedHandCursor)
            return

        if event.button() == Qt.LeftButton:
            if self.editor.editor_mode == 1:  # Object placement mode
                self.editor.place_object_with_mouse(wx, wz)
                self.object_selected.emit(self.editor.current_link)
            else:  # Road / Edit mode
                self.editor.set_link_with_mouse(wx, wz)
                self.object_selected.emit(self.editor.current_link)
            self.update()

        elif event.button() == Qt.RightButton:
            self.editor.object_rot_angle = (self.editor.object_rot_angle + 45) % 360
            self.update()

    def mouseMoveEvent(self, event):
        wx, wz = self.screen_to_world(event.x(), event.y())
        self.mouse_moved.emit(wx, wz)

        if self.is_panning:
            delta = event.pos() - self.last_mouse_pos
            self.last_mouse_pos = event.pos()
            s = self.editor.display_scale if self.editor.display_scale != 0 else 0.25
            self.editor.display_x_offset += delta.x() / s
            self.editor.display_y_offset += delta.y() / s
            self.update()

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.MiddleButton or self.is_panning:
            self.is_panning = False
            self.setCursor(Qt.ArrowCursor)

    def wheelEvent(self, event):
        angle = event.angleDelta().y()
        if angle > 0:
            self.editor.display_scale = min(4.0, self.editor.display_scale * 1.25)
        else:
            self.editor.display_scale = max(0.02, self.editor.display_scale / 1.25)
        self.update()


class PropertyInspector(QWidget):
    properties_changed = pyqtSignal()

    def __init__(self, editor_state, parent=None):
        super().__init__(parent)
        self.editor = editor_state
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(8, 8, 8, 8)
        layout.setSpacing(6)

        # Placed Objects Selector
        self.obj_selector = QComboBox()
        self.obj_selector.currentIndexChanged.connect(self.on_object_selected_from_combo)
        layout.addWidget(QLabel("Placed Object Selection:"))
        layout.addWidget(self.obj_selector)

        # Form fields inside scroll area
        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        form_widget = QWidget()
        form = QFormLayout(form_widget)
        form.setContentsMargins(4, 4, 4, 4)
        form.setSpacing(6)

        self.y_edit = QDoubleSpinBox()
        self.y_edit.setRange(-10000, 10000)
        self.y_edit.setSingleStep(1.0)

        self.angle_edit = QDoubleSpinBox()
        self.angle_edit.setRange(0, 360)
        self.angle_edit.setSingleStep(15.0)

        self.obj_name_combo = QComboBox()
        self.obj_name_combo.setEditable(True)

        self.model_combo = QComboBox()
        self.model_combo.setEditable(True)

        self.texture_combo = QComboBox()
        self.texture_combo.setEditable(True)

        self.ability_spin = QSpinBox()
        self.ability_spin.setRange(-1000, 10000)

        self.ctext_edit = QTextEdit()
        self.ctext_edit.setMaximumHeight(60)

        # Light properties
        self.r_spin = QDoubleSpinBox()
        self.r_spin.setRange(0, 1)
        self.r_spin.setSingleStep(0.05)

        self.g_spin = QDoubleSpinBox()
        self.g_spin.setRange(0, 1)
        self.g_spin.setSingleStep(0.05)

        self.b_spin = QDoubleSpinBox()
        self.b_spin.setRange(0, 1)
        self.b_spin.setSingleStep(0.05)

        self.dirx_spin = QDoubleSpinBox()
        self.dirx_spin.setRange(-100, 100)

        self.diry_spin = QDoubleSpinBox()
        self.diry_spin.setRange(-100, 100)

        self.dirz_spin = QDoubleSpinBox()
        self.dirz_spin.setRange(-100, 100)

        self.ltype_combo = QComboBox()
        self.ltype_combo.addItems(["Spotlight", "Point", "Directional", "Flicker"])

        form.addRow("Position Y:", self.y_edit)
        form.addRow("Rotation Angle:", self.angle_edit)
        form.addRow("Object / Type:", self.obj_name_combo)
        form.addRow("Model Name:", self.model_combo)
        form.addRow("Texture Name:", self.texture_combo)
        form.addRow("Param / Ability:", self.ability_spin)
        form.addRow("Custom Text:", self.ctext_edit)

        # Light Group
        light_box = QGroupBox("Lighting Settings")
        light_layout = QFormLayout(light_box)
        light_layout.addRow("Light Type:", self.ltype_combo)
        light_layout.addRow("Color R:", self.r_spin)
        light_layout.addRow("Color G:", self.g_spin)
        light_layout.addRow("Color B:", self.b_spin)
        light_layout.addRow("Dir X:", self.dirx_spin)
        light_layout.addRow("Dir Y:", self.diry_spin)
        light_layout.addRow("Dir Z:", self.dirz_spin)
        form.addRow(light_box)

        scroll.setWidget(form_widget)
        layout.addWidget(scroll)

        # Apply Button
        btn_apply = QPushButton("Apply Properties")
        btn_apply.setStyleSheet("background-color: #007acc; color: white; font-weight: bold; padding: 6px;")
        btn_apply.clicked.connect(self.apply_properties)
        layout.addWidget(btn_apply)

        self.populate_options()

    def populate_options(self):
        self.obj_name_combo.clear()
        for name in self.editor.world.object_names.keys():
            self.obj_name_combo.addItem(name)

        self.texture_combo.clear()
        for t_name in self.editor.world.texture_names:
            self.texture_combo.addItem(t_name)

        self.model_combo.clear()
        for m in self.editor.world.model_names:
            self.model_combo.addItem(m["name"])

    def refresh_placed_objects_list(self):
        self.obj_selector.blockSignals(True)
        self.obj_selector.clear()
        for idx, obj in enumerate(self.editor.world.oblist):
            if obj.inactive == 0:
                self.obj_selector.addItem(f"ID:{idx} {obj.name}", idx)
        if 0 <= self.editor.current_link < len(self.editor.links_list):
            lnk = self.editor.links_list[self.editor.current_link]
            for i in range(self.obj_selector.count()):
                if self.obj_selector.itemData(i) == lnk.objectid:
                    self.obj_selector.setCurrentIndex(i)
                    break
        self.obj_selector.blockSignals(False)
        self.update_inspector_fields()

    def on_object_selected_from_combo(self, index):
        if index >= 0:
            target_obj_idx = self.obj_selector.itemData(index)
            for i, lnk in enumerate(self.editor.links_list):
                if lnk.objectid == target_obj_idx and lnk.inactive == 0:
                    self.editor.current_link = i
                    self.editor.last_x = lnk.last_x
                    self.editor.last_z = lnk.last_z
                    self.editor.last_angle = lnk.last_angle
                    break
            self.update_inspector_fields()
            self.properties_changed.emit()

    def update_inspector_fields(self):
        if 0 <= self.editor.current_link < len(self.editor.links_list):
            lnk = self.editor.links_list[self.editor.current_link]
            obj_idx = lnk.objectid
            if 0 <= obj_idx < len(self.editor.world.oblist):
                obj = self.editor.world.oblist[obj_idx]
                self.y_edit.setValue(obj.y)
                self.angle_edit.setValue(obj.rot_angle)
                self.obj_name_combo.setCurrentText(obj.name)
                self.ability_spin.setValue(obj.ability)
                self.ctext_edit.setPlainText(obj.ctext)
                self.r_spin.setValue(obj.rcolour)
                self.g_spin.setValue(obj.gcolour)
                self.b_spin.setValue(obj.bcolour)
                self.dirx_spin.setValue(obj.dirx)
                self.diry_spin.setValue(obj.diry)
                self.dirz_spin.setValue(obj.dirz)
                if 0 <= obj.ltype < self.ltype_combo.count():
                    self.ltype_combo.setCurrentIndex(obj.ltype)

    def apply_properties(self):
        if 0 <= self.editor.current_link < len(self.editor.links_list):
            lnk = self.editor.links_list[self.editor.current_link]
            obj_idx = lnk.objectid
            if 0 <= obj_idx < len(self.editor.world.oblist):
                obj = self.editor.world.oblist[obj_idx]
                obj.y = self.y_edit.value()
                obj.rot_angle = self.angle_edit.value()
                obj.name = self.obj_name_combo.currentText()
                obj.type = self.editor.world.object_names.get(obj.name, obj.type)
                obj.ability = self.ability_spin.value()
                obj.ctext = self.ctext_edit.toPlainText()

                # Model and Texture IDs
                m_name = self.model_combo.currentText()
                if m_name:
                    obj.monsterid = find_model_id(self.editor.world, m_name)
                t_name = self.texture_combo.currentText()
                if t_name:
                    obj.monstertexture = find_texture_id(self.editor.world, t_name)

                obj.rcolour = self.r_spin.value()
                obj.gcolour = self.g_spin.value()
                obj.bcolour = self.b_spin.value()
                obj.dirx = self.dirx_spin.value()
                obj.diry = self.diry_spin.value()
                obj.dirz = self.dirz_spin.value()
                obj.ltype = self.ltype_combo.currentIndex()

                self.editor.ylocation = obj.y
                self.editor.object_rot_angle = int(obj.rot_angle)
                self.editor.ability = obj.ability
                self.editor.gctext = obj.ctext

                self.properties_changed.emit()


class PaletteWidget(QWidget):
    road_clicked = pyqtSignal(int)
    preset_clicked = pyqtSignal(str)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(4, 4, 4, 4)

        tabs = QTabWidget()

        # Roads Tab
        roads_widget = QWidget()
        r_grid = QGridLayout(roads_widget)
        r_grid.setSpacing(6)

        road_buttons = [
            ("Straight", STRAIGHT),
            ("Left Curve", LEFT_HAND_CURVE),
            ("Right Curve", RIGHT_HAND_CURVE),
            ("Small Straight", SMALL_STRAIGHT),
            ("Left Corner", LEFT_CORNER),
            ("Right Corner", RIGHT_CORNER),
            ("Crossroads", CROSSROAD),
            ("T-Junction", T_JUNCTION),
            ("Left T-Junction", LEFT_T_JUNCTION),
            ("Right T-Junction", RIGHT_T_JUNCTION),
            ("Zebra Crossing", ZEBRA),
        ]

        for i, (label, cmd) in enumerate(road_buttons):
            btn = QPushButton(label)
            btn.setMinimumHeight(32)
            btn.clicked.connect(lambda checked, c=cmd: self.road_clicked.emit(c))
            r_grid.addWidget(btn, i // 2, i % 2)

        tabs.addTab(roads_widget, "Road Sections")

        # Preset Objects Tab
        presets_widget = QWidget()
        p_grid = QGridLayout(presets_widget)
        p_grid.setSpacing(6)

        preset_items = [
            ("Flame / Torch", "!flamesnohit"),
            ("Torch Wall", "torch"),
            ("Lamp Post", "lamp_post"),
            ("Spotlight Lamp", "lamp_post"),
            ("Item: Diamond", "diamond"),
            ("Weapon: Sword", "BASTARDSWORD"),
            ("Potion", "POTION"),
            ("Monster: Goblin", "goblin"),
            ("Coins (5)", "coin_5"),
            ("Coins (10)", "coin_10"),
            ("House", "house_semileft"),
            ("Shop", "mbshop1"),
        ]

        for i, (label, p_id) in enumerate(preset_items):
            btn = QPushButton(label)
            btn.setMinimumHeight(32)
            btn.clicked.connect(lambda checked, p=p_id: self.preset_clicked.emit(p))
            p_grid.addWidget(btn, i // 2, i % 2)

        tabs.addTab(presets_widget, "Objects / Monsters")

        layout.addWidget(tabs)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Dungeon Stomp Level Editor")
        self.resize(1100, 750)

        # Initialize World and Editor
        self.world = World()
        load_object_data("objects.dat", self.world)
        self.editor = EditorState(self.world)

        # Auto load level1.map if exists
        if os.path.exists("level1.map"):
            load_map_in_editor("level1.map", self.world, self.editor.links_list)

        self.init_ui()

    def init_ui(self):
        # Canvas
        self.canvas = MapCanvas(self.editor)
        self.setCentralWidget(self.canvas)

        # Dock Widget for Palette & Properties
        self.dock = QDockWidget("Editor Controls & Inspector", self)
        self.dock.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)

        dock_container = QWidget()
        dock_layout = QVBoxLayout(dock_container)
        dock_layout.setContentsMargins(4, 4, 4, 4)

        # Palette
        self.palette = PaletteWidget()
        self.palette.road_clicked.connect(self.on_road_button_clicked)
        self.palette.preset_clicked.connect(self.on_preset_button_clicked)
        dock_layout.addWidget(self.palette)

        # Separator line
        line = QFrame()
        line.setFrameShape(QFrame.HLine)
        line.setFrameShadow(QFrame.Sunken)
        dock_layout.addWidget(line)

        # Property Inspector
        self.inspector = PropertyInspector(self.editor)
        self.inspector.properties_changed.connect(self.canvas.update)
        dock_layout.addWidget(self.inspector)

        self.dock.setWidget(dock_container)
        self.addDockWidget(Qt.RightDockWidgetArea, self.dock)

        # Connect Canvas events
        self.canvas.mouse_moved.connect(self.update_status_bar_coords)
        self.canvas.object_selected.connect(self.on_canvas_selection_changed)

        # Create Actions & Toolbars
        self.create_actions()
        self.create_menus()
        self.create_toolbar()
        self.create_statusbar()

        self.inspector.refresh_placed_objects_list()

    def create_actions(self):
        self.act_new = QAction("New / Clear Map", self)
        self.act_new.setShortcut(QKeySequence.New)
        self.act_new.triggered.connect(self.new_map)

        self.act_open = QAction("Open Map...", self)
        self.act_open.setShortcut(QKeySequence.Open)
        self.act_open.triggered.connect(self.open_map)

        self.act_save = QAction("Save Map", self)
        self.act_save.setShortcut(QKeySequence.Save)
        self.act_save.triggered.connect(self.save_map)

        self.act_save_as = QAction("Save Map As...", self)
        self.act_save_as.triggered.connect(self.save_map_as)

        self.act_exit = QAction("Exit", self)
        self.act_exit.setShortcut("Alt+F4")
        self.act_exit.triggered.connect(self.close)

        self.act_undo = QAction("Undo Road Link", self)
        self.act_undo.setShortcut(QKeySequence.Undo)
        self.act_undo.triggered.connect(self.undo_action)

        self.act_delete = QAction("Delete Selected Object", self)
        self.act_delete.setShortcut(QKeySequence.Delete)
        self.act_delete.triggered.connect(self.delete_action)

        self.act_rotate_45 = QAction("Rotate +45°", self)
        self.act_rotate_45.triggered.connect(self.rotate_45)

        self.act_rotate_90 = QAction("Rotate +90°", self)
        self.act_rotate_90.triggered.connect(self.rotate_90)

        self.act_zoom_in = QAction("Zoom In", self)
        self.act_zoom_in.triggered.connect(self.zoom_in)

        self.act_zoom_out = QAction("Zoom Out", self)
        self.act_zoom_out.triggered.connect(self.zoom_out)

        self.act_mode_road = QAction("Auto Road Mode", self, checkable=True)
        self.act_mode_road.setChecked(True)
        self.act_mode_road.triggered.connect(self.set_mode_road)

        self.act_mode_object = QAction("Object Placement Mode", self, checkable=True)
        self.act_mode_object.triggered.connect(self.set_mode_object)

    def create_menus(self):
        menubar = self.menuBar()

        file_menu = menubar.addMenu("&File")
        file_menu.addAction(self.act_new)
        file_menu.addAction(self.act_open)
        file_menu.addAction(self.act_save)
        file_menu.addAction(self.act_save_as)
        file_menu.addSeparator()
        file_menu.addAction(self.act_exit)

        edit_menu = menubar.addMenu("&Edit")
        edit_menu.addAction(self.act_undo)
        edit_menu.addAction(self.act_delete)
        edit_menu.addSeparator()
        edit_menu.addAction(self.act_rotate_45)
        edit_menu.addAction(self.act_rotate_90)

        view_menu = menubar.addMenu("&View")
        view_menu.addAction(self.act_zoom_in)
        view_menu.addAction(self.act_zoom_out)

        mode_menu = menubar.addMenu("&Mode")
        mode_menu.addAction(self.act_mode_road)
        mode_menu.addAction(self.act_mode_object)

    def create_toolbar(self):
        toolbar = QToolBar("Main Toolbar")
        self.addToolBar(toolbar)

        toolbar.addAction(self.act_new)
        toolbar.addAction(self.act_open)
        toolbar.addAction(self.act_save)
        toolbar.addSeparator()
        toolbar.addAction(self.act_undo)
        toolbar.addAction(self.act_delete)
        toolbar.addSeparator()
        toolbar.addAction(self.act_rotate_45)
        toolbar.addAction(self.act_rotate_90)
        toolbar.addSeparator()
        toolbar.addAction(self.act_zoom_in)
        toolbar.addAction(self.act_zoom_out)

    def create_statusbar(self):
        self.statusbar = QStatusBar()
        self.setStatusBar(self.statusbar)
        self.lbl_coord = QLabel("X: 0.0, Z: 0.0")
        self.lbl_scale = QLabel("Zoom: 25%")
        self.lbl_mode = QLabel("Mode: Auto Road")

        self.statusbar.addPermanentWidget(self.lbl_coord)
        self.statusbar.addPermanentWidget(self.lbl_scale)
        self.statusbar.addPermanentWidget(self.lbl_mode)

    def update_status_bar_coords(self, wx, wz):
        self.lbl_coord.setText(f"X: {wx:.1f}, Z: {wz:.1f}")
        self.lbl_scale.setText(f"Zoom: {int(self.editor.display_scale * 100)}%")

    def on_canvas_selection_changed(self, link_idx):
        self.inspector.refresh_placed_objects_list()

    def on_road_button_clicked(self, cmd):
        self.editor.draw_road_section(cmd)
        self.inspector.refresh_placed_objects_list()
        self.canvas.update()

    def on_preset_button_clicked(self, preset_id):
        self.editor.editor_mode = 1  # Switch to placement
        self.act_mode_object.setChecked(True)
        self.act_mode_road.setChecked(False)
        self.lbl_mode.setText("Mode: Object Placement")

        self.editor.current_object_name = preset_id
        self.editor.current_object_id = self.world.object_names.get(preset_id, OBJECT_ID_HOUSE)

        if preset_id == "!flamesnohit":
            self.editor.monstertexture = 0
            self.editor.modelname1 = ""
        elif preset_id == "torch":
            self.editor.monstertexture = 0
        elif preset_id == "goblin":
            self.editor.current_object_name = "!monster1"
            self.editor.current_object_id = self.world.object_names.get("!monster1", 5)
            self.editor.modelname1 = "GOBLIN"
            self.editor.monsterid = find_model_id(self.world, "GOBLIN")
        elif preset_id in ("diamond", "BASTARDSWORD", "POTION", "COIN"):
            self.editor.current_object_name = "!monster1"
            self.editor.current_object_id = self.world.object_names.get("!monster1", 5)
            self.editor.modelname1 = preset_id
            self.editor.monsterid = find_model_id(self.world, preset_id)

        self.inspector.update_inspector_fields()

    def set_mode_road(self):
        self.editor.editor_mode = 0
        self.act_mode_road.setChecked(True)
        self.act_mode_object.setChecked(False)
        self.lbl_mode.setText("Mode: Auto Road")

    def set_mode_object(self):
        self.editor.editor_mode = 1
        self.act_mode_object.setChecked(True)
        self.act_mode_road.setChecked(False)
        self.lbl_mode.setText("Mode: Object Placement")

    def undo_action(self):
        self.editor.undo_last_link()
        self.inspector.refresh_placed_objects_list()
        self.canvas.update()

    def delete_action(self):
        self.editor.delete_current_object()
        self.inspector.refresh_placed_objects_list()
        self.canvas.update()

    def rotate_45(self):
        self.editor.object_rot_angle = (self.editor.object_rot_angle + 45) % 360
        self.canvas.update()

    def rotate_90(self):
        self.editor.object_rot_angle = (self.editor.object_rot_angle + 90) % 360
        self.canvas.update()

    def zoom_in(self):
        self.editor.display_scale = min(4.0, self.editor.display_scale * 1.5)
        self.canvas.update()

    def zoom_out(self):
        self.editor.display_scale = max(0.02, self.editor.display_scale / 1.5)
        self.canvas.update()

    def new_map(self):
        self.editor.init_link_list()
        self.world.oblist.clear()
        self.inspector.refresh_placed_objects_list()
        self.canvas.update()

    def open_map(self):
        fname, _ = QFileDialog.getOpenFileName(self, "Open Dungeon Stomp Map", "", "Map Files (*.map);;All Files (*)")
        if fname:
            self.editor.init_link_list()
            if load_map_in_editor(fname, self.world, self.editor.links_list):
                self.inspector.refresh_placed_objects_list()
                self.canvas.update()
                self.statusbar.showMessage(f"Loaded {fname}", 3000)

    def save_map(self):
        if save_map("level1.map", self.world, self.editor.links_list):
            self.statusbar.showMessage("Map saved to level1.map and level1.cmp", 3000)
            QMessageBox.information(self, "Save Successful", "Map saved to level1.map and precompiled level1.cmp!")

    def save_map_as(self):
        fname, _ = QFileDialog.getSaveFileName(self, "Save Map As", "level1.map", "Map Files (*.map);;All Files (*)")
        if fname:
            if save_map(fname, self.world, self.editor.links_list):
                self.statusbar.showMessage(f"Map saved to {fname}", 3000)
