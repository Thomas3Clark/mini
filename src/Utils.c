#include "pebble.h"
#include "MiniDungeon.h"
#include "Utils.h"

// Right justified
void IntToString(char *buffer, size_t bufferSize, int value)
{
	int i = bufferSize - 1;
	int digit;
	int temp = value;

	do
	{
		digit = temp % 10;
		buffer[i] = '0' + digit;
		temp /= 10;
		--i;
	} while(temp);

	while(i >= 0)
	{
#if PAD_WITH_SPACES
		buffer[i] = ' ';
#else
		buffer[i] = '0';
#endif
		--i;
	}
}

void UIntToString(char *buffer, uint32_t value) {
	snprintf(buffer, sizeof(buffer), "%lu", value);
}

// Pulled from the pebble developer forums
// forums.getpebble.com/discussion/comment/28908/#comment_28908
uint16_t Random(uint16_t max)
{
	return (uint16_t)(rand() % max);
}

