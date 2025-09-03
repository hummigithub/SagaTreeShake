class CfgPatches
{
    class SagaTreeShake
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data","DZ_Scripts"};
    };
};

class CfgMods
{
    class SagaTreeShake
    {
        dir = "SagaTreeShake";
        picture = "";
        action = "";
        hideName = 0;
        hidePicture = 1;
        name = "SagaTreeShake";
        author = "hummigummi";
        version = "2.5";		
        type = "mod";
        dependencies[] = {"Game","World","Mission"};

        class defs
        {
            class worldScriptModule
            {
 
                files[] = {"SagaTreeShake/scripts/4_World"};
            };
            class missionScriptModule
            {

                files[] = {"SagaTreeShake/scripts/5_Mission"};
            };
        };
    };
};