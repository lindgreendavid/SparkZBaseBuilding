# Standalone Construction Design

## Status

This document records the first agreed design direction for the standalone
SparkZ base-building system. It is a product/design specification, not a
claim that these features have already been implemented.

Where this document conflicts with the old BBP-hooked prototype, this
standalone design takes precedence together with `docs/BRIEF.md`. No BBP
code, assets, runtime dependency, or hooks will be used.

## Product direction

SparkZBaseBuilding will be a completely standalone DayZ construction mod
with its own models, kits, placement controller, snapping, ownership,
persistence, dismantling, and eventual raid integration.

Useful patterns may be carried over from SparkZStorage and the old
SparkZBase prototype where they genuinely fit, particularly
server-authoritative placement, persistent ownership, animated collision,
tool-based dismantling, and SparkZGroup/SparkZCore integration. Their code
and behaviour must not be copied blindly into the new system.

## Initial construction set

### First model prototype

The first asset under active development is the wooden wall with an animated
door. Its editable source lives under `Source/Models/WoodDoorWall/`.

The initial Blender prototype uses a 4.00 m-wide × 4.25 m-tall outer wall,
with a 1.20 m-wide × 2.45 m-tall clear doorway target. These measurements
are provisional review values, not yet the approved common module standard.
They must be checked visually and against real DayZ player and vehicle
collision envelopes before the remaining model family inherits them.

The prototype is made from individually modelled, UV-mapped boards with a
clean square timber frame. Its animated `door` hierarchy, hinge-axis memory
points, preliminary snap markers, and simplified Geometry, Fire Geometry,
and View Geometry proxies are authored in the Blender source. P3D export,
`model.cfg`, config registration, runtime actions, persistence, and in-game
collision/ballistics validation remain separate implementation work.

The first model set is expected to include:

- full and half wood walls
- wood wall with an interactive door
- wood wall with an interactive window
- wood garage-door wall with an interactive garage door
- full, half, and triangular floor pieces
- wooden stairs
- wood-and-glass wall
- metal wall variants with interactive door, window, and garage door

The exact module dimensions, pivots, origins, named selections, animation
axes, collision LODs, and snap sockets must be standardized before the model
set is considered final. Wood and other materials may visually use suitable
DayZ assets where permitted, but no third-party workshop mod assets or code
will be copied.

### Modular dimension rules

Compatible parts share a common horizontal module span, but they do not
share every dimension. In particular:

- a full wall, a half-height wall, and one edge of a full floor use the same
  horizontal span
- a floor snapped to the bottom socket of a wall fits that wall exactly
  across its width
- a half wall retains the full horizontal span and reduces the wall height;
  a separate half-width part, if added later, must be named and dimensioned
  separately
- full wall height is deliberately greater than the edge length of a floor
- rotating a floor upright therefore does not make it a functional
  substitute for a wall: it leaves a deliberate shortfall at the top when
  aligned with the normal wall/floor module
- model pivots and snap sockets, rather than visible mesh bounds alone,
  define the exact connection plane

The final numeric dimensions remain unresolved until the common module
specification is approved. Whichever values are chosen must preserve these
relationships mathematically so repeated floors and walls remain aligned
without cumulative gaps.

### Wall and opening clearance

Every full-height wall variant shares the same approved structural height.
That includes plain walls, door walls, window walls, and garage-door walls.
The wall height must be comfortably greater than a standing player's height,
not merely equal to the character envelope.

Ordinary door openings must provide enough clear width and height for a
player to walk through naturally without crouching, clipping the frame, or
catching on hidden collision. The visible opening and every relevant
collision LOD must agree on that clearance.

A garage-door wall has exactly the same outer width and height as every
other standard full wall. It is one standard module, not a multi-module or
double-width exception. The animated garage-door panel and its opening sit
inside that common wall envelope.

