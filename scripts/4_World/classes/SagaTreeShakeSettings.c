class SagaTreeShakeSettings
{
    static const int CURRENT_SETTINGS_VERSION = 4;
    int settings_version = -1;

    float apple_drop_chance = 0.10;
    float plum_drop_chance  = 0.10;
    float pear_drop_chance  = 0.10;

    float shake_duration_seconds = 10.0;

    int tree_cooldown_minutes = 60;

    int apple_min_count = 2;
    int apple_max_count = 4;

    int plum_min_count  = 2;
    int plum_max_count  = 4;

    int pear_min_count  = 2;
    int pear_max_count  = 4;

    bool  food_stage_enable   = true;
    float food_dried_chance   = 0.15;
    float food_rotten_chance  = 0.05;

    static const string DIR  = "$profile:\\SagaTreeShake";
    static const string FILE = "$profile:\\SagaTreeShake\\settings.json";

    private static ref SagaTreeShakeSettings s_Instance;

    static SagaTreeShakeSettings Get()
    {
        if (!s_Instance)
        {
            s_Instance = new SagaTreeShakeSettings();
            s_Instance.Load();
        }
        return s_Instance;
    }

    void Load()
    {
        if (!FileExist(DIR))
        {
            MakeDirectory(DIR);
            Print("[SagaTreeShake] Created profiles directory: " + DIR);
        }

        if (FileExist(FILE))
        {
            JsonFileLoader<SagaTreeShakeSettings>.JsonLoadFile(FILE, this);

            if (settings_version != CURRENT_SETTINGS_VERSION)
            {
                int oldVer = settings_version;

                SagaTreeShakeSettings old = this;
                SagaTreeShakeSettings fresh = new SagaTreeShakeSettings();

                fresh.apple_drop_chance   = old.apple_drop_chance;
                fresh.plum_drop_chance    = old.plum_drop_chance;
                fresh.pear_drop_chance    = old.pear_drop_chance;

                fresh.shake_duration_seconds = old.shake_duration_seconds;

                fresh.apple_min_count = old.apple_min_count;
                fresh.apple_max_count = old.apple_max_count;

                fresh.plum_min_count = old.plum_min_count;
                fresh.plum_max_count = old.plum_max_count;

                fresh.pear_min_count = old.pear_min_count;
                fresh.pear_max_count = old.pear_max_count;

                fresh.food_stage_enable  = old.food_stage_enable;
                fresh.food_dried_chance  = old.food_dried_chance;
                fresh.food_rotten_chance = old.food_rotten_chance;

                fresh.tree_cooldown_minutes = old.tree_cooldown_minutes;

                fresh.settings_version = CURRENT_SETTINGS_VERSION;
                fresh.Clamp();

                JsonFileLoader<SagaTreeShakeSettings>.JsonSaveFile(FILE, fresh);

                JsonFileLoader<SagaTreeShakeSettings>.JsonLoadFile(FILE, this);

                Print("[SagaTreeShake] settings.json version mismatch (found=" + oldVer + ", expected=" + CURRENT_SETTINGS_VERSION + "). Merged user values into new defaults and saved.");
            }
            else
            {
                Clamp();
                Print("[SagaTreeShake] Loaded settings (version " + settings_version + ")");
            }
        }
        else
        {
            settings_version = CURRENT_SETTINGS_VERSION;
            Clamp();
            JsonFileLoader<SagaTreeShakeSettings>.JsonSaveFile(FILE, this);
            Print("[SagaTreeShake] Created default settings at: " + FILE + " (version " + settings_version + ")");
        }
    }

    void Save()
    {
        if (settings_version != CURRENT_SETTINGS_VERSION)
            settings_version = CURRENT_SETTINGS_VERSION;

        Clamp();
        JsonFileLoader<SagaTreeShakeSettings>.JsonSaveFile(FILE, this);
    Print("[SagaTreeShake] Saved settings (version " + settings_version + ") to: " + FILE);
    }

    void Clamp()
    {
        apple_drop_chance = Math.Clamp(apple_drop_chance, 0.0, 1.0);
        plum_drop_chance  = Math.Clamp(plum_drop_chance,  0.0, 1.0);
        pear_drop_chance  = Math.Clamp(pear_drop_chance,  0.0, 1.0);

        if (shake_duration_seconds < 0.5) shake_duration_seconds = 0.5;
        if (shake_duration_seconds > 60.0) shake_duration_seconds = 60.0;

    if (tree_cooldown_minutes < 0) tree_cooldown_minutes = 0;
    if (tree_cooldown_minutes > 10080) tree_cooldown_minutes = 10080;

        ClampPair(apple_min_count, apple_max_count);
        ClampPair(plum_min_count,  plum_max_count);
        ClampPair(pear_min_count,  pear_max_count);

        food_dried_chance  = Math.Clamp(food_dried_chance,  0.0, 1.0);
        food_rotten_chance = Math.Clamp(food_rotten_chance, 0.0, 1.0);
    }

    void ClampPair(inout int minC, inout int maxC)
    {
        if (minC < 0)  minC = 0;
        if (maxC < 0)  maxC = 0;
        if (minC > 50) minC = 50;
        if (maxC > 50) maxC = 50;
        if (minC > maxC)
        {
            int t = minC;
            minC = maxC;
            maxC = t;
        }
    }

    float ChanceFor(int fruitType)
    {
        switch (fruitType)
        {
            case eFruitType.APPLE: return apple_drop_chance;
            case eFruitType.PLUM:  return plum_drop_chance;
            case eFruitType.PEAR:  return pear_drop_chance;
        }
        return 0.0;
    }

    void CountRangeFor(int fruitType, out int minC, out int maxC)
    {
        switch (fruitType)
        {
            case eFruitType.APPLE:
                minC = apple_min_count; maxC = apple_max_count; return;
            case eFruitType.PLUM:
                minC = plum_min_count;  maxC = plum_max_count;  return;
            case eFruitType.PEAR:
                minC = pear_min_count;  maxC = pear_max_count;  return;
        }
        minC = 0; maxC = 0;
    }
}
