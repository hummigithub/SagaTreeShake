modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        // Erzwingt Erstellen/Laden der Profiles-Config beim Serverstart
        SagaTreeShakeSettings.Get();
    }
}