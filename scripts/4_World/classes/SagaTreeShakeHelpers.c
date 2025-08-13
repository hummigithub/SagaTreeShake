enum eFruitType
{
    NONE = 0,
    APPLE = 1,
    PLUM = 2,
    PEAR = 3
}

class SagaTreeShakeHelpers
{
    // Whitelists (konkrete Baum-Klassen – ergänzbar je nach Map)
    static ref TStringArray s_AppleTreeWhitelist;
    static ref TStringArray s_PlumTreeWhitelist;
    static ref TStringArray s_PearTreeWhitelist;

    // Heuristische Hints (falls Klassen variieren)
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
            // Bekannte Apfelbäume (bei Bedarf erweitern)
            s_AppleTreeWhitelist.Insert("TreeSoft_t_malusDomestica_1s");
            s_AppleTreeWhitelist.Insert("TreeSoft_t_malusDomestica_2s");
            // Optional: Falls deine Map “Hard”-Varianten hat, kannst du sie hier ergänzen
            // s_AppleTreeWhitelist.Insert("TreeHard_t_malusDomestica_3s");
        }
        if (s_PlumTreeWhitelist.Count() == 0)
        {
            // Deine Pflaumenbaum-Klasse
            s_PlumTreeWhitelist.Insert("TreeHard_t_prunusDomestica_2s");
            // Optional: weitere Varianten, falls vorhanden
            // s_PlumTreeWhitelist.Insert("TreeSoft_t_prunusDomestica_1s");
        }
        if (s_PearTreeWhitelist.Count() == 0)
        {
            // Deine Birnenbaum-Klasse
            s_PearTreeWhitelist.Insert("TreeHard_t_pyrusCommunis_3s");
            // Optional: weitere Varianten, falls vorhanden
            // s_PearTreeWhitelist.Insert("TreeSoft_t_pyrusCommunis_1s");
        }

        if (!s_AppleTreeHints) s_AppleTreeHints = new TStringArray;
        if (!s_PlumTreeHints)  s_PlumTreeHints  = new TStringArray;
        if (!s_PearTreeHints)  s_PearTreeHints  = new TStringArray;

        if (s_AppleTreeHints.Count() == 0)
        {
            s_AppleTreeHints.Insert("apple");
            s_AppleTreeHints.Insert("malus");
            s_AppleTreeHints.Insert("malusdomestica");
            s_AppleTreeHints.Insert("b_apple");
        }
        if (s_PlumTreeHints.Count() == 0)
        {
            s_PlumTreeHints.Insert("plum");
            s_PlumTreeHints.Insert("prunus");
            s_PlumTreeHints.Insert("prunusdomestica");
            s_PlumTreeHints.Insert("b_plum");
        }
        if (s_PearTreeHints.Count() == 0)
        {
            s_PearTreeHints.Insert("pear");
            s_PearTreeHints.Insert("pyrus");
            s_PearTreeHints.Insert("pyruscommunis");
            s_PearTreeHints.Insert("b_pear");
        }
    }

    // Ermittelt, ob es ein Obstbaum ist, und liefert den Typ (Apple/Plum/Pear)
    static bool IsFruitTree(Object obj, out int fruitType)
    {
        fruitType = eFruitType.NONE;
        if (!obj) return false;

        Init();

        string typeName = obj.GetType();

        // 1) Exakte Whitelist
        if (s_AppleTreeWhitelist.Find(typeName) != -1) { fruitType = eFruitType.APPLE; return true; }
        if (s_PlumTreeWhitelist.Find(typeName)  != -1) { fruitType = eFruitType.PLUM;  return true; }
        if (s_PearTreeWhitelist.Find(typeName)  != -1) { fruitType = eFruitType.PEAR;  return true; }

        // 2) Heuristiken (Namensbestandteile)
        string tLower = typeName;
        tLower.ToLower();
        foreach (string h1 : s_AppleTreeHints) if (tLower.Contains(h1)) { fruitType = eFruitType.APPLE; return true; }
        foreach (string h2 : s_PlumTreeHints)  if (tLower.Contains(h2)) { fruitType = eFruitType.PLUM;  return true; }
        foreach (string h3 : s_PearTreeHints)  if (tLower.Contains(h3)) { fruitType = eFruitType.PEAR;  return true; }

        // 3) Optional: CfgVehicles <TreeClass> fruitType="apple|plum|pear"
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

        return false;
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

            GetGame().CreateObjectEx(className, spawnPos, ECE_PLACE_ON_SURFACE);
        }
    }
}