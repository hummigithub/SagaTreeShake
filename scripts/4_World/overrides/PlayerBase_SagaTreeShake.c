modded class PlayerBase
{
    static const int STS_RPC_CooldownQuery    = 200001;
    static const int STS_RPC_CooldownResponse = 200002;
    private int m_STS_RemainingCooldownSec;

    void SetSagaTreeShakeRemaining(int sec)
    {
        m_STS_RemainingCooldownSec = sec;
    }

    int GetSagaTreeShakeRemaining()
    {
        return m_STS_RemainingCooldownSec;
    }

    void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
    {
        if (!GetGame().IsServer() && rpc_type == STS_RPC_CooldownResponse)
        {
            Param1<int> p;
            if (!ctx.Read(p)) return;
            SetSagaTreeShakeRemaining(p.param1);
            Print("[SagaTreeShake][Client] Received cooldown response: " + p.param1);
            return;
        }

        if (GetGame().IsServer() && rpc_type == STS_RPC_CooldownQuery)
        {
            Param1<string> q;
            if (!ctx.Read(q)) return;
            string key = q.param1;
            Print("[SagaTreeShake][Server] PlayerBase received query for key=" + key);

            int expiry = SagaTreeShakeRuntime.GetCooldown(key);
            int now = GetGame().GetTime();
            int remaining = 0;
            if (expiry > now) remaining = expiry - now;
            if (remaining <= 0) return;

            int remainingSec = Math.Ceil(remaining / 1000.0);
            Param1<int> resp = new Param1<int>(remainingSec);
            Print("[SagaTreeShake][Server] Sending remainingSec=" + remainingSec + " to client");
            GetGame().RPCSingleParam(this, STS_RPC_CooldownResponse, resp, true, sender);
            return;
        }
    }
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionShakeAppleTree);
    }
}