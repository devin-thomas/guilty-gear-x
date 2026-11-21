# Tagged compiler stand-ins

These functions pass the same strict code, data, padding, and full-ELF gate as every other
counted function. Their tagged constructs tune compilation and are not claimed to be the
original source form.

| Function | File | What is forced | Natural attempts |
| --- | --- | --- | --- |
| ScoreMain | src/ggxeuc/fight.c | A code-free dead store prevents a final FontDraw sibling call. | Thirteen control-flow forms produced 1,160-1,180 bytes instead of 1,144. |
| CPURandAttack | src/ggxeuc/cpu.c | An empty-effect switch case preserves dispatch and register allocation. | Seventeen natural forms failed with both game assemblers. |
| TensionDraw | src/ggxeuc/cockpit.c | Register pins, two empty asm statements, and a deleted branch preserve the initial division block's allocation and schedule. | Natural C has the correct 1,456-byte size and data, but sixteen differing words after thirty variants. |
| SetItigekiVallSpeed | src/ggxeuc/vee_act.c | A dead `if (0) polydraw_PBox(0, ...)` (nine float arguments): cc1 expands it, giving 16 bytes of outgoing-argument space at sp+0, then deletes it. The frame is 0x50 with those bytes untouched and the final `j SetYSpeed` sibling call kept. X Plus's `do last = ...; while (0);` keeps `li rst,1` in the GetEnemyPlayer jal slot (2 words without it). | 300 random natural variants (types and `register` of every local, loop, ABS and expression shapes, temporaries, statement order, tail forms) and 9 long long, double and float temporaries: all frame 0x40. Any local or address-taken value present at expansion kills the sibling call (cc1's sibcall pass checks frame_offset right after expansion). A later reload or caller-save slot would need a spill this code never has. |
| func_00178CF0 (X Plus DZBTAct_SHW1_HomingLaser) | src/ggxeuc/dze_act.c | X Plus's empty volatile asm after `last` (tagged SCHED_BARRIER there) keeps the `last` load out of the GetEnemyPlayer schedule block, so `li rst,1` fills the jal slot and the attackst test is a beql. An unused `NL_POINT3` local gives the original's 16 untouched frame bytes above c_arg (0x70, natural C 0x60). | `last` hoisted before the test: 1 word (beqz for beql). In the else, after the if, in both arms, chase block in the else, do/while(0) around `last` or the chase block: 2 to 9 words. `rst = 1` at its declaration, before or after the call: no change. Inverted test: 122. Earlier passes: last/rst types, inline helpers, function-scope rst. |

TensionDraw's two asm statements are empty and emit no game instructions. All drawing logic
is C. Its tagged form comes from the Guilty Gear X Plus decompilation,
with X's sprite IDs. The source comments identify every forcing construct.

ObjCollisionExecute initially inherited a dead-store workaround. A later change replaced it with a
semantically used DefaultColliFlag, preserving the bytes naturally; it is no longer listed.

Remove an entry when natural C replaces its tags and passes the integrated gate.
