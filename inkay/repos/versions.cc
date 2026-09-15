#include "eg2/external/sha1.hh"
#include "eg2/external/core.hh"
#include "eg2/heap/mem2.hh"
#include "inkay/repos/versions.hh"
#include <string>
#include <cstring>
#include <cctype>

namespace Inkay {
    namespace Versions {
        namespace WMS {
            std::string LocalSHA1;
            std::string PretendoSHA1;
        }
        namespace WPS {
            std::string LocalSHA1;
            std::string PretendoSHA1;
        }

        static std::string ActiveEnvironment = "aroma";
        LocalFileState FileState = LocalFileState::Missing;

        static std::string ReadDefaultCFG(void) {
            u32 size = 0;
            void* data = IO::ReadFile("fs:/vol/external01/wiiu/environments/default.cfg", FILEMODE_TXT, &size);
            if (!data || size == 0) return "";
            std::string env((char*)data, size);
            Heap::MEM2::Free(data);
            while (!env.empty() && (env.back() == '\n' || env.back() == '\r' || env.back() == ' ')) env.pop_back();
            return env;
        }

        static std::string ResolveEnvironment(void) {
            std::string cfg = ReadDefaultCFG();
            if (!cfg.empty()) return cfg;
            auto envs = IO::ListDirs("fs:/vol/external01/wiiu/environments");
            if (envs.empty()) return "aroma";
            if (envs.size() == 1) return envs[0];
            return "aroma";
        }

        static bool FileContainsCaseInsensitive(const std::string& path, const char* needle) {
            u32 size = 0;
            void* raw = IO::ReadFile(path.c_str(), FILEMODE_BIN, &size);
            if (!raw || size == 0) return false;

            const char* data = static_cast<const char*>(raw);
            const size_t needleLen = std::strlen(needle);
            bool found = false;

            if (needleLen && size >= needleLen) {
                for (u32 i = 0; i <= size - needleLen && !found; ++i) {
                    bool match = true;
                    for (size_t j = 0; j < needleLen; ++j) {
                        unsigned char a = static_cast<unsigned char>(data[i + j]);
                        unsigned char b = static_cast<unsigned char>(needle[j]);
                        if (std::tolower(a) != std::tolower(b)) {
                            match = false;
                            break;
                        }
                    }
                    if (match) found = true;
                }
            }

            Heap::MEM2::Free(raw);
            return found;
        }

        bool VerifyInstall(void) {
            ActiveEnvironment = ResolveEnvironment();
            const std::string base = "fs:/vol/external01/wiiu/environments/" + ActiveEnvironment + "/";
            const std::string wmsPath = base + "modules/Inkay-pretendo.wms";
            const std::string wpsPath = base + "plugins/Inkay-pretendo.wps";

            auto wms = IO::SHA1::ReadFile(wmsPath.c_str());
            auto wps = IO::SHA1::ReadFile(wpsPath.c_str());

            if (!wms.exists || !wps.exists) {
                FileState = LocalFileState::Missing;
                return false;
            }

            WMS::LocalSHA1 = wms.sha1;
            WPS::LocalSHA1 = wps.sha1;

            if (!WMS::PretendoSHA1.empty() && !WPS::PretendoSHA1.empty() &&
                wms.sha1 == WMS::PretendoSHA1 && wps.sha1 == WPS::PretendoSHA1) {
                FileState = LocalFileState::ValidPretendo;
                return true;
            }

            // Protarium builds expose the project name/version in the plugin/module binary.
            if (FileContainsCaseInsensitive(wmsPath, "protarium") ||
                FileContainsCaseInsensitive(wpsPath, "protarium")) {
                FileState = LocalFileState::ValidProtarium;
                return true;
            }

            FileState = LocalFileState::ValidUnknown;
            return false;
        }
    }
}