The fully open doorway must still be wide and tall enough for the largest
supported vehicle, including a truck, to pass through with practical
clearance for mirrors, bodywork, suspension movement, steering error, and
the driver's imperfect alignment. Consequently, the common wall and floor
module span must be chosen wide enough to contain that vehicle opening plus
the garage frame's side posts. Ordinary walls, door walls, window walls,
garage walls, and the matching floor edge all retain that same horizontal
span and snap footprint.

Exact doorway and garage-opening measurements will be chosen only after
checking the real player collision envelope and the bounding/collision
dimensions of the intended supported DayZ vehicles. Visual model dimensions
alone are not sufficient acceptance criteria.

## Kit and placement flow

Each structure is deployed from a corresponding kit held by the player.
Starting placement opens a compact on-screen control legend that remains
visible only while placement mode is active.

The placement controller is intended to support:

- independent X, Y, and Z axis selection
- movement along the selected axis
- rotation around the selected axis
- normal and micro adjustment increments
- freezing and unfreezing the hologram
- enabling and disabling snapping
- cycling or selecting an intended snap side
- confirming or cancelling placement

Inputs must be registered as configurable mod controls. Suggested keys such
as F, Page Up, and Page Down are provisional defaults, not hard-coded
assumptions, because they may conflict with normal DayZ controls.

### Terrain-independent transforms

Terrain must not force the hologram's height, pitch, roll, or final
alignment. A player may deliberately:

- place elevated floors
- position a wall above the player
- sink a wall partially through a floor
- build on slopes without inheriting the terrain angle
- fine-tune a piece after snapping

Intersection warnings may be shown through hologram colour, but ordinary
intersection alone should not prevent confirmation. Firm restrictions such
as territory, player distance, and server-side validity still apply.

### Frozen hologram inspection

The player can freeze the hologram at its current world transform, walk
around it, inspect it from every side, and continue making axis-based
adjustments. Unfreezing returns it to the normal aim-following placement
mode.

Confirmation sends the exact preview transform to the server. The server
must independently validate the transform before creating the structure or
consuming the kit, ensuring the persistent object matches the accepted
preview.

### Placement reach

Placement should not be restricted to the small radius used by
SparkZStorage. It must support upper-storey and slightly distant placement,
including pieces higher than the player.

The intended hard maximum is approximately 15 metres from the player.
Final distance measurement should account for large models so a nearby edge
is not rejected merely because the model origin is farther away.

## Snapping model

Snapping will use authored sockets on construction models rather than
guessing connections from bounding boxes. Depending on the part, sockets
may include left, right, top, bottom, floor edges, corners, stair
connections, and triangular edges.

When the player aims near an existing part, the system selects the
appropriate visible or targeted side and aligns the compatible socket on the
new piece. It should support:

- snapping to either side of compatible parts
- orientation inherited from the existing structure
- cycling nearby candidate sockets where more than one is valid
- flipping or rotating the proposed snapped orientation
- disabling snapping for unrestricted placement
- freezing a snapped result for inspection
- leaving snapped mode when manual micro-adjustment intentionally breaks the
  exact connection

All compatible pieces must be derived from one dimensional specification so
repeated joins do not accumulate gaps or drift.

## Plot pole and build territory

A plot pole displaying wording such as "Construction in Progress" will act
as the base anchor. Its placement preview must show the complete proposed
build boundary so the player understands the territory before confirming
it.

The intended build area is 65 metres and construction must not extend
beyond it. The following details remain to be confirmed before
implementation:

- whether 65 metres means radius or total diameter
- whether the boundary is cylindrical or spherical
- the permitted vertical range
- minimum spacing and overlap rules between plot poles

All construction placement will be server-authoritative and checked against
the active plot. The initial ownership requirement is owner-only placement
and dismantling. The persisted schema should leave room for later
SparkZGroup roles or explicitly authorized builders without invalidating
existing structures.

## Ownership and dismantling

Every placed structure must retain persistent ownership data. Initially,
only the authorized owner may place within the territory or dismantle its
parts.

