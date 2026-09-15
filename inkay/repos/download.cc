#include "eg2/sys/report.hh"
#include "eg2/network/curl.hh"
#include "inkay/repos/core.hh"
#include "inkay/repos/download.hh"
#include "inkay/filesystem/zip.hh"
#include <vector>
#include <atomic>

namespace Inkay {
    namespace Download {
        std::string LastError;
        std::string PendingVersion;
        std::string SelectedEnvironment;
        std::atomic<bool> HasError{false};
        std::atomic<DownloadState> State{DOWNLOADSTATE_SELECT};

        static std::vector<u8> gZipData;
        static std::string gCurrentName;
        static std::atomic<bool> gPendingExtract{false};

        static void SetError(const std::string& msg) {
            LastError = msg;
            HasError.store(true, std::memory_order_release);
            State.store(DOWNLOADSTATE_ERROR, std::memory_order_release);
            SYS::Report::Log("Pret-arium: %s\n", msg.c_str());
        }

        void UpdateDownloads(void) {
            if (!gPendingExtract.load(std::memory_order_acquire)) return;
            if (!Network::IsDownloadFinished()) return;
            gPendingExtract.store(false, std::memory_order_release);

            if (!Network::WasDownloadSuccessful()) {
                SetError(gCurrentName + " download failed");
                return;
            }
            if (gZipData.size() < 4 || gZipData[0] != 'P' || gZipData[1] != 'K') {
                SetError(gCurrentName + " did not return a valid ZIP");
                return;
            }
            if (!Inkay::ZIP::Extract(gZipData)) {
                SetError(gCurrentName + " extraction failed");
                return;
            }

            SYS::Report::Log("Pret-arium: %s install complete\n", gCurrentName.c_str());
            State.store(DOWNLOADSTATE_FINISHED, std::memory_order_release);
        }

        static void StartDownload(const std::string& name, const std::string& url) {
            if (url.empty()) {
                SetError(name + " release URL unavailable");
                return;
            }

            gCurrentName = name;
            gZipData.clear();
            HasError.store(false, std::memory_order_release);
            State.store(DOWNLOADSTATE_DOWNLOADING, std::memory_order_release);
            gPendingExtract.store(true, std::memory_order_release);
            Network::DownloadAsync(url, &gZipData);
        }

        void PretendoDownload(void) {
            StartDownload("Pretendo", Inkay::Repos::Web::PretendoFileURL);
        }

        void ProtariumDownload(void) {
            StartDownload("Protarium", Inkay::Repos::Web::ProtariumFileURL);
        }
    }
}
