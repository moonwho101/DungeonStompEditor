# Dungeon Stomp Level Editor (Python)

![Dungeon Stomp Level Editor](../main/src/screenshot1.jpg)

![Dungeon Stomp Level Editor](../main/src/screenshot2.jpg)

This is the Python conversion of the 2D Dungeon Stomp Level Editor. All original level data formats (`.map`, `.dat`, `.cmp`), road section mathematics, object structures, and light/monster parameters are preserved and fully compatible, with a modernized PyQt5 interface.

## Requirements

- Python 3.8+
- PyQt5

Install dependencies:

```bash
pip install -r requirements.txt
```

## Running the Editor

To start the level editor:

```bash
python3 src/main.py
```

## Running Tests

To run the test suite:

```bash
python3 tests/test_editor.py
python3 tests/test_world.py
python3 tests/test_editor_logic.py
```

## Features

- **2D Map Viewport**: Grid background (20-unit minor grid, 260-unit cell grid), wireframe object rendering, crosshair marker, panning (middle-click or ctrl-drag), and zooming (mouse wheel or buttons).
- **Auto Road Mode**: Connected road creation (Straight, Curves, Corners, T-Junctions, Crossroads, Zebra Crossing).
- **Object Placement Mode**: Snapped grid placement for monsters, torches, lamp posts, houses, items, weapons, and shops.
- **Inspector Panel**: Detailed controls for position Y, rotation angle, models, textures, parameter/ability, text, and lighting source properties (R/G/B colors, direction vectors, light types).
- **File Format Compatibility**: Fully loads and saves `.map` level files and exports precompiled grid `.cmp` files.
