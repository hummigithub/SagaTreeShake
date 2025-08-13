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
        return ContinuousDefaultActionInput; // LMB halten
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem   = new CCINone;
        m_ConditionTarget = new CCTCursor(); // wichtig für TreeHard_*
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target) return false;
        Object obj = target.GetObject();
        if (!obj) return false;

        int fruitType;
        return SagaTreeShakeHelpers.IsFruitTree(obj, fruitType);
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

    override bool HasTarget() { return true; }
}