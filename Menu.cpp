#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "XorStr.hpp"
#include "mac.h"
#include <fstream>

#define WINDOW_WIDTH 659
#define WINDOW_HEIGHT 808

YetiWindow Menu::Window;

void KnifeApplyCallbk()
{
	static ConVar* Meme = m_pCVar->FindVar(XorStr("cl_fullupdate"));
	Meme->nFlags &= ~FCVAR_CHEAT;
	m_pEngine->ClientCmd_Unrestricted(XorStr("cl_fullupdate"));
}

struct Config_t {
	int id;
	std::string name;
};

std::vector<Config_t> configs;

void save_callback()
{
	int should_save = Menu::Window.MiscTab.ConfigListBox.GetIndex();
	std::string config_directory = "yeti\\cfg\\";
	config_directory += configs[should_save].name; config_directory += ".xml";
	GUI.SaveWindowState(&Menu::Window, XorStr(config_directory.c_str()));
}

void load_callback()
{
	int should_load = Menu::Window.MiscTab.ConfigListBox.GetIndex();
	std::string config_directory = "yeti\\cfg\\";
	config_directory += configs[should_load].name; config_directory += ".xml";
	GUI.LoadWindowState(&Menu::Window, XorStr(config_directory.c_str()));
}

void list_configs() {
	configs.clear();
	Menu::Window.MiscTab.ConfigListBox.ClearItems();

	std::ifstream file_in;
	file_in.open("yeti\\cfg\\yeti_configs.txt");

	if (file_in.fail()) {
		std::ofstream("yeti\\cfg\\yeti_configs.txt");
		file_in.open("yeti\\cfg\\yeti_configs.txt");
	}

	int line_count;

	while (!file_in.eof()) {
		Config_t config;
		file_in >> config.name;
		config.id = line_count;
		configs.push_back(config);

		line_count++;

		Menu::Window.MiscTab.ConfigListBox.AddItem(config.name);
	}

	file_in.close();

	if (configs.size() > 7) Menu::Window.MiscTab.ConfigListBox.AddItem(" ");
}

void add_config() {
	std::fstream file;
	file.open("yeti\\cfg\\yeti_configs.txt", std::fstream::app);

	if (file.fail()) {
		std::fstream("yeti\\cfg\\yeti_configs.txt");
		file.open("yeti\\cfg\\yeti_configs.txt", std::fstream::app);
	}

	file << std::endl << Menu::Window.MiscTab.NewConfigName.getText();

	file.close();

	list_configs();

	int should_add = Menu::Window.MiscTab.ConfigListBox.GetIndex();
	std::string config_directory = "yeti\\cfg\\";
	config_directory += Menu::Window.MiscTab.NewConfigName.getText(); config_directory += ".xml";
	GUI.SaveWindowState(&Menu::Window, XorStr(config_directory.c_str()));

	Menu::Window.MiscTab.NewConfigName.SetText("");
}

void remove_config() {
	int should_remove = Menu::Window.MiscTab.ConfigListBox.GetIndex();

	std::string config_directory = "yeti\\cfg\\";
	config_directory += configs[should_remove].name; config_directory += ".xml";
	std::remove(config_directory.c_str());

	std::ofstream ofs("yeti\\cfg\\yeti_configs.txt", std::ios::out | std::ios::trunc);
	ofs.close();

	std::fstream file;
	file.open("yeti\\cfg\\yeti_configs.txt", std::fstream::app);

	if (file.fail()) {
		std::fstream("yeti\\cfg\\yeti_configs.txt");
		file.open("yeti\\cfg\\yeti_configs.txt", std::fstream::app);
	}
	
	for (int i = 0; i < configs.size(); i++) {
		if (i == should_remove) continue;
		Config_t config = configs[i];
		file << std::endl << config.name;
	}

	file.close();

	list_configs();
}

void UnLoadCallbk()
{
	DoUnload = true;
}

void YetiWindow::Setup()
{
	SetPosition(100, 49);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle(XorStr("Yeti"));

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&PlayersTab);
	RegisterTab(&SkinsTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	PlayersTab.Setup();
	SkinsTab.Setup();
}

