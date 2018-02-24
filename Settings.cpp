#include "Settings.h"
#include "Menu.h"

RagebotConfig ragebotconfig;
HitscanConfig hitscanconfig;
AAConfig antiaimconfig;
ResolverConfig resolverconfig;
LegitConfig legitconfig;
VisualsConfig visualconfig;
MiscConfig miscconfig;
SkinChangerConfig skinconfig;
Settings settings;

void Settings::Update( )
{
	//Ragebot
	ragebotconfig.bEnabled = Menu::Window.RageBotTab.Enabled.GetState( );
	ragebotconfig.iTargetSelection = Menu::Window.RageBotTab.TargetSelection.GetIndex( );
	ragebotconfig.bFriendlyFire = Menu::Window.RageBotTab.FriendlyFire.GetState( );
	ragebotconfig.flPointscale = Menu::Window.RageBotTab.Pointscale.GetValue( );
	ragebotconfig.bAutoFire = Menu::Window.RageBotTab.AutoFire.GetState( );
	ragebotconfig.iAutoFireMode = Menu::Window.RageBotTab.AutoFireMode.GetIndex( );
	ragebotconfig.iAutoFireRevolverMode = Menu::Window.RageBotTab.AutoFireRevolverMode.GetIndex( );
	ragebotconfig.iAutoFireHitbox = Menu::Window.RageBotTab.AutoFireTarget.GetIndex( );
	ragebotconfig.bPreferBaim = Menu::Window.RageBotTab.PreferBaim.GetState( );
	ragebotconfig.iAutoFireKey = Menu::Window.RageBotTab.AutoFireKey.GetKey( );
	ragebotconfig.bAutoWall = Menu::Window.RageBotTab.AutoWall.GetState( );
	ragebotconfig.iAimbotMode = Menu::Window.RageBotTab.AimbotMode.GetIndex( );
	ragebotconfig.bHitchance = Menu::Window.RageBotTab.MinimumHitChance.GetState( );
	ragebotconfig.flHitchanceAmt = Menu::Window.RageBotTab.MinimumHitChanceAmount.GetValue( );
	ragebotconfig.flMaxFov = Menu::Window.RageBotTab.MaximumFov.GetValue( );
	ragebotconfig.flMinDmg = Menu::Window.RageBotTab.MinimumDamage.GetValue( );
	ragebotconfig.bAutoScope = Menu::Window.RageBotTab.AutomaticScope.GetState( );
	ragebotconfig.bAimstep = Menu::Window.RageBotTab.AimStep.GetState( );
	ragebotconfig.flBaimOnX = Menu::Window.RageBotTab.BaimOnXHealth.GetValue( );
	ragebotconfig.bBaimIfDeadly = Menu::Window.RageBotTab.BaimIfDeadly.GetState( );
	ragebotconfig.bAutostop = Menu::Window.RageBotTab.Autostop.GetState( );
	ragebotconfig.iAutostopType = Menu::Window.RageBotTab.AutostopType.GetIndex( );
	ragebotconfig.bAccuracyWhileStanding = Menu::Window.RageBotTab.AccuracyNotCrouching.GetState( );
	ragebotconfig.bBaimWithAwp = Menu::Window.RageBotTab.BodyAimAwp.GetState( );
	ragebotconfig.iBaimAwpMode = Menu::Window.RageBotTab.BodyAimAwpMode.GetIndex( );
	ragebotconfig.bBaimWithScout = Menu::Window.RageBotTab.BodyAimScout.GetState( );
	ragebotconfig.iBaimScoutMode = Menu::Window.RageBotTab.BodyAimScoutMode.GetIndex( );

	ragebotconfig.bRemoveRecoil = Menu::Window.RageBotTab.RemoveRecoil.GetState( );

	//Hitscan
	hitscanconfig.hitboxes = Menu::Window.RageBotTab.HitscanBones.items;

	//Anti Aim
	antiaimconfig.stagnant.iRealPitch = Menu::Window.RageBotTab.stagnant_aa.RealPitch.GetIndex( );
	antiaimconfig.stagnant.iRealYaw = Menu::Window.RageBotTab.stagnant_aa.RealYaw.GetIndex( );
	antiaimconfig.stagnant.iFakeYaw = Menu::Window.RageBotTab.stagnant_aa.FakeYaw.GetIndex( );
	antiaimconfig.stagnant.flRealYawOffset = Menu::Window.RageBotTab.stagnant_aa.RealYawOffset.GetValue( );
	antiaimconfig.stagnant.flFakeYawOffset = Menu::Window.RageBotTab.stagnant_aa.FakeYawOffset.GetValue( );
	antiaimconfig.stagnant.bCleanUp = Menu::Window.RageBotTab.stagnant_aa.CleanUp.GetState( );
	antiaimconfig.moving.iRealPitch = Menu::Window.RageBotTab.moving_aa.RealPitch.GetIndex( );
	antiaimconfig.moving.iRealYaw = Menu::Window.RageBotTab.moving_aa.RealYaw.GetIndex( );
	antiaimconfig.moving.iFakeYaw = Menu::Window.RageBotTab.moving_aa.FakeYaw.GetIndex( );
	antiaimconfig.moving.flRealYawOffset = Menu::Window.RageBotTab.moving_aa.RealYawOffset.GetValue( );
	antiaimconfig.moving.flFakeYawOffset = Menu::Window.RageBotTab.moving_aa.FakeYawOffset.GetValue( );
	antiaimconfig.moving.bCleanUp = Menu::Window.RageBotTab.moving_aa.CleanUp.GetState( );
	antiaimconfig.edge.iWallDtc = Menu::Window.RageBotTab.edge_aa.Type.GetIndex( );
	antiaimconfig.edge.iRealPitch = Menu::Window.RageBotTab.edge_aa.RealPitch.GetIndex( );
	antiaimconfig.edge.iRealYaw = Menu::Window.RageBotTab.edge_aa.RealYaw.GetIndex( );
	antiaimconfig.edge.iFakeYaw = Menu::Window.RageBotTab.edge_aa.FakeYaw.GetIndex( );
	antiaimconfig.edge.flRealYawOffset = Menu::Window.RageBotTab.edge_aa.RealYawOffset.GetValue( );
	antiaimconfig.edge.flFakeYawOffset = Menu::Window.RageBotTab.edge_aa.FakeYawOffset.GetValue( );
	antiaimconfig.bDormantCheck = Menu::Window.RageBotTab.DormantCheck.GetState( );
	antiaimconfig.iRealYawDirection = Menu::Window.RageBotTab.RealYawDirection.GetIndex( );
	antiaimconfig.flRotateSpeed = Menu::Window.RageBotTab.RotateSpeed.GetValue( );
	antiaimconfig.flJitterRange = Menu::Window.RageBotTab.JitterRange.GetValue( );
	antiaimconfig.bLbyBreaker = Menu::Window.RageBotTab.Lbybreaker.GetState( );
	antiaimconfig.flLbyDelta = Menu::Window.RageBotTab.LbyDelta.GetValue( );

	//Resolver
	resolverconfig.bAntiAimCorrection = Menu::Window.RageBotTab.AntiAimCorrection.GetState( );
	resolverconfig.bResolverOverride = Menu::Window.RageBotTab.ResolverOverride.GetState( );
	resolverconfig.iResolverOverrideKey = Menu::Window.RageBotTab.ResolverOverrideKey.GetKey( );
	resolverconfig.bResolverDebug = Menu::Window.RageBotTab.ResolverDebug.GetState( );
	resolverconfig.bDisableInterp = Menu::Window.RageBotTab.DisableInterpolation.GetState( );
	resolverconfig.bBacktracking = Menu::Window.RageBotTab.Backtracking.GetState( );
	resolverconfig.bLinearExtraps = Menu::Window.RageBotTab.LinearExtrapolations.GetState( );
	resolverconfig.bAngleLodge = Menu::Window.RageBotTab.AngleLodge.GetState( );
	resolverconfig.flAngleLodgeMaxTicks = Menu::Window.RageBotTab.AngleLodgeSpeed.GetValue( );
	resolverconfig.bLowerbodyIndicator = Menu::Window.RageBotTab.LowerbodyIndicator.GetState( );

	//Legit
	legitconfig.pistol.bActivated = Menu::Window.LegitTab.pistol.Enabled.GetState( );
	legitconfig.pistol.iAimbotMode = Menu::Window.LegitTab.pistol.AimbotMode.GetIndex( );
	legitconfig.pistol.bAutoFire = Menu::Window.LegitTab.pistol.AutoFire.GetState( );
	legitconfig.pistol.iAutoFireMode = Menu::Window.LegitTab.pistol.AutoFireMode.GetIndex( );
	legitconfig.pistol.iAutoFireTarget = Menu::Window.LegitTab.pistol.AutoFireTarget.GetIndex( );
	legitconfig.pistol.iAutoFireKey = Menu::Window.LegitTab.pistol.AutoFireKey.GetKey( );
	legitconfig.pistol.flFOV = Menu::Window.LegitTab.pistol.FieldOfView.GetValue( );
	legitconfig.pistol.iReactionTime = Menu::Window.LegitTab.pistol.ReactionTime.GetIndex( );
	legitconfig.pistol.flRecoil = Menu::Window.LegitTab.pistol.Recoil.GetValue( );
	legitconfig.pistol.flSmoothing = Menu::Window.LegitTab.pistol.Smoothing.GetValue( );

	legitconfig.smg.bActivated = Menu::Window.LegitTab.smg.Enabled.GetState( );
	legitconfig.smg.iAimbotMode = Menu::Window.LegitTab.smg.AimbotMode.GetIndex( );
	legitconfig.smg.bAutoFire = Menu::Window.LegitTab.smg.AutoFire.GetState( );
	legitconfig.smg.iAutoFireMode = Menu::Window.LegitTab.smg.AutoFireMode.GetIndex( );
	legitconfig.smg.iAutoFireTarget = Menu::Window.LegitTab.smg.AutoFireTarget.GetIndex( );
	legitconfig.smg.iAutoFireKey = Menu::Window.LegitTab.smg.AutoFireKey.GetKey( );
	legitconfig.smg.flFOV = Menu::Window.LegitTab.smg.FieldOfView.GetValue( );
	legitconfig.smg.iReactionTime = Menu::Window.LegitTab.smg.ReactionTime.GetIndex( );
	legitconfig.smg.flRecoil = Menu::Window.LegitTab.smg.Recoil.GetValue( );
	legitconfig.smg.flSmoothing = Menu::Window.LegitTab.smg.Smoothing.GetValue( );

	legitconfig.rifle.bActivated = Menu::Window.LegitTab.rifle.Enabled.GetState( );
	legitconfig.rifle.iAimbotMode = Menu::Window.LegitTab.rifle.AimbotMode.GetIndex( );
	legitconfig.rifle.bAutoFire = Menu::Window.LegitTab.rifle.AutoFire.GetState( );
	legitconfig.rifle.iAutoFireMode = Menu::Window.LegitTab.rifle.AutoFireMode.GetIndex( );
	legitconfig.rifle.iAutoFireTarget = Menu::Window.LegitTab.rifle.AutoFireTarget.GetIndex( );
	legitconfig.rifle.iAutoFireKey = Menu::Window.LegitTab.rifle.AutoFireKey.GetKey( );
	legitconfig.rifle.flFOV = Menu::Window.LegitTab.rifle.FieldOfView.GetValue( );
	legitconfig.rifle.iReactionTime = Menu::Window.LegitTab.rifle.ReactionTime.GetIndex( );
	legitconfig.rifle.flRecoil = Menu::Window.LegitTab.rifle.Recoil.GetValue( );
	legitconfig.rifle.flSmoothing = Menu::Window.LegitTab.rifle.Smoothing.GetValue( );

	legitconfig.shotgun.bActivated = Menu::Window.LegitTab.shotgun.Enabled.GetState( );
	legitconfig.shotgun.iAimbotMode = Menu::Window.LegitTab.shotgun.AimbotMode.GetIndex( );
	legitconfig.shotgun.bAutoFire = Menu::Window.LegitTab.shotgun.AutoFire.GetState( );
	legitconfig.shotgun.iAutoFireMode = Menu::Window.LegitTab.shotgun.AutoFireMode.GetIndex( );
	legitconfig.shotgun.iAutoFireTarget = Menu::Window.LegitTab.shotgun.AutoFireTarget.GetIndex( );
	legitconfig.shotgun.iAutoFireKey = Menu::Window.LegitTab.shotgun.AutoFireKey.GetKey( );
	legitconfig.shotgun.flFOV = Menu::Window.LegitTab.shotgun.FieldOfView.GetValue( );
	legitconfig.shotgun.iReactionTime = Menu::Window.LegitTab.shotgun.ReactionTime.GetIndex( );
	legitconfig.shotgun.flRecoil = Menu::Window.LegitTab.shotgun.Recoil.GetValue( );
	legitconfig.shotgun.flSmoothing = Menu::Window.LegitTab.shotgun.Smoothing.GetValue( );

	legitconfig.sniper.bActivated = Menu::Window.LegitTab.sniper.Enabled.GetState( );
	legitconfig.sniper.iAimbotMode = Menu::Window.LegitTab.sniper.AimbotMode.GetIndex( );
	legitconfig.sniper.bAutoFire = Menu::Window.LegitTab.sniper.AutoFire.GetState( );
	legitconfig.sniper.iAutoFireMode = Menu::Window.LegitTab.sniper.AutoFireMode.GetIndex( );
	legitconfig.sniper.iAutoFireTarget = Menu::Window.LegitTab.sniper.AutoFireTarget.GetIndex( );
	legitconfig.sniper.iAutoFireKey = Menu::Window.LegitTab.sniper.AutoFireKey.GetKey( );
	legitconfig.sniper.flFOV = Menu::Window.LegitTab.sniper.FieldOfView.GetValue( );
	legitconfig.sniper.iReactionTime = Menu::Window.LegitTab.sniper.ReactionTime.GetIndex( );
	legitconfig.sniper.flRecoil = Menu::Window.LegitTab.sniper.Recoil.GetValue( );
	legitconfig.sniper.flSmoothing = Menu::Window.LegitTab.sniper.Smoothing.GetValue( );

	//Visuals
	visualconfig.player.iActivationType = Menu::Window.VisualsTab.player.ActivationType.GetIndex( );
	visualconfig.player.bTeammates = Menu::Window.VisualsTab.player.EspTeammates.GetState( );
	visualconfig.player.bBox = Menu::Window.VisualsTab.player.EspBox.GetState( );
	visualconfig.player.iBoxType = Menu::Window.VisualsTab.player.BoxType.GetIndex( );
	visualconfig.player.iEspMode = Menu::Window.VisualsTab.player.EspMode.GetIndex( );
	visualconfig.player.bBoxOutlines = Menu::Window.VisualsTab.player.EspOutlines.GetState( );
	visualconfig.player.bBoxFill = Menu::Window.VisualsTab.player.BoxFill.GetState( );
	visualconfig.player.flBoxFillOpacity = Menu::Window.VisualsTab.player.BoxFillOpacity.GetValue( );
	visualconfig.player.bDormant = Menu::Window.VisualsTab.player.Dormant.GetState( );
	visualconfig.player.bHealth = Menu::Window.VisualsTab.player.ShowHealth.GetState( );
	visualconfig.player.bHealthText = Menu::Window.VisualsTab.player.ShowHealthText.GetState( );
	visualconfig.player.bArmor = Menu::Window.VisualsTab.player.ShowArmor.GetState( );
	visualconfig.player.bSkeletons = Menu::Window.VisualsTab.player.ShowSkeletons.GetState( );
	visualconfig.player.bHitbones = Menu::Window.VisualsTab.player.ShowHitbones.GetState( );
	visualconfig.player.bPlayerNames = Menu::Window.VisualsTab.player.ShowPlayerNames.GetState( );
	visualconfig.player.bWeaponNames = Menu::Window.VisualsTab.player.ShowWeaponNames.GetState( );
	visualconfig.player.bPlayerAmmo = Menu::Window.VisualsTab.player.PlayerAmmo.GetState( );
	visualconfig.player.bSnapLines = Menu::Window.VisualsTab.player.SnapLines.GetState( );
	visualconfig.player.bGlow = Menu::Window.VisualsTab.player.Glow.GetState( );
	visualconfig.player.flGlowOpacity = Menu::Window.VisualsTab.player.GlowOpacity.GetValue( );
	visualconfig.player.bHitAngle = Menu::Window.VisualsTab.player.HitAngle.GetState( );
	visualconfig.player.bScoped = Menu::Window.VisualsTab.player.Scoped.GetState( );
	visualconfig.player.bDirectionArrow = Menu::Window.VisualsTab.player.DirectionArrow.GetState( );

	visualconfig.player.cBoxColor = Menu::Window.VisualsTab.player.BoxColor.GetValue( );
	visualconfig.player.cArmorColor = Menu::Window.VisualsTab.player.ArmorColor.GetValue( );
	visualconfig.player.cSkeletonsColor = Menu::Window.VisualsTab.player.SkeletonColor.GetValue( );
	visualconfig.player.cSnaplinesColor = Menu::Window.VisualsTab.player.SnapLinesColor.GetValue( );
	visualconfig.player.cGlowColor = Menu::Window.VisualsTab.player.GlowColor.GetValue( );
	visualconfig.player.cHitbonesColor = Menu::Window.VisualsTab.player.HitbonesColor.GetValue( );
	visualconfig.player.cDirectionArrowColor = Menu::Window.VisualsTab.player.DirectionArrowColor.GetValue( );

	visualconfig.bWeapons = Menu::Window.VisualsTab.Weapons.GetState( );
	visualconfig.cWeaponsColor = Menu::Window.VisualsTab.WeaponsColor.GetValue( );
	visualconfig.bDroppedWeapons = Menu::Window.VisualsTab.DroppedWeapons.GetState( );
	visualconfig.bDroppedC4 = Menu::Window.VisualsTab.DroppedC4.GetState( );
	visualconfig.bDroppedDefuseKit = Menu::Window.VisualsTab.DroppedDefuseKit.GetState( );
	visualconfig.bBulletImpacts = Menu::Window.VisualsTab.BulletImpacts.GetState( );
	visualconfig.cBuletImpactsColor = Menu::Window.VisualsTab.BulletImpactsColor.GetValue( );

	visualconfig.bRemoveParticles = Menu::Window.VisualsTab.RemoveParticles.GetState( );
	visualconfig.bRemoveSmoke = Menu::Window.VisualsTab.RemoveSmoke.GetState( );
	visualconfig.bNoVisualRecoil = Menu::Window.VisualsTab.NoVisualRecoil.GetState( );
	visualconfig.bRemoveWeaponScope = Menu::Window.VisualsTab.RemoveWeaponScope.GetState( );
	visualconfig.flFieldOfView = Menu::Window.VisualsTab.FovChanger.GetValue( );
	visualconfig.bThirdperson = Menu::Window.VisualsTab.Thirdperson.GetState( );
	visualconfig.iThirdpersonToggle = Menu::Window.VisualsTab.ThirdpersonToggle.GetKey( );
	visualconfig.iVisualizedAngle = Menu::Window.VisualsTab.VisualizedAngle.GetIndex( );
	visualconfig.bNightMode = Menu::Window.VisualsTab.NightMode.GetState( );
	visualconfig.bDisablePostProcessing = Menu::Window.VisualsTab.DisablePostProcessing.GetState( );
	visualconfig.bSpreadCrosshair = Menu::Window.VisualsTab.SpreadCrosshair.GetState( );
	visualconfig.bPenetrationReticle = Menu::Window.VisualsTab.PenetrationReticle.GetState( );
	visualconfig.bHitmarkers = Menu::Window.VisualsTab.Hitmarkers.GetState( );
	visualconfig.bDamageIndicators = Menu::Window.VisualsTab.DamageIndicators.GetState( );
	visualconfig.bGhostChams = Menu::Window.VisualsTab.GhostChams.GetState( );
	visualconfig.bAntiaimLines = Menu::Window.VisualsTab.AntiAimLines.GetState( );

	visualconfig.colored_models.iPlayerChamType = Menu::Window.VisualsTab.PlayerChamType.GetIndex( );
	visualconfig.colored_models.bEnemies = Menu::Window.VisualsTab.ChamsEnemies.GetState( );
	visualconfig.colored_models.bEnemiesNoVis = Menu::Window.VisualsTab.ChamsEnemiesBehindWall.GetState( );
	visualconfig.colored_models.bTeammates = Menu::Window.VisualsTab.ChamsTeammates.GetState( );
	visualconfig.colored_models.bTeammatesNoVis = Menu::Window.VisualsTab.ChamsTeammatesBehindWall.GetState( );

	visualconfig.colored_models.cTeammatesVisible = Menu::Window.VisualsTab.ChamsTeammates_Color.GetValue( );
	visualconfig.colored_models.cTeammatesNonVisible = Menu::Window.VisualsTab.ChamsTeammatesBehindWall_Color.GetValue( );
	visualconfig.colored_models.cEnemiesVisible = Menu::Window.VisualsTab.ChamsEnemies_Color.GetValue( );
	visualconfig.colored_models.cEnemiesNonVisible = Menu::Window.VisualsTab.ChamsEnemiesBehindWall_Color.GetValue( );

	//Misc
	miscconfig.bAutoJump = Menu::Window.MiscTab.BunnyHop.GetState( );
	miscconfig.bAutoStrafe = Menu::Window.MiscTab.Autostrafe.GetState( );
	miscconfig.iAutoStrafeType = Menu::Window.MiscTab.AutoStrafeType.GetIndex( );
	miscconfig.bZStrafe = Menu::Window.MiscTab.ZHop.GetState( );
	miscconfig.iZStrafeKey = Menu::Window.MiscTab.ZHopKey.GetKey( );
	miscconfig.bCircleStrafer = Menu::Window.MiscTab.CircleStrafe.GetState( );
	miscconfig.bFakeWalk = Menu::Window.MiscTab.FakeWalk.GetState( );
	miscconfig.iFakeWalkKey = Menu::Window.MiscTab.FakeWalkKey.GetKey( );
	miscconfig.bSpectatorList = Menu::Window.MiscTab.SpectatorList.GetState( );
	miscconfig.bBlockbot = Menu::Window.MiscTab.Blockbot.GetState( );
	miscconfig.iBlockbotKey = Menu::Window.MiscTab.BlockbotBind.GetKey( );
	miscconfig.bClanChanger = Menu::Window.MiscTab.ClanChanger.GetState( );
	miscconfig.bNameChanger = Menu::Window.MiscTab.NameChanger.GetState( );
	miscconfig.iNameChangerType = Menu::Window.MiscTab.NameChangerType.GetIndex( );
	miscconfig.bAutoAccept = Menu::Window.MiscTab.AutoAccept.GetState( );

	miscconfig.iMenuKey = Menu::Window.MiscTab.MenuKey.GetKey( );
	miscconfig.cMenuForecolor = Menu::Window.MiscTab.MenuColor.GetValue( );
	miscconfig.cCheckboxUnchecked = Menu::Window.MiscTab.CheckboxUncheckedColor.GetValue( );
	miscconfig.bAntiUntrusted = Menu::Window.MiscTab.AntiUntrusted.GetState( );

	miscconfig.bFlagEnabled = Menu::Window.MiscTab.FlagEnable.GetState( );
	miscconfig.iFlagActivationType = Menu::Window.MiscTab.FlagActivationType.GetIndex( );
	miscconfig.iFlagType = Menu::Window.MiscTab.FlagType.GetIndex( );
	miscconfig.flFlagLimit = Menu::Window.MiscTab.FlagLimit.GetValue( );
	miscconfig.bFlagDisableWhileShooting = Menu::Window.MiscTab.DisableFlagWhileShooting.GetState( );

	//Skins
	skinconfig.bEnabled = Menu::Window.SkinsTab.SkinChangerEnabled.GetState( );
	skinconfig.bOverrideKnife = Menu::Window.SkinsTab.OverrideKnife.GetState( );
	skinconfig.iKnife = Menu::Window.SkinsTab.KnifeSelection.GetIndex( );
	skinconfig.iKnifePaintkit = Menu::Window.SkinsTab.KnifeSkin.GetValue( );
	skinconfig.flKnifeWear = Menu::Window.SkinsTab.KnifeWear.GetValue( );
	skinconfig.iKnifeQuality = Menu::Window.SkinsTab.KnifeEntityQuality.GetIndex( );
}
