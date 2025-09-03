class ActionShakeAppleTreeCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float dur = 10.0;
        SagaTreeShakeSettings s = SagaTreeShakeSettings.Get();
        if (s)
        {
            dur = s.shake_duration_seconds;
            if (dur <= 0) dur = 10.0;
        }
        m_ActionData.m_ActionComponent = new CAContinuousTime(dur);
    }
}

class ActionShakeAppleTree : ActionContinuousBase
{
    void ActionShakeAppleTree()
    {
        m_CallbackClass = ActionShakeAppleTreeCB;

        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEIDX_ERECT | DayZPlayerConstants.STANCEIDX_CROUCH;

        m_Text = "Shake Tree";
    }

    override bool HasProgress() { return true; }

    override typename GetInputType()
    {
        return ContinuousDefaultActionInput;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem   = new CCINone;
        m_ConditionTarget = new CCTCursor();
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target) return false;
        Object obj = target.GetObject();
        if (!obj) return false;

        if (obj.IsInherited(ItemBase)) return false;

        int fruitType;
        if (!SagaTreeShakeHelpers.IsFruitTree(obj, fruitType))
            return false;

        vector pos = obj.GetPosition();
        int rx = Math.Round(pos[0]);
        int ry = Math.Round(pos[1]);
        int rz = Math.Round(pos[2]);
        string key = obj.GetType() + "_" + rx + "_" + ry + "_" + rz;

        if (GetGame() && GetGame().IsServer())
        {
            int expiry = SagaTreeShakeRuntime.GetCooldown(key);
            if (expiry > 0)
            {
                int now = GetGame().GetTime();
                if (now < expiry)
                    return false;
            }
        }

        if (GetGame() && !GetGame().IsServer())
        {
            int expiryC = SagaTreeShakeRuntime.GetCooldown(key);
            if (expiryC > 0)
            {
                int nowC = GetGame().GetTime();
                if (nowC < expiryC)
                    return false;
            }
            m_Text = "Shake Tree";
        }

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        super.OnFinishProgressServer(action_data);
        if (!GetGame() || !GetGame().IsServer()) return;

        PlayerBase player = action_data.m_Player;
        if (!player) return;

        Object obj = action_data.m_Target.GetObject();
        if (!obj) return;

        int fruitType;
        if (!SagaTreeShakeHelpers.IsFruitTree(obj, fruitType))
            return;

        SagaTreeShakeSettings s = SagaTreeShakeSettings.Get();

        if (s && s.tree_cooldown_minutes > 0)
        {
            vector pos = obj.GetPosition();
            int rx = Math.Round(pos[0]);
            int ry = Math.Round(pos[1]);
            int rz = Math.Round(pos[2]);
            string key = obj.GetType() + "_" + rx + "_" + ry + "_" + rz;
            int now = GetGame().GetTime();
            int expiry = now + s.tree_cooldown_minutes * 60 * 1000;
            SagaTreeShakeRuntime.SetCooldown(key, expiry);
        }

        float chance = s.ChanceFor(fruitType);
        if (Math.RandomFloat01() >= chance)
            return;

        int minC, maxC;
        s.CountRangeFor(fruitType, minC, maxC);
        int count = Math.RandomIntInclusive(minC, maxC);

        string fruitClass = "";
        switch (fruitType)
        {
            case eFruitType.APPLE: fruitClass = "Apple"; break;
            case eFruitType.PLUM:  fruitClass = "Plum";  break;
            case eFruitType.PEAR:  fruitClass = "Pear";  break;
            default: return;
        }

        if (count > 0)
            SagaTreeShakeHelpers.SpawnFruitInFront(player, fruitClass, count);
    }

    override void OnFinishProgressClient(ActionData action_data)
    {
        super.OnFinishProgressClient(action_data);
        if (!GetGame() || GetGame().IsServer()) return;

        PlayerBase player = action_data.m_Player;
        if (!player) return;

        Object obj = action_data.m_Target.GetObject();
        if (!obj) return;

        SagaTreeShakeSettings s = SagaTreeShakeSettings.Get();
        if (!s) return;

        if (s.tree_cooldown_minutes > 0)
        {
            vector pos = obj.GetPosition();
            int rx = Math.Round(pos[0]);
            int ry = Math.Round(pos[1]);
            int rz = Math.Round(pos[2]);
            string key = obj.GetType() + "_" + rx + "_" + ry + "_" + rz;
            int now = GetGame().GetTime();
            int expiry = now + s.tree_cooldown_minutes * 60 * 1000;
            SagaTreeShakeRuntime.SetCooldown(key, expiry);
        }
    }

    override bool HasTarget() { return true; }
}