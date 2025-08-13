class SagaTreeShakeSettings
{
    // Drop-Chancen (0.0–1.0)
    float apple_drop_chance = 0.10; // 10%
    float plum_drop_chance  = 0.10; // 10%
    float pear_drop_chance  = 0.10; // 10%

    // Schüttel-Dauer (Sekunden)
    float shake_duration_seconds = 10.0;

    // Drop-Mengen pro Frucht
    int apple_min_count = 2;
    int apple_max_count = 4;

    int plum_min_count  = 2;
    int plum_max_count  = 4;

    int pear_min_count  = 2;
    int pear_max_count  = 4;

    // Profile-Pfade
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
            Clamp();
            Print("[SagaTreeShake] Loaded settings");
        }
        else
        {
            Clamp();
            JsonFileLoader<SagaTreeShakeSettings>.JsonSaveFile(FILE, this);
            Print("[SagaTreeShake] Created default settings at: " + FILE);
        }
    }

    void Save()
    {
        Clamp();
        JsonFileLoader<SagaTreeShakeSettings>.JsonSaveFile(FILE, this);
    }

    // Sicherheitsklemmen und Plausibilitäten
    void Clamp()
    {
        apple_drop_chance = Math.Clamp(apple_drop_chance, 0.0, 1.0);
        plum_drop_chance  = Math.Clamp(plum_drop_chance,  0.0, 1.0);
        pear_drop_chance  = Math.Clamp(pear_drop_chance,  0.0, 1.0);

        // Dauer (0.5s – 60s)
        if (shake_duration_seconds < 0.5) shake_duration_seconds = 0.5;
        if (shake_duration_seconds > 60.0) shake_duration_seconds = 60.0;

        // Mengen (0 – 50) und min <= max sicherstellen
        ClampPair(apple_min_count, apple_max_count);
        ClampPair(plum_min_count,  plum_max_count);
        ClampPair(pear_min_count,  pear_max_count);
    }

    // inout: Eingabewert wird gelesen und kann verändert werden
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