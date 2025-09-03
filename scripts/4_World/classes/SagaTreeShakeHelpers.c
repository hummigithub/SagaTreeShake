enum eFruitType
{
    NONE = 0,
    APPLE = 1,
    PLUM = 2,
    PEAR = 3
}

class SagaTreeShakeHelpers
{
    static ref TStringArray s_AppleTreeWhitelist;
    static ref TStringArray s_PlumTreeWhitelist;
    static ref TStringArray s_PearTreeWhitelist;

    static ref TStringArray s_AppleTreeHints;
    static ref TStringArray s_PlumTreeHints;
    static ref TStringArray s_PearTreeHints;

    static void Init()
    {
        if (!s_AppleTreeWhitelist) s_AppleTreeWhitelist = new TStringArray;
        if (!s_PlumTreeWhitelist)  s_PlumTreeWhitelist  = new TStringArray;
        if (!s_PearTreeWhitelist)  s_PearTreeWhitelist  = new TStringArray;

        if (s_AppleTreeWhitelist.Count() == 0)
        {
            s_AppleTreeWhitelist.Insert("TreeSoft_t_malusDomestica_1s");
            s_AppleTreeWhitelist.Insert("TreeSoft_t_malusDomestica_2s");
        }
        if (s_PlumTreeWhitelist.Count() == 0)
        {
            s_PlumTreeWhitelist.Insert("TreeHard_t_prunusDomestica_2s");
        }
        if (s_PearTreeWhitelist.Count() == 0)
        {
            s_PearTreeWhitelist.Insert("TreeHard_t_pyrusCommunis_3s");
        }

        if (!s_AppleTreeHints) s_AppleTreeHints = new TStringArray;
        if (!s_PlumTreeHints)  s_PlumTreeHints  = new TStringArray;
        if (!s_PearTreeHints)  s_PearTreeHints  = new TStringArray;

        if (s_AppleTreeHints.Count() == 0)
        {
            s_AppleTreeHints.Insert("malus");
            s_AppleTreeHints.Insert("malusdomestica");
            s_AppleTreeHints.Insert("b_apple");
            s_AppleTreeHints.Insert("apple");
        }
        if (s_PlumTreeHints.Count() == 0)
        {
            s_PlumTreeHints.Insert("prunus");
            s_PlumTreeHints.Insert("prunusdomestica");
            s_PlumTreeHints.Insert("b_plum");
            s_PlumTreeHints.Insert("plum");
        }
        if (s_PearTreeHints.Count() == 0)
        {
            s_PearTreeHints.Insert("pyrus");
            s_PearTreeHints.Insert("pyruscommunis");
            s_PearTreeHints.Insert("b_pear");
            s_PearTreeHints.Insert("pear");
        }
    }

    static bool IsFruitTree(Object obj, out int fruitType)
    {
        fruitType = eFruitType.NONE;
        if (!obj) return false;

        if (obj.IsInherited(ItemBase)) return false;
        if (obj.IsInherited(Man)) return false;

        Init();

        string typeName = obj.GetType();

    if (s_AppleTreeWhitelist.Find(typeName) != -1) { fruitType = eFruitType.APPLE; return true; }
    if (s_PlumTreeWhitelist.Find(typeName)  != -1) { fruitType = eFruitType.PLUM;  return true; }
    if (s_PearTreeWhitelist.Find(typeName)  != -1) { fruitType = eFruitType.PEAR;  return true; }
        if (GetGame())
        {
            string cfgPath = "CfgVehicles " + typeName + " fruitType";
            if (GetGame().ConfigIsExisting(cfgPath))
            {
                string ftype;
                GetGame().ConfigGetText(cfgPath, ftype);
                ftype.ToLower();
                if (ftype == "apple") { fruitType = eFruitType.APPLE; return true; }
                if (ftype == "plum")  { fruitType = eFruitType.PLUM;  return true; }
                if (ftype == "pear")  { fruitType = eFruitType.PEAR;  return true; }
            }
        }

        string tLower = typeName;
        tLower.ToLower();
        bool looksLikeTree = tLower.Contains("tree") || (tLower.IndexOf("b_") == 0);
        if (!looksLikeTree)
            return false;

        foreach (string h1 : s_AppleTreeHints) if (tLower.Contains(h1)) { fruitType = eFruitType.APPLE; return true; }
        foreach (string h2 : s_PlumTreeHints)  if (tLower.Contains(h2)) { fruitType = eFruitType.PLUM;  return true; }
        foreach (string h3 : s_PearTreeHints)  if (tLower.Contains(h3)) { fruitType = eFruitType.PEAR;  return true; }

        return false;
    }

    static int RollFoodStage()
    {
        SagaTreeShakeSettings s = SagaTreeShakeSettings.Get();
        if (!s || !s.food_stage_enable)
            return -1;

        float dried  = Math.Clamp(s.food_dried_chance,  0.0, 1.0);
        float rotten = Math.Clamp(s.food_rotten_chance, 0.0, 1.0);

        float sum = dried + rotten;
        if (sum > 1.0)
        {
            dried  = dried  / sum;
            rotten = rotten / sum;
        }

        float r = Math.RandomFloat01();
        if (r < dried)              return FoodStageType.DRIED;
        if (r < dried + rotten)     return FoodStageType.ROTTEN;
        return -1;
    }

    static void ApplyFoodStage(Edible_Base edible, int stage)
    {
        if (!edible) return;
        if (stage < 0) return;

        FoodStage fs = edible.GetFoodStage();
        if (fs)
        {
            fs.ChangeFoodStage(stage);
        }
    }

    static void SpawnFruitInFront(PlayerBase player, string className, int count)
    {
        if (!GetGame() || !player) return;

        vector pos = player.GetPosition();
        vector dir = player.GetDirection();
        dir.Normalize();

        vector dropCenter = pos + dir * 1.0;

        for (int i = 0; i < count; i++)
        {
            vector offset = "0 0 0";
            offset[0] = Math.RandomFloat(-0.35, 0.35);
            offset[2] = Math.RandomFloat(-0.35, 0.35);
            vector spawnPos = dropCenter + offset;

            Object o = GetGame().CreateObjectEx(className, spawnPos, ECE_PLACE_ON_SURFACE);

            int stage = RollFoodStage();
            if (stage >= 0)
            {
                Edible_Base edible = Edible_Base.Cast(o);
                if (edible)
                {
                    ApplyFoodStage(edible, stage);
                }
            }
        }
    }
}