#include "typesetter.h"

Typesetter::Typesetter()
{
	content_ = "abc";
	//content_ = "\"The quick brown fox jumps over the lazy dog\" is an English-language pangram¡ªa phrase that contains all of the letters of the alphabet. It is commonly used for touch-typing practice. It is also used to test typewriters and computer keyboards, show fonts, and other applications involving all of the letters in the English alphabet. Owing to its brevity and coherence, it has become widely known.";


}

Typesetter::~Typesetter()
{
}

void Typesetter::Render(RenderTarget target)
{
	if (target == RenderTarget::SVG)
	{

	}
}