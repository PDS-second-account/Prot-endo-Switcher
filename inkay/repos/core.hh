#pragma once

#include <string>

namespace Inkay {
    namespace Repos {
        namespace Web {
            extern std::string PretendoFileURL;
            extern std::string ProtariumFileURL;
        }

        void Init(void);
        void Shutdown(void);
    }
}
