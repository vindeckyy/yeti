

#pragma once

#include "GUI.h"
#include "Controls.h"

class CRageBotTab : public CTab
{
public:
	void Setup();

	CGroupBox AimbotGroup;
	CCheckBox Enabled;
	CComboBox TargetSelection;
	CCheckBox FriendlyFire;
	CSlider Pointscale;
	CCheckBox AutoFire;
	CComboBox AutoFireMode;
	CComboBox AutoFireTarget;
	CComboBox AutoFireRevolverMode;
	CKeyBind AutoFireKey;
	CCheckBox AutoWall;
	CComboBox AimbotMode;
	CSlider MaximumFov;
	CSlider MinimumDamage;
	CCheckBox AimStep;
	CCheckBox PreferBaim;
	CSlider BaimOnXHealth;
	CCheckBox BaimIfDeadly;
	CCheckBox BodyAimAwp;
	CComboBox BodyAimAwpMode;
	CCheckBox BodyAimScout;
	CComboBox BodyAimScoutMode;
	CCheckBox MinimumHitChance;
	CSlider MinimumHitChanceAmount;
	CCheckBox AutomaticScope;
	CCheckBox Autostop;
	CComboBox AutostopType;
	CCheckBox AccuracyNotCrouching;
	CCheckBox RemoveRecoil;
	CMultiBox HitscanBones;

	CGroupBox DefaultAntiAimGroup;
	class StagnantAASettings
	{
	public:
		CComboBox RealPitch;
		CComboBox RealYaw;
		CSlider RealYawOffset;
		CComboBox FakeYaw;
		CSlider FakeYawOffset;
		CCheckBox CleanUp;
	};
	StagnantAASettings stagnant_aa;

	class MovingAASettings
	{
	public:
		CComboBox RealPitch;
		CComboBox RealYaw;
		CSlider RealYawOffset;
		CComboBox FakeYaw;
		CSlider FakeYawOffset;
		CCheckBox CleanUp;
	};
	MovingAASettings moving_aa;

	class EdgeAASettings
	{
	public:
		CComboBox Type;
		CComboBox RealPitch;
		CComboBox RealYaw;
		CSlider RealYawOffset;
		CComboBox FakeYaw;
		CSlider FakeYawOffset;
	};
	EdgeAASettings edge_aa;

	CCheckBox DormantCheck;
	CComboBox RealYawDirection;
	CSlider RotateSpeed;
	CSlider JitterRange;
	CCheckBox Lbybreaker;
	CSlider LbyDelta;

	CGroupBox ResolverOptions;
	CCheckBox AntiAimCorrection;
	CCheckBox ResolverOverride;
	CKeyBind ResolverOverrideKey;
	CCheckBox DisableInterpolation;
	CCheckBox Backtracking;
	CCheckBox LinearExtrapolations;
	CCheckBox AngleLodge;
	CSlider AngleLodgeSpeed;
	CCheckBox ResolverDebug;
	CCheckBox LowerbodyIndicator;
};

class CLegitTab : public CTab
{
public:
	void Setup();

	CGroupBox AimbotGroup;

	class CPistol
	{
	public:
		CCheckBox Enabled;
		CComboBox AimbotMode;
		CCheckBox AutoFire;
		CComboBox AutoFireMode;
		CComboBox AutoFireTarget;
		CKeyBind AutoFireKey;
		CSlider FieldOfView;
		CComboBox ReactionTime;
		CSlider Recoil;
		CSlider Smoothing;
	};
	CPistol pistol;

	class CSMG
	{
	public:
		CCheckBox Enabled;
		CComboBox AimbotMode;
		CCheckBox AutoFire;
		CComboBox AutoFireMode;
		CComboBox AutoFireTarget;
		CKeyBind AutoFireKey;
		CSlider FieldOfView;
		CComboBox ReactionTime;
		CSlider Recoil;
		CSlider Smoothing;
	};
	CSMG smg;

	class CRifles
	{
	public:
		CCheckBox Enabled;
		CComboBox AimbotMode;
		CCheckBox AutoFire;
		CComboBox AutoFireMode;
		CComboBox AutoFireTarget;
		CKeyBind AutoFireKey;
		CSlider FieldOfView;
		CComboBox ReactionTime;
		CSlider Recoil;
		CSlider Smoothing;
	};
	CRifles rifle;

	class CShotgun
	{
	public:
		CCheckBox Enabled;
		CComboBox AimbotMode;
		CCheckBox AutoFire;
		CComboBox AutoFireMode;
		CComboBox AutoFireTarget;
		CKeyBind AutoFireKey;
		CSlider FieldOfView;
		CComboBox ReactionTime;
		CSlider Recoil;
		CSlider Smoothing;
	};
	CShotgun shotgun;

	class CSnipers
	{
	public:
		CCheckBox Enabled;
		CComboBox AimbotMode;
		CCheckBox AutoFire;
		CComboBox AutoFireMode;
		CComboBox AutoFireTarget;
		CKeyBind AutoFireKey;
		CSlider FieldOfView;
		CComboBox ReactionTime;
		CSlider Recoil;
		CSlider Smoothing;
	};
	CSnipers sniper;

};

class CVisualTab : public CTab
{
public:
	void Setup();

	CGroupBox PlayerESP;