void CRageBotTab::Setup( )
{
	SetTitle( XorStr( "a" ) );

	//Aimbot
	AimbotGroup.SetText( XorStr( "Aimbot" ) );
	AimbotGroup.SetSize( 293, 540 );
	AimbotGroup.SetPosition( 24, 16 );
	AimbotGroup.AddTab( CGroupTab( "Simple", 1 ) );
	AimbotGroup.AddTab( CGroupTab( "Advanced", 2 ) );
	AimbotGroup.AddTab( CGroupTab( "Hitscan", 3 ) );
	RegisterControl( &AimbotGroup );

	Enabled.SetFileId( XorStr( "r_enabled" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Activate" ), this, &Enabled );

	AimbotMode.SetFileId( XorStr( "r_aimbotmode" ) );
	AimbotMode.AddItem( XorStr( "Visualized" ) );
	AimbotMode.AddItem( XorStr( "Non-visualized" ) );
	AimbotMode.AddItem( XorStr( "Anti-aim" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Aimbot mode" ), this, &AimbotMode );

	TargetSelection.SetFileId( XorStr( "r_selection" ) );
	TargetSelection.AddItem( XorStr( "FoV" ) );
	TargetSelection.AddItem( XorStr( "Distance" ) );
	TargetSelection.AddItem( XorStr( "Lowest health" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Selection type" ), this, &TargetSelection );

	FriendlyFire.SetFileId( XorStr( "r_friendlyfire" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Friendly fire" ), this, &FriendlyFire );

	AutoFire.SetFileId( XorStr( "r_autofire" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Auto fire" ), this, &AutoFire );

	AutoFireKey.SetFileId( XorStr( "r_autofirekey" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "" ), this, &AutoFireKey );

	AutoFireMode.SetFileId( XorStr( "r_autofiremode" ) );
	AutoFireMode.AddItem( XorStr( "Bone" ) );
	AutoFireMode.AddItem( XorStr( "Key Press" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "" ), this, &AutoFireMode );

	AutoFireRevolverMode.SetFileId( XorStr( "r_autofirerevolver" ) );
	AutoFireRevolverMode.AddItem( XorStr( "Off" ) );
	AutoFireRevolverMode.AddItem( XorStr( "Primary" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Revolver shoot type" ), this, &AutoFireRevolverMode );

	AutoFireTarget.SetFileId( XorStr( "r_autofiretarget" ) );
	AutoFireTarget.AddItem( XorStr( "Head" ) );
	AutoFireTarget.AddItem( XorStr( "Neck" ) );
	AutoFireTarget.AddItem( XorStr( "Chest" ) );
	AutoFireTarget.AddItem( XorStr( "Stomach" ) );
	AutoFireTarget.AddItem( XorStr( "Pelvis" ) );
	AutoFireTarget.AddItem( XorStr( "Left arm" ) );
	AutoFireTarget.AddItem( XorStr( "Right arm" ) );
	AutoFireTarget.AddItem( XorStr( "Left leg" ) );
	AutoFireTarget.AddItem( XorStr( "Right leg" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Autofire bone" ), this, &AutoFireTarget );

	MaximumFov.SetFileId( XorStr( "r_fieldofview" ) );
	MaximumFov.SetBoundaries( 0, 180 );
	MaximumFov.extension = XorStr( "°" );
	MaximumFov.SetValue( 0 );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Limit FOV" ), this, &MaximumFov );

	AutoWall.SetFileId( XorStr( "r_autowall" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Automatic penetration" ), this, &AutoWall );

	MinimumDamage.SetFileId( XorStr( "r_mindmg" ) );
	MinimumDamage.SetBoundaries( 1, 100 );
	MinimumDamage.SetValue( 1 );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Minimum damage" ), this, &MinimumDamage );

	AimStep.SetFileId( XorStr( "r_aimstep" ) );
	AimbotGroup.PlaceLabledControl( 1, XorStr( "Aim step" ), this, &AimStep );

	PreferBaim.SetFileId( XorStr( "r_preferbaim" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Prefer body" ), this, &PreferBaim );

	BaimOnXHealth.SetFileId( XorStr( "r_baimonxhp" ) );
	BaimOnXHealth.SetBoundaries( 0, 100 );
	BaimOnXHealth.SetValue( 0 );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Body aim on x hp" ), this, &BaimOnXHealth );

	BaimIfDeadly.SetFileId( XorStr( "r_baimifdeadly" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Body aim if deadly" ), this, &BaimIfDeadly );

	BodyAimAwp.SetFileId( XorStr( "r_bodyaimawp" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Body aim with awp" ), this, &BodyAimAwp );

	BodyAimAwpMode.SetFileId( XorStr( "r_bodyaimawpmode" ) );
	BodyAimAwpMode.AddItem( XorStr( "Always" ) );
	BodyAimAwpMode.AddItem( XorStr( "Prefer" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "" ), this, &BodyAimAwpMode );

	BodyAimScout.SetFileId( XorStr( "r_bodyaimscout" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Body aim with scout" ), this, &BodyAimScout );

	BodyAimScoutMode.SetFileId( XorStr( "r_bodyaimscoutmode" ) );
	BodyAimScoutMode.AddItem( XorStr( "Always" ) );
	BodyAimScoutMode.AddItem( XorStr( "Prefer" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "" ), this, &BodyAimScoutMode );

	Pointscale.SetFileId( XorStr( "r_pointscale" ) );
	Pointscale.SetBoundaries( 0, 100 );
	Pointscale.extension = XorStr( "%%" );
	Pointscale.SetValue( 90 );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Pointscale" ), this, &Pointscale );

	MinimumHitChance.SetFileId( XorStr( "r_minhitchance" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Minimum hit chance" ), this, &MinimumHitChance );

	MinimumHitChanceAmount.SetFileId( XorStr( "r_minhitchanceamount" ) );
	MinimumHitChanceAmount.SetBoundaries( 0, 100 );
	MinimumHitChanceAmount.extension = XorStr( "%%" );
	MinimumHitChanceAmount.SetValue( 0 );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "" ), this, &MinimumHitChanceAmount );

	AutomaticScope.SetFileId( XorStr( "r_autoscope" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Automatic scope" ), this, &AutomaticScope );

	Autostop.SetFileId( XorStr( "r_autostop" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Automatic stop" ), this, &Autostop );

	AutostopType.SetFileId( XorStr( "r_autostoptype" ) );
	AutostopType.AddItem( XorStr( "Simple" ) );
	AutostopType.AddItem( XorStr( "Trigonometry" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "" ), this, &AutostopType );

	AccuracyNotCrouching.SetFileId( XorStr( "r_accuracynocrouch" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Accuracy while standing" ), this, &AccuracyNotCrouching );

	RemoveRecoil.SetFileId( XorStr( "r_removerecoil" ) );
	AimbotGroup.PlaceLabledControl( 2, XorStr( "Remove recoil" ), this, &RemoveRecoil );

	HitscanBones.SetFileId( XorStr( "r_hitscanbones" ) );
	HitscanBones.SetTitle( XorStr( "Bones" ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Head" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Neck" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Lower neck" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Upper chest" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Chest" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Lower chest" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Stomach" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Pelvis" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Left upper arm" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Left lower arm" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Left hand" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Right upper arm" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Right lower arm" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Right hand" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Left thigh" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Left shin" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Left foot" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Right thigh" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Right shin" ) ) );
	HitscanBones.items.push_back( MultiBoxItem( false, XorStr( "Right foot" ) ) );
	AimbotGroup.PlaceLabledControl( 3, XorStr( "" ), this, &HitscanBones );

	//Resolver
	ResolverOptions.SetText( XorStr( "Hit optimazation" ) );
	ResolverOptions.SetSize( 293, 260 );
	ResolverOptions.SetPosition( 333, 16 );
	RegisterControl( &ResolverOptions );

	AntiAimCorrection.SetFileId( XorStr( "r_antiaimcorrection" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "Anti-aim correction" ), this, &AntiAimCorrection );

	ResolverOverride.SetFileId( XorStr( "r_resolveroverride" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "Resolver override" ), this, &ResolverOverride );

	ResolverOverrideKey.SetFileId( XorStr( "r_resolveroverridekey" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "" ), this, &ResolverOverrideKey );

	DisableInterpolation.SetFileId( XorStr( "r_disableinterp" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "Disable interpolation" ), this, &DisableInterpolation );

	Backtracking.SetFileId( XorStr( "r_backtracking" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "Backtracking" ), this, &Backtracking );

	LinearExtrapolations.SetFileId( XorStr( "r_linearextrapolation" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "Linear Extrapolations" ), this, &LinearExtrapolations );

	/*AngleLodge.SetFileId(XorStr("r_anglelodge"));
	ResolverOptions.PlaceLabledControl(2, XorStr("Angle Lodge"), this, &AngleLodge);

	AngleLodgeSpeed.SetFileId(XorStr("r_anglelodgespeed"));
	AngleLodgeSpeed.SetFormat(SliderFormat::FORMAT_DECDIG2);
	AngleLodgeSpeed.SetValue(0.20);
	AngleLodgeSpeed.SetBoundaries(0, 1);
	ResolverOptions.PlaceLabledControl(2, XorStr(""), this, &AngleLodgeSpeed);*/

	ResolverDebug.SetFileId( XorStr( "r_resolverdebug" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "Debug" ), this, &ResolverDebug );

	LowerbodyIndicator.SetFileId( XorStr( "r_lbyindicator" ) );
	ResolverOptions.PlaceLabledControl( 0, XorStr( "Lowerbody indicator" ), this, &LowerbodyIndicator );

	//Anti Aim
	DefaultAntiAimGroup.SetText( XorStr( "Anti Aim" ) );
	DefaultAntiAimGroup.SetSize( 293, 272 );
	DefaultAntiAimGroup.SetPosition( 333, 284 );
	DefaultAntiAimGroup.AddTab( CGroupTab( "Stagnant", 1 ) );
	DefaultAntiAimGroup.AddTab( CGroupTab( "Moving", 2 ) );
	DefaultAntiAimGroup.AddTab( CGroupTab( "Edge", 3 ) );
	DefaultAntiAimGroup.AddTab( CGroupTab( "Modifiers", 4 ) );
	RegisterControl( &DefaultAntiAimGroup );

	stagnant_aa.RealPitch.SetFileId( XorStr( "r_stagnant_realpitch" ) );
	stagnant_aa.RealPitch.AddItem( XorStr( "Off" ) );
	stagnant_aa.RealPitch.AddItem( XorStr( "Down" ) );
	stagnant_aa.RealPitch.AddItem( XorStr( "Fake down" ) );
	stagnant_aa.RealPitch.AddItem( XorStr( "Up" ) );
	stagnant_aa.RealPitch.AddItem( XorStr( "Fake up" ) );
	stagnant_aa.RealPitch.AddItem( XorStr( "Random" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 1, XorStr( "Pitch" ), this, &stagnant_aa.RealPitch );

	stagnant_aa.RealYaw.SetFileId( XorStr( "r_stagnant_realyaw" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "Off" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "90°" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "180°" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "Crooked" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "Jitter" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "Swap" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "Rotate" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "Lowerbody" ) );
	stagnant_aa.RealYaw.AddItem( XorStr( "Corruption" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 1, XorStr( "Real yaw" ), this, &stagnant_aa.RealYaw );

	stagnant_aa.RealYawOffset.SetFileId( XorStr( "r_stagnant_realyawoffset" ) );
	stagnant_aa.RealYawOffset.SetBoundaries( -180, 180 );
	stagnant_aa.RealYawOffset.extension = XorStr( "°" );
	stagnant_aa.RealYawOffset.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 1, XorStr( "" ), this, &stagnant_aa.RealYawOffset );

	stagnant_aa.FakeYaw.SetFileId( XorStr( "r_stagnant_fakeyaw" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "Off" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "90°" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "180°" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "Crooked" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "Jitter" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "Swap" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "Rotate" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "Lowerbody" ) );
	stagnant_aa.FakeYaw.AddItem( XorStr( "Corruption" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 1, XorStr( "Fake yaw" ), this, &stagnant_aa.FakeYaw );

	stagnant_aa.FakeYawOffset.SetFileId( XorStr( "r_stagnant_fakeyawoffset" ) );
	stagnant_aa.FakeYawOffset.SetBoundaries( -180, 180 );
	stagnant_aa.FakeYawOffset.extension = XorStr( "°" );
	stagnant_aa.FakeYawOffset.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 1, XorStr( "" ), this, &stagnant_aa.FakeYawOffset );

	stagnant_aa.CleanUp.SetFileId( XorStr( "r_moving_cleanup" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 1, XorStr( "Clean up" ), this, &stagnant_aa.CleanUp );

	moving_aa.RealPitch.SetFileId( XorStr( "r_moving_realpitch" ) );
	moving_aa.RealPitch.AddItem( XorStr( "Off" ) );
	moving_aa.RealPitch.AddItem( XorStr( "Down" ) );
	moving_aa.RealPitch.AddItem( XorStr( "Fake down" ) );
	moving_aa.RealPitch.AddItem( XorStr( "Up" ) );
	moving_aa.RealPitch.AddItem( XorStr( "Fake up" ) );
	moving_aa.RealPitch.AddItem( XorStr( "Random" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 2, XorStr( "Pitch" ), this, &moving_aa.RealPitch );

	moving_aa.RealYaw.SetFileId( XorStr( "r_moving_realyaw" ) );
	moving_aa.RealYaw.AddItem( XorStr( "Off" ) );
	moving_aa.RealYaw.AddItem( XorStr( "90°" ) );
	moving_aa.RealYaw.AddItem( XorStr( "180°" ) );
	moving_aa.RealYaw.AddItem( XorStr( "Crooked" ) );
	moving_aa.RealYaw.AddItem( XorStr( "Jitter" ) );
	moving_aa.RealYaw.AddItem( XorStr( "Swap" ) );
	moving_aa.RealYaw.AddItem( XorStr( "Rotate" ) );
	moving_aa.RealYaw.AddItem( XorStr( "Lowerbody" ) );
	moving_aa.RealYaw.AddItem( XorStr( "Corruption" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 2, XorStr( "Real yaw" ), this, &moving_aa.RealYaw );

	moving_aa.RealYawOffset.SetFileId( XorStr( "r_moving_realyawoffset" ) );
	moving_aa.RealYawOffset.SetBoundaries( -180, 180 );
	moving_aa.RealYawOffset.extension = XorStr( "°" );
	moving_aa.RealYawOffset.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 2, XorStr( "" ), this, &moving_aa.RealYawOffset );

	moving_aa.FakeYaw.SetFileId( XorStr( "r_moving_fakeyaw" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "Off" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "90°" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "180°" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "Crooked" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "Jitter" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "Swap" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "Rotate" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "Lowerbody" ) );
	moving_aa.FakeYaw.AddItem( XorStr( "Corruption" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 2, XorStr( "Fake yaw" ), this, &moving_aa.FakeYaw );

	moving_aa.FakeYawOffset.SetFileId( XorStr( "r_moving_fakeyawoffset" ) );
	moving_aa.FakeYawOffset.SetBoundaries( -180, 180 );
	moving_aa.FakeYawOffset.extension = XorStr( "°" );
	moving_aa.FakeYawOffset.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 2, XorStr( "" ), this, &moving_aa.FakeYawOffset );

	moving_aa.CleanUp.SetFileId( XorStr( "r_moving_cleanup" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 2, XorStr( "Clean up" ), this, &moving_aa.CleanUp );

	edge_aa.Type.SetFileId( XorStr( "r_edge_walldtc" ) );
	edge_aa.Type.AddItem( XorStr( "Off" ) );
	edge_aa.Type.AddItem( XorStr( "Freestanding" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 3, XorStr( "Edge type" ), this, &edge_aa.Type );

	edge_aa.RealPitch.SetFileId( XorStr( "r_edge_realpitch" ) );
	edge_aa.RealPitch.AddItem( XorStr( "Off" ) );
	edge_aa.RealPitch.AddItem( XorStr( "Down" ) );
	edge_aa.RealPitch.AddItem( XorStr( "Fake down" ) );
	edge_aa.RealPitch.AddItem( XorStr( "Up" ) );
	edge_aa.RealPitch.AddItem( XorStr( "Fake up" ) );
	edge_aa.RealPitch.AddItem( XorStr( "Random" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 3, XorStr( "Pitch" ), this, &edge_aa.RealPitch );

	edge_aa.RealYaw.SetFileId( XorStr( "r_edge_realyaw" ) );
	edge_aa.RealYaw.AddItem( XorStr( "Off" ) );
	edge_aa.RealYaw.AddItem( XorStr( "90°" ) );
	edge_aa.RealYaw.AddItem( XorStr( "180°" ) );
	edge_aa.RealYaw.AddItem( XorStr( "Crooked" ) );
	edge_aa.RealYaw.AddItem( XorStr( "Jitter" ) );
	edge_aa.RealYaw.AddItem( XorStr( "Swap" ) );
	edge_aa.RealYaw.AddItem( XorStr( "Rotate" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 3, XorStr( "Real yaw" ), this, &edge_aa.RealYaw );

	edge_aa.RealYawOffset.SetFileId( XorStr( "r_edge_realyawoffset" ) );
	edge_aa.RealYawOffset.SetBoundaries( -180, 180 );
	edge_aa.RealYawOffset.extension = XorStr( "°" );
	edge_aa.RealYawOffset.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 3, XorStr( "" ), this, &edge_aa.RealYawOffset );

	edge_aa.FakeYaw.SetFileId( XorStr( "r_edge_fakeyaw" ) );
	edge_aa.FakeYaw.AddItem( XorStr( "Off" ) );
	edge_aa.FakeYaw.AddItem( XorStr( "90°" ) );
	edge_aa.FakeYaw.AddItem( XorStr( "180°" ) );
	edge_aa.FakeYaw.AddItem( XorStr( "Crooked" ) );
	edge_aa.FakeYaw.AddItem( XorStr( "Jitter" ) );
	edge_aa.FakeYaw.AddItem( XorStr( "Swap" ) );
	edge_aa.FakeYaw.AddItem( XorStr( "Rotate" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 3, XorStr( "Fake yaw" ), this, &edge_aa.FakeYaw );

	edge_aa.FakeYawOffset.SetFileId( XorStr( "r_edge_fakeyawoffset" ) );
	edge_aa.FakeYawOffset.SetBoundaries( -180, 180 );
	edge_aa.FakeYawOffset.extension = XorStr( "°" );
	edge_aa.FakeYawOffset.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 3, XorStr( "" ), this, &edge_aa.FakeYawOffset );

	DormantCheck.SetFileId( XorStr( "r_dormantcheck" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 4, XorStr( "Dormant check" ), this, &DormantCheck );

	RealYawDirection.SetFileId( XorStr( "r_realyawdirection" ) );
	RealYawDirection.AddItem( XorStr( "Free look" ) );
	RealYawDirection.AddItem( XorStr( "Static" ) );
	RealYawDirection.AddItem( XorStr( "Dynamic" ) );
	DefaultAntiAimGroup.PlaceLabledControl( 4, XorStr( "Yaw direction" ), this, &RealYawDirection );

	RotateSpeed.SetFileId( XorStr( "r_rotatespeed" ) );
	RotateSpeed.SetBoundaries( 0, 5 );
	RotateSpeed.SetValue( 0 );
	RotateSpeed.SetFormat( SliderFormat::FORMAT_DECDIG2 );
	DefaultAntiAimGroup.PlaceLabledControl( 4, XorStr( "Rotate speed" ), this, &RotateSpeed );

	JitterRange.SetFileId( XorStr( "r_jitterrange" ) );
	JitterRange.SetBoundaries( 0, 180 );
	JitterRange.extension = XorStr( "°" );
	JitterRange.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 4, XorStr( "Jitter range" ), this, &JitterRange );

	Lbybreaker.SetFileId( "r_lbybreaker" );
	DefaultAntiAimGroup.PlaceLabledControl( 4, XorStr( "Lowerbody breaker" ), this, &Lbybreaker );

	LbyDelta.SetFileId( XorStr( "r_lbydelta" ) );
	LbyDelta.SetBoundaries( -180, 180 );
	LbyDelta.extension = XorStr( "°" );
	LbyDelta.SetValue( 0 );
	DefaultAntiAimGroup.PlaceLabledControl( 4, XorStr( "" ), this, &LbyDelta );
}

void CLegitTab::Setup()
{
	SetTitle(XorStr("b"));

	AimbotGroup.SetText(XorStr("Aimbot"));
	AimbotGroup.SetSize(293, 540);
	AimbotGroup.SetPosition(24, 16);
	AimbotGroup.AddTab(CGroupTab("Pistol", 1));
	AimbotGroup.AddTab(CGroupTab("SMG", 2));
	AimbotGroup.AddTab(CGroupTab("Rifle", 3));
	AimbotGroup.AddTab(CGroupTab("Shotgun", 4));
	AimbotGroup.AddTab(CGroupTab("Snipers", 5));
	RegisterControl(&AimbotGroup);

	// pistol
	pistol.Enabled.SetFileId(XorStr("pistol_enabled"));
	AimbotGroup.PlaceLabledControl(1, XorStr("Activate"), this, &pistol.Enabled);

	pistol.AimbotMode.SetFileId(XorStr("pistol_mode"));
	pistol.AimbotMode.AddItem(XorStr("Visualized"));
	pistol.AimbotMode.AddItem(XorStr("Non-visualized"));
	pistol.AimbotMode.AddItem(XorStr("Anti-aim"));
	AimbotGroup.PlaceLabledControl(1, XorStr("Aimbot mode"), this, &pistol.AimbotMode);

	pistol.AutoFire.SetFileId(XorStr("pistol_autofire"));
	AimbotGroup.PlaceLabledControl(1, XorStr("Auto fire"), this, &pistol.AutoFire);

	pistol.AutoFireKey.SetFileId(XorStr("pistol_autofirekey"));
	AimbotGroup.PlaceLabledControl(1, XorStr(""), this, &pistol.AutoFireKey);

	pistol.AutoFireMode.SetFileId(XorStr("pistol_autofiremode"));
	pistol.AutoFireMode.AddItem(XorStr("Bone"));
	pistol.AutoFireMode.AddItem(XorStr("Key Press"));
	AimbotGroup.PlaceLabledControl(1, XorStr(""), this, &pistol.AutoFireMode);

	pistol.AutoFireTarget.SetFileId(XorStr("pistol_autofiretarget"));
	pistol.AutoFireTarget.AddItem(XorStr("Head"));
	pistol.AutoFireTarget.AddItem(XorStr("Neck"));
	pistol.AutoFireTarget.AddItem(XorStr("Chest"));
	pistol.AutoFireTarget.AddItem(XorStr("Stomach"));
	pistol.AutoFireTarget.AddItem(XorStr("Pelvis"));
	pistol.AutoFireTarget.AddItem(XorStr("Left arm"));
	pistol.AutoFireTarget.AddItem(XorStr("Right arm"));
	pistol.AutoFireTarget.AddItem(XorStr("Left leg"));
	pistol.AutoFireTarget.AddItem(XorStr("Right leg"));
	AimbotGroup.PlaceLabledControl(1, XorStr("Autofire bone"), this, &pistol.AutoFireTarget);

	pistol.FieldOfView.SetFileId(XorStr("pistol_fov"));
	pistol.FieldOfView.SetBoundaries(0, 90);
	pistol.FieldOfView.extension = XorStr( "°" );
	pistol.FieldOfView.SetValue(0);
	pistol.FieldOfView.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(1, XorStr("Limit FOV"), this, &pistol.FieldOfView);

	if (macsys.is_dev)
	{
		pistol.ReactionTime.SetFileId(XorStr("pistol_reaction"));
		pistol.ReactionTime.AddItem(XorStr("Slow"));
		pistol.ReactionTime.AddItem(XorStr("Medium"));
		pistol.ReactionTime.AddItem(XorStr("Fast"));
		pistol.ReactionTime.AddItem(XorStr("Insane"));
		AimbotGroup.PlaceLabledControl(1, XorStr("Reaction time"), this, &pistol.ReactionTime);
	}

	pistol.Smoothing.SetFileId(XorStr("pistol_smoothing"));
	pistol.Smoothing.SetBoundaries(0, 5);
	pistol.Smoothing.SetValue(0);
	pistol.Smoothing.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(1, XorStr("Sensitivity"), this, &pistol.Smoothing);

	pistol.Recoil.SetFileId(XorStr("pistol_recoil"));
	pistol.Recoil.SetBoundaries(0, 2);
	pistol.Recoil.SetValue(0);
	pistol.Recoil.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(1, XorStr("Recoil"), this, &pistol.Recoil);

	// smg
	smg.Enabled.SetFileId(XorStr("smg_enabled"));
	AimbotGroup.PlaceLabledControl(2, XorStr("Activate"), this, &smg.Enabled);

	smg.AimbotMode.SetFileId(XorStr("smg_mode"));
	smg.AimbotMode.AddItem(XorStr("Visualized"));
	smg.AimbotMode.AddItem(XorStr("Non-visualized"));
	smg.AimbotMode.AddItem(XorStr("Anti-aim"));
	AimbotGroup.PlaceLabledControl(2, XorStr("Aimbot mode"), this, &smg.AimbotMode);

	smg.AutoFire.SetFileId(XorStr("smg_autofire"));
	AimbotGroup.PlaceLabledControl(2, XorStr("Auto fire"), this, &smg.AutoFire);

	smg.AutoFireKey.SetFileId(XorStr("smg_autofirekey"));
	AimbotGroup.PlaceLabledControl(2, XorStr(""), this, &smg.AutoFireKey);

	smg.AutoFireMode.SetFileId(XorStr("smg_autofiremode"));
	smg.AutoFireMode.AddItem(XorStr("Bone"));
	smg.AutoFireMode.AddItem(XorStr("Key Press"));
	AimbotGroup.PlaceLabledControl(2, XorStr(""), this, &smg.AutoFireMode);

	smg.AutoFireTarget.SetFileId(XorStr("smg_autofiretarget"));
	smg.AutoFireTarget.AddItem(XorStr("Head"));
	smg.AutoFireTarget.AddItem(XorStr("Neck"));
	smg.AutoFireTarget.AddItem(XorStr("Chest"));
	smg.AutoFireTarget.AddItem(XorStr("Stomach"));
	smg.AutoFireTarget.AddItem(XorStr("Pelvis"));
	smg.AutoFireTarget.AddItem(XorStr("Left arm"));
	smg.AutoFireTarget.AddItem(XorStr("Right arm"));
	smg.AutoFireTarget.AddItem(XorStr("Left leg"));
	smg.AutoFireTarget.AddItem(XorStr("Right leg"));
	AimbotGroup.PlaceLabledControl(2, XorStr("Autofire bone"), this, &smg.AutoFireTarget);

	smg.FieldOfView.SetFileId(XorStr("smg_fov"));
	smg.FieldOfView.SetBoundaries(0, 90);
	smg.FieldOfView.extension = XorStr( "°" );
	smg.FieldOfView.SetValue(0);
	smg.FieldOfView.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(2, XorStr("Limit FOV"), this, &smg.FieldOfView);

	if (macsys.is_dev)
	{
		smg.ReactionTime.SetFileId(XorStr("smg_reaction"));
		smg.ReactionTime.AddItem(XorStr("Slow"));
		smg.ReactionTime.AddItem(XorStr("Medium"));
		smg.ReactionTime.AddItem(XorStr("Fast"));
		smg.ReactionTime.AddItem(XorStr("Insane"));
		AimbotGroup.PlaceLabledControl(2, XorStr("Reaction time"), this, &smg.ReactionTime);
	}

	smg.Smoothing.SetFileId(XorStr("smg_smoothing"));
	smg.Smoothing.SetBoundaries(0, 5);
	smg.Smoothing.SetValue(0);
	smg.Smoothing.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(2, XorStr("Sensitivity"), this, &smg.Smoothing);

	smg.Recoil.SetFileId(XorStr("smg_recoil"));
	smg.Recoil.SetBoundaries(0, 2);
	smg.Recoil.SetValue(0);
	smg.Recoil.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(2, XorStr("Recoil"), this, &smg.Recoil);

	// Rifle
	rifle.Enabled.SetFileId(XorStr("rifle_enabled"));
	AimbotGroup.PlaceLabledControl(3, XorStr("Activate"), this, &rifle.Enabled);

	rifle.AimbotMode.SetFileId(XorStr("rifle_mode"));
	rifle.AimbotMode.AddItem(XorStr("Visualized"));
	rifle.AimbotMode.AddItem(XorStr("Non-visualized"));
	rifle.AimbotMode.AddItem(XorStr("Anti-aim"));
	AimbotGroup.PlaceLabledControl(3, XorStr("Aimbot mode"), this, &rifle.AimbotMode);

	rifle.AutoFire.SetFileId(XorStr("rifle_autofire"));
	AimbotGroup.PlaceLabledControl(3, XorStr("Auto fire"), this, &rifle.AutoFire);

	rifle.AutoFireKey.SetFileId(XorStr("rifle_autofirekey"));
	AimbotGroup.PlaceLabledControl(3, XorStr(""), this, &rifle.AutoFireKey);

	rifle.AutoFireMode.SetFileId(XorStr("rifle_autofiremode"));
	rifle.AutoFireMode.AddItem(XorStr("Bone"));
	rifle.AutoFireMode.AddItem(XorStr("Key Press"));
	AimbotGroup.PlaceLabledControl(3, XorStr(""), this, &rifle.AutoFireMode);

	rifle.AutoFireTarget.SetFileId(XorStr("rifle_autofiretarget"));
	rifle.AutoFireTarget.AddItem(XorStr("Head"));
	rifle.AutoFireTarget.AddItem(XorStr("Neck"));
	rifle.AutoFireTarget.AddItem(XorStr("Chest"));
	rifle.AutoFireTarget.AddItem(XorStr("Stomach"));
	rifle.AutoFireTarget.AddItem(XorStr("Pelvis"));
	rifle.AutoFireTarget.AddItem(XorStr("Left arm"));
	rifle.AutoFireTarget.AddItem(XorStr("Right arm"));
	rifle.AutoFireTarget.AddItem(XorStr("Left leg"));
	rifle.AutoFireTarget.AddItem(XorStr("Right leg"));
	AimbotGroup.PlaceLabledControl(3, XorStr("Autofire bone"), this, &rifle.AutoFireTarget);

	rifle.FieldOfView.SetFileId(XorStr("rifle_fov"));
	rifle.FieldOfView.SetBoundaries(0, 90);
	rifle.FieldOfView.extension = XorStr( "°" );
	rifle.FieldOfView.SetValue(0);
	rifle.FieldOfView.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(3, XorStr("Limit FOV"), this, &rifle.FieldOfView);

	if (macsys.is_dev)
	{
		rifle.ReactionTime.SetFileId(XorStr("rifle_reaction"));
		rifle.ReactionTime.AddItem(XorStr("Slow"));
		rifle.ReactionTime.AddItem(XorStr("Medium"));
		rifle.ReactionTime.AddItem(XorStr("Fast"));
		rifle.ReactionTime.AddItem(XorStr("Insane"));
		AimbotGroup.PlaceLabledControl(3, XorStr("Reaction time"), this, &rifle.ReactionTime);
	}

	rifle.Smoothing.SetFileId(XorStr("rifle_smoothing"));
	rifle.Smoothing.SetBoundaries(0, 5);
	rifle.Smoothing.SetValue(0);
	rifle.Smoothing.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(3, XorStr("Sensitivity"), this, &rifle.Smoothing);

	rifle.Recoil.SetFileId(XorStr("rifle_recoil"));
	rifle.Recoil.SetBoundaries(0, 2);
	rifle.Recoil.SetValue(0);
	rifle.Recoil.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(3, XorStr("Recoil"), this, &rifle.Recoil);

	// Shotgun
	shotgun.Enabled.SetFileId(XorStr("shotgun_enabled"));
	AimbotGroup.PlaceLabledControl(4, XorStr("Activate"), this, &shotgun.Enabled);

	shotgun.AimbotMode.SetFileId(XorStr("shotgun_mode"));
	shotgun.AimbotMode.AddItem(XorStr("Visualized"));
	shotgun.AimbotMode.AddItem(XorStr("Non-visualized"));
	shotgun.AimbotMode.AddItem(XorStr("Anti-aim"));
	AimbotGroup.PlaceLabledControl(4, XorStr("Aimbot mode"), this, &shotgun.AimbotMode);

	shotgun.AutoFire.SetFileId(XorStr("shotgun_autofire"));
	AimbotGroup.PlaceLabledControl(4, XorStr("Auto fire"), this, &shotgun.AutoFire);

	shotgun.AutoFireKey.SetFileId(XorStr("shotgun_autofirekey"));
	AimbotGroup.PlaceLabledControl(4, XorStr(""), this, &shotgun.AutoFireKey);

	shotgun.AutoFireMode.SetFileId(XorStr("shotgun_autofiremode"));
	shotgun.AutoFireMode.AddItem(XorStr("Bone"));
	shotgun.AutoFireMode.AddItem(XorStr("Key Press"));
	AimbotGroup.PlaceLabledControl(4, XorStr(""), this, &shotgun.AutoFireMode);

	shotgun.AutoFireTarget.SetFileId(XorStr("shotgun_autofiretarget"));
	shotgun.AutoFireTarget.AddItem(XorStr("Head"));
	shotgun.AutoFireTarget.AddItem(XorStr("Neck"));
	shotgun.AutoFireTarget.AddItem(XorStr("Chest"));
	shotgun.AutoFireTarget.AddItem(XorStr("Stomach"));
	shotgun.AutoFireTarget.AddItem(XorStr("Pelvis"));
	shotgun.AutoFireTarget.AddItem(XorStr("Left arm"));
	shotgun.AutoFireTarget.AddItem(XorStr("Right arm"));
	shotgun.AutoFireTarget.AddItem(XorStr("Left leg"));
	shotgun.AutoFireTarget.AddItem(XorStr("Right leg"));
	AimbotGroup.PlaceLabledControl(4, XorStr("Autofire bone"), this, &shotgun.AutoFireTarget);

	shotgun.FieldOfView.SetFileId(XorStr("shotgun_fov"));
	shotgun.FieldOfView.SetBoundaries(0, 90);
	shotgun.FieldOfView.extension = XorStr( "°" );
	shotgun.FieldOfView.SetValue(0);
	shotgun.FieldOfView.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(4, XorStr("Limit FOV"), this, &shotgun.FieldOfView);

	if (macsys.is_dev)
	{
		shotgun.ReactionTime.SetFileId(XorStr("shotgun_reaction"));
		shotgun.ReactionTime.AddItem(XorStr("Slow"));
		shotgun.ReactionTime.AddItem(XorStr("Medium"));
		shotgun.ReactionTime.AddItem(XorStr("Fast"));
		shotgun.ReactionTime.AddItem(XorStr("Insane"));
		AimbotGroup.PlaceLabledControl(4, XorStr("Reaction time"), this, &shotgun.ReactionTime);
	}

	shotgun.Smoothing.SetFileId(XorStr("shotgun_smoothing"));
	shotgun.Smoothing.SetBoundaries(0, 5);
	shotgun.Smoothing.SetValue(0);
	shotgun.Smoothing.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(4, XorStr("Sensitivity"), this, &shotgun.Smoothing);

	shotgun.Recoil.SetFileId(XorStr("shotgun_recoil"));
	shotgun.Recoil.SetBoundaries(0, 2);
	shotgun.Recoil.SetValue(0);
	shotgun.Recoil.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(4, XorStr("Recoil"), this, &shotgun.Recoil);
	
	// Sniper
	sniper.Enabled.SetFileId(XorStr("sniper_enabled"));
	AimbotGroup.PlaceLabledControl(5, XorStr("Activate"), this, &sniper.Enabled);

	sniper.AimbotMode.SetFileId(XorStr("sniper_mode"));
	sniper.AimbotMode.AddItem(XorStr("Visualized"));
	sniper.AimbotMode.AddItem(XorStr("Non-visualized"));
	sniper.AimbotMode.AddItem(XorStr("Anti-aim"));
	AimbotGroup.PlaceLabledControl(5, XorStr("Aimbot mode"), this, &sniper.AimbotMode);

	sniper.AutoFire.SetFileId(XorStr("sniper_autofire"));
	AimbotGroup.PlaceLabledControl(5, XorStr("Auto fire"), this, &sniper.AutoFire);

	sniper.AutoFireKey.SetFileId(XorStr("sniper_autofirekey"));
	AimbotGroup.PlaceLabledControl(5, XorStr(""), this, &sniper.AutoFireKey);

	sniper.AutoFireMode.SetFileId(XorStr("sniper_autofiremode"));
	sniper.AutoFireMode.AddItem(XorStr("Bone"));
	sniper.AutoFireMode.AddItem(XorStr("Key Press"));
	AimbotGroup.PlaceLabledControl(5, XorStr(""), this, &sniper.AutoFireMode);

	sniper.AutoFireTarget.SetFileId(XorStr("sniper_autofiretarget"));
	sniper.AutoFireTarget.AddItem(XorStr("Head"));
	sniper.AutoFireTarget.AddItem(XorStr("Neck"));
	sniper.AutoFireTarget.AddItem(XorStr("Chest"));
	sniper.AutoFireTarget.AddItem(XorStr("Stomach"));
	sniper.AutoFireTarget.AddItem(XorStr("Pelvis"));
	sniper.AutoFireTarget.AddItem(XorStr("Left arm"));
	sniper.AutoFireTarget.AddItem(XorStr("Right arm"));
	sniper.AutoFireTarget.AddItem(XorStr("Left leg"));
	sniper.AutoFireTarget.AddItem(XorStr("Right leg"));
	AimbotGroup.PlaceLabledControl(5, XorStr("Autofire bone"), this, &sniper.AutoFireTarget);

	sniper.FieldOfView.SetFileId(XorStr("sniper_fov"));
	sniper.FieldOfView.SetBoundaries(0, 90);
	sniper.FieldOfView.extension = XorStr( "°" );
	sniper.FieldOfView.SetValue(0);
	sniper.FieldOfView.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(5, XorStr("Limit FOV"), this, &sniper.FieldOfView);

	if (macsys.is_dev)
	{
		sniper.ReactionTime.SetFileId(XorStr("sniper_reaction"));
		sniper.ReactionTime.AddItem(XorStr("Slow"));
		sniper.ReactionTime.AddItem(XorStr("Medium"));
		sniper.ReactionTime.AddItem(XorStr("Fast"));
		sniper.ReactionTime.AddItem(XorStr("Insane"));
		AimbotGroup.PlaceLabledControl(5, XorStr("Reaction time"), this, &sniper.ReactionTime);
	}

	sniper.Smoothing.SetFileId(XorStr("sniper_smoothing"));
	sniper.Smoothing.SetBoundaries(0, 5);
	sniper.Smoothing.SetValue(0);
	sniper.Smoothing.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(5, XorStr("Sensitivity"), this, &sniper.Smoothing);

	sniper.Recoil.SetFileId(XorStr("sniper_recoil"));
	sniper.Recoil.SetBoundaries(0, 2);
	sniper.Recoil.SetValue(0);
	sniper.Recoil.SetFormat(FORMAT_DECDIG2);
	AimbotGroup.PlaceLabledControl(5, XorStr("Recoil"), this, &sniper.Recoil);
}

void CVisualTab::Setup()
{
	SetTitle(XorStr("c"));

	// Player ESP
	PlayerESP.SetText(XorStr("Player ESP"));
	PlayerESP.SetSize(293, 380);
	PlayerESP.SetPosition(24, 16);
	PlayerESP.AddTab(CGroupTab(XorStr("One"), 1));
	PlayerESP.AddTab(CGroupTab(XorStr("Two"), 2));
	RegisterControl(&PlayerESP);

	player.ActivationType.SetFileId(XorStr("v_player_activationtype"));
	player.ActivationType.AddItem(XorStr("Default"));
	player.ActivationType.AddItem(XorStr("Visible Only"));
	PlayerESP.PlaceLabledControl(1, XorStr("Activation type"), this, &player.ActivationType);

	player.EspTeammates.SetFileId(XorStr("v_player_teammates"));
	PlayerESP.PlaceLabledControl(1, XorStr("Teammates"), this, &player.EspTeammates);

	player.EspBox.SetFileId(XorStr("v_player_espbox"));
	PlayerESP.PlaceLabledControl(1, XorStr("Box"), this, &player.EspBox);

	player.BoxColor.SetFileId(XorStr("v_player_espbox_color"));
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.BoxColor);

	player.BoxType.SetFileId(XorStr("v_player_boxtype"));
	player.BoxType.AddItem(XorStr("Full"));
	player.BoxType.AddItem(XorStr("Cornered"));
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.BoxType);

	player.EspMode.SetFileId(XorStr("v_player_espmode"));
	player.EspMode.AddItem(XorStr("Static"));
	player.EspMode.AddItem(XorStr("Dynamic"));
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.EspMode);

	player.EspOutlines.SetFileId(XorStr("v_player_outlines"));
	PlayerESP.PlaceLabledControl(1, XorStr("Outlined"), this, &player.EspOutlines);

	player.BoxFill.SetFileId(XorStr("v_player_boxfill"));
	PlayerESP.PlaceLabledControl(1, XorStr("Box Fill"), this, &player.BoxFill);

	player.BoxFillOpacity.SetFileId(XorStr("v_player_boxfillopacity"));
	player.BoxFillOpacity.SetBoundaries(0, 100);
	player.BoxFillOpacity.extension = XorStr( "%%" );
	player.BoxFillOpacity.SetValue(0);
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.BoxFillOpacity);

	player.Glow.SetFileId(XorStr("v_player_glow"));
	PlayerESP.PlaceLabledControl(1, XorStr("Glow"), this, &player.Glow);

	player.GlowColor.SetFileId(XorStr("v_player_glowcolor"));
	player.GlowColor.SetColor(24, 130, 230, 255);
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.GlowColor);

	player.GlowOpacity.SetFileId(XorStr("v_player_glowopacity"));
	player.GlowOpacity.SetBoundaries(0, 100);
	player.GlowOpacity.extension = XorStr( "%%" );
	player.GlowOpacity.SetValue(0);
	//player.GlowOpacity.SetFollowingChar("%");
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.GlowOpacity);

	player.ShowHealth.SetFileId(XorStr("v_player_health"));
	PlayerESP.PlaceLabledControl(1, XorStr("Health"), this, &player.ShowHealth);

	player.ShowHealthText.SetFileId(XorStr("v_player_healthtext"));
	PlayerESP.PlaceLabledControl(1, XorStr("Health text"), this, &player.ShowHealthText);

	player.ShowArmor.SetFileId(XorStr("v_player_armor"));
	PlayerESP.PlaceLabledControl(1, XorStr("Armor"), this, &player.ShowArmor);
	
	player.ArmorColor.SetFileId(XorStr("v_player_armor_color"));
	player.ArmorColor.SetColor(86, 123, 190, 255);
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.ArmorColor);

	player.ShowSkeletons.SetFileId(XorStr("v_player_skeletons"));
	PlayerESP.PlaceLabledControl(1, XorStr("Skeletons"), this, &player.ShowSkeletons);

	player.SkeletonColor.SetFileId(XorStr("v_player_skeletons_color"));
	player.SkeletonColor.SetColor(255, 255, 255, 255);
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.SkeletonColor);

	player.ShowHitbones.SetFileId(XorStr("v_player_hitbones"));
	PlayerESP.PlaceLabledControl(1, XorStr("Hitbones"), this, &player.ShowHitbones);

	player.HitbonesColor.SetFileId(XorStr("v_player_hitbones_color"));
	player.HitbonesColor.SetColor(255, 255, 255, 255);
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.HitbonesColor);

	player.SnapLines.SetFileId(XorStr("v_player_snaplines"));
	PlayerESP.PlaceLabledControl(1, XorStr("Snap lines"), this, &player.SnapLines);

	player.SnapLinesColor.SetFileId(XorStr("v_player_snaplines_color"));
	player.SnapLinesColor.SetColor(255, 255, 255, 255);
	PlayerESP.PlaceLabledControl(1, XorStr(""), this, &player.SnapLinesColor);

	player.DirectionArrow.SetFileId( XorStr( "v_direction_arrow" ) );
	PlayerESP.PlaceLabledControl( 1, XorStr( "Direction arrow" ), this, &player.DirectionArrow );

	player.DirectionArrowColor.SetFileId( XorStr( "v_direction_arrowcolor" ) );
	PlayerESP.PlaceLabledControl( 1, XorStr( "" ), this, &player.DirectionArrowColor );

	player.ShowPlayerNames.SetFileId(XorStr("v_player_playernames"));
	PlayerESP.PlaceLabledControl(2, XorStr("Player name"), this, &player.ShowPlayerNames);

	player.ShowWeaponNames.SetFileId(XorStr("v_player_weaponnames"));
	PlayerESP.PlaceLabledControl(2, XorStr("Weapon name"), this, &player.ShowWeaponNames);

	player.PlayerAmmo.SetFileId(XorStr("v_player_playerammo"));
	PlayerESP.PlaceLabledControl(2, XorStr("Ammo"), this, &player.PlayerAmmo);

	player.HitAngle.SetFileId(XorStr("v_player_hitangle"));
	PlayerESP.PlaceLabledControl(2, XorStr("Hit angle"), this, &player.HitAngle);

	player.Scoped.SetFileId(XorStr("v_player_scoped"));
	PlayerESP.PlaceLabledControl(2, XorStr("Scoped"), this, &player.Scoped);

	if (macsys.is_administrator)
	{
		player.Dormant.SetFileId(XorStr("v_player_dormant"));
		PlayerESP.PlaceLabledControl(2, XorStr("Dormant"), this, &player.Dormant);
	}

	// Chams Group
	ChamsGroup.SetText(XorStr("Colored models"));
	ChamsGroup.SetSize(293, 144);
	ChamsGroup.SetPosition(24, 412);
	RegisterControl(&ChamsGroup);

	PlayerChamType.SetFileId(XorStr("v_coloredmodels_mode"));
	PlayerChamType.AddItem(XorStr("Default"));
	PlayerChamType.AddItem(XorStr("Color"));
	PlayerChamType.AddItem(XorStr( "Flat" ) );
	PlayerChamType.AddItem(XorStr("Wireframe"));
	PlayerChamType.AddItem(XorStr("Ghost"));
	ChamsGroup.PlaceLabledControl(0, XorStr("Mode"), this, &PlayerChamType);

	ChamsEnemies.SetFileId(XorStr("v_coloredmodels_enemies"));
	ChamsGroup.PlaceLabledControl(0, XorStr("Enemies"), this, &ChamsEnemies);

	ChamsEnemies_Color.SetFileId(XorStr("v_coloredmodels_enemies_color"));
	ChamsGroup.PlaceLabledControl(0, XorStr(""), this, &ChamsEnemies_Color);

	ChamsEnemiesBehindWall.SetFileId(XorStr("v_coloredmodels_enemiesnovis"));
	ChamsGroup.PlaceLabledControl(0, XorStr("Enemies (behind wall)"), this, &ChamsEnemiesBehindWall);

	ChamsEnemiesBehindWall_Color.SetFileId(XorStr("v_coloredmodels_enemiesnovis_color"));
	ChamsGroup.PlaceLabledControl(0, XorStr(""), this, &ChamsEnemiesBehindWall_Color);

	ChamsTeammates.SetFileId(XorStr("v_coloredmodels_teammates"));
	ChamsGroup.PlaceLabledControl(0, XorStr("Teammates"), this, &ChamsTeammates);

	ChamsTeammates_Color.SetFileId(XorStr("v_coloredmodels_teammates_color"));
	ChamsGroup.PlaceLabledControl(0, XorStr(""), this, &ChamsTeammates_Color);

	ChamsTeammatesBehindWall.SetFileId(XorStr("v_coloredmodels_teammatesnovis"));
	ChamsGroup.PlaceLabledControl(0, XorStr("Teammates (behind wall)"), this, &ChamsTeammatesBehindWall);

	ChamsTeammatesBehindWall_Color.SetFileId(XorStr("v_coloredmodels_teammatesnovis_color"));
	ChamsGroup.PlaceLabledControl(0, XorStr(""), this, &ChamsTeammatesBehindWall_Color);

	// Other ESP
	OtherESP.SetText(XorStr("Other ESP"));
	OtherESP.SetSize(293, 209);
	OtherESP.SetPosition(333, 16);
	RegisterControl(&OtherESP);

	Weapons.SetFileId(XorStr("v_weapons"));
	OtherESP.PlaceLabledControl(0, XorStr("Weapons"), this, &Weapons);

	WeaponsColor.SetFileId(XorStr("v_weapons_color"));
	OtherESP.PlaceLabledControl(0, XorStr(""), this, &WeaponsColor);

	DroppedWeapons.SetFileId(XorStr("v_droppedweapons"));
	OtherESP.PlaceLabledControl(0, XorStr("Dropped weapons"), this, &DroppedWeapons);

	DroppedC4.SetFileId(XorStr("v_droppedc4"));
	OtherESP.PlaceLabledControl(0, XorStr("C4"), this, &DroppedC4);

	DroppedDefuseKit.SetFileId(XorStr("v_droppeddefusekit"));
	OtherESP.PlaceLabledControl(1, XorStr("Defuse kits"), this, &DroppedDefuseKit);

	BulletImpacts.SetFileId(XorStr("v_bulletimpacts"));
	OtherESP.PlaceLabledControl(0, XorStr("Bullet impacts"), this, &BulletImpacts);

	BulletImpactsColor.SetFileId(XorStr("v_bulletimpacts_color"));
	OtherESP.PlaceLabledControl(0, XorStr(""), this, &BulletImpactsColor);

	Hitmarkers.SetFileId(XorStr("v_hitmarkers"));
	OtherESP.PlaceLabledControl(0, XorStr("Hitmarkers"), this, &Hitmarkers);

	DamageIndicators.SetFileId(XorStr("v_damageindicators"));
	OtherESP.PlaceLabledControl(0, XorStr("Damage indicators"), this, &DamageIndicators);

	SpreadCrosshair.SetFileId(XorStr("v_spreadcrosshair"));
	OtherESP.PlaceLabledControl(0, XorStr("Spread crosshair"), this, &SpreadCrosshair);

	PenetrationReticle.SetFileId(XorStr("v_penetrationreticle"));
	OtherESP.PlaceLabledControl(0, XorStr("Penetration reticle"), this, &PenetrationReticle);

	AntiAimLines.SetFileId( "v_aalines" );
	OtherESP.PlaceLabledControl( 0, XorStr( "Anti-aim lines" ), this, &AntiAimLines );

	if (macsys.is_dev) {
		GhostChams.SetFileId(XorStr("v_ghostchams"));
		OtherESP.PlaceLabledControl(0, XorStr("Ghost chams"), this, &GhostChams);
	}

	// Effects
	Effects.SetText(XorStr("Effects"));
	Effects.SetSize(293, 208);
	Effects.SetPosition(333, 241);
	RegisterControl(&Effects);

	FovChanger.SetFileId(XorStr("v_fieldofview"));
	FovChanger.SetBoundaries(0, 90);
	FovChanger.SetValue(0);
	Effects.PlaceLabledControl(0, XorStr("Field of view"), this, &FovChanger);

	RemoveParticles.SetFileId(XorStr("v_removeparticles"));
	Effects.PlaceLabledControl(0, XorStr("Remove particles"), this, &RemoveParticles);

	RemoveSmoke.SetFileId(XorStr("v_removesmoke"));
	Effects.PlaceLabledControl(0, XorStr("Remove smoke"), this, &RemoveSmoke);

	NoVisualRecoil.SetFileId(XorStr("v_novisualrecoil"));
	Effects.PlaceLabledControl(0, XorStr("Remove visual recoil"), this, &NoVisualRecoil);

	RemoveWeaponScope.SetFileId(XorStr("v_removeweaponscope"));
	Effects.PlaceLabledControl(0, XorStr("Remove weapon scope"), this, &RemoveWeaponScope);

	Thirdperson.SetFileId(XorStr("v_thirdperson"));
	Effects.PlaceLabledControl(0, XorStr("Force third person"), this, &Thirdperson);

	ThirdpersonToggle.SetFileId(XorStr("v_thirdpersontoggle"));
	Effects.PlaceLabledControl(0, XorStr(""), this, &ThirdpersonToggle);

	VisualizedAngle.SetFileId(XorStr("v_visualizedangle"));
	VisualizedAngle.AddItem(XorStr("Real"));
	VisualizedAngle.AddItem(XorStr("Fake"));
	VisualizedAngle.AddItem(XorStr("Lowerbody"));
	Effects.PlaceLabledControl(0, XorStr(""), this, &VisualizedAngle);

	NightMode.SetFileId(XorStr("v_nightmode"));
	Effects.PlaceLabledControl(0, XorStr("Brightness adjustment"), this, &NightMode);

	DisablePostProcessing.SetFileId(XorStr("v_disablepostprocessing"));
	Effects.PlaceLabledControl(0, XorStr("Disable post processing"), this, &DisablePostProcessing);
}

void CMiscTab::Setup()
{
	SetTitle(XorStr("d"));

	//Miscellaneous
	Miscellaneous.SetText(XorStr("Miscellaneous"));
	Miscellaneous.SetSize(293, 540);
	Miscellaneous.SetPosition(24, 16);
	RegisterControl(&Miscellaneous);

	BunnyHop.SetFileId(XorStr("m_autohop"));
	Miscellaneous.PlaceLabledControl(0, XorStr("Bunny hop"), this, &BunnyHop);

	Autostrafe.SetFileId(XorStr("m_autostrafe"));
	Miscellaneous.PlaceLabledControl(0, XorStr("Air strafe"), this, &Autostrafe);

	AutoStrafeType.SetFileId(XorStr("m_autostrafetype"));
	AutoStrafeType.AddItem(XorStr("Normal"));
	AutoStrafeType.AddItem(XorStr("Silent"));
	Miscellaneous.PlaceLabledControl(0, XorStr(""), this, &AutoStrafeType);

	/*ZHop.SetFileId(XorStr("m_zhop"));
	MainGroup.PlaceLabledControl(1, XorStr("Z Strafe"), this, &ZHop);

	ZHopKey.SetFileId(XorStr("m_zhopkey"));
	MainGroup.PlaceLabledControl(1, XorStr(""), this, &ZHopKey);*/

	if ( macsys.is_administrator )
	{
		CircleStrafe.SetFileId( XorStr( "m_circlestrafe" ) );
		Miscellaneous.PlaceLabledControl( 0, XorStr( "Circle strafe" ), this, &CircleStrafe );

		FakeWalk.SetFileId( XorStr( "m_fakewalk" ) );
		Miscellaneous.PlaceLabledControl( 0, XorStr( "Fake walk" ), this, &FakeWalk );

		FakeWalkKey.SetFileId( XorStr( "m_fakewalkkey" ) );
		Miscellaneous.PlaceLabledControl( 0, XorStr( "" ), this, &FakeWalkKey );

		SpectatorList.SetFileId( XorStr( "m_spectatorlist" ) );
		Miscellaneous.PlaceLabledControl( 0, XorStr( "Spectator list" ), this, &SpectatorList );
	}

	Blockbot.SetFileId( "m_blockbot" );
	Miscellaneous.PlaceLabledControl( 0, "Blockbot", this, &Blockbot );

	BlockbotBind.SetFileId( "m_blockbotkey" );
	Miscellaneous.PlaceLabledControl( 0, "", this, &BlockbotBind );

	AutoAccept.SetFileId(XorStr("m_autoaccept"));
	Miscellaneous.PlaceLabledControl(0, XorStr("Auto-accept matchmaking"), this, &AutoAccept);

	ClanChanger.SetFileId(XorStr("m_clanchanger"));
	Miscellaneous.PlaceLabledControl(0, XorStr("Clan tag spammer"), this, &ClanChanger);

	NameChanger.SetFileId(XorStr("m_namechanger"));
	Miscellaneous.PlaceLabledControl(0, XorStr("Name spammer"), this, &NameChanger);

	NameChangerType.SetFileId(XorStr("m_namechangertype"));
	NameChangerType.AddItem(XorStr("Off"));
	NameChangerType.AddItem(XorStr("Ayyware crasher"));
	NameChangerType.AddItem(XorStr("Name stealer"));
	NameChangerType.AddItem(XorStr("Silent"));
	Miscellaneous.PlaceLabledControl(0, XorStr(""), this, &NameChangerType);

	//DeveloperConsole.SetFileId(XorStr("m_devconsole"));
	//MainGroup.PlaceLabledControl(0, XorStr("Developer Console"), this, &DeveloperConsole);

	//Settings
	SettingsGroup.SetText(XorStr("Settigns"));
	SettingsGroup.SetSize(293, 90);
	SettingsGroup.SetPosition(333, 16);
	RegisterControl(&SettingsGroup);

	MenuKeyLabel.SetText(XorStr("Menu key"));
	SettingsGroup.PlaceLabledControl(0, XorStr(""), this, &MenuKeyLabel);

	MenuKey.SetFileId(XorStr("m_menukey"));
	MenuKey.SetKey(VK_INSERT);
	SettingsGroup.PlaceLabledControl(0, XorStr(""), this, &MenuKey);

	MenuColor.SetFileId(XorStr("m_menucolor"));
	MenuColor.SetColor(24, 130, 230, 255);
	SettingsGroup.PlaceLabledControl(0, XorStr("Menu color"), this, &MenuColor);

	CheckboxUncheckedColor.SetFileId(XorStr("m_checkboxunchecked"));
	CheckboxUncheckedColor.SetColor(255, 255, 255, 255);
	SettingsGroup.PlaceLabledControl(0, XorStr("Checkbox unchecked"), this, &CheckboxUncheckedColor);

	AntiUntrusted.SetFileId(XorStr("m_antiuntrusted"));
	AntiUntrusted.SetState(true);
	SettingsGroup.PlaceLabledControl(0, XorStr("Anti-unstrusted"), this, &AntiUntrusted);

	//Flag
	FlagGroup.SetText(XorStr("Fake Lag"));
	FlagGroup.SetSize(293, 140);
	FlagGroup.SetPosition(333, 122);
	RegisterControl(&FlagGroup);

	FlagEnable.SetFileId(XorStr("m_flagenabled"));
	FlagGroup.PlaceLabledControl(0, XorStr("Fake lag"), this, &FlagEnable);

	FlagActivationType.SetFileId(XorStr("m_flagactivationtype"));
	FlagActivationType.AddItem(XorStr("Always"));
	FlagActivationType.AddItem(XorStr("While moving"));
	FlagActivationType.AddItem(XorStr("In air"));
	FlagGroup.PlaceLabledControl(0, XorStr(""), this, &FlagActivationType);

	FlagType.SetFileId(XorStr("m_flagtype"));
	FlagType.AddItem(XorStr("Off"));
	FlagType.AddItem(XorStr("Factor"));
	FlagType.AddItem(XorStr("Step up"));
	FlagType.AddItem(XorStr("Dynamic"));
	FlagGroup.PlaceLabledControl(0, XorStr(""), this, &FlagType);

	FlagLimit.SetFileId(XorStr("m_flaglimit"));
	FlagLimit.SetBoundaries(0.f, 14.f);
	FlagLimit.SetValue(0.f);
	FlagGroup.PlaceLabledControl(0, XorStr("Limit"), this, &FlagLimit);

	DisableFlagWhileShooting.SetFileId(XorStr("m_disableflagwhileshooting"));
	FlagGroup.PlaceLabledControl(0, XorStr("Disable while shooting"), this, &DisableFlagWhileShooting);

	//Presets
	ConfigGroup.SetText(XorStr("Presets"));
	ConfigGroup.SetSize(293, 278);
	ConfigGroup.SetPosition(333, 278);
	RegisterControl(&ConfigGroup);

	/*SelectedConfig.SetFileId(XorStr("m_selctedconfig"));
	SelectedConfig.AddItem(XorStr("Legit"));
	SelectedConfig.AddItem(XorStr("Dualies"));
	SelectedConfig.AddItem(XorStr("Scout"));
	SelectedConfig.AddItem(XorStr("AK47"));
	SelectedConfig.AddItem(XorStr("Auto"));
	SelectedConfig.AddItem(XorStr("Casual"));
	SelectedConfig.AddItem(XorStr("Rage"));
	SelectedConfig.AddItem(XorStr("Nospread"));
	ConfigGroup.PlaceLabledControl(0, XorStr(""), this, &SelectedConfig);*/

	ConfigListBox.SetHeightInItems(7);
	list_configs();
	ConfigGroup.PlaceLabledControl(0, XorStr(""), this, &ConfigListBox);

	LoadConfig.SetText(XorStr("Load"));
	LoadConfig.SetCallback(&load_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &LoadConfig);

	SaveConfig.SetText(XorStr("Save"));
	SaveConfig.SetCallback(&save_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &SaveConfig);

	RemoveConfig.SetText(XorStr("Remove"));
	RemoveConfig.SetCallback(&remove_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &RemoveConfig);

	ConfigGroup.PlaceLabledControl(0, "", this, &NewConfigName);

	AddConfig.SetText(XorStr("Add"));
	AddConfig.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &AddConfig);
}

void CPlayersTab::Setup()
{
	SetTitle(XorStr("f"));

	PlayerListControl.SetPosition(24, 16);
	PlayerListControl.SetSize(293, 540);
	PlayerListControl.SetHeightInItems(20);
	RegisterControl(&PlayerListControl);

	PlayerSettingsGroup.SetText(XorStr("Settings"));
	PlayerSettingsGroup.SetSize(293, 540);
	PlayerSettingsGroup.SetPosition(333, 16);
	RegisterControl(&PlayerSettingsGroup);

	PlayerForcePitch_Pitch.AddItem(XorStr("Eye angles"));
	PlayerForcePitch_Pitch.AddItem(XorStr("Up"));
	PlayerForcePitch_Pitch.AddItem(XorStr("Emotion"));

	PlayerForceYaw_Yaw.AddItem(XorStr("Default"));
	PlayerForceYaw_Yaw.AddItem(XorStr("Correction"));
	PlayerForceYaw_Yaw.AddItem(XorStr("Eye angles"));
	PlayerForceYaw_Yaw.AddItem(XorStr("Sideways left"));
	PlayerForceYaw_Yaw.AddItem(XorStr("Sideways right"));
	PlayerForceYaw_Yaw.AddItem(XorStr("180°"));

	PlayerSettingsGroup.PlaceLabledControl(0, XorStr("Priority"), this, &PlayerPriority);
	PlayerSettingsGroup.PlaceLabledControl(0, XorStr("Whitelist"), this, &PlayerFriendly);
	PlayerSettingsGroup.PlaceLabledControl(0, XorStr("Prioritize body"), this, &PlayerPreferBody);
	PlayerSettingsGroup.PlaceLabledControl(0, XorStr("Force pitch"), this, &PlayerForcePitch);
	PlayerSettingsGroup.PlaceLabledControl(0, XorStr(""), this, &PlayerForcePitch_Pitch);
	PlayerSettingsGroup.PlaceLabledControl(0, XorStr("Force yaw"), this, &PlayerForceYaw);
	PlayerSettingsGroup.PlaceLabledControl(0, XorStr(""), this, &PlayerForceYaw_Yaw);
}

void CSkinsTab::Setup()
{
	SetTitle(XorStr("e"));

	SkinChangerEnabled.SetFileId(XorStr("s_enabled"));
	SkinChangerEnabled.SetPosition(24, 16);
	SkinChangerEnabled.SetSize(10, 100);
	RegisterControl(&SkinChangerEnabled);

	EnabledLabel.SetText(XorStr("Enable"));
	EnabledLabel.SetPosition(40, 16);
	RegisterControl(&EnabledLabel);
	 
	OverrideModelGroup.SetText(XorStr("Knife Changer"));
	OverrideModelGroup.SetPosition(24, 40);
	OverrideModelGroup.SetSize(293, 325);
	RegisterControl(&OverrideModelGroup);

	OverrideKnife.SetFileId(XorStr("s_overrideknife"));
	OverrideModelGroup.PlaceLabledControl(0, "Override Knife", this, &OverrideKnife);

	KnifeSelection.SetFileId(XorStr("s_knife"));
	KnifeSelection.AddItem(XorStr("Default"));
	KnifeSelection.AddItem(XorStr("Bayonet"));
	KnifeSelection.AddItem(XorStr("M9 Bayonet"));
	KnifeSelection.AddItem(XorStr("Butterfly"));
	KnifeSelection.AddItem(XorStr("Flip"));
	KnifeSelection.AddItem(XorStr("Gut"));
	KnifeSelection.AddItem(XorStr("Karambit"));
	KnifeSelection.AddItem(XorStr("Huntsman"));
	KnifeSelection.AddItem(XorStr("Falchion"));
	KnifeSelection.AddItem(XorStr("Shadow Daggers"));
	KnifeSelection.AddItem(XorStr("Bowie"));
	OverrideModelGroup.PlaceLabledControl(0, XorStr(""), this, &KnifeSelection);

	KnifeSkin.SetFileId(XorStr("s_knifeskin"));
	KnifeSkin.AddItem(XorStr("Vanilla"), 0);
	KnifeSkin.AddItem(XorStr("Forest DDPAT (5)"), 5);
	KnifeSkin.AddItem(XorStr("Doppler | Phase 1 (418)"), 418);
	KnifeSkin.AddItem(XorStr("Doppler | Phase 2 (419)"), 419);
	KnifeSkin.AddItem(XorStr("Doppler | Phase 3 (420)"), 420);
	KnifeSkin.AddItem(XorStr("Doppler | Phase 4 (421)"), 421);
	KnifeSkin.AddItem(XorStr("Doppler | Ruby (415)"), 415);
	KnifeSkin.AddItem(XorStr("Doppler | Sapphire (416)"), 416);
	KnifeSkin.AddItem(XorStr("Doppler | Blackpearl (417)"), 417);
	KnifeSkin.AddItem(XorStr("Crimson Web (12)"), 12);
	KnifeSkin.AddItem(XorStr("Slaughter (59)"), 59);
	KnifeSkin.AddItem(XorStr("Fade (38)"), 38);
	KnifeSkin.AddItem(XorStr("Night (40)"), 40);
	KnifeSkin.AddItem(XorStr("Blue Steel (42)"), 42);
	KnifeSkin.AddItem(XorStr("Stained (43)"), 43);
	KnifeSkin.AddItem(XorStr("Case Hardened (44)"), 44);
	KnifeSkin.AddItem(XorStr("Safari Mesh (72)"), 72);
	KnifeSkin.AddItem(XorStr("Boreal Forest (77)"), 77);
	KnifeSkin.AddItem(XorStr("Ultraviolet (98)"), 98);
	KnifeSkin.AddItem(XorStr("Urban Masked (143)"), 143);
	KnifeSkin.AddItem(XorStr("Damascus Steel (410)"), 410);
	KnifeSkin.AddItem(XorStr("Scorched (175)"), 175);
	KnifeSkin.AddItem(XorStr("Tiger Tooth (409)"), 409);
	KnifeSkin.AddItem(XorStr("Marble Fade (413)"), 413);
	KnifeSkin.AddItem(XorStr("Rust Coat (323)"), 323);
	KnifeSkin.AddItem(XorStr("Bright Water (578)"), 578);
	KnifeSkin.AddItem(XorStr("Lore | Bayonet (558)"), 558);
	KnifeSkin.AddItem(XorStr("Lore | Flip (559)"), 559);
	KnifeSkin.AddItem(XorStr("Lore | Gut (560)"), 560);
	KnifeSkin.AddItem(XorStr("Lore | Karambit (561)"), 561);
	KnifeSkin.AddItem(XorStr("Lore | M9 (562)"), 562);
	KnifeSkin.AddItem(XorStr("Autotronic | Bayonet (573)"), 573);
	KnifeSkin.AddItem(XorStr("Autotronic | Flip (574)"), 574);
	KnifeSkin.AddItem(XorStr("Autotronic | Gut (575)"), 575);
	KnifeSkin.AddItem(XorStr("Autotronic | Karambit (576)"), 576);
	KnifeSkin.AddItem(XorStr("Autotronic | M9 (577)"), 577);
	KnifeSkin.AddItem(XorStr("Gamme Doppler | Phase 1 (569)"), 569);
	KnifeSkin.AddItem(XorStr("Gamme Doppler | Phase 2 (570)"), 570);
	KnifeSkin.AddItem(XorStr("Gamme Doppler | Phase 3 (571)"), 571);
	KnifeSkin.AddItem(XorStr("Gamme Doppler | Phase 4 (572)"), 572);
	KnifeSkin.AddItem(XorStr("Gamme Doppler | Emerald (568)"), 568);
	KnifeSkin.AddItem(XorStr("Black Laminate | Bayonet (563)"), 563);
	KnifeSkin.AddItem(XorStr("Black Laminate | Flip (564)"), 564);
	KnifeSkin.AddItem(XorStr("Black Laminate | Gut (565)"), 565);
	KnifeSkin.AddItem(XorStr("Black Laminate | Karambit (566)"), 566);
	KnifeSkin.AddItem(XorStr("Black Laminate | M9 (567)"), 567);
	KnifeSkin.AddItem(XorStr("Freehand | 1 (581)"), 581);
	KnifeSkin.AddItem(XorStr("Freehand | 2 (582)"), 582);
	KnifeSkin.AddItem(XorStr("Filler"), 99999);
	KnifeSkin.SetHeightInItems(10);
	OverrideModelGroup.PlaceLabledControl(0, XorStr(""), this, &KnifeSkin);

	KnifeWear.SetFileId(XorStr("s_knifewear"));
	KnifeWear.SetBoundaries(0, 1);
	KnifeWear.SetValue(0.01);
	KnifeWear.SetFormat(SliderFormat::FORMAT_DECDIG2);
	OverrideModelGroup.PlaceLabledControl(0, XorStr("Fallback wear"), this, &KnifeWear);

	KnifeEntityQuality.SetFileId(XorStr("s_knifequality"));
	KnifeEntityQuality.AddItem(XorStr("Normal"));
	KnifeEntityQuality.AddItem(XorStr("Genuine"));
	KnifeEntityQuality.AddItem(XorStr("Vintage"));
	KnifeEntityQuality.AddItem(XorStr("Unusual"));
	KnifeEntityQuality.AddItem(XorStr("Community"));
	KnifeEntityQuality.AddItem(XorStr("Developer"));
	KnifeEntityQuality.AddItem(XorStr("Self-Made"));
	KnifeEntityQuality.AddItem(XorStr("Customized"));
	KnifeEntityQuality.AddItem(XorStr("Strange"));
	KnifeEntityQuality.AddItem(XorStr("Completed"));
	KnifeEntityQuality.AddItem(XorStr("Tournament"));
	OverrideModelGroup.PlaceLabledControl(0, XorStr("Entity quality"), this, &KnifeEntityQuality);

	KnifeFullUpdate.SetText(XorStr("Update"));
	KnifeFullUpdate.SetCallback(&KnifeApplyCallbk);
	OverrideModelGroup.PlaceLabledControl(0, XorStr(""), this, &KnifeFullUpdate);

}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoFrame()
{	
	GUI.Update();
	GUI.Draw();

	plist.paint();
}