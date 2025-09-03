# SagaShakeTree
This is a Mod for the PC Game "DayZ".

To change the ingame message text, go to file ActionAppleTree.c and find line 26 and 80.
m_Text = "Shake Tree"; <------ you can change the text "Shake Tree" in your language.

What can this mod?
This mod allows you to shake fruit trees to get fruit.
It works with
plum trees
pear trees
apple trees

You can set the probability of fruit appearing in the configuration file
profiles/SagaShakeTree/

    "settings_version": 4,  // DON`T TOUCH THIS
    "apple_drop_chance": 0.10000000149011612,  // DROP CHANCE OF APPLE 1.0 means 100% and 0.25 means          25%
    "plum_drop_chance": 0.10000000149011612,  // DROP CHANCE OF PEAR 1.0 means 100% and 0.25 means 25%
    "pear_drop_chance": 0.10000000149011612,  // DROP CHANCE OF PEAR 1.0 means 100% and 0.25 means 25%
    "shake_duration_seconds": 10.0,		//Time the player must hold the action to shake.
    NEW:
    "tree_cooldown_minutes": 60,	//Per-tree cooldown before it can be shaken again.
    "apple_min_count": 2,  // Min/max number of apples spawned on a successful shake.
    "apple_max_count": 4,	// Min/max number of apples spawned on a successful shake.
    "plum_min_count": 2,	// Min/max number of plum spawned on a successful shake.
    "plum_max_count": 4,	// Min/max number of plum spawned on a successful shake.
    "pear_min_count": 2,	// Min/max number of pear spawned on a successful shake.
    "pear_max_count": 4,	// Min/max number of pear spawned on a successful shake.
    "food_stage_enable": 1, //Enables optional food stages (dried/rotten) for spawned fruit.
    "food_dried_chance": 0.15000000596046449, //Probability a spawned fruit is “dried” (only if food_stage_enable = true).
    "food_rotten_chance": 0.05000000074505806 Probability a spawned fruit is “rotten” (only if food_stage_enable = true).

it does mean:
1.0  (100%) you always get fruit when you shake it.
lower like 0.5 or 0.25 (50% or 25%) you get less fruit when you shake it.

You can do, what u want! Repack is allowed! Feel free, to use the mod in your serverpack!
But please give me credit in your serverpacket and workshop page!

i am happy, you support me with a little tip! :-)
https://ko-fi.com/hummigummi