	class VisualPlayerControls
	{
	public:
		CComboBox ActivationType;
		CCheckBox EspTeammates;
		CCheckBox EspBox;
		CColorSelector BoxColor;
		CComboBox BoxType;
		CComboBox EspMode;
		CCheckBox EspOutlines;
		CCheckBox BoxFill;
		CSlider BoxFillOpacity;
		CCheckBox ShowHealth;
		CCheckBox ShowHealthText;
		CCheckBox ShowArmor;
		CColorSelector ArmorColor;
		CCheckBox ShowSkeletons;
		CColorSelector SkeletonColor;
		CCheckBox ShowHitbones;
		CColorSelector HitbonesColor;
		CCheckBox ShowPlayerNames;
		CCheckBox ShowWeaponNames;
		CCheckBox PlayerAmmo;
		CCheckBox SnapLines;
		CColorSelector SnapLinesColor;
		CCheckBox DirectionArrow;
		CColorSelector DirectionArrowColor;
		CCheckBox Glow;
		CColorSelector GlowColor;
		CSlider GlowOpacity;
		CCheckBox HitAngle;
		CCheckBox Scoped;
		CCheckBox Dormant;
	};
	VisualPlayerControls player;

	CGroupBox ChamsGroup;
	CComboBox PlayerChamType;
	CCheckBox ChamsEnemies;
	CColorSelector ChamsEnemies_Color;
	CCheckBox ChamsEnemiesBehindWall;
	CColorSelector ChamsEnemiesBehindWall_Color;
	CCheckBox ChamsTeammates;
	CColorSelector ChamsTeammates_Color;
	CCheckBox ChamsTeammatesBehindWall;
	CColorSelector ChamsTeammatesBehindWall_Color;

	CGroupBox Effects;
	CCheckBox RemoveParticles;
	CCheckBox RemoveSmoke;
	CCheckBox NoVisualRecoil;
	CCheckBox RemoveWeaponScope;
	CSlider FovChanger;
	CCheckBox Thirdperson;
	CKeyBind ThirdpersonToggle;
	CComboBox VisualizedAngle;
	CCheckBox NightMode;
	CCheckBox DisablePostProcessing;
	CCheckBox Hitmarkers;
	CCheckBox DamageIndicators;
	CCheckBox GhostChams;
	CCheckBox SpreadCrosshair;
	CCheckBox PenetrationReticle;
	CCheckBox AntiAimLines;

	CGroupBox OtherESP;

	CCheckBox Weapons;
	CColorSelector WeaponsColor;
	CCheckBox DroppedWeapons;
	CCheckBox DroppedC4;
	CCheckBox DroppedDefuseKit;
	CCheckBox BulletImpacts;
	CColorSelector BulletImpactsColor;
};

class CMiscTab : public CTab
{
public:
	void Setup();

	CGroupBox Miscellaneous;
	CCheckBox BunnyHop;
	CCheckBox Autostrafe;
	CComboBox AutoStrafeType;
	CCheckBox ZHop;
	CKeyBind ZHopKey;
	CCheckBox CircleStrafe;
	CCheckBox FakeWalk;
	CKeyBind FakeWalkKey;
	CCheckBox SpectatorList;
	CCheckBox Blockbot;
	CKeyBind BlockbotBind;
	CCheckBox AutoAccept;
	CCheckBox ClanChanger;
	CCheckBox NameChanger;
	CComboBox NameChangerType;
	CCheckBox DeveloperConsole;

	CGroupBox SettingsGroup;
	CLabel MenuKeyLabel;
	CKeyBind MenuKey;
	CColorSelector MenuColor;
	CColorSelector CheckboxUncheckedColor;
	CCheckBox AntiUntrusted;

	CCheckBox FlagEnable;
	CGroupBox FlagGroup;
	CComboBox FlagActivationType;
	CComboBox FlagType;
	CSlider FlagLimit;
	CCheckBox DisableFlagWhileShooting;

	CGroupBox ConfigGroup;
	//CComboBox SelectedConfig;
	CListBox ConfigListBox;
	CButton SaveConfig;
	CButton LoadConfig;
	CButton RemoveConfig;
	CTextField NewConfigName;
	CButton AddConfig;

	CCheckBox Playerlist;
	CLabel PlayerlistLabel;
};

class CPlayersTab : public CTab
{
public:
	void Setup();

	CListBox PlayerListControl;

	CGroupBox PlayerSettingsGroup;
	CCheckBox PlayerPriority;
	CCheckBox PlayerFriendly;
	CCheckBox PlayerPreferBody;
	CCheckBox PlayerForcePitch;
	CComboBox PlayerForcePitch_Pitch;
	CCheckBox PlayerForceYaw;
	CComboBox PlayerForceYaw_Yaw;
};

class CSkinsTab : public CTab
{
public:
	void Setup();

	CCheckBox SkinChangerEnabled;
	CLabel EnabledLabel;

	CGroupBox OverrideModelGroup;
	CCheckBox OverrideKnife;
	CComboBox KnifeSelection;
	CButton KnifeFullUpdate;
	CListBox KnifeSkin;
	CSlider KnifeWear;
	CTextField KnifeSeed;
	CComboBox KnifeEntityQuality;

	CGroupBox SkinsGroup;
};

class YetiWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitTab LegitTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CPlayersTab PlayersTab;
	CSkinsTab SkinsTab;
};

namespace Menu
{
	void SetupMenu();
	void DoFrame();

	extern YetiWindow Window;
};