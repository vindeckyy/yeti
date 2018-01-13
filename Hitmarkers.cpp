#include "Hitmarkers.h"

#pragma comment(lib, "Winmm.lib")

CHitMarker hitmarkers;

void CHitMarkerEventManager::FireGameEvent(IGameEvent *event)
{
	const char* szEventName = event->GetName();


	if (strcmp(szEventName, "player_hurt") == 0)
	{
		if (m_pEngine->GetPlayerForUserID(event->GetInt("attacker", 0)) == m_pEngine->GetLocalPlayer())
			bDoHitmarker = true;
	}

	if (strcmp(szEventName, "round_start") == 0)
		bDoRoundStart = true;
}

void CHitMarkerEventManager::RegisterSelf()
{
	m_nDebugID = 42;
	m_pGameEventManager->AddListener(this, "player_hurt", false);
	m_pGameEventManager->AddListener(this, "round_start", false);
}

bool CHitMarkerEventManager::ShouldHitmarker()
{
	if (bDoHitmarker)
	{
		bDoHitmarker = false;
		return true;
	}
	else
		return false;
}

bool CHitMarkerEventManager::ShouldRoundStart()
{
	if (bDoRoundStart)
	{
		bDoRoundStart = false;
		return true;
	}
	else
		return false;
}

void CHitMarker::Draw()
{
	static bool doHitmarker = false;
	static clock_t start_t = clock();

	if (!m_pGameEventManager->FindListener(&eventmanger, "player_hurt"))
		eventmanger.RegisterSelf();

	if (eventmanger.ShouldHitmarker())
	{
		doHitmarker = true;
		start_t = clock();
		PlaySoundA(_soundFileName, NULL, SND_ASYNC);
	}

	if (doHitmarker)
	{
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < 0.5)
		{
			int width, height;
			m_pEngine->GetScreenSize(width, height);


			float ratio = timeSoFar;
			if (timeSoFar > 0.5f) timeSoFar = 0.5f;
			float alpha = 255.f * (1.f - (timeSoFar / 0.5f));

			int sides[4][2] = { { -1, -1 },{ 1, 1 },{ -1, 1 },{ 1, -1 } };
			for (auto& side : sides)
				draw.line(width / 2 + (5 * side[0]), height / 2 + (5 * side[1]), width / 2 + (8 * side[0]), height / 2 + (8 * side[1]), Color(255, 255, 255, alpha));
		}
		else
			doHitmarker = false;
	}
	else
		doHitmarker = false;
}