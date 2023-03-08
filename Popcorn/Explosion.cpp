#include "Explosion.h"

// AExplosion
//------------------------------------------------------------------------------------------------------------
AExplosion::AExplosion()
	:Explosive_Balls(Explosive_Balls_Count)
{
}
//------------------------------------------------------------------------------------------------------------
void AExplosion::Start_Explosion(RECT& explosion_rect)
{
	bool is_red;
	int x_offset, y_offset;
	int size, half_size, remained_size;
	int time_offset;

	int scale = AsConfig::Global_Scale;

	int half_width = (explosion_rect.right - explosion_rect.left) / 2;
	int half_height = (explosion_rect.bottom - explosion_rect.top) / 2;
	int x_pos = explosion_rect.left + half_width;
	int y_pos = explosion_rect.top + half_height;

	half_size = half_width;

	if (half_height < half_size)
		half_size = half_height;

	for (auto& explosive_ball : Explosive_Balls)
	{
		x_offset = AsTools::Rand(half_width) - half_width / 2;
		y_offset = AsTools::Rand(half_height) - half_height / 2;

		remained_size = half_size - (int)sqrt(x_offset * x_offset + y_offset * y_offset);

		size = AsTools::Rand(remained_size / 2) + remained_size / 2;

		if (size < scale)
			size = scale;

		time_offset = AsTools::Rand(AsConfig::FPS * 3 / 2);

		is_red = (bool)AsTools::Rand(2);
		explosive_ball.Explode(x_pos + x_offset, y_pos + y_offset, size * 2, is_red, time_offset, 10);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AExplosion::Act_On_Explosion()
{
	bool destroyed = true;

	for (auto& explosive_ball : Explosive_Balls)
	{
		explosive_ball.Act();

		destroyed &= explosive_ball.Is_Finished();
	}

	return destroyed;
}
//------------------------------------------------------------------------------------------------------------
void AExplosion::Draw_Explosion(HDC hdc, RECT& paint_area)
{
	for (auto& explosive_ball : Explosive_Balls)
		explosive_ball.Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------




