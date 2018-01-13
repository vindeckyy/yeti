#pragma once

#include "Hooks.h"
#include "Controls.h"

struct RagebotConfig
{
	bool bEnabled;
	int iTargetSelection;
	bool bFriendlyFire;
	float flPointscale;
	bool bAutoFire;
	int iAutoFireMode;
	bool iAutoFireRevolverMode;
	int iAutoFireHitbox;
	bool bPreferBaim;
	int iAutoFireKey;
	bool bAutoWall;
	int iAimbotMode;
	bool bHitchance;
	float flHitchanceAmt;
	float flMaxFov;
	float flMinDmg;
	bool bAutoScope;
	bool bAimstep;
	int flBaimOnX;
	bool bBaimIfDeadly;
	bool bAutostop;
	int iAutostopType;
	bool bAccuracyWhileStanding;
	bool bBaimWithAwp;
	int iBaimAwpMode;
	bool bBaimWithScout;
	int iBaimScoutMode;

	bool bRemoveRecoil;
};

struct HitscanConfig
{
	std::vector<MultiBoxItem> hitboxes;
};

struct AAConfig
{
	class StagnantAAConfig
	{
	public:
		int iRealPitch;
		int iRealYaw;
		int iFakeYaw;
		float flRealYawOffset;
		float flFakeYawOffset;
		bool bCleanUp;
	};
	StagnantAAConfig stagnant;

	class MovingAAConfig
	{
	public:
		int iRealPitch;
		int iRealYaw;
		int iFakeYaw;
		float flRealYawOffset;
		float flFakeYawOffset;
		bool bCleanUp;
	};
	MovingAAConfig moving;

	class EdgeAAConfig
	{
	public:
		int iWallDtc;
		int iRealPitch;
		int iRealYaw;
		int iFakeYaw;
		float flRealYawOffset;
		float flFakeYawOffset;
	};
	EdgeAAConfig edge;

	bool bDormantCheck;
	int iRealYawDirection;
	float flRotateSpeed;
	float flJitterRange;
	bool bLbyBreaker;
	float flLbyDelta;
};

struct ResolverConfig
{
	bool bAntiAimCorrection;
	bool bResolverOverride;
	int iResolverOverrideKey;
	bool bResolverDebug;
	bool bDisableInterp;
	bool bBacktracking;
	bool bLinearExtraps;
	bool bAngleLodge;
	float flAngleLodgeMaxTicks;
	bool bLowerbodyIndicator;
};

struct LegitConfig
{
	class Pistol
	{
	public:
		bool bActivated;
		int iAimbotMode;
		bool bAutoFire;
		int iAutoFireMode;
		int iAutoFireTarget;
		int iAutoFireKey;
		float flFOV;
		int iReactionTime;
		float flRecoil;
		float flSmoothing;
	};
	Pistol pistol;

	class SMG
	{
	public:
		bool bActivated;
		int iAimbotMode;
		bool bAutoFire;
		int iAutoFireMode;
		int iAutoFireTarget;
		int iAutoFireKey;
		float flFOV;
		int iReactionTime;
		float flRecoil;
		float flSmoothing;
	};
	SMG smg;

	class Rifle
	{
	public:
		bool bActivated;
		int iAimbotMode;
		bool bAutoFire;
		int iAutoFireMode;
		int iAutoFireTarget;
		int iAutoFireKey;
		float flFOV;
		int iReactionTime;
		float flRecoil;
		float flSmoothing;
	};
	Rifle rifle;

	class Shotgun
	{
	public:
		bool bActivated;
		int iAimbotMode;
		bool bAutoFire;
		int iAutoFireMode;
		int iAutoFireTarget;
		int iAutoFireKey;
		float flFOV;
		int iReactionTime;
		float flRecoil;
		float flSmoothing;
	};
	Shotgun shotgun;

