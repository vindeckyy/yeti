// Constants
const float RADIUS = 30.f;
const int X_CENTER = screen_size_x / 2;
const int Y_CENTER = screen_size_y / 2;

// Draws crosshair and penetration reticle
void DrawCrosshairAndPenetrationReticle() {
    // Get local player
    auto m_local = game::localdata.localplayer();

    // Check if local player is alive and has a weapon
    if (m_local && m_local->IsAlive()) {
        auto m_weapon = m_local->GetWeapon();
        if (m_weapon && !m_weapon->IsKnife()) {
            // Draw spread crosshair
            if (visualconfig.bSpreadCrosshair) {
                // Calculate accuracy of the weapon
                auto accuracy = m_weapon->GetCone() * 550.f;

                // Draw filled circle with the calculated accuracy
                draw.filled_circle(X_CENTER, Y_CENTER, RADIUS, accuracy, Color(27, 27, 27, 140));
            }

            // Draw penetration reticle
            if (visualconfig.bPenetrationReticle) {
                // Calculate damage and color based on whether the bullet can penetrate or not
                float damage;
                Color rect_color = trace_autowallable(damage) ? Color(130, 241, 13) : Color(255, 102, 102);

                // Draw outlined rectangle with the calculated color
                draw.outlined_rect(X_CENTER - 2, Y_CENTER - 2, 5, 5, rect_color);
            }
        }
    }
}
