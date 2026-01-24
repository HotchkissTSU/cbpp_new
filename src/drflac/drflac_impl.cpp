#define DR_FLAC_NO_STDIO
#define DR_FLAC_NO_CRC

#include "cbpp/Sound.h"
#define DR_FLAC_BUFFER_SIZE CBPP_SOUND_STREAM_BUFF_SIZE

#define DR_FLAC_IMPLEMENTATION
#include "drflac/dr_flac.h"
