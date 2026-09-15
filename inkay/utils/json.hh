#pragma once

#include <string>

namespace Inkay {
    namespace JSON {
        void ParseGitHubAssetURL(const char* url, const char* preferredAsset, std::string& outUrl);
        void ParseAromaSHA1(const char* url, std::string& outWmsSHA1, std::string& outWpsSHA1);
    }
}
