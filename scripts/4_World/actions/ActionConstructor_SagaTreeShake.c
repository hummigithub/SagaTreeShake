modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(ActionShakeAppleTree);
		SagaTreeShakeSettings.Get();
        Print("[SagaTreeShake] ActionShakeAppleTree registered and settings ensured");
    }
}