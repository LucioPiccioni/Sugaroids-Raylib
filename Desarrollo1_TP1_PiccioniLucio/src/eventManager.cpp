#include "eventManager.h"

void EventManager::MusicControl(Menus gameState)
{
	if (gameState == "menu" && !mainMenuMusic:isPlaying())
	{
		StopAudioStream(gamePlayMusic);
		StopAudioStream(gameOverMusic);
			StopAudioStream(creditsMusic)
			mainMenuMusic : seek(0)
			love.audio.play(mainMenuMusic)

			elseif gameState == "credits" and not creditsMusic : isPlaying() then

			love.audio.stop(gamePlayMusic)
			love.audio.stop(gameOverMusic)
			love.audio.stop(mainMenuMusic)
			creditsMusic : seek(0)
			love.audio.play(creditsMusic)

			elseif gameOver and not gameOverMusic : isPlaying() and not dieSound : isPlaying() then

			love.audio.stop(creditsMusic)
			love.audio.stop(gamePlayMusic)
			love.audio.stop(mainMenuMusic)
			gameOverMusic : seek(0)
			love.audio.play(gameOverMusic)

			elseif not gameOver and gameState == "playing" and not gamePlayMusic : isPlaying() then
			love.audio.stop(creditsMusic)
			love.audio.stop(gameOverMusic)
			love.audio.stop(mainMenuMusic)
			gamePlayMusic : seek(0)
			love.audio.play(gamePlayMusic)

	}
}
