class SagaTreeShakeRuntime
{
    private static ref map<string,int> s_TreeCooldowns;

    static void Ensure()
    {
        if (!s_TreeCooldowns) s_TreeCooldowns = new map<string,int>();
    }

    static int GetCooldown(string key)
    {
        Ensure();
        int v;
        if (s_TreeCooldowns.Find(key, v)) return v;
        return 0;
    }

    static void SetCooldown(string key, int expiry)
    {
        Ensure();
        s_TreeCooldowns.Set(key, expiry);
    }

    static void GetAllKeys(out array<string> outKeys)
    {
        Ensure();
    outKeys = s_TreeCooldowns.GetKeyArray();
    }

    static void Clear()
    {
        if (s_TreeCooldowns) s_TreeCooldowns.Clear();
    }
    static void SetFromMap(notnull map<string,int> src)
    {
        Ensure();
        s_TreeCooldowns.Clear();
    array<string> keys = src.GetKeyArray();
        foreach (string k : keys)
        {
            int v;
            src.Find(k, v);
            s_TreeCooldowns.Set(k, v);
        }
    }
    static void ExportToMap(out map<string,int> dest)
    {
        Ensure();
        if (!dest) dest = new map<string,int>();
        dest.Clear();
    array<string> keys = s_TreeCooldowns.GetKeyArray();
        foreach (string k : keys)
        {
            int v;
            s_TreeCooldowns.Find(k, v);
            dest.Set(k, v);
        }
    }
}