	class Sniper
	{
	public:
		bool bActivated;
		int iAimbotMode;
		bool bAutoFire;
		int iAutoFireMode;
		int iAutoFireTarget;
		int iAutoFireKey;
		float flFOV;
		int iReactionTime;
		float flRecoil;
		float flSmoothing;
	};
	Sniper sniper;
};

struct VisualsConfig
{
	class PlayerEspConfig
	{
	public:
		int iActivationType;
		bool bTeammates;
		bool bBox;
		int iBoxType;
		int iEspMode;
		bool bBoxOutlines;
		bool bBoxFill;
		float flBoxFillOpacity;
		bool bDormant;
		bool bHealth;
		bool bHealthText;
		int iHealthLocation;
		bool bArmor;
		int iArmorLocation;
		bool bSkeletons;
		bool bHitbones;
		bool bPlayerNames;
		bool bWeaponNames;
		bool bDirectionArrow;
		bool bPlayerAmmo;
		bool bSnapLines;
		bool bGlow;
		float flGlowOpacity;
		bool bHitAngle;
		bool bScoped;

		int* cBoxColor;
		int* cArmorColor;
		int* cSkeletonsColor;
		int* cSnaplinesColor;
		int* cGlowColor;
		int* cHitbonesColor;
		int* cDirectionArrowColor;
	};
	
	PlayerEspConfig player;

	bool bWeapons;
	int* cWeaponsColor;
	bool bDroppedWeapons;
	bool bDroppedC4;
	bool bDroppedDefuseKit;
	bool bBulletImpacts;
	int* cBuletImpactsColor;
	
	bool bRemoveParticles;
	bool bRemoveSmoke;
	bool bNoVisualRecoil;
	bool bRemoveWeaponScope;
	float flFieldOfView;
	bool bThirdperson;
	int iThirdpersonToggle;
	int iVisualizedAngle;
	bool bNightMode;
	bool bDisablePostProcessing;
	bool bSpreadCrosshair;
	bool bPenetrationReticle;
	bool bHitmarkers;
	bool bDamageIndicators;
	bool bGhostChams;
	bool bAntiaimLines;

	class ColoredModelSettings 
	{ 
	public:
		int iPlayerChamType;
		bool bTeammates;
		int* cTeammatesVisible;
		bool bTeammatesNoVis;
		int* cTeammatesNonVisible;
		bool bEnemies;
		int* cEnemiesVisible;
		bool bEnemiesNoVis;
		int* cEnemiesNonVisible;
	};
	ColoredModelSettings colored_models;
};

struct MiscConfig
{
	bool bAutoJump;
	bool bAutoStrafe;
	int iAutoStrafeType;
	bool bZStrafe;
	int iZStrafeKey;
	bool bCircleStrafer;
	bool bFakeWalk;
	int iFakeWalkKey;
	bool bSpectatorList;
	bool bBlockbot;
	int iBlockbotKey;
	bool bClanChanger;
	bool bNameChanger;
	int iNameChangerType;
	bool bAutoAccept;

	int iMenuKey;
	int* cMenuForecolor;
	int* cCheckboxUnchecked;
	bool bAntiUntrusted;

	bool bFlagEnabled;
	int iFlagActivationType;
	int iFlagType;
	float flFlagDifference;
	float flFlagLimit;
	bool bFlagDisableWhileShooting;
};

struct SkinChangerConfig
{
	bool bEnabled;
	bool bOverrideKnife;
	int iKnife;
	int iKnifePaintkit;
	float flKnifeWear;
	int iKnifeQuality;
};

extern RagebotConfig ragebotconfig;
extern HitscanConfig hitscanconfig;
extern AAConfig antiaimconfig;
extern ResolverConfig resolverconfig;
extern LegitConfig legitconfig;
extern VisualsConfig visualconfig;
extern MiscConfig miscconfig;
extern SkinChangerConfig skinconfig;

struct Settings
{
	void Update();
};

extern Settings settings;