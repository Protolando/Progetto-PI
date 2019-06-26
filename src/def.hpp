#ifndef DEFINC
#define DEFINC

#include <string>

#define DIRPATH (std::string(getenv("HOME")) + std::string("/.GunsGame")).c_str()
#define TEXPATH (DIRPATH + std::string("/textures")).c_str()
#define MAPSPATH (DIRPATH + std::string("/maps")).c_str()
#define ASSETSPATH (DIRPATH + std::string("/assets")).c_str()

#endif
