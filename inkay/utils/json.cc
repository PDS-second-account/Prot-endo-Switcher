#include "eg2/libs/nlohmannjson.hh"
#include "eg2/sys/report.hh"
#include "eg2/network/curl.hh"
#include "inkay/utils/json.hh"
#include <string>

namespace Inkay {
    namespace JSON {
        void ParseGitHubAssetURL(const char* url, const char* preferredAsset, std::string& outUrl) {
            outUrl.clear();
            if (!Network::SetConnection(url)) {
                SYS::Report::Log("Pret-arium: curl failed for %s\n", url);
                return;
            }

            auto j = nlohmann::json::parse(Network::GetLastResponse(), nullptr, false);
            if (j.is_discarded() || !j.contains("assets") || !j["assets"].is_array()) return;

            // First try the exact asset we want.
            for (const auto& asset : j["assets"]) {
                if (!asset.contains("name") || !asset.contains("browser_download_url")) continue;
                if (asset["name"].get<std::string>() == preferredAsset) {
                    outUrl = asset["browser_download_url"].get<std::string>();
                    return;
                }
            }

            // Safe fallback: first ZIP asset only. Never treat a .wms/.wps as a ZIP.
            for (const auto& asset : j["assets"]) {
                if (!asset.contains("name") || !asset.contains("browser_download_url")) continue;
                std::string name = asset["name"].get<std::string>();
                if (name.size() >= 4 && name.substr(name.size() - 4) == ".zip") {
                    outUrl = asset["browser_download_url"].get<std::string>();
                    return;
                }
            }
        }

        void ParseAromaSHA1(const char* url, std::string& outWmsSHA1, std::string& outWpsSHA1) {
            if (!Network::SetConnection(url)) {
                SYS::Report::Log("Pret-arium: Aroma API request failed\n");
                return;
            }

            auto j = nlohmann::json::parse(Network::GetLastResponse(), nullptr, false);
            if (j.is_discarded() || !j.is_array()) return;

            for (const auto& root : j) {
                if (!root.contains("category")) continue;
                for (const auto& cat : root["category"]) {
                    if (!cat.contains("packages")) continue;
                    for (const auto& pkg : cat["packages"]) {
                        if (!pkg.contains("id") || pkg["id"] != "inkay") continue;
                        if (!pkg.contains("repositories")) continue;
                        for (const auto& repo : pkg["repositories"]) {
                            if (!repo.contains("files")) continue;
                            for (const auto& file : repo["files"]) {
                                if (!file.contains("path") || !file.contains("sha1")) continue;
                                std::string path = file["path"];
                                std::string sha1 = file["sha1"];
                                if (path.find(".wms") != std::string::npos) outWmsSHA1 = sha1;
                                if (path.find(".wps") != std::string::npos) outWpsSHA1 = sha1;
                            }
                        }
                    }
                }
            }
        }
    }
}