Dismantling requires the configured screwdriver, returns the correct kit,
and may damage the tool slightly, following the successful broad pattern
used by SparkZStorage. The server must perform the final permission, tool,
territory, and object-state checks.

## Damage and future raiding

Construction pieces, doors, windows, floors, stairs, and the plot pole will
ignore ordinary damage, including:

- bullets
- melee attacks
- grenades
- ordinary explosives
- incidental impact and environmental damage

Indestructibility applies to damage state, not projectile collision. Every
solid construction model must include an appropriate bullet-blocking
Fire Geometry LOD and the required material/component configuration so
bullets cannot pass through walls, floors, stairs, closed windows, closed
doors, or closed garage doors. Visual geometry alone is not sufficient.

Animated ballistic geometry must follow the moving selection. A closed door
or garage door must stop projectiles; once opened, its ballistic obstruction
must move with the panel so the doorway itself is genuinely clear. Window
behaviour must follow the same rule for whichever portion is designed to
open. Geometry, View Geometry, Fire Geometry, roadway/walkable surfaces,
and ordinary collision must be validated separately because success in one
LOD does not prove the others are correct.

Raiding will later be implemented through a purpose-built C4 system specific
to SparkZBaseBuilding. The construction design should therefore reserve an
explicit raid/breach path that future C4 can invoke while keeping ordinary
damage ineffective. Future C4 is intended to breach doors and garage doors,
not make ordinary bullets or grenades effective. No third-party breaching
schema or class names will be guessed.

## Door and window interaction

Doors, windows, and garage doors will be animated and interactable. Their
visual animation, collision state, persistence, and multiplayer
synchronization must remain aligned.

The following access details remain open:

- whether unlocked doors and windows may be operated by everyone
- whether operation is owner/squad-only before locks exist
- how future lock objects attach and alter access
- whether windows merely open or can later be breached as raid targets

## Workbench and crafting

A dedicated workbench is an essential later feature. It will allow players
to craft construction kits from wood, nails, and other configured materials.
Workbench recipes, materials, timings, and permissions will be designed
after the initial models and placement behaviour are proven.

## Development sequence

1. Define the common dimensional, pivot, animation, collision, and snap
   conventions.
2. Produce the initial models against that specification.
3. Implement a floor and plain wall as the placement-controller prototype.
4. Prove free placement, axis rotation, freeze mode, micro adjustment,
   distance validation, and basic side snapping.
5. Add animated door and window pieces and validate animated collision and
   persistence.
6. Extend the proven system to the remaining floor, stair, garage, glass,
   and metal models.
7. Add the plot pole boundary preview, ownership, and territory validation.
8. Add persistent ownership and screwdriver dismantling.
9. Add the workbench and crafting economy.
10. Add the dedicated C4 raid system.

## Implementation safeguards

- The final object transform must be server-authoritative.
- A kit must only be consumed after successful server-side placement.
- Cancelled or rejected placement must not lose the kit.
- Restart persistence must preserve transform, owner, open/closed state, and
  any other required entity state.
- Client hologram controls must not be trusted for permission, range, or
  territory validation.
- No implementation work begins from the old BBP hooks.
- SparkZGroup and SparkZCore snapshots require the cross-repository workflow
  in `docs/CONTRIBUTING.md` before modification.
- Actual DayZ script compilation and multiplayer/in-game testing remain
  required; static review is not a substitute.

## Decisions still required

Before construction code begins, collaborators should agree and record:

- exact dimensions for every modular part
- the meaning of the 65-metre boundary and its vertical shape
- default and fine movement/rotation increments
- default input bindings
- snap tolerance and candidate-selection behaviour
- owner-only versus squad-role placement and dismantling
- door/window access before locks are implemented
- instant kit-to-object placement versus an additional build-progress stage
- per-entity persistence versus a central structure registry, or a hybrid
- exact model/material sources and usage permissions
