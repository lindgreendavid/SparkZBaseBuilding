class CfgPatches
{
    class SparkZGroup
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Worlds_Chernarusplus_World",
            "SparkZCore",
            "DZM_VPPAdminToolsScripts",
            "Vehicle3PP_Scripts"
        };
    };
};

class CfgMods
{
    class SparkZGroup
    {
        dir = "SparkZGroup";
        name = "SparkZGroup";
        picture = "SparkZGroup\\gui\\branding\\SZLogoo.paa";
        logo = "SparkZGroup\\gui\\branding\\SZLogoo.paa";
        logoSmall = "SparkZGroup\\gui\\branding\\SZLogoo.paa";
        logoOver = "SparkZGroup\\gui\\branding\\SZLogoo.paa";
        action = "";
        hideName = 0;
        hidePicture = 0;
        credits = "";
        author = "SparkZ";
        authorID = "0";
        version = "0.8.24";
        extra = 0;
        type = "mod";
        inputs = "SparkZGroup/inputsSparkZGroup.xml";
        dependencies[] =
        {
            "Game",
            "World",
            "Mission"
        };

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZGroup/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZGroup/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZGroup/scripts/5_Mission"
                };
            };
        };
    };
};

class CfgLocationTypes
{
    class Name;

    class BigMilitary: Name
    {
        color[] = {0.95, 0.05, 0.03, 1};
        textSize = 0.05;
        importance = 7;
    };

    class MediumMilitary: Name
    {
        color[] = {0.95, 0.05, 0.03, 1};
        textSize = 0.042;
        importance = 6;
    };

    class SmallMilitary: Name
    {
        color[] = {0.95, 0.05, 0.03, 1};
        textSize = 0.035;
        importance = 5;
    };
};

class CfgWorlds
{
    class CAWorld;

