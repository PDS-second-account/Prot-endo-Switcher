#pragma once

#include <atomic>
#include <string>

enum DownloadState {
    DOWNLOADSTATE_SELECT,
    DOWNLOADSTATE_SELECT_ENV,
    DOWNLOADSTATE_DOWNLOADING,
    DOWNLOADSTATE_ERROR,
    DOWNLOADSTATE_FINISHED
};

namespace Inkay {
    namespace Download {
        extern std::string LastError;
        extern std::atomic<bool> HasError;
        extern std::string PendingVersion;
        extern std::string SelectedEnvironment;
        extern std::atomic<DownloadState> State;

        void PretendoDownload(void);
        void ProtariumDownload(void);
        void UpdateDownloads(void);
    }
}
