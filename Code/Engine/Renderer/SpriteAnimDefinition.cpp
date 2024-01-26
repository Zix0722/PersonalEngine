#include "SpriteAnimDefinition.hpp"
#include "../Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"

SpriteAnimDefinition::SpriteAnimDefinition(SpriteSheet const& sheet, int startSpriteIndex, int endSpriteIndex, float framesPerSecond /*= 20.f*/, SpriteAnimPlaybackType playbackType /*= SpriteAnimPlaybackType::LOOP*/)
	:m_spriteSheet(sheet)
	,m_secondsPerFrame(1.f / framesPerSecond)
	,m_playbackType(playbackType)
	,m_startSpriteIndex(startSpriteIndex)
	,m_endSpriteIndex(endSpriteIndex)
{

}

SpriteDefinition const& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{
	switch (m_playbackType)
	{
	case SpriteAnimPlaybackType::ONCE:	     return GetSpriteDefAtTimeOnce(seconds);
	case SpriteAnimPlaybackType::LOOP:		 return GetSpriteDefAtTimeLoop(seconds);
	case SpriteAnimPlaybackType::PINGPONG:   return GetSpriteDefAtTimePingPong(seconds);
	default:
		ERROR_AND_DIE("Invalid PlaybackType!");
		break;
	}
}

bool SpriteAnimDefinition::LoadFromXmlElement(XmlElement const& element, XmlElement const& anim)
{
	std::string playbackMode;
	playbackMode = ParseXMLAttribute(element, "playbackMode", playbackMode);
	if (playbackMode == "Loop")
	{
		m_playbackType = SpriteAnimPlaybackType::LOOP;
	}
	else if(playbackMode == "Once")
	{
		m_playbackType = SpriteAnimPlaybackType::ONCE;
	}
	m_secondsPerFrame = ParseXMLAttribute(element, "secondsPerFrame", m_secondsPerFrame);
	
	m_startSpriteIndex = ParseXMLAttribute(anim, "startFrame", m_startSpriteIndex);
	m_endSpriteIndex   = ParseXMLAttribute(anim, "endFrame", m_endSpriteIndex);

	return true;
}

float SpriteAnimDefinition::GetSecondsPerFrame() const
{
	return m_secondsPerFrame;
}

int SpriteAnimDefinition::GetNumOfFrameHaveToPlay() const
{
	return (m_endSpriteIndex - m_startSpriteIndex) + 1;
}


SpriteDefinition const& SpriteAnimDefinition::GetSpriteDefAtTimeOnce(float seconds) const
{
	int numOfSpriteFrame = m_endSpriteIndex - m_startSpriteIndex;
	if (numOfSpriteFrame == 0)
	{
		return m_spriteSheet.GetSpriteDef(m_startSpriteIndex);
	}
	float totalDurationSec = numOfSpriteFrame * m_secondsPerFrame;
	float clampedSec = GetClamped(seconds, 0.f, totalDurationSec);
	float clampedFrameIndex = GetClamped(clampedSec / m_secondsPerFrame, 0.f, (float)numOfSpriteFrame);

	int index = RoundDownToInt(clampedFrameIndex);
	return m_spriteSheet.GetSpriteDef(index + m_startSpriteIndex);
}

SpriteDefinition const& SpriteAnimDefinition::GetSpriteDefAtTimeLoop(float seconds) const
{
	int numOfSpriteFrame = m_endSpriteIndex - m_startSpriteIndex;
	if (numOfSpriteFrame == 0)
	{
		return m_spriteSheet.GetSpriteDef(m_startSpriteIndex);
	}
	int numFramesPassed = RoundDownToInt(seconds / m_secondsPerFrame);
	int index = numFramesPassed % numOfSpriteFrame;
	return m_spriteSheet.GetSpriteDef(index + m_startSpriteIndex);
}

SpriteDefinition const& SpriteAnimDefinition::GetSpriteDefAtTimePingPong(float seconds) const
{
	int numFramesPassed = RoundDownToInt(seconds / m_secondsPerFrame);
	int numOfSpriteFrame = m_endSpriteIndex - m_startSpriteIndex;
	if (numOfSpriteFrame == 0)
	{
		return m_spriteSheet.GetSpriteDef(m_startSpriteIndex);
	}
	int posInCircle = numFramesPassed % (2 * (numOfSpriteFrame));
	int index;
	if (posInCircle < numOfSpriteFrame)
	{
		index = posInCircle;
	}
	else
	{
		index = 2 * (numOfSpriteFrame) - posInCircle;
	}

	return m_spriteSheet.GetSpriteDef(index + m_startSpriteIndex);
}