    class ChernarusPlus: CAWorld {
		class Names {
			class Settlement_Chernogorsk {
				name = "Chernogorsk";
				position[] = {6778.619, 2321.774};
				type = "Capital";
			};
			class Settlement_Novodmitrovsk {
				name = "Novodmitrovsk";
				position[] = {11339.77, 14391.05};
				type = "Capital";
			};
			class Settlement_Novoselki {
				name = "Novoselki";
				position[] = {6115.91, 3256.36};
				type = "City";
			};
			class Settlement_Dubovo {
				name = "Dubovo";
				position[] = {6754.51, 3595.257};
				type = "City";
			};
			class Settlement_Vysotovo {
				name = "Vysotovo";
				position[] = {5701.24, 2556.11};
				type = "City";
			};
			class Settlement_Zelenogorsk {
				name = "Zelenogorsk";
				position[] = {2796, 5166};
				type = "City";
			};
			class Settlement_Berezino {
				name = "Berezino";
				position[] = {12372.383, 9760.981};
				type = "City";
			};
			class Settlement_Elektrozavodsk {
				name = "Elektrozavodsk";
				position[] = {10286.68, 1992.77};
				type = "City";
			};
			class Settlement_NovayaPetrovka {
				name = "Novaya Petrovka";
				position[] = {3477.169, 12931.243};
				type = "City";
			};
			class Settlement_Gorka {
				name = "Gorka";
				position[] = {9524, 8945};
				type = "City";
			};
			class Settlement_Solnechny {
				name = "Solnechny";
				position[] = {13487.724, 6225.884};
				type = "City";
			};
			class Settlement_StarySobor {
				name = "Stary Sobor";
				position[] = {6114, 7849};
				type = "City";
			};
			class Settlement_Vybor {
				name = "Vybor";
				position[] = {3909.831, 9019.614};
				type = "City";
			};
			class Settlement_Severograd {
				name = "Severograd";
				position[] = {8168.484, 12573.602};
				type = "City";
			};
			class Settlement_Bor {
				name = "Bor";
				position[] = {3316.406, 4070.966};
				type = "Village";
			};
			class Settlement_Svetloyarsk {
				name = "Svetloyarsk";
				position[] = {13989.74, 13263.78};
				type = "City";
			};
			class Settlement_Krasnostav {
				name = "Krasnostav";
				position[] = {11203.167, 12340.808};
				type = "City";
			};
			class Settlement_ChernayaPolyana {
				name = "Chernaya Polyana";
				position[] = {12158.54, 13770.22};
				type = "City";
			};
			class Settlement_Polyana {
				name = "Polyana";
				position[] = {10794, 8160};
				type = "City";
			};
			class Settlement_Tulga {
				name = "Tulga";
				position[] = {12768, 4384};
				type = "Village";
			};
			class Settlement_Msta {
				name = "Msta";
				position[] = {11333.79, 5421.4};
				type = "Village";
			};
			class Settlement_Staroye {
				name = "Staroye";
				position[] = {10212.52, 5385.05};
				type = "Village";
			};
			class Settlement_Shakhovka {
				name = "Shakhovka";
				position[] = {9728.64, 6567.3};
				type = "Village";
			};
			class Settlement_Dolina {
				name = "Dolina";
				position[] = {11285.17, 6633.58};
				type = "Village";
			};
			class Settlement_Orlovets {
				name = "Orlovets";
				position[] = {12250, 7253};
				type = "Village";
			};
			class Settlement_NovySobor {
				name = "Novy Sobor";
				position[] = {7123.63, 7774.63};
				type = "Village";
			};
			class Settlement_Kabanino {
				name = "Kabanino";
				position[] = {5300.71, 8645.9};
				type = "Village";
			};
			class Settlement_Mogilevka {
				name = "Mogilevka";
				position[] = {7650.41, 5096.46};
				type = "Village";
			};
			class Settlement_Nadezhdino {
				name = "Nadezhdino";
				position[] = {5894, 4780};
				type = "Village";
			};
			class Settlement_Guglovo {
				name = "Guglovo";
				position[] = {8445, 6579};
				type = "Village";
			};
			class Settlement_Kamyshovo {
				name = "Kamyshovo";
				position[] = {12170, 3458};
				type = "Village";
			};
			class Settlement_Pusta {
				name = "Pusta";
				position[] = {9197, 3873};
				type = "Village";
			};
			class Settlement_Dubrovka {
				name = "Dubrovka";
				position[] = {10324.299, 9888.237};
				type = "Village";
			};
			class Settlement_VyshnayaDubrovka {
				name = "Vyshnaya Dubrovka";
				position[] = {9926.823, 10468.919};
				type = "Village";
			};
			class Settlement_Khelm {
				name = "Khelm";
				position[] = {12329.27, 10771.01};
				type = "Village";
			};
			class Settlement_Olsha {
				name = "Olsha";
				position[] = {13402, 12898};
				type = "Village";
			};
			class Settlement_Gvozdno {
				name = "Gvozdno";
				position[] = {8644.751, 11934.115};
				type = "Village";
			};
			class Settlement_Grishino {
				name = "Grishino";
				position[] = {5980, 10257};
				type = "Village";
			};
			class Settlement_Rogovo {
				name = "Rogovo";
				position[] = {4802, 6748};
				type = "Village";
			};
			class Settlement_Pogorevka {
				name = "Pogorevka";
				position[] = {4445, 6322};
				type = "Village";
			};
			class Settlement_Pustoshka {
				name = "Pustoshka";
				position[] = {3137.944, 7890.925};
				type = "Village";
			};
			class Settlement_Kozlovka {
				name = "Kozlovka";
				position[] = {4402, 4736};
				type = "Village";
			};
			class Settlement_Karmanovka {
				name = "Karmanovka";
				position[] = {12700.628, 14691.312};
				type = "Village";
			};
			class Settlement_Balota {
				name = "Balota";
				position[] = {4527.58, 2465.32};
				type = "Village";
			};
			class Settlement_Komarovo {
				name = "Komarovo";
				position[] = {3702, 2440};
				type = "Village";
			};
			class Settlement_Kamenka {
				name = "Kamenka";
				position[] = {1981, 2207};
				type = "Village";
			};
			class Settlement_Myshkino {
				name = "Myshkino";
				position[] = {2048, 7258};
				type = "Village";
			};
			class Settlement_Pavlovo {
				name = "Pavlovo";
				position[] = {1693.98, 3858.65};
				type = "Village";
			};
			class Settlement_Lopatino {
				name = "Lopatino";
				position[] = {2801, 10029};
				type = "Village";
			};
			class Settlement_Vyshnoye {
				name = "Vyshnoye";
				position[] = {6629, 6051};
				type = "Village";
			};
			class Settlement_Prigorodki {
				name = "Prigorodki";
				position[] = {7760, 3336};
				type = "Village";
			};
			class Settlement_Drozhino {
				name = "Drozhino";
				position[] = {3374, 4861};
				type = "Village";
			};
			class Settlement_Sosnovka {
				name = "Sosnovka";
				position[] = {2556, 6307};
				type = "Village";
			};
			class Settlement_Nizhneye {
				name = "Nizhneye";
				position[] = {12824.88, 8097.7};
				type = "Village";
			};
			class Settlement_Pulkovo {
				name = "Pulkovo";
				position[] = {5015, 5607};
				type = "Village";
			};
			class Settlement_Berezhki {
				name = "Berezhki";
				position[] = {15020, 13905};
				type = "Village";
			};
			class Settlement_Turovo {
				name = "Turovo";
				position[] = {13630, 14146};
				type = "Village";
			};
			class Settlement_BelayaPolyana {
				name = "Belaya Polyana";
				position[] = {14166, 14968};
				type = "Village";
			};
			class Settlement_Dobroye {
				name = "Dobroye";
				position[] = {12996, 15066};
				type = "Village";
			};
			class Settlement_Nagornoye {
				name = "Nagornoye";
				position[] = {9286, 14677};
				type = "Village";
			};
			class Settlement_Svergino {
				name = "Svergino";
				position[] = {9493.52, 13875.27};
				type = "Village";
			};
			class Settlement_Ratnoye {
				name = "Ratnoye";
				position[] = {6195.878, 12761.802};
				type = "Village";
			};
			class Settlement_Kamensk {
				name = "Kamensk";
				position[] = {6748.741, 14430.046};
				type = "Village";
			};
			class Settlement_Krasnoye {
				name = "Krasnoye";
				position[] = {6411.374, 15029.961};
				type = "Village";
			};
			class Settlement_StaryYar {
				name = "Stary Yar";
				position[] = {4987.86, 14992.976};
				type = "Village";
			};
			class Settlement_Polesovo {
				name = "Polesovo";
				position[] = {5955.737, 13558.046};
				type = "Village";
			};
			class Settlement_Tisy {
				name = "Tisy";
				position[] = {3441.65, 14799.55};
				type = "Village";
			};
			class Settlement_Topolniki {
				name = "Topolniki";
				position[] = {2913.745, 12366.357};
				type = "Village";
			};
			class Settlement_Zaprudnoye {
				name = "Zaprudnoye";
				position[] = {5214, 12792};
				type = "Village";
			};
			class Settlement_Sinystok {
				name = "Sinystok";
				position[] = {1538.278, 11904.578};
				type = "Village";
			};
			class Settlement_Vavilovo {
				name = "Vavilovo";
				position[] = {2263, 11103};
				type = "Village";
			};
			class Settlement_Kumyrna {
				name = "Kumyrna";
				position[] = {8300, 6054};
				type = "Village";
			};
			class Settlement_Kalinovka {
				name = "Kalinovka";
				position[] = {7448, 13406};
				type = "Village";
			};
			class Settlement_Bogatyrka {
				name = "Bogatyrka";
				position[] = {1453.62, 8870.92};
				type = "Village";
			};
			class Settlement_SvyatoyRomanSkiResort {
				name = "Svyatoy Roman Ski Resort";
				position[] = {638, 11667};
				type = "Local";
			};
			class Settlement_SKVSChBiathlonArena {
				name = "SKVSCh Biathlon Arena";
				position[] = {357, 11142};
				type = "Local";
			};
			class Settlement_Zvir {
				name = "Zvir";
				position[] = {577.33, 5317.42};
				type = "Village";
			};
			class Settlement_Zabolotye {
				name = "Zabolotye";
				position[] = {1256.859, 9993.26};
				type = "Village";
			};
			class Settlement_Galkino {
				name = "Galkino";
				position[] = {1102.68, 8798.27};
				type = "Local";
			};
			class Camp_Arsenovo {
				name = "";
				position[] = {8509.661, 13918.628};
				type = "Camp";
			};
			class Camp_Stroytel {
				name = "Stroytel";
				position[] = {7017.63, 4365.21};
				type = "Camp";
			};
			class Camp_Romashka {
				name = "Romashka";
				position[] = {8177.602, 10980.289};
				type = "Camp";
			};
			class Camp_Kometa {
				name = "Kometa";
				position[] = {10271.697, 3558.228};
				type = "Camp";
			};
			class Camp_Druzhba {
				name = "Druzhba";
				position[] = {11437, 10695};
				type = "Camp";
			};
			class Camp_Nadezhda {
				name = "Nadezhda";
				position[] = {7279.968, 7014.35};
				type = "Camp";
			};
			class Camp_YouthPioneer {
				name = "Youth Pioneer";
				position[] = {11155.825, 7074.143};
				type = "Camp";
			};
			class Camp_ProudChernarus {
				name = "Proud Chernarus";
				position[] = {3199.617, 6174.932};
				type = "Camp";
			};
			class Camp_Shkolnik {
				name = "Shkolnik";
				position[] = {14856.503, 14571.028};
				type = "Camp";
			};
			class Camp_Pobeda {
				name = "Pobeda";
				position[] = {3703.382, 14876.384};
				type = "Camp";
			};
			class Camp_Metalurg {
				name = "Metalurg";
				position[] = {1035.406, 6677.383};
				type = "Camp";
			};
			class Hill_Zelenayagora {
				name = "Zelenayagora";
				position[] = {3767.17, 6010.54};
				type = "Hill";
			};
			class Local_Dichina {
				name = "Dichina";
				position[] = {4618, 7821};
				type = "Local";
			};
			class Local_Novylug {
				name = "Novylug";
				position[] = {9251, 11360};
				type = "Local";
			};
			class Local_Staryeluga {
				name = "Staryeluga";
				position[] = {6919, 8995};
				type = "Local";
			};
			class Hill_Vysota {
				name = "Vysota";
				position[] = {6591.63, 3400};
				type = "Hill";
			};
			class Hill_Kopyto {
				name = "Kopyto";
				position[] = {7871.479, 3869.102};
				type = "Hill";
			};
			class Local_Grubyeskaly {
				name = "Grubyeskaly";
				position[] = {13115.06, 11900};
				type = "Local";
			};
			class Local_Chernyles {
				name = "Chernyles";
				position[] = {9031, 7803};
				type = "Local";
			};
			class Hill_Altar {
				name = "Altar";
				position[] = {8143.4, 9159.68};
				type = "Hill";
			};
			class Local_RadioZenit {
				name = "Radio Zenit";
				position[] = {8080.01, 9341.68};
				type = "Local";
			};
			class Hill_PikKozlova {
				name = "Pik Kozlova";
				position[] = {8850.17, 2880.53};
				type = "Hill";
			};
			class Local_Pustoykhrebet {
				name = "Pustoykhrebet";
				position[] = {10890, 5665};
				type = "Local";
			};
			class Hill_Bashnya {
				name = "Bashnya";
				position[] = {4178.27, 11771.22};
				type = "Hill";
			};
			class Hill_Veresnik {
				name = "Veresnik";
				position[] = {4440.17, 8070.54};
				type = "Hill";
			};
			class Hill_Kurgan {
				name = "Kurgan";
				position[] = {3368.52, 5296.87};
				type = "Hill";
			};
			class Hill_Kustryk {
				name = "Kustryk";
				position[] = {4912.88, 5063.45};
				type = "Hill";
			};
			class Hill_Vetryanayagora {
				name = "Vetryanayagora";
				position[] = {3892.74, 4200.59};
				type = "Hill";
			};
			class Hill_Kalmyk {
				name = "Kalmyk";
				position[] = {6903.78, 4919.66};
				type = "Hill";
			};
			class Hill_PopIvan {
				name = "Pop Ivan";
				position[] = {6420.26, 6570.66};
				type = "Hill";
			};
			class Hill_Erbenka {
				name = "Erbenka";
				position[] = {2842.916, 4014.811};
				type = "Hill";
			};
			class Local_Lesnoykhrebet {
				name = "Lesnoykhrebet";
				position[] = {8122.77, 7815.54};
				type = "Local";
			};
			class Hill_Vysokiykamen {
				name = "Vysokiykamen";
				position[] = {8940.19, 4380.52};
				type = "Hill";
			};
			class Hill_Dobry {
				name = "Dobry";
				position[] = {10552.9, 3061.03};
				type = "Hill";
			};
			class Hill_Baranchik {
				name = "Baranchik";
				position[] = {10153.147, 6208.521};
				type = "Hill";
			};
			class Hill_Malinovka {
				name = "Malinovka";
				position[] = {10897.7, 7575.59};
				type = "Hill";
			};
			class Hill_Dubina {
				name = "Dubina";
				position[] = {11107.9, 8474.83};
				type = "Hill";
			};
			class Hill_Klen {
				name = "Klen";
				position[] = {11473.511, 11315.393};
				type = "Hill";
			};
			class Hill_Chernayagora {
				name = "Chernayagora";
				position[] = {10280.816, 12053.928};
				type = "Hill";
			};
			class Ruin_Zolotar {
				name = "Zolotar";
				position[] = {10179, 11998};
				type = "Ruin";
			};
			class Hill_Ostry {
				name = "Ostry";
				position[] = {10792.747, 12829.504};
				type = "Hill";
			};
			class Hill_Olsha {
				name = "Olsha";
				position[] = {12975.7, 12775.2};
				type = "Hill";
			};
			class Marine_Tikhiyzaliv {
				name = "Tikhiyzaliv";
				position[] = {1221.49, 2111.89};
				type = "Marine";
			};
			class Marine_Mutnyizaliv {
				name = "Mutnyizaliv";
				position[] = {5735.479, 1918.92};
				type = "Marine";
			};
			class Marine_Chernyzaliv {
				name = "Chernyzaliv";
				position[] = {7599.069, 2486.586};
				type = "Marine";
			};
			class Marine_Zelenyzaliv {
				name = "Zelenyzaliv";
				position[] = {11227.019, 2989.838};
				type = "Marine";
			};
			class Marine_Skalistyproliv {
				name = "Skalistyproliv";
				position[] = {13385.92, 3613.94};
				type = "Marine";
			};
			class Marine_Nizhniyzaliv {
				name = "Nizhniyzaliv";
				position[] = {12989.3, 8515.76};
				type = "Marine";
			};
			class Marine_ZalivGuba {
				name = "Zaliv Guba";
				position[] = {14328.374, 13136.732};
				type = "Marine";
			};
			class Marine_Rify {
				name = "Rify";
				position[] = {13931.73, 11288.61};
				type = "Local";
			};
			class Marine_Ivovoyeozero {
				name = "Ivovoyeozero";
				position[] = {13248, 11572};
				type = "Marine";
			};
			class Marine_Chernoyeozero {
				name = "Chernoyeozero";
				position[] = {13381, 12002};
				type = "Marine";
			};
			class Marine_PlotinaTopolka {
				name = "Plotina Topolka";
				position[] = {10231.33, 3691.15};
				type = "Marine";
			};
			class Marine_PlotinaPobeda {
				name = "Plotina Pobeda";
				position[] = {9928.294, 13874.643};
				type = "Marine";
			};
			class Marine_PlotinaTishina {
				name = "Plotina Tishina";
				position[] = {1150.82, 6432.454};
				type = "Marine";
			};
			class Marine_Ozerko {
				name = "Ozerko";
				position[] = {6777.63, 4492.42};
				type = "Marine";
			};
			class Marine_Prud {
				name = "Prud";
				position[] = {6610.98, 9308.57};
				type = "Marine";
			};
			class Ruin_Chortovzamok {
				name = "Chortovzamok";
				position[] = {6883.239, 11501.288};
				type = "Ruin";
			};
			class Ruin_Zub {
				name = "Zub";
				position[] = {6574.28, 5573.85};
				type = "Ruin";
			};
			class Ruin_Rog {
				name = "Rog";
				position[] = {11267.206, 4293.036};
				type = "Ruin";
			};
			class Local_Grozovypereval {
				name = "Grozovypereval";
				position[] = {3318.12, 15250.55};
				type = "Local";
			};
			class Local_Sosnovypereval {
				name = "Sosnovypereval";
				position[] = {2687.4, 6590.28};
				type = "Local";
			};
			class Local_PerevalOreshka {
				name = "Pereval Oreshka";
				position[] = {9822.191, 6088.793};
				type = "Local";
			};
			class Local_Turovskiypereval {
				name = "Turovskiypereval";
				position[] = {14674.241, 14089.306};
				type = "Local";
			};
			class Local_Tridoliny {
				name = "Tridoliny";
				position[] = {12764.47, 5412.21};
				type = "Local";
			};
			class Local_Grozovyedoly {
				name = "Grozovyedoly";
				position[] = {10403.982, 14793.168};
				type = "Local";
			};
			class Ruin_Klyuch {
				name = "Klyuch";
				position[] = {9282.57, 13476.67};
				type = "Ruin";
			};
			class Hill_Lysayagora {
				name = "Lysayagora";
				position[] = {6722.459, 14050.42};
				type = "Hill";
			};
			class Marine_Glubokoyeozero {
				name = "Glubokoyeozero";
				position[] = {1692.555, 14821.565};
				type = "Marine";
			};
			class Local_Skalka {
				name = "Skalka";
				position[] = {5758, 14486};
				type = "Local";
			};
			class Local_Vidy {
				name = "Vidy";
				position[] = {785.054, 14398.815};
				type = "Local";
			};
			class Hill_Tumannyverkh {
				name = "Tumannyverkh";
				position[] = {331.224, 12982.99};
				type = "Hill";
			};
			class Local_Adamovka {
				name = "Adamovka";
				position[] = {5340.66, 11380.01};
				type = "Local";
			};
			class Hill_Shishok {
				name = "Shishok";
				position[] = {3559.313, 9422.27};
				type = "Hill";
			};
			class Settlement_Skalisty {
				name = "Skalisty";
				position[] = {13715.454, 3118.78};
				type = "Village";
			};
			class Ruin_Storozh {
				name = "Storozh";
				position[] = {2816.601, 1277.96};
				type = "Ruin";
			};
			class Local_MysGolova {
				name = "Mys Golova";
				position[] = {8286.053, 2405.598};
				type = "Local";
			};
			class Local_Drakon {
				name = "Drakon";
				position[] = {11191, 2443};
				type = "Local";
			};
			class Local_Otmel {
				name = "Otmel";
				position[] = {11581.25, 3213.24};
				type = "Local";
			};
			class Local_MysKrutoy {
				name = "Mys Krutoy";
				position[] = {13578.19, 3976.82};
				type = "Local";
			};
			class Hill_Tokarnya {
				name = "Tokarnya";
				position[] = {8890.478, 5672.553};
				type = "Hill";
			};
			class Hill_Ostrog {
				name = "Ostrog";
				position[] = {2655.734, 2264.632};
				type = "Hill";
			};
			class Local_Maryanka {
				name = "Maryanka";
				position[] = {2789, 3386};
				type = "Local";
			};
			class Local_Polonina {
				name = "Polonina";
				position[] = {1004.486, 4242.869};
				type = "Local";
			};
			class Local_Kalinka {
				name = "Kalinka";
				position[] = {3346.5, 11292.097};
				type = "Local";
			};
			class Hill_Kikimora {
				name = "Kikimora";
				position[] = {1812, 6200};
				type = "Hill";
			};
			class Hill_BolshoyKotel {
				name = "Bolshoy Kotel";
				position[] = {714.649, 6520.431};
				type = "Hill";
			};
			class Hill_Simurg {
				name = "Simurg";
				position[] = {134, 7569.99};
				type = "Hill";
			};
			class Hill_Volchiypik {
				name = "Volchiypik";
				position[] = {320.254, 2719.269};
				type = "Hill";
			};
			class Ruin_Krona {
				name = "Krona";
				position[] = {1493, 9270.34};
				type = "Ruin";
			};
			class Local_TriKresta {
				name = "Tri Kresta";
				position[] = {301, 9478};
				type = "Local";
			};
			class Local_Rostoki {
				name = "Rostoki";
				position[] = {744.45, 8647.86};
				type = "Local";
			};
			class Marine_OrlyeOzero {
				name = "Orlye Ozero";
				position[] = {597, 5990};
				type = "Marine";
			};
			class Local_Makosh {
				name = "Makosh";
				position[] = {7849.72, 6480.19};
				type = "Local";
			};
			class Local_Klenovyipereval {
				name = "Klenovyipereval";
				position[] = {10862.702, 11585.512};
				type = "Local";
			};
			class Local_Zmeinykhrebet {
				name = "Zmeinykhrebet";
				position[] = {11446.17, 13606.92};
				type = "Local";
			};
			class Hill_Sokol {
				name = "Sokol";
				position[] = {12001, 14813};
				type = "Hill";
			};
			class Local_Krutyeskaly {
				name = "Krutyeskaly";
				position[] = {14880.63, 13671.76};
				type = "Local";
			};
			class Local_Bogat {
				name = "Bogat";
				position[] = {7058.011, 12023.571};
				type = "Local";
			};
			class Local_Dubnik {
				name = "Dubnik";
				position[] = {3298.29, 10330.89};
				type = "Local";
			};
			class Hill_Viselitsa {
				name = "Viselitsa";
				position[] = {12722.157, 7504.05};
				type = "Hill";
			};
			class Local_Dazhbog {
				name = "Dazhbog";
				position[] = {6581, 8590};
				type = "Hill";
			};
			class Marine_Verbnik {
				name = "Verbnik";
				position[] = {4414.879, 9103.002};
				type = "Marine";
			};
			class Local_Medvezhilugi {
				name = "Medvezhilugi";
				position[] = {9641, 13236};
				type = "Local";
			};
			class Ruin_Voron {
				name = "Voron";
				position[] = {13517.93, 3311.245};
				type = "Ruin";
			};
			class Ruin_Gnomovzamok {
				name = "Gnomovzamok";
				position[] = {7446.915, 9097.17};
				type = "Ruin";
			};
			class Marine_Glaza {
				name = "Glaza";
				position[] = {7383, 9320};
				type = "Marine";
			};
			class Local_KarerKrasnayaZarya {
				name = "Karer Krasnaya Zarya";
				position[] = {8565.349, 13418.684};
				type = "Local";
			};
			class Local_Matveyevo {
				name = "Matveyevo";
				position[] = {4281, 7380};
				type = "Local";
			};
			class Local_Kotka {
				name = "Kotka";
				position[] = {5873.7, 6883.24};
				type = "Local";
			};
			class Local_Chernyeskaly {
				name = "Chernyeskaly";
				position[] = {3932.572, 14599.496};
				type = "Local";
			};
			class Hill_SvyatoyRoman {
				name = "Svyatoy Roman";
				position[] = {84.832, 11901.246};
				type = "Hill";
			};
			class Hill_Koman {
				name = "Koman";
				position[] = {1660.652, 7415.661};
				type = "Hill";
			};
			class Hill_Mayak {
				name = "Mayak";
				position[] = {12323.813, 4553.107};
				type = "Hill";
			};
			class Local_MB_VMC {
				name = "Veresnik Military Camp";
				position[] = {4497, 8291};
				type = "MediumMilitary";
			};
			class Local_MB_Tisy {
				name = "Tisy Military Base";
				position[] = {1570, 14069};
				type = "BigMilitary";
			};
			class Local_MB_Kamensk {
				name = "Kamensk Military Base";
				position[] = {7838.812, 14704.823};
				type = "MediumMilitary";
			};
			class Local_MB_Zeleno {
				name = "Zeleno Military Base";
				position[] = {2410.375, 5133.681};
				type = "MediumMilitary";
			};
			class Local_MB_Pavlovo {
				name = "Pavlovo Military Base";
				position[] = {2016, 3408};
				type = "MediumMilitary";
			};
			class Local_AF_Balota {
				name = "Balota Airfield";
				position[] = {5082.031, 2376.328};
				type = "MediumMilitary";
			};
			class Local_AF_Krasno {
				name = "North East Airfield";
				position[] = {12100.681, 12573.896};
				type = "BigMilitary";
			};
			class Local_AF_Vybor {
				name = "North West Airfield";
				position[] = {4428, 10179};
				type = "BigMilitary";
			};
			class RailroadStation_Elektro {
				name = "";
				position[] = {10309.912, 2092.203};
				type = "RailroadStation";
			};
			class RailroadStation_Prigorodki {
				name = "";
				position[] = {8056.034, 3264.908};
				type = "RailroadStation";
			};
			class RailroadStation_Cherno {
				name = "";
				position[] = {6540.626, 2638.775};
				type = "RailroadStation";
			};
			class RailroadStation_Balota {
				name = "";
				position[] = {4399.259, 2309.397};
				type = "RailroadStation";
			};
			class RailroadStation_Komarovo {
				name = "";
				position[] = {3670.667, 2397.525};
				type = "RailroadStation";
			};
			class RailroadStation_Kamenka {
				name = "";
				position[] = {1877.935, 2168.559};
				type = "RailroadStation";
			};
			class RailroadStation_Zeleno {
				name = "";
				position[] = {2485.266, 5214.664};
				type = "RailroadStation";
			};
			class RailroadStation_Vavilovo {
				name = "";
				position[] = {1991.503, 11289.488};
				type = "RailroadStation";
			};
			class RailroadStation_Novaya {
				name = "";
				position[] = {3534.425, 12537.095};
				type = "RailroadStation";
			};
			class RailroadStation_Severograd {
				name = "";
				position[] = {7846.581, 12401.488};
				type = "RailroadStation";
			};
			class RailroadStation_Novo {
				name = "";
				position[] = {11610.908, 14133.702};
				type = "RailroadStation";
			};
			class RailroadStation_Svetlo {
				name = "";
				position[] = {13970.473, 13486.344};
				type = "RailroadStation";
			};
			class RailroadStation_Berezino {
				name = "";
				position[] = {12989.844, 10200.673};
				type = "RailroadStation";
			};
			class RailroadStation_Solnich {
				name = "";
				position[] = {13146.109, 7094.472};
				type = "RailroadStation";
			};
			class RailroadStation_Solnich2 {
				name = "";
				position[] = {13356.109, 6154.472};
				type = "RailroadStation";
			};
			class RailroadStation_Kamyshovo {
				name = "";
				position[] = {11970.969, 3548.723};
				type = "RailroadStation";
			};
			class RailroadStation_Dobroye {
				name = "";
				position[] = {12860.658, 15050.221};
				type = "RailroadStation";
			};
			class LocalOffice_Novaya {
				name = "";
				position[] = {3469.172, 13135.165};
				type = "LocalOffice";
			};
			class LocalOffice_Vybor {
				name = "";
				position[] = {3857.656, 8946.14};
				type = "LocalOffice";
			};
			class LocalOffice_Sinys {
				name = "";
				position[] = {1439.08, 12011.26};
				type = "LocalOffice";
			};
			class LocalOffice_Vavil {
				name = "";
				position[] = {2220.84, 11075.83};
				type = "LocalOffice";
			};
			class LocalOffice_Lopat {
				name = "";
				position[] = {2720.72, 9978.14};
				type = "LocalOffice";
			};
			class LocalOffice_Pustosh {
				name = "";
				position[] = {3051.44, 7804.68};
				type = "LocalOffice";
			};
			class LocalOffice_Pogorev {
				name = "";
				position[] = {4480.71, 6462};
				type = "LocalOffice";
			};
			class LocalOffice_Kabanin {
				name = "";
				position[] = {5382, 8589};
				type = "LocalOffice";
			};
			class LocalOffice_Stary {
				name = "";
				position[] = {6056.08, 7715.35};
				type = "LocalOffice";
			};
			class LocalOffice_Novy {
				name = "";
				position[] = {7166.96, 7526.01};
				type = "LocalOffice";
			};
			class LocalOffice_Grishino {
				name = "";
				position[] = {5983, 10305};
				type = "LocalOffice";
			};
			class LocalOffice_Severo {
				name = "";
				position[] = {7986, 12732};
				type = "LocalOffice";
			};
			class LocalOffice_Gorka {
				name = "";
				position[] = {9503, 8805};
				type = "LocalOffice";
			};
			class LocalOffice_Mogi {
				name = "";
				position[] = {7562, 5149};
				type = "LocalOffice";
			};
			class LocalOffice_Nadez {
				name = "";
				position[] = {5906, 4844};
				type = "LocalOffice";
			};
			class LocalOffice_Cherno {
				name = "";
				position[] = {6607, 2418};
				type = "LocalOffice";
			};
			class LocalOffice_Kozlov {
				name = "";
				position[] = {4394, 4629};
				type = "LocalOffice";
			};
			class LocalOffice_Komar {
				name = "";
				position[] = {3676, 2501};
				type = "LocalOffice";
			};
			class LocalOffice_Pavlovo {
				name = "";
				position[] = {1655, 3851};
				type = "LocalOffice";
			};
			class LocalOffice_Zeleno {
				name = "";
				position[] = {2854, 5282};
				type = "LocalOffice";
			};
			class LocalOffice_Elektro {
				name = "";
				position[] = {10212, 2295};
				type = "LocalOffice";
			};
			class LocalOffice_Kamys {
				name = "";
				position[] = {12155, 3514};
				type = "LocalOffice";
			};
			class LocalOffice_Staroye {
				name = "";
				position[] = {10106, 5457};
				type = "LocalOffice";
			};
			class LocalOffice_Dolina {
				name = "";
				position[] = {11217, 6564};
				type = "LocalOffice";
			};
			class LocalOffice_Solnich {
				name = "";
				position[] = {13383, 6225};
				type = "LocalOffice";
			};
			class LocalOffice_Polana {
				name = "";
				position[] = {10676, 7989};
				type = "LocalOffice";
			};
			class LocalOffice_Berezino {
				name = "";
				position[] = {12272, 9474};
				type = "LocalOffice";
			};
			class LocalOffice_Krasno {
				name = "";
				position[] = {11257, 12211};
				type = "LocalOffice";
			};
			class LocalOffice_Svetlo {
				name = "";
				position[] = {14013, 13335};
				type = "LocalOffice";
			};
			class LocalOffice_Cpol {
				name = "";
				position[] = {12110, 13779};
				type = "LocalOffice";
			};
			class LocalOffice_Novo {
				name = "";
				position[] = {11564, 14742};
				type = "LocalOffice";
			};
			class ViewPoint_1 {
				name = "";
				position[] = {567, 6997};
				type = "ViewPoint";
			};
			class ViewPoint_2 {
				name = "";
				position[] = {687, 6874};
				type = "ViewPoint";
			};
			class ViewPoint_3 {
				name = "";
				position[] = {724, 6510};
				type = "ViewPoint";
			};
			class ViewPoint_4 {
				name = "";
				position[] = {298, 3460};
				type = "ViewPoint";
			};
			class ViewPoint_5 {
				name = "";
				position[] = {172, 3412};
				type = "ViewPoint";
			};
			class ViewPoint_6 {
				name = "";
				position[] = {258, 3283};
				type = "ViewPoint";
			};
			class ViewPoint_7 {
				name = "";
				position[] = {322, 2697};
				type = "ViewPoint";
			};
			class ViewPoint_8 {
				name = "";
				position[] = {1671, 2211};
				type = "ViewPoint";
			};
			class ViewPoint_9 {
				name = "";
				position[] = {2915, 3995};
				type = "ViewPoint";
			};
			class ViewPoint_10 {
				name = "";
				position[] = {3371, 4551};
				type = "ViewPoint";
			};
			class ViewPoint_11 {
				name = "";
				position[] = {414, 9574};
				type = "ViewPoint";
			};
			class ViewPoint_12 {
				name = "";
				position[] = {241, 11867};
				type = "ViewPoint";
			};
			class ViewPoint_13 {
				name = "";
				position[] = {1153, 12411};
				type = "ViewPoint";
			};
			class ViewPoint_14 {
				name = "";
				position[] = {2320, 13650};
				type = "ViewPoint";
			};
			class ViewPoint_15 {
				name = "";
				position[] = {4084, 14570};
				type = "ViewPoint";
			};
			class ViewPoint_16 {
				name = "";
				position[] = {4445, 15045};
				type = "ViewPoint";
			};
			class ViewPoint_17 {
				name = "";
				position[] = {4926, 14820};
				type = "ViewPoint";
			};
			class ViewPoint_18 {
				name = "";
				position[] = {5172, 14841};
				type = "ViewPoint";
			};
			class ViewPoint_19 {
				name = "";
				position[] = {5345, 14873};
				type = "ViewPoint";
			};
			class ViewPoint_20 {
				name = "";
				position[] = {5480, 14822};
				type = "ViewPoint";
			};
			class ViewPoint_21 {
				name = "";
				position[] = {5692, 14771};
				type = "ViewPoint";
			};
			class ViewPoint_22 {
				name = "";
				position[] = {5896, 14747};
				type = "ViewPoint";
			};
			class ViewPoint_23 {
				name = "";
				position[] = {6246, 14707};
				type = "ViewPoint";
			};
			class ViewPoint_24 {
				name = "";
				position[] = {6434, 14816};
				type = "ViewPoint";
			};
			class ViewPoint_25 {
				name = "";
				position[] = {6707, 13961};
				type = "ViewPoint";
			};
			class ViewPoint_26 {
				name = "";
				position[] = {6861, 14115};
				type = "ViewPoint";
			};
			class ViewPoint_27 {
				name = "";
				position[] = {8710, 14200};
				type = "ViewPoint";
			};
			class ViewPoint_28 {
				name = "";
				position[] = {10822, 14651};
				type = "ViewPoint";
			};
			class ViewPoint_29 {
				name = "";
				position[] = {11369, 15037};
				type = "ViewPoint";
			};
			class ViewPoint_30 {
				name = "";
				position[] = {13604, 14632};
				type = "ViewPoint";
			};
			class ViewPoint_31 {
				name = "";
				position[] = {13877, 14703};
				type = "ViewPoint";
			};
			class ViewPoint_32 {
				name = "";
				position[] = {14140, 15244};
				type = "ViewPoint";
			};
			class ViewPoint_33 {
				name = "";
				position[] = {13840, 13833};
				type = "ViewPoint";
			};
			class ViewPoint_34 {
				name = "";
				position[] = {14075, 13889};
				type = "ViewPoint";
			};
			class ViewPoint_35 {
				name = "";
				position[] = {14895, 13611};
				type = "ViewPoint";
			};
			class ViewPoint_36 {
				name = "";
				position[] = {14828, 13759};
				type = "ViewPoint";
			};
			class ViewPoint_37 {
				name = "";
				position[] = {11982, 13388};
				type = "ViewPoint";
			};
			class ViewPoint_38 {
				name = "";
				position[] = {11690, 13511};
				type = "ViewPoint";
			};
			class ViewPoint_39 {
				name = "";
				position[] = {11473, 13580};
				type = "ViewPoint";
			};
			class ViewPoint_40 {
				name = "";
				position[] = {11376, 13661};
				type = "ViewPoint";
			};
			class ViewPoint_41 {
				name = "";
				position[] = {13937, 11250};
				type = "ViewPoint";
			};
			class ViewPoint_42 {
				name = "";
				position[] = {11481, 8429};
				type = "ViewPoint";
			};
			class ViewPoint_43 {
				name = "";
				position[] = {11574, 7743};
				type = "ViewPoint";
			};
			class ViewPoint_44 {
				name = "";
				position[] = {12123, 6997};
				type = "ViewPoint";
			};
			class ViewPoint_45 {
				name = "";
				position[] = {12980, 5967};
				type = "ViewPoint";
			};
			class ViewPoint_46 {
				name = "";
				position[] = {12352, 4519};
				type = "ViewPoint";
			};
			class ViewPoint_47 {
				name = "";
				position[] = {12304, 3976};
				type = "ViewPoint";
			};
			class ViewPoint_48 {
				name = "";
				position[] = {10584, 2867};
				type = "ViewPoint";
			};
			class ViewPoint_49 {
				name = "";
				position[] = {8767, 2959};
				type = "ViewPoint";
			};
			class ViewPoint_50 {
				name = "";
				position[] = {8811, 2850};
				type = "ViewPoint";
			};
			class ViewPoint_51 {
				name = "";
				position[] = {8547, 3872};
				type = "ViewPoint";
			};
			class ViewPoint_52 {
				name = "";
				position[] = {9029, 4380};
				type = "ViewPoint";
			};
			class ViewPoint_53 {
				name = "";
				position[] = {10741, 4180};
				type = "ViewPoint";
			};
			class ViewPoint_54 {
				name = "";
				position[] = {6897, 11648};
				type = "ViewPoint";
			};
			class ViewPoint_55 {
				name = "";
				position[] = {5633, 6885};
				type = "ViewPoint";
			};
			class ViewPoint_56 {
				name = "";
				position[] = {1456, 9611};
				type = "ViewPoint";
			};
			class ViewPoint_57 {
				name = "";
				position[] = {1941, 6768};
				type = "ViewPoint";
			};
		};
	};
};
