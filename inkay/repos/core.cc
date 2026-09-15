#include "eg2/network/curl.hh"
#include "eg2/sys/report.hh"
#include "inkay/repos/core.hh"
#include "inkay/repos/versions.hh"
#include "inkay/utils/json.hh"
#include <string>

namespace Inkay {
    namespace Repos {
        namespace Web {
            std::string PretendoFileURL;
            std::string ProtariumFileURL;
        }

        static const char* PretendoREPO = "https://api.github.com/repos/PretendoNetwork/Inkay/releases/latest";
        static const char* ProtariumREPO = "https://api.github.com/repos/Protarium-Network/Inkay-GitHub-Release/releases/latest";
        static const char* AromaAPI = "https://aroma.foryour.cafe/api/check_versions";

        void Init(void) {
            Network::GiveCertCA("fs:/vol/content/shoumeisho/CA.pem");
            Network::SetUserAgent("Pret-arium-Switcher/0.1 (WiiU; Aroma; curl)");
            Network::SetHeader("Accept: application/vnd.github+json");
            Network::SetHeader("X-GitHub-Api-Version: 2022-11-28");

            Inkay::JSON::ParseGitHubAssetURL(PretendoREPO, "Inkay-pretendo.zip", Web::PretendoFileURL);
            Inkay::JSON::ParseGitHubAssetURL(ProtariumREPO, "wiiu.zip", Web::ProtariumFileURL);
            Inkay::JSON::ParseAromaSHA1(AromaAPI, Inkay::Versions::WMS::PretendoSHA1, Inkay::Versions::WPS::PretendoSHA1);
            Inkay::Versions::VerifyInstall();

            SYS::Report::Log("\n-----------------------------------\n");
            SYS::Report::Log("Pret-arium Switcher\n");
            SYS::Report::Log("Pretendo URL: %s\n", Web::PretendoFileURL.c_str());
            SYS::Report::Log("Protarium URL: %s\n", Web::ProtariumFileURL.c_str());
            SYS::Report::Log("-----------------------------------\n\n");
        }

        void Shutdown(void) {
            Web::PretendoFileURL.clear();
            Web::ProtariumFileURL.clear();
            Inkay::Versions::WMS::LocalSHA1.clear();
            Inkay::Versions::WMS::PretendoSHA1.clear();
            Inkay::Versions::WPS::LocalSHA1.clear();
            Inkay::Versions::WPS::PretendoSHA1.clear();
        }
    }
}
