modded class MissionServer
{
    static const int STS_RPC_CooldownQuery    = 200001;
    static const int STS_RPC_CooldownResponse = 200002;

    private ref map<string, int> m_TreeCooldowns;

    static const string COOLDOWN_FILE = "$profile:/SagaTreeShake/cooldowns.json";

    void MissionServer_SagaTreeShake()
    {
        if (!m_TreeCooldowns) m_TreeCooldowns = new map<string, int>();
    }

    override void OnInit()
    {
        super.OnInit();
        SagaTreeShakeSettings.Get();
        LoadTreeCooldowns();
        Print("[SagaTreeShake] MissionServer.OnInit ensured settings are loaded");
    }

    int GetTreeCooldownExpiry(string key)
    {
        if (!m_TreeCooldowns) return 0;
        int val;
        if (m_TreeCooldowns.Find(key, val))
            return val;
        return 0;
    }

    void SetTreeCooldownExpiry(string key, int expiryUnixSeconds)
    {
        if (!m_TreeCooldowns) m_TreeCooldowns = new map<string, int>();
        m_TreeCooldowns.Set(key, expiryUnixSeconds);
        SaveTreeCooldowns();

        SagaTreeShakeRuntime.SetCooldown(key, expiryUnixSeconds);
    }

    void SaveTreeCooldowns()
    {
        SagaTreeShakeRuntime.ExportToMap(m_TreeCooldowns);
        if (!m_TreeCooldowns) return;

        FileHandle fh = OpenFile(COOLDOWN_FILE, FileMode.WRITE);
        if (!fh) return;

    array<string> keys = m_TreeCooldowns.GetKeyArray();

        for (int i = 0; i < keys.Count(); i++)
        {
            string k = keys[i];
            int v;
            m_TreeCooldowns.Find(k, v);
            FPrint(fh, k + ": " + v.ToString() + "\n");
        }
        CloseFile(fh);
    }

    void LoadTreeCooldowns()
    {
        if (!FileExist(COOLDOWN_FILE))
        {
            if (!m_TreeCooldowns) m_TreeCooldowns = new map<string, int>();
            SagaTreeShakeRuntime.Clear();
            return;
        }

        FileHandle fh = OpenFile(COOLDOWN_FILE, FileMode.READ);
        if (!fh)
        {
            if (!m_TreeCooldowns) m_TreeCooldowns = new map<string, int>();
            return;
        }

        if (!m_TreeCooldowns) m_TreeCooldowns = new map<string, int>();
        m_TreeCooldowns.Clear();

        string line;
        while (FGets(fh, line) > 0)
        {
            line = line.Trim();
            if (line.Length() == 0) continue;
            int colon = line.IndexOf(":");
            if (colon == -1) continue;
            string key = line.Substring(0, colon).Trim();
            string rhs = line.Substring(colon + 1, line.Length() - colon - 1).Trim();
            if (rhs.Length() > 0 && rhs.Substring(rhs.Length() - 1, 1) == ",") rhs = rhs.Substring(0, rhs.Length() - 1).Trim();
            int val = rhs.ToInt();
            m_TreeCooldowns.Set(key, val);
        }
        CloseFile(fh);

        SagaTreeShakeRuntime.SetFromMap(m_TreeCooldowns);
    }

}